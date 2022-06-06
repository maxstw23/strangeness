#!/bin/csh
# -------------------------------------- 
# Script generated on Wed Jun 01 22:23:13 EDT 2022 by the Simple Unified Meta Scheduler 1.10.50 and submitted with:
# cd /star/data01/pwg/xiatong/git/strangeness; condor_submit -verbose /star/data01/pwg/xiatong/git/strangeness/script/sched21EF1A6A9139C9FD2A38E59AFB466F9A_2650_2749.condor
# --------------------------------------



if ( $?SUMS_EXECUTION ) then
    goto USERCODESECTION
endif





/bin/echo 'We are starting on node: '`/bin/hostname`


# Preparing environment variables
ENVSETUPSECTION:

setenv FILEBASENAME "no_name"
setenv FILELIST "/star/data01/pwg/xiatong/git/strangeness/log/sched21EF1A6A9139C9FD2A38E59AFB466F9A_2732.list"
setenv FILELIST_ALL "/star/data01/pwg/xiatong/git/strangeness/script/sched21EF1A6A9139C9FD2A38E59AFB466F9A.list"
setenv INPUTFILECOUNT "37"
setenv JOBID "21EF1A6A9139C9FD2A38E59AFB466F9A_2732"
setenv JOBINDEX "2732"
setenv LOGGING "STD"
setenv REQUESTID "21EF1A6A9139C9FD2A38E59AFB466F9A"
setenv SUBMITATTEMPT "1"
setenv SUBMITTINGDIRECTORY "/star/data01/pwg/xiatong/git/strangeness"
setenv SUBMITTINGNODE "rcas6010.rcf.bnl.gov"
setenv SUBMIT_TIME "2022-06-02 02:23:13"
setenv SUMS_AUTHENTICATED_USER "maxwoo@rhic.bnl.gov"
setenv SUMS_USER "maxwoo"
setenv SUMS_nProcesses "2950"
setenv SUMS_name "null"
setenv USEXROOTD "1"
setenv INPUTFILE0 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890134631.root"
setenv INPUTFILE1 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890136101.root"
setenv INPUTFILE2 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890136471.root"
setenv INPUTFILE3 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890136571.root"
setenv INPUTFILE4 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890136621.root"
setenv INPUTFILE5 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890136801.root"
setenv INPUTFILE6 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890136921.root"
setenv INPUTFILE7 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890138501.root"
setenv INPUTFILE8 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890139401.root"
setenv INPUTFILE9 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1890182461.root"
setenv INPUTFILE10 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891133981.root"
setenv INPUTFILE11 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891134061.root"
setenv INPUTFILE12 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891134341.root"
setenv INPUTFILE13 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891134361.root"
setenv INPUTFILE14 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891134971.root"
setenv INPUTFILE15 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891135531.root"
setenv INPUTFILE16 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891135861.root"
setenv INPUTFILE17 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891136351.root"
setenv INPUTFILE18 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891136411.root"
setenv INPUTFILE19 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891136851.root"
setenv INPUTFILE20 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891136921.root"
setenv INPUTFILE21 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891138231.root"
setenv INPUTFILE22 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891139071.root"
setenv INPUTFILE23 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1891180821.root"
setenv INPUTFILE24 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892134081.root"
setenv INPUTFILE25 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892134191.root"
setenv INPUTFILE26 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892134231.root"
setenv INPUTFILE27 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892134271.root"
setenv INPUTFILE28 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892134461.root"
setenv INPUTFILE29 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892134681.root"
setenv INPUTFILE30 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892136731.root"
setenv INPUTFILE31 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892136781.root"
setenv INPUTFILE32 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892136791.root"
setenv INPUTFILE33 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892136831.root"
setenv INPUTFILE34 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892136871.root"
setenv INPUTFILE35 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892136901.root"
setenv INPUTFILE36 "/star/data01/pwg/xiatong/AMPT/14GeV/data_1892136911.root"

# This block is used by CONDOR where $HOME in not set

if ( ! $?USER ) then
    echo "USER is not defined"
    set USER=`id | sed "s/).*//" | sed "s/.*(//"`
endif
if ( ! $?HOME ) then
    echo "HOME is not defined"

    if ( -x /usr/bin/getent ) then
	# we have getent, should not be on aix, bsd, Tru64 however
	# will work for Linux
	echo "Using getent method"
	setenv HOME `/usr/bin/getent passwd $USER | /bin/sed 's|.*\:.*\:.*\:.*\:\([^\:]*\):.*|\1|'`
    else 
	set PTEST=`which perl`
	if ( "$PTEST" != "" ) then
	    echo "Using perl method"
	    # we have perl defined, we can get info from there
	    /bin/cat <<EOF >test$$.pl
my(\$user) = getpwuid(\$<);
@items = getpwnam(\$user);
print \$items[7];
EOF
	    setenv HOME `$PTEST test$$.pl` && /bin/rm  -f test$$.pl
	else
	    set CTEST=`which cc`
	    if ( "$CTEST" != "" ) then
		echo "Using C code method"
		# use C code for doing this
		/bin/cat <<EOF >test$$.c
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>


int main()
{
  struct passwd *info;
  uid_t          uid;

  /* get process UID */
  uid = getuid();
  info= getpwuid(uid);

  (void) printf("%s\n",info->pw_dir);
  return 0;
} 

EOF
		$CTEST -o test$$ test$$.c
		/bin/chmod +x test$$
		setenv HOME `./test$$` && /bin/rm -f test$$ test$$.c
	    else
		echo "We have no ways to define HOME and it is not defined"
		exit
	    endif
	endif
    endif
endif

echo "HOME is now $HOME"


# Default value for path if not defined.
if ( ! $?PATH ) then
   setenv PATH /usr/local/bin:/bin:/usr/bin
endif


/usr/bin/test -r $HOME/.cshrc && source $HOME/.cshrc

# Creating the scratch directory, return failure status
set SUMS_try=0
set SUMS_PAD=""

MKDTRY:
setenv SCRATCH "/tmp/$USER$SUMS_PAD/$JOBID"
/bin/mkdir -p $SCRATCH   >& /dev/null
set STS=$status
if (! -d $SCRATCH) then
       #test if porper UID
       set SUMS_IsUID=`(/usr/bin/test -O /tmp/$USER$SUMS_PAD && echo 1) || echo 0`
       if ( $SUMS_try == 0 &&  -e "/tmp/$USER$SUMS_PAD" && ! $SUMS_IsUID) then
            # First try, directory exists but is not owned by $USER
            # Create a different path and try again
            echo "Scheduler:: $SCRATCH not owned by $USER, trying alternative"
            @ try++
            set SUMS_seed=`/bin/date +%j%H%M%S`
            set SUMS_PAD=`expr $SUMS_seed  \* 25973 \% 100`
            goto MKDTRY
        else
		 echo "Scheduler:: Failed to create $SCRATCH on $HOST"
	         exit $STS
        endif

endif


##########################################################
# Detached script pid so that signals will propagate to  #
# child processes such as root4star, giving a chance to  #
# close out open files.                                  #
##########################################################
if ( ! $?SUMS_EXECUTION ) then
    echo "$$ We will spawn ourselves ($0 $*)"
    echo "$$ We BEGIN on [`/bin/date`]"

    onintr SUMS_SIGHANDLER
    ((setenv SUMS_EXECUTION 1 && nohup $0 $*) > $SCRATCH/.output.log ) >& $SCRATCH/.errror.log & 
    set PID=$!

    echo "$$ We are waiting for $PID "
    set loop=0
    set DELAY=1
    set CHLDPID=""
    set CHKP=10
    while (`/bin/ps --no-headers -p $PID` != "")
       if ( $loop % $CHKP == 0 ) then
           # check this only once every CHKP*DELAY seconds
           set GTEST=`/bin/ps -efH | /bin/grep -e "^[a-z]\+ \+[0-9]\+ \+$PID .*" | /bin/grep -v $$ |  /bin/grep -v grep | /usr/bin/head -1 | /usr/bin/awk '{print $2}'`
           if ( "$GTEST" != "" && "$GTEST" != "$CHLDPID") then
               echo "$$ Grandchild PID is $GTEST - found in $loop x $DELAY seconds"
               if ( "$CHLDPID" == "") then
                   echo "$$ Establishing first watcher   on    `/bin/date`"
                   set DELAY=2
               else
                   echo "$$ Grandchild to watch has changed on `/bin/date`"
               endif
               set CHLDPID="$GTEST"
               echo "$$ Process tree is $$ (main) -> $PID (child) -> $CHLDPID (grandchild)"
               /bin/ps -l $CHLDPID
            endif
       endif
       @ loop++
       sleep $DELAY
   end
   cat $SCRATCH/.output.log >> /dev/stdout
    cat $SCRATCH/.errror.log >> /dev/stderr

