#!/bin/bash
# Usage: ./batchComp.sh [/path/to/result/]

resultPath=$1
mkdir $resultPath/summary

for resultFolder in $(ls $resultPath); do
  echo "# $resultPath/$resultFolder"
  if [ -f $resultPath/$resultFolder/summary.txt ]; then
    mkdir $resultPath/summary/$resultFolder
    cat $resultPath/$resultFolder/summary.txt | ./cntSumm > $resultPath/summary/$resultFolder/count.txt
  else
    echo "file not found"
  fi
done
