#!/usr/bin/env bash

if [ -z ${MARLIN_DLL} ]; then
   source /media/libo/HD1/workplace/ArborPFA/test/init_ilcsoft.sh
fi

export LD_BIND_NOW=1

#unset MARLIN_DLL
# set shared library paths of mlpack
export LD_LIBRARY_PATH="/projects/soft/mlpack-3.0.3/lib":$LD_LIBRARY_PATH
export LD_LIBRARY_PATH="/usr/local/cuda/lib64:/usr/share/man/man7":$LD_LIBRARY_PATH

MyArborPFACode=/home/libo/workplace/ArborPFA/code
DDMarlinArbor=${MyArborPFACode}/DDMarlinArbor
ArborContent=${MyArborPFACode}/ArborContent

export MARLIN_DLL="/projects/ilcsoft/v01-19-05/v01-19-05/MarlinDD4hep/v00-05/lib/libMarlinDD4hep.so:/projects/ilcsoft/v01-19-05/v01-19-05/MarlinReco/v01-22/lib/libMarlinReco.so:/projects/ilcsoft/v01-19-05/v01-19-05/PandoraAnalysis/v02-00-00/lib/libPandoraAnalysis.so:/projects/ilcsoft/v01-19-05/v01-19-05/LCFIVertex/v00-07-04/lib/libLCFIVertexProcessors.so:/projects/ilcsoft/v01-19-05/v01-19-05/CEDViewer/v01-15/lib/libCEDViewer.so:/projects/ilcsoft/v01-19-05/v01-19-05/Overlay/v00-20/lib/libOverlay.so:/projects/ilcsoft/v01-19-05/v01-19-05/LCTuple/v01-10/lib/libLCTuple.so:/projects/ilcsoft/v01-19-05/v01-19-05/MarlinKinfit/v00-05/lib/libMarlinKinfit.so:/projects/ilcsoft/v01-19-05/v01-19-05/MarlinTrkProcessors/v02-09-01/lib/libMarlinTrkProcessors.so:/projects/ilcsoft/v01-19-05/v01-19-05/MarlinKinfitProcessors/v00-03/lib/libMarlinKinfitProcessors.so:/projects/ilcsoft/v01-19-05/v01-19-05/ILDPerformance/v01-04/lib/libILDPerformance.so:/projects/ilcsoft/v01-19-05/v01-19-05/Clupatra/v01-02/lib/libClupatra.so:/projects/ilcsoft/v01-19-05/v01-19-05/Physsim/v00-04/lib/libPhyssim.so:/projects/ilcsoft/v01-19-05/v01-19-05/LCFIPlus/v00-06-07/lib/libLCFIPlus.so:/projects/ilcsoft/v01-19-05/v01-19-05/FCalClusterer/v00-06/lib/libFCalClusterer.so:/projects/ilcsoft/v01-19-05/v01-19-05/ForwardTracking/v01-13/lib/libForwardTracking.so:/projects/ilcsoft/v01-19-05/v01-19-05/ConformalTracking/v01-04/lib/libConformalTracking.so:/projects/ilcsoft/v01-19-05/v01-19-05/LICH/v00-01/lib/libLICH.so:/projects/ilcsoft/v01-19-05/v01-19-05/Garlic/v3.0.4/lib/libGarlic.so"

export MARLIN_DLL=${DDMarlinArbor}/lib/libDDMarlinArbor.so:${MARLIN_DLL}
