#!/bin/bash

source /data/root-v5.34.21/bin/thisroot.sh
g++ -o $1 $1.cc `root-config --glibs --cflags` -I$STC_PATH/headers -L$STC_PATH -lSTConverter
