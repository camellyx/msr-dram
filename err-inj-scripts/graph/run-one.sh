#!/bin/bash
# Usage: ./run-one.sh [/path/to/result/]
pushd `dirname $BASH_SOURCE`
# pre-injection 
killall tunkrank
rm ~/Downloads/graph-release/Twitter-dataset/stdout.txt
~/Downloads/graph-release/Twitter-dataset/run.sh

# inject error
./checkInj.sh

# run workload

# post injection
killall tunkrank
popd
