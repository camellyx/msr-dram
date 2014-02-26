#!/usr/bin/env bash

rootFolder=$1
pushd $rootFolder

for resultFolder in $(ls -d */); do
  pushd $resultFolder
  sort -r -g -k 2 yxtest_1_of_4 | head -100 > yxtest_1_of_4.100.txt
  sort -r -g -k 2 yxtest_2_of_4 | head -100 > yxtest_2_of_4.100.txt
  sort -r -g -k 2 yxtest_3_of_4 | head -100 > yxtest_3_of_4.100.txt
  sort -r -g -k 2 yxtest_4_of_4 | head -100 > yxtest_4_of_4.100.txt
  sort -m yxtest_*_of_4.100.txt | head -100 | awk '{print $1}' > yxtest_sorted.100.txt
  #sort -r -g -k 2 yxtest_*_of_4 -o yxtest_sorted.txt
  popd
done

popd
