#!/bin/csh
set testlist="./test.list"
set jobid=$1
set mode=$2
root4star -b -q check_Q.C\(\"$testlist\",\"$jobid\",$mode\)