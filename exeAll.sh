#!/usr/bin/env bash

nap()
{
    maxMarlinProc=$1

    marlinProc=`ps -U $USER | grep "Marlin" | wc -l`

    while [ $marlinProc -ge $maxMarlinProc ]; do
        #echo "Marlin processes #: $marlinProc, max: $maxMarlinProc ---> sleeping... "
        sleep 5
        marlinProc=`ps -U $USER | grep "Marlin" | wc -l`
    done
}

rm -f rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds*

numFiles=$1   # input file number

nInputFiles=10

if [ ${#numFiles} != 0 ]; then
   if [ ${numFiles} -gt 0 -a ${numFiles} -le 10 ]; then
      nInputFiles=${numFiles}
   fi
fi

echo "File number:" ${nInputFiles}

# runArborTest.sh 1 1                # single job mode, with mc truth PFA
./runArborTest.sh 0 0 ${nInputFiles} # muti job mode, PFA, file number

# wait until Marlin is finished
nap 1

./fileCombine.sh ${nInputFiles}
./plotJER
