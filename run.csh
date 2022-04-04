#!/bin/tcsh
min=1
mode=1
printf -v var '*.root'

#star-submit-template -template ./Analysis.xml -entities input_dir=$var,mode=$mode
#star-submit-template -template ./Analysis_default.xml -entities input_dir=$var,mode=$mode
star-submit-template -template ./Analysis_urqmd.xml -entities input_dir=$var,mode=$mode
