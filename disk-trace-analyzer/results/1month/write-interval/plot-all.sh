#!/usr/bin/env bash

for file in *.csv
do
  ../../scripts/bar-plot.py $file
done
