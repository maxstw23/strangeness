#!/bin/bash

user="maxwoo"
node="6015"
# The section below is for the other Job ID (the one provided to the xml), 
# which can not be used to condor_rm jobs
# idarray=($(condor_q -global -submitter maxwoo | grep `pwd` | grep " $1 " | sed 's/^.*sched//; s/_.*//'))
# for i in “${idarray[@]}”
# do
# 	for node in $(seq -f "%02g" 5 15)
#	do
#		echo "condor_rm -name rcas60$node $i"
#		condor_rm -name rcas60$node $i
#	done
# done

idarray=($(condor_q -global -submitter maxwoo | grep `pwd` | grep " $1 " | sed 's/ .*//'))
for i in "${idarray[@]}"
do
	# for node in $(seq -f "%02g" 5 15)
	# do
		echo "condor_rm -name rcas$node $i"
		condor_rm -name rcas$node $i
	# done
	
done  
