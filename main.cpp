#include "mainwindow.h"
#include "dbc/dbchandler.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QNetworkDatagram>
#include <QDateTime>
#include <QDebug>
#include <bitset>
#include <QUdpSocket>
#include <QTimer>
#define STAMP "yyyyMMdd HH:mm:ss.zzz"
#define DEFAULT_MESSAGE "BCU_04"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationVersion(QDateTime::currentDateTime().toString(STAMP));
    QCommandLineParser p;
    p.setApplicationDescription("DBC data with UDP network emulator");
    p.addHelpOption();
    p.addVersionOption();
    p.addOption({{"t", "timer inteval"}, "timer inteval. ms,default is 1000ms","t","1000"});
    p.addOption({{"u", "use time"}, "use massage timer inteval. ms","u",""});
    p.addOption({{"m", "message name"}, "message name. string,default is " DEFAULT_MESSAGE,"m",DEFAULT_MESSAGE});
    p.addOption({{"I", "message id"}, "message ID. int,default is 91","I","91"});
    p.addOption({{"s", "signal idx"}, "signal idx. int,default is 0","s"});
    p.addOption({{"n", "no run"}, "do not increment,default is 0","n","0"});
    p.addOption({{"i", "ip"}, "UDP host IP. string,default is 127.0.0.1","i","127.0.0.1"});
    p.addOption({{"p", "port"}, "UDP host port,int. default is 11000","p","11000"});
    p.addOption({{"V","verbose"}, "Verbose mode. Prints out more information."});
    p.addPositionalArgument("input", "DBC file.", "input");
    p.process(app);

    QScopedPointer<MainWindow> win;

    QStringList arglist = p.positionalArguments();
    if(arglist.size()>0){
        QString dbcfile = arglist.at(0);
        DBCFile newFile;
        newFile.loadFile(dbcfile);

        DBCMessageHandler *messageHandler = newFile.messageHandler;
        DBC_MESSAGE *message = messageHandler->findMsgByName(DEFAULT_MESSAGE);
        {
            DBC_MESSAGE *a = messageHandler->findMsgByName(p.value("m"));
            DBC_MESSAGE *b = messageHandler->findMsgByID(quint32(p.value("I").toInt()));
            if(p.isSet("m")&& a) message = a;
            if(p.isSet("I")&& b) message = b;
        }

        int cycletime = p.value("t").toInt();
        if(arglist.size()>1){
            cycletime = arglist.at(1).toInt();
        }
        if(p.isSet("u")&&p.value("u").toInt()>0){
            if(message){
                DBC_ATTRIBUTE_VALUE *attr = message->findAttrValByName("GenMsgCycleTime");
                if(attr){
                    cycletime = attr->value.toInt();
                }
            }
        }

        ///////////////////////////////////////////////////
        for(int m=0;m<messageHandler->getCount();++m){
            DBC_MESSAGE *msg = messageHandler->findMsgByIdx(m);
            DBCSignalHandler *sigHandler = msg->sigHandler;
            for(int i=0;i<sigHandler->getCount();++i){
                DBC_SIGNAL* sig = sigHandler->findSignalByIdx(i);
                qInfo() <<sig->parentMessage->ID<<sig->parentMessage->len  << sig->name<<sig->startBit<<sig->signalSize;
            }
            qInfo()<< msg->ID;
        }

        QTimer *timer = new QTimer(&app);
        QObject::connect(timer,&QTimer::timeout,[&](){
            if(not message or message->len>8) return ;
            static quint64 cnt = 0;
            static QStringList lst = p.value("s").split(QRegExp("\\W+"),QString::SkipEmptyParts);

            std::bitset<64> payload;
            if(p.value("n").toInt()<=0){
                DBCSignalHandler *sigHandler = message->sigHandler;
    //            for(int i=0;i<sigHandler->getCount();++i){
                    payload = std::bitset<64>(++cnt);
    //            }
            }
            for(auto s:lst) payload.set(s.toInt());

            QUdpSocket sock;
            QNetworkDatagram gram;
            QByteArray upacket;
            QDataStream stream(&upacket, QIODevice::WriteOnly);
            stream.setByteOrder(QDataStream::BigEndian);
            stream << quint32(message->ID);
            stream << quint8(message->len);
            quint64 payl = payload.to_ullong();
            stream.writeRawData(reinterpret_cast<const char*>(&payl),int(message->len));
            gram.setData(upacket);
            gram.setDestination(QHostAddress(p.value("i")),quint16(p.value("p").toInt()));
            qInfo()<<p.value("i")<<p.value("p")<<cycletime<<"name:"<<message->name<<"ID:"<<message->ID<<"payload:"<<message->len<<"total:"<<upacket.size()<< gram.data().toHex('-');
            sock.writeDatagram(gram);
        });
        timer->start(cycletime);

    } else {
        win.reset(new MainWindow);
        win->show();
    }

    return app.exec();
}
