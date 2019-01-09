#!/usr/bin/env bash

curLinkFile=`ls -l arborSetting.xml | awk -F '->' '{print $2}'`

echo $curLinkFile

rm -f arborSetting.xml

if [ $curLinkFile = "arborSetting_photonClustering.xml" ]; then
	ln -s arborSetting_PefectPFA.xml arborSetting.xml
else
	ln -s arborSetting_photonClustering.xml arborSetting.xml
fi

