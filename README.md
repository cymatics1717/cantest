# usage

    Usage: ./cantest [options] input
    DBC data with UDP network emulator

    Options:
    -h, --help               Displays this help.
    -v, --version            Displays version information.
    -t, --timer inteval <t>  timer inteval. ms,default is 1000ms
    -u, --use time <u>       use massage timer inteval. ms
    -m, --message name <m>   message name. string,default is BCU_04
    -I, --message id <I>     message ID. int,default is 91
    -s, --signal idx <s>     signal idx. int,default is 0
    -n, --no run <n>         do not increment,default is 0
    -i, --ip <i>             UDP host IP. string,default is 127.0.0.1
    -p, --port <p>           UDP host port,int. default is 11000
    -V, --verbose            Verbose mode. Prints out more information.

    Arguments:
    input                    DBC file.

# push default message to a specified UDP socket
    ./cantest ~/Downloads/V6_3.dbc -i 10.11.153.15 -p 11100

# push default message with a specified time inteval
    ./cantest ~/Downloads/V6_3.dbc -t 100
    ./cantest ~/Downloads/V6_3.dbc 100

# push message with a specified name
    ./cantest ~/Downloads/V6_3.dbc -m WLC_CDC_NFC


# push message with a specified ID
    ./cantest ~/Downloads/V6_3.dbc -I 1198

# push message with the time inteval specified in the DBC file [GenMsgCycleTime]
    ./cantest ~/Downloads/V6_3.dbc -I 1198 -u 1

# push message with signal value specified
    ./cantest ~/Downloads/V6_3.dbc -I 1198 -s 24,25,26,27,28

# do not increment the value
    ./cantest ~/Downloads/V6_3.dbc -I 1198 -s 24,25,26,27,28 -n 1


