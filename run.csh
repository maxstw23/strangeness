#!/bin/tcsh

# historic for selecting fractions of files
set min=1
set var='*.root'

# mode: {0: AMPT default, 1: AMPT SM, 2: UrQMD}
# before UrQMD cen def, use 1 for UrQMD as well

#star-submit-template -template ./Analysis.xml -entities input_dir=$var,mode=1
#star-submit-template -template ./Analysis_default.xml -entities input_dir=$var,mode=0
#star-submit-template -template ./Analysis_urqmd.xml -entities input_dir=$var,mode=2

#star-submit-template -template ./Analysis_sness.xml -entities input_dir=$var,mode=1

star-submit-template -template ./Analysis_QA.xml -entities input_dir=$var,mode=1
