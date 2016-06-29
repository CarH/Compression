# BTW, Huffman e Run-length isolados
# Huffman + Run Lenght (estudar melhor ordem)
# BTW =>Huffman
# BTW => RunLength
# BTW => RunLength => Huffman  (Estudar melhor ordem)
# 
# ordem bwt huff runlength l databse
# #tamanho dos blocos: 100,500,1000,10000

BASESDIR=${1}
AUX="resultsDecoded/"
DEST_FOLDER=$BASESDIR$AUX
AUX2="results/"
BASESDIR_MOD=$BASESDIR$AUX2

echo $DEST_FOLDER

for values in $BASESDIR_MOD*.out;
do 
	if [ -d "${values}" ] ; then
		continue;
	fi
	sh ./decode.sh $values $DEST_FOLDER
done