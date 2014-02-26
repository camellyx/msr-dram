#!/bin/bash
# Usage: ./batchComp.sh [/path/to/result/]

resultPath=$1
summaryFile=$resultPath/summary.txt

rm -f $summaryFile
for resultFolder in $(ls $resultPath); do
  echo "# $resultPath/$resultFolder" >> $summaryFile
  if [ -f $resultPath/$resultFolder/response.log.txt ]; then
    ./compResp goldenResp.log.txt $resultPath/$resultFolder/response.log.txt >> $summaryFile
  else
    echo "file not found" >> $summaryFile
  fi
done
