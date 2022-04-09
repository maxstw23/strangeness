#!/bin/tcsh

# historic for selecting fractions of files
min=1
printf -v var '*.root'

# mode: {0: AMPT default, 1: AMPT SM, 2: UrQMD}
# before UrQMD cen def, use 1 for UrQMD as well

# Correlation
#star-submit-template -template ./Analysis_2.xml -entities input_dir=$var,mode=1
#star-submit-template -template ./Analysis_default_2.xml -entities input_dir=$var,mode=0
#star-submit-template -template ./Analysis_urqmd_2.xml -entities input_dir=$var,mode=1

# Strangeness Conservation
star-submit-template -template ./Analysis_sness_2.xml -entities input_dir=$var
