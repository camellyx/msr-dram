#!/bin/bash
# Usage: ./inject.sh [Process_Name]

pushd `dirname $BASH_SOURCE`

if [ ! -f ./parsePmap ]; then
  make
fi
processid=`pidof $1`
pmap $processid > ./$1.pmap
virtual=`./parsePmap $1.pmap anon`
#echo $virtual
original=`sudo gdb --batch --pid=$processid -ex="x /xb 0x$virtual" | awk 'END{print $NF}'`
corrupted=`./corrupt $original`
sudo gdb --batch-silent --pid=$processid -ex="set {char}0x$virtual = 0x$corrupted"
corrupted=`sudo gdb --batch --pid=$processid -ex="x /xb 0x$virtual" | awk 'END{print $NF}'`
echo "Corrupted 0x$virtual from $original to $corrupted"
echo 0x$virtual

rm -f $1.pmap

popd
