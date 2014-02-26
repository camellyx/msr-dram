#!/bin/bash
# Usage: ./watch.sh [Process_Name] [Mem_Region]

pushd `dirname $BASH_SOURCE`

if [ ! -f ./parsePmap ]; then
  make
fi
processid=`pidof $1`
pmap $processid > ./$1.pmap
cp watchAddr.gdb watchAddr.gdb.new

for i in {0..3}; do
  virtual=`./parsePmap $1.pmap $2`
  sed s:Addr$i:$virtual: watchAddr.gdb.new > watchAddr.gdb.newnew
  mv watchAddr.gdb.newnew watchAddr.gdb.new
  #cat watchAddr.gdb.new
done

sudo gdb --batch --pid=$processid -x watchAddr.gdb.new

rm -f $1.pmap

popd
