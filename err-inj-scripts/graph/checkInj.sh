#!/bin/bash
# Usage: ./checkInj.sh
pushd `dirname $BASH_SOURCE`

started=0
while [ $started -eq 0 ]; do
  if [ -f ~/Downloads/graph-release/Twitter-dataset/stdout.txt ]; then
    started=$(grep vertices ~/Downloads/graph-release/Twitter-dataset/stdout.txt -c)
  else
    started=0
  fi
  sleep 1
done

# inject error
../common/inject.sh tunkrank > $1/virtual.txt

popd
