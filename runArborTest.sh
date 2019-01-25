#!/usr/bin/env bash

source /media/libo/HD1/workplace/ArborPFA/test/ArborPFASetup.sh

#Marlin ILDConfig/StandardConfig/production/SteeringFile_ILD_l5_o2_v02/MarlinStdReco_ILD_l5_o2_v02_SD_PFA.xml \

run()
{
   energy=$1
   fileIndex=$2
   skipnum=$3
   maxnum=$4
   
   fileDir="/projects/samples/ILD_l5_o2_v02"
   fileName="rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds${energy}.n0"${fileIndex}".d_rec_u035.slcio"
   inputFile=${fileDir}/${fileName}

   Marlin /home/libo/workplace/ArborPFA_newDev/test/ILDConfig/StandardConfig/production/SteeringFile_ILD_l5_o2_v02/MarlinStdReco_ILD_l5_o2_v02_SD_TEST.xml \
   --constant.DetectorModel="ILD_l5_o2_v02" \
   --constant.OutputBaseName=${fileName}\_${skipnum} \
   --constant.SDHcalEnergyFactors="0.0367023 0.0745279 0.363042" \
   --constant.PandoraEcalToHadBarrelScale="1.28848786128075" \
   --constant.PandoraEcalToHadEndcapScale="1.28848786128075" \
   --constant.PandoraHcalToHadScale="0.96566858822654" \
   --constant.ArborPFAOutputFile="${fileName}_ArborPerfectPFA_${skipnum}.root" \
   --global.LCIOInputFiles=${inputFile} \
   --global.SkipNEvents=${skipnum} \
   --global.MaxRecordNumber=${maxnum}
}

runBatch()
{
   energy=$1
   fileIndex=$2
   skipnum=$3
   maxnum=$4
   
   fileDir="/projects/samples/ILD_l5_o2_v02"
   fileName="rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds${energy}.n0"${fileIndex}".d_rec_u035.slcio"
   inputFile=${fileDir}/${fileName}

   Marlin /home/libo/workplace/ArborPFA_newDev/test/ILDConfig/StandardConfig/production/SteeringFile_ILD_l5_o2_v02/MarlinStdReco_ILD_l5_o2_v02_SD_TEST.xml \
   --constant.DetectorModel="ILD_l5_o2_v02" \
   --constant.OutputBaseName=${fileName}\_${skipnum} \
   --constant.SDHcalEnergyFactors="0.0367023 0.0745279 0.363042" \
   --constant.PandoraEcalToHadBarrelScale="1.28848786128075" \
   --constant.PandoraEcalToHadEndcapScale="1.28848786128075" \
   --constant.PandoraHcalToHadScale="0.96566858822654" \
   --constant.ArborPFAOutputFile="${fileName}_ArborPerfectPFA_${skipnum}.root" \
   --global.LCIOInputFiles=${inputFile} \
   --global.SkipNEvents=${skipnum} \
   --global.MaxRecordNumber=${maxnum} > ${fileName}\_${skipnum}.log 2>&1 &
}

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

setSteeringFile()
{
	fileType=$1
	
	fileDir="/media/libo/HD1/workplace/ArborPFA/test/Steering"
	fileTarget="arborSetting.xml"

	fileOrigin="arborSetting_photonClustering.xml"

	if [ ${fileType} = "perfect" ]; then
		fileOrigin="arborSetting_PefectPFA.xml"
	fi

	cp ${fileDir}/${fileOrigin} ${fileDir}/${fileTarget}
}

setEvetDisplayInSteeringFile()
{
	displayEvent=$1 # true or false

    #echo ${displayEvent}
    steeringFile="/media/libo/HD1/workplace/ArborPFA/test/Steering/arborSetting.xml"
    sed -i "s/<IsMonitoringEnabled>.*<\/IsMonitoringEnabled>/<IsMonitoringEnabled>${displayEvent}<\/IsMonitoringEnabled>/g" ${steeringFile}
}

#----------------------------------------------
# run: ./runArborTest.sh [setup=0] [mcMode=0]
#---------------------------------------------

setup=$1    # run mode setup
mcMode=$2   # PFA or MC truth PFA

if [ ${#setup} = 0 ]; then
   setup=0
   #echo "setup 0"
else
   if [ ${setup} = "1" ]; then
      setup=1
      #echo "setup 1"
   else
      setup=0
      #echo "setup 0"
   fi
fi

if [ ${setup} = 0 ]; then
   Energy=(91)
   FileIndex=(0 1 2 3 4)
   #FileIndex=(0 1 2 3 4 5 6 7 8 9)
   StartingEvent=(0 200 400 600 800)
   maxRecordNumber=200
else
   Energy=(91)
   FileIndex=(0)           
   StartingEvent=0
   maxRecordNumber=1
fi
########################################

nEnergy=${#Energy[@]}
nFiles=${#FileIndex[@]}

multiJob=1
if [ ${nEnergy} -eq 1 -a ${nFiles} -eq 1 ]; then
   multiJob=0
fi

#echo "multiJob="${multiJob}

if [ ${mcMode} = "1" ]; then
   setSteeringFile perfect
else
   setSteeringFile
fi

if [ ${multiJob} -eq 0 ]; then
   setEvetDisplayInSteeringFile true
   runCmd=run
else
   setEvetDisplayInSteeringFile false
   runCmd=runBatch
fi

echo "run mode : "${runCmd}

#######################################################################
for en in ${Energy[@]}; do
    for index in ${FileIndex[@]}; do
         for skip in ${StartingEvent[@]}; do
             $runCmd ${en} ${index} ${skip} ${maxRecordNumber}
             nap 25
		 done
     done
done
#######################################################################
