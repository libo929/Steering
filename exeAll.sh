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

# runArborTest.sh 1 1 # single job mode, with mc truth PFA
./runArborTest.sh

# wait until Marlin is finished
nap 1

./fileCombine.sh
./plotAll2
