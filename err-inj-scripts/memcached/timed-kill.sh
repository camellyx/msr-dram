#!/bin/bash
# Usage: ./timed-kill [/path/to/result] &

sleep 1200
loader=`pidof loader`
if [ "$loader" != "" ]; then
  killall loader
  echo Timeout $1/error.log.txt
fi
