#!/bin/bash


database=${1}

bin="../bin/decode"
destinationFolder=${2}

outNameSuffix="";


app_flags_final=""


# if [ "$RUNL_FIRST" = true ]:
# then
# 	app_flags_final="$app_flags_final $app_flags_runl_first_true"
# 	outNameSuffix=$outNameSuffix"_runlFirst"
# else
# 	app_flags_final="$app_flags_final $app_flags_runl_first_false"
# fi

outNameSuffix=".decoded"

LAST_PART=$(basename "$database" "")


outName=$destinationFolder$LAST_PART$outNameSuffix
app_flags_final="$app_flags_final -i $database -o $outName"




AWEEE="$bin $app_flags_final"

mkdir -p $destinationFolder
echo time $AWEEE 

START=`date +%s`

$AWEEE 
# sleep 2
END=`date +%s`
ELAPSED=$(( ($END - $START) ))


timesName="timesDecoded.txt"

echo $outName "\t" `awk "BEGIN {print($ELAPSED/60)}"` >> $destinationFolder$timesName