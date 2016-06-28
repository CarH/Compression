#!/bin/bash


BWT=${1}
HUFF=${2}
RUNL=${3}
L=${4}
database=${5}

bin="../bin/encode"
destinationFolder=${6}

outNameSuffix="";

app_flags_enable_bwt_100="--bwt=true --txtblck=100"
app_flags_enable_bwt_200="--bwt=true --txtblck=200"
app_flags_enable_bwt_500="--bwt=true --txtblck=500"
app_flags_enable_bwt_1000="--bwt=true --txtblck=1000"
app_flags_enable_bwt_10000="--bwt=true --txtblck=10000"
app_flags_disable_bwt="--bwt=false --txtblck=1"

app_flags_enable_huffman="--huffman=true"
app_flags_disable_huffman="--huffman=false"

app_flags_enable_runl="--runl=true"
app_flags_disable_runl="--runl=false"

app_flags_runl_first_true="--runlFirst=true"
app_flags_runl_first_false="--runlFirst=false"

app_flags_final=""







if [ "$BWT" = false ];
then
	# echo OI
	app_flags_final="$app_flags_final $app_flags_disable_bwt"
else
	# echo TCHAU
	case "$L" in
	   100) app_flags_final="$app_flags_final $app_flags_enable_bwt_100"
			outNameSuffix=$outNameSuffix"_bwt_100"
	   ;;
	   200) app_flags_final="$app_flags_final $app_flags_enable_bwt_200"
			outNameSuffix=$outNameSuffix"_bwt_200"
	   ;;
	   500) app_flags_final="$app_flags_final $app_flags_enable_bwt_500"
			outNameSuffix=$outNameSuffix"_bwt_500"
	   ;;
	   1000) app_flags_final="$app_flags_final $app_flags_enable_bwt_1000"
			outNameSuffix=$outNameSuffix"_bwt_1000"
	   ;;
	   10000) app_flags_final="$app_flags_final $app_flags_enable_bwt_1000"
			outNameSuffix=$outNameSuffix"_bwt_10000"
	   ;;
	esac
fi

if [ "$HUFF" = true ]:
then
	app_flags_final="$app_flags_final $app_flags_enable_huffman"
	outNameSuffix=$outNameSuffix"_huff"
else
	app_flags_final="$app_flags_final $app_flags_disable_huffman"
fi

if [ "$RUNL" = true ]:
then
	app_flags_final="$app_flags_final $app_flags_enable_runl"
	outNameSuffix=$outNameSuffix"_runl"
else
	app_flags_final="$app_flags_final $app_flags_disable_runl"
fi

# if [ "$RUNL_FIRST" = true ]:
# then
# 	app_flags_final="$app_flags_final $app_flags_runl_first_true"
# 	outNameSuffix=$outNameSuffix"_runlFirst"
# else
# 	app_flags_final="$app_flags_final $app_flags_runl_first_false"
# fi

outNameSuffix="$outNameSuffix.out"

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


timesName="times.txt"

echo $outName "\t" `awk "BEGIN {print($ELAPSED/60)}"` >> $destinationFolder$timesName