#!/usr/bin/env bash

source ArborPFASetup.sh

#------------------------------------------------------------------------
numFiles=$1 # number of files of each energy

if [ ${#numFiles} = 0 ]; then
   nFiles=5
else
   if [ ${numFiles} -gt 0 -a ${numFiles} -le 10 ]; then
      nFiles=${numFiles}
   fi
fi

for (( iFile=0; ${iFile}<${nFiles}; iFile=${iFile}+1 )); do
	FileIndex+=(${iFile})
done
#---------------------------------
Energy=(91)
StartingEvent=(0 200 400 600 800)
#------------------------------------------------------------------------

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
    hadd -f "rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds${energy}_AIDA.root" ${fileListAIDA}
done 

####################
