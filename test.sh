#!/bin/bash
testlist="./test.list"
jobid=$1
mode=$2
starpro
root4star -b -q check_Q.C\(\"$testlist\",\"$jobid\",$mode\)