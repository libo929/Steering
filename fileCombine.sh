#!/usr/bin/env bash

#source /cvmfs/ilc.desy.de/sw/x86_64_gcc49_sl6/v01-19-05/init_ilcsoft.sh

####################

#for energy in 120 160 200 240 30 300 350 40 400 500 60 91; do
#for energy in 200 350 500 ; do

Energy=(91)
FileIndex=(0 1 2 3 4)
#FileIndex=(0 1 2 3 4 5 6 7 8 9)

StartingEvent=(0 200 400 600 800)

for energy in ${Energy[@]}; do
    fileListANA=""
    fileListAIDA=""

    for index in ${FileIndex[@]}; do
         for skip in ${StartingEvent[@]}; do
	
		     fileNameANA="rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds${energy}.n0${index}.d_rec_u035.slcio_ArborPerfectPFA_${skip}.root"
		     fileNameAIDA="rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds${energy}.n0${index}.d_rec_u035.slcio_${skip}_AIDA.root"

             #ls -l $fileNameANA
             #ls -l $fileNameAIDA

             fileListANA+=" "${fileNameANA}
             fileListAIDA+=" "${fileNameAIDA}
		 done
	done

    #echo ${fileListANA}
    hadd -f "rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds${energy}.root" ${fileListANA}

    #echo ${fileListAIDA}
    #hadd -f "rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds${energy}_AIDA.root" ${fileListAIDA}
done 

####################
