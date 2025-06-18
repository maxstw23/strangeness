#/bin/bash

# save file list into a list file
# the dir is /star/data01/pwg/xiatong/AMPT/14GeV/*root
# to avoid argument list too long, use find

find /star/data01/pwg/xiatong/AMPT/14GeV/ -name "*.root" > AMPT_14GeV.list