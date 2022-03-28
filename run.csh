#!/bin/tcsh
min=1
printf -v var '*.root'

#star-submit-template -template ./Analysis.xml -entities input_dir=$var
star-submit-template -template ./Analysis_default.xml -entities input_dir=$var
#star-submit-template -template ./Analysis_urqmd.xml -entities input_dir=$var
