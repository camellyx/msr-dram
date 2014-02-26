#!/bin/bash
# Usage: ./run-one.sh [/path/to/result/]
pushd `dirname $BASH_SOURCE`
# pre-injection 
killall memcached
~/Downloads/memcached/start-server.sh
time ~/Downloads/memcached/warmup.sh

# inject error
../common/inject.sh memcached > $1/virtual.txt

# run workload
~/Downloads/memcached/start-client.sh | grep @ > $1/response.log.txt

# post injection
killall memcached
killall timed-kill.sh
killall sleep
popd
