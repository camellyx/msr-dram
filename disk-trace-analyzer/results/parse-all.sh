#!/usr/bin/env bash

for file in *.stdout
do
  echo "write_interval_in_hour, count" > ${file%.stdout}.csv
  ../scripts/parse-hash.pl $file | sort -n -k 1 >> ${file%.stdout}.csv
done