# Delete the scratch directory
    /bin/rm -fr $SCRATCH
    echo "$$ Execution of ($0 $*) has ended, exiting normally"
    echo "$$ We END on [`/bin/date`]"
    exit 0

    SUMS_SIGHANDLER:
        echo "$$ We received a signal - sending TERM to PID=$PID group"
        kill -TERM -$PID
        echo "$$ We have sent an TERM signal to $PID and waiting for $CHLDPID on `/bin/date`"
        while (`/bin/ps --no-headers -p $CHLDPID` != "")
           sleep 2
        end
        echo "$$ Dumping the output/error channels ---->"
        cat $SCRATCH/.output.log >> /dev/stdout
        cat $SCRATCH/.errror.log >> /dev/stderr
        echo "$$ <---- done with output/error channels"
        echo "$$ We are now leaving with error on [`/bin/date`]"
        exit 1
endif

USERCODESECTION:
echo "$$ Stepping into the main SUMS wrapper program with [$*]"




#import packages 
echo '.... building sandbox'
setenv pwd `pwd` 
/bin/mkdir -p $SCRATCH/./
foreach object ( `find ./ -maxdepth 1 -name 'Check_Sness.C' ` ) 
 
 
          if($object != './') then 
 
                 foreach DIRObject ( `find $object`) 
 
                        setenv  NewLink  $SCRATCH/$DIRObject 
 
                        if(! -e $NewLink) then 
                             if(-f $DIRObject) then 
                                   /bin/ln -s  `echo $DIRObject | /bin/sed "s|\(^[^/]\)\(.*\)|$pwd/\1\2|"`  $NewLink 
                             else 
                                   /bin/mkdir -p   $NewLink 
                             endif 
                        endif 
               end 
 
 
        endif 
 
end 
/bin/mkdir -p $SCRATCH/./
foreach object ( `find ./ -maxdepth 1 -name 'check_S.C' ` ) 
 
 
          if($object != './') then 
 
                 foreach DIRObject ( `find $object`) 
 
                        setenv  NewLink  $SCRATCH/$DIRObject 
 
                        if(! -e $NewLink) then 
                             if(-f $DIRObject) then 
                                   /bin/ln -s  `echo $DIRObject | /bin/sed "s|\(^[^/]\)\(.*\)|$pwd/\1\2|"`  $NewLink 
                             else 
                                   /bin/mkdir -p   $NewLink 
                             endif 
                        endif 
               end 
 
 
        endif 
 
end 
/bin/mkdir -p $SCRATCH/./lib/
foreach object ( `find ./lib/ ` ) 
 
 
          if($object != './lib/') then 
 
                 foreach DIRObject ( `find $object`) 
 
                        setenv  NewLink  $SCRATCH/$DIRObject 
 
                        if(! -e $NewLink) then 
                             if(-f $DIRObject) then 
                                   /bin/ln -s  `echo $DIRObject | /bin/sed "s|\(^[^/]\)\(.*\)|$pwd/\1\2|"`  $NewLink 
                             else 
                                   /bin/mkdir -p   $NewLink 
                             endif 
                        endif 
               end 
 
 
        endif 
 
end 


# Used for cleanning up the sandbox later
/bin/ls $SCRATCH > ${SCRATCH}/.sandboxFiles
echo ".sandboxFiles" >> ${SCRATCH}/.sandboxFiles



#Note: The default directory in which jobs start has been fix to $SCRATCH
cd $SCRATCH

#--------------- Copy input to SCRATCH --------------------

setenv LocalInputFileDir ${SCRATCH}/INPUTFILES
mkdir ${LocalInputFileDir}

setenv NewFILELIST ${LocalInputFileDir}/${JOBID}.local.list
touch $NewFILELIST

