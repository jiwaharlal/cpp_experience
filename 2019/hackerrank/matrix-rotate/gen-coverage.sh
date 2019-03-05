#!/bin/bash

COV_FILE_NAME=main_coverage
COV_FILE_BASE=$COV_FILE_NAME.base
COV_FILE_ORIG=$COV_FILE_NAME.info.orig
COV_FILE=$COV_FILE_NAME.info
COV_REPORT_DIR=out

#COV_DIR=CMakeFiles/ring.dir
COV_DIR=.

#cd build

#make -j3
#if [ $? -ne 0 ]; then
   #echo "Failed to build project, no coverage report to generate."
   #exit
#fi

# exit on command execution error
set -e

# cleanup lcov and create baseline
lcov --directory $COV_DIR --zerocounters
lcov --capture --initial --directory $COV_DIR --output-file $COV_FILE_BASE

# run tests
make test

# capture counters and generate report
lcov --capture --directory $COV_DIR --output-file $COV_FILE_ORIG
lcov -a $COV_FILE_BASE -a $COV_FILE_ORIG -o $COV_FILE_NAME.total

# remove standard library coverage
lcov --remove $COV_FILE_NAME.total "/usr/*" -o $COV_FILE

genhtml $COV_FILE --output-directory $COV_REPORT_DIR --highlight --legend
rm -f $COV_FILE_NAME.total $COV_FILE_BASE $COV_FILE

firefox $COV_REPORT_DIR/index.html
