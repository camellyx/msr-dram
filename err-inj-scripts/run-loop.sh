#!/bin/bash
# Usage: ./run-loop.sh [benchmark]

killall timed-kill.sh
killall sleep

# find new folder
bkupNum=0
if [ -f ./$1/run-one.sh ]; then
  while true; do
    while [ -d ~/Documents/Yixin-ErrInj/$1/$1.bk.$bkupNum ]; do
      bkupNum=`expr $bkupNum + 1`
    done
    echo "Running test #$bkupNum"
    mkdir ~/Documents/Yixin-ErrInj/$1/$1.bk.$bkupNum
    ./$1/timed-kill.sh ~/Documents/Yixin-ErrInj/$1/$1.bk.$bkupNum &
    ./$1/run-one.sh ~/Documents/Yixin-ErrInj/$1/$1.bk.$bkupNum 
    sleep 5
  done
fi