set XRootCopySuccessCount=0
set XRootCopyCount=0
set FileIndex=0
foreach RemoteFile ( `cat ${FILELIST} | awk '{printf($1"\n")}'` )
        set RemoteFileWithoutEvents=${RemoteFile}
        set RemoteFile=`cat ${FILELIST} | grep ${RemoteFile}`
        set nEvents=`echo ${RemoteFile} | awk '{printf($2"\n")}'`
        set isRootFile=`echo ${RemoteFile} | grep -e "^[Rr]oot" | wc -l`
        set isS3File=`echo ${RemoteFile} | grep -e "^[Ss]3:" | wc -l`

        if(${isS3File}) then
                   set LocalFile=${RemoteFile}
        else if(${isRootFile}) then
                  @ XRootCopyCount++
                  set LocalFile=${LocalInputFileDir}/`echo ${RemoteFileWithoutEvents} | sed 's|\(.*/\)\([^\]*\)$|\2|g'`
                  echo running: xrdcp --retry 3 ${RemoteFileWithoutEvents} ${LocalFile} \>\& /dev/null
                  xrdcp --retry 3 ${RemoteFileWithoutEvents} ${LocalFile} >& /dev/null
                  echo "xrdcp copy done: `/bin/date`"
                  if( -e ${LocalFile} ) then
                        @ XRootCopySuccessCount++
                        echo "${LocalFile} ${nEvents}" >> $NewFILELIST
                  else
                        set LocalFile="${RemoteFile}"
                        echo ${LocalFile} >> $NewFILELIST
                  endif
        else
                set LocalFile=${LocalInputFileDir}/`echo ${RemoteFileWithoutEvents} | sed 's|\(.*/\)\([^\]*\)$|\2|g'`
                echo Running: cp ${RemoteFileWithoutEvents}  ${LocalFile}
                cp ${RemoteFileWithoutEvents}  ${LocalFile}
                if( -e ${LocalFile} ) then
                        echo "${LocalFile} ${nEvents}" >> $NewFILELIST
                else
                        echo ${RemoteFile} >> $NewFILELIST
                        set LocalFile="${RemoteFile}"
                endif
        endif

        set LocalFileWithoutEvents=`echo ${LocalFile} | awk '{printf($1"\n")}'`
        setenv INPUTFILE${FileIndex} ${LocalFileWithoutEvents}
        @ FileIndex++
end

echo Swapping \$FILELIST with: ${NewFILELIST}
setenv FILELIST ${NewFILELIST}

if( ${XRootCopyCount} != 0 ) then
        set XRootCopySuccessPercent=`echo "${XRootCopySuccessCount} * 100 / ${XRootCopyCount}" | bc`
        echo "Copied ${XRootCopyCount} files from Xrootd, ${XRootCopySuccessCount} succeeded (${XRootCopySuccessPercent} %)"
endif


echo +++++++++++local copy debug++++++++++++++++++++
echo newFileList: ${FILELIST}
echo ++++++++++++++++cat FILELIST+++++++++++++++++++
cat ${FILELIST}
echo ++++++++++++++++++++find.++++++++++++++++++++++
find .
echo ++++++++++++++++INPUTFILE Var++++++++++++++++++
printenv | grep INPUTFILE
echo +++++++++++++++++++++++++++++++++++++++++++++++

#----------------- End of input copy ----------------------



echo "--------------------------------"
echo 'Simple Unified Meta Scheduler 1.10.50 we are starting in $SCRATCH :' `/bin/pwd`
echo "--------------------------------"
/bin/ls -l

###################################################
# User command BEGIN ----------------------------->

    stardev
	root -b -q check_S.C++\(\"$FILELIST\",\"$JOBID\",1\)
    
# <------------------------------User command END
###################################################

#Sandbox cleanup script, so user will not copy back sandbox files by using *.*
foreach SANDBOXFILE (`/bin/cat ${SCRATCH}/.sandboxFiles`)
   echo "Cleaning up sandbox file:  ${SCRATCH}/${SANDBOXFILE}"
   /bin/rm -fr ${SCRATCH}/${SANDBOXFILE}
end


# Copy output files (if any where specified)
    
    ########### Copy Command Block ########### 
         
    rm -f $SCRATCH/count.$JOBID.test            
    (/bin/ls $SCRATCH/*.root > $SCRATCH/count.$JOBID.test) >& /dev/null
    if ( -z $SCRATCH/count.$JOBID.test ) then               
        echo 'Error: Scheduler could not find any files in $SCRATCH matching your <output> tag fromScratch value: *.root '
    else                      
        echo "Starting copy block"
        foreach file ( $SCRATCH/*.root )
             set i=0

             if ( -d $file ) then
                set cpCommand="/bin/cp -r $file /star/data01/pwg/xiatong/git/strangeness/data/"
             else
                set cpCommand="/bin/cp -fp $file /star/data01/pwg/xiatong/git/strangeness/data/"
             endif

             RETRY_CP:
             echo "Running copy command: " $cpCommand
             $cpCommand
             if ( $status && $i < 15 ) then
                 echo "The copy failed, waiting to try again."
                 @ i++
                 sleep 60
                 echo "Trying copy again at "`/bin/date`
                 goto RETRY_CP
             endif
        end
        
    endif
    ##########################################
            
