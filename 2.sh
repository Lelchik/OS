#!/bin/bash

NETCAT=nc
STTY=stty
CLEAR=clear

MY_CHAR='X'
ENEMY_CHAR='0'
MSG_WIN = ''
COUNT=0
quit() {
    read -t 0.5 -n 20
    $STTY echo
    exit $PORT
}

usage() {
    echo "Crosses and zeros"
    echo "For turn enter 2 number: X and Y"
    echo "X - number of string. 1<=X<=3"
    echo "Y - number of column. 1<=Y<=3"
    echo "For example: 1 2"
    echo "Press any key for continue"
}

redraw() {
    clear
    tput sc
    tput cup 0 0; echo  "${THY1[0]}"
    tput cup 0 1; echo "|";
    tput cup 0 2; echo "${THY1[1]}"
    tput cup 0 3; echo "|";
    tput cup 0 4; echo "${THY1[2]}"
    tput cup 2 1; echo "|";
    tput cup 2 0; echo  "${THY1[3]}"
    tput cup 2 2; echo  "${THY1[4]}"
    tput cup 2 4; echo  "${THY1[5]}"
    tput cup 4 0; echo  "${THY1[6]}"
    tput cup 4 2; echo  "${THY1[7]}"
    tput cup 4 4; echo  "${THY1[8]}"
    tput cup 2 3; echo "|";
    tput cup 4 1; echo "|";
    tput cup 4 3; echo "|";
    tput cup 3 3; echo "+";
    tput cup 3 1; echo "+";
    tput cup 1 3; echo "+";
    tput cup 1 1; echo "+";
    tput cup 4 3; echo "|";
    tput cup 1 0; echo "-";
    tput cup 1 2; echo "-";
    tput cup 1 4; echo "-";
    tput cup 3 0; echo "-";
    tput cup 3 2; echo "-";
    tput cup 3 4; echo "-";
    tput cup 10 0; echo "$MSG_WIN"
    tput cup 11 0; echo "$MSG3";
    tput cup 12 0;
}

initfields() {
    local i j
    for (( i = 0; i < 9; ++i )) ; do
        THY1[$i]=''
    done
}

checkconnection() {
    if [ -z "$COPROC_PID" ] ; then
        echo "Connection closed"
        quit
    fi
}


waitturn() {
    redraw
    while true; do
        checkconnection
        if ! read -u "${COPROC[0]}" B ; then
            echo "Connection closed"
            quit
        fi
        THY1["$B"]=$ENEMY_CHAR
        COUNT=$(($COUNT + 1 ));
        break 
    done
    redraw
    return 0
}

check_WIN() {
    if  [ "${THY1[0]}" = "${THY1[4]}" ] && [ "${THY1[0]}" = "${THY1[8]}" ] && [ "${THY1[0]}" != "" ];
        then WIN=1; 
        if  [ "${THY1[0]}" = "$MY_CHAR" ]; then MSG2=''; MSG_WIN="YOU WIN!"; else MSG_WIN="YOU LOOSE!"; 
        fi
    fi
   
    if  [ "${THY1[0]}" = "${THY1[3]}" ] && [ "${THY1[0]}" = "${THY1[6]}" ] && [ "${THY1[0]}" != "" ];
        then WIN=1; 
        if  [ "${THY1[0]}" = "$MY_CHAR" ]; then MSG2='';  MSG_WIN="YOU WIN!"; else MSG_WIN="YOU LOOSE!"; 
        fi
    fi

    if  [ "${THY1[0]}" = "${THY1[1]}" ] && [ "${THY1[0]}" = "${THY1[2]}" ] && [ "${THY1[0]}" != "" ];
        then WIN=1; 
        if  [ "${THY1[0]}" = "$MY_CHAR" ]; then MSG2='';  MSG_WIN="YOU WIN!"; else MSG_WIN="YOU loose!"; 
        fi
    fi
    if  [ "${THY1[1]}" = "${THY1[4]}" ] && [ "${THY1[1]}" = "${THY1[7]}" ] && [ "${THY1[1]}" != "" ];
        then WIN=1; 
        if  [ "${THY1[1]}" = "$MY_CHAR" ]; then MSG2='';  MSG_WIN="YOU WIN!"; else MSG_WIN="YOU loose!"; 
        fi
    fi
  
    if  [ "${THY1[2]}" = "${THY1[5]}" ] && [ "${THY1[2]}" = "${THY1[8]}" ] && [ "${THY1[2]}" != "" ]; 
        then WIN=1; 
        if  [ "${THY1[2]}" = "$MY_CHAR" ]; then MSG2='';  MSG_WIN="YOU WIN!"; else MSG_WIN="YOU loose!"; 
        fi
    fi
  
    if  [ "${THY1[3]}" = "${THY1[4]}" ] && [ "${THY1[3]}" = "${THY1[5]}" ] && [ "${THY1[3]}" != "" ]; 
        then WIN=1; 
        if  [ "${THY1[3]}" = "$MY_CHAR" ]; then MSG2='';  MSG_WIN="YOU WIN!"; else MSG_WIN="YOU loose!"; 
        fi
    fi
    
    if  [ "${THY1[6]}" = "${THY1[7]}" ] && [ "${THY1[6]}" = "${THY1[8]}" ] && [ "${THY1[6]}" != "" ]; 
        then WIN=1; 
        if  [ "${THY1[6]}" = "$MY_CHAR" ]; then MSG2='';  MSG_WIN="YOU WIN!"; else MSG_WIN="YOU loose!"; 
        fi
    fi
    
    if  [ "${THY1[2]}" = "${THY1[4]}" ] && [ "${THY1[2]}" = "${THY1[6]}" ] && [ "${THY1[2]}" != "" ];  
        then WIN=1; 
        if  [ "${THY1[2]}" = "$MY_CHAR" ]; then  MSG2=''; MSG_WIN="YOU WIN!"; else MSG_WIN="YOU loose!"; 
        fi
    fi

    if [ $COUNT -ge 9 ] && [ $WIN -eq 0 ]; 
        then 
            WIN=1; 
            MSG2='';
            MSG_WIN="Nobody win!"; 
    fi 
    return $WIN
}

maketurn() {
    while true; do
        read X Y;
        echo $X $Y >>/dev/tty
        if [ -z "${X//[0-9]/}" ] || [ -z "${Y//[0-9]/}" ]; then 
            if [ $X -ge 4 ] && [ $X -le 0 ]; 
                then 
                    echo "ERROR">>/dev/tty 
                else
                    if [ $Y -ge 4 ] && [ $Y -le 1 ]; 
                        then echo "ERROR">>/dev/tty  
                        else 
                            Z=$(($X * 3 + $Y - 4));
                        if [ "${THY1[$Z]}" = "" ]; 
                        then 
                            echo "$Z">&${COPROC[1]}
                            COUNT=$(($COUNT + 1 ));
                            THY1[$Z]=$MY_CHAR
                            break 1
                        else echo "NOT FREE">>/dev/tty  
                        fi
                    fi 
            fi
        else echo "Not number">>/dev/tty 
        fi
    done
    redraw
    return 0
}


WIN=0
$STTY -echo
HOST="localhost"
$CLEAR
PORT=31361

if [ -n "`netstat -aln | grep :$PORT `" ]; then
    echo "Connecting to"
    echo "localhost:$PORT..."
    coproc $NETCAT  -q 0 localhost $PORT
    READTIMEOUT=5
else
    echo "Waiting for incoming"
    echo "connection on port $PORT..."
    coproc $NETCAT -q 0 -l -p $PORT
    READTIMEOUT=180
fi

initfields
usage
checkconnection
read 

while true; do
    THYNUMBER=$RANDOM
    echo "$THYNUMBER" >&${COPROC[1]}
    if ! read -u "${COPROC[0]}" FOENUMBER ; then
        echo "Connection closed"
        quit
    fi
    if (( $FOENUMBER > $THYNUMBER )) ; then
        MY_CHAR="0"
        ENEMY_CHAR="X"
        waitturn || break
    else
        MY_CHAR="X"
        ENEMY_CHAR="0"
        redraw
    fi
    while (($WIN != 1)); do
        echo "Your turn"
        maketurn
        check_WIN
        if [ $WIN -eq 1 ]; then break ; fi
        waitturn
        check_WIN
    done
    redraw
    tput cup 14 0
    echo "Play again? (y/n)"
    WIN=0
    MSG_WIN=''
    COUNT=0
    while read -n 1 YESNO ; do
        checkconnection
        if [ "$YESNO" == 'y' ] || [ "$YESNO" == 'Y' ] ; then
            initfields
            redraw
            continue 2
        fi
        if [ "$YESNO" == 'n' ] || [ "$YESNO" == 'N' ] ; then
           break 2
       fi
    done
    break
done

if [ -n "$COPROC_PID" ] ; then
    kill $COPROC_PID
    wait $COPROC_PID
fi
quit