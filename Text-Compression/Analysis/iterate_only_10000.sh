# BTW, Huffman e Run-length isolados
# Huffman + Run Lenght (estudar melhor ordem)
# BTW =>Huffman
# BTW => RunLength
# BTW => RunLength => Huffman  (Estudar melhor ordem)
# 
# ordem bwt huff runlength l databse
# #tamanho dos blocos: 100,500,1000,10000

BASESDIR=${1}
AUX="results/"
DEST_FOLDER=$BASESDIR$AUX

echo $DEST_FOLDER

for values in $BASESDIR*;
do 
	if [ -d "${values}" ] ; then
		continue;
	fi
# Cada um Isolado
	# BWT
	# sh ./exec.sh true false false 100 $values $DEST_FOLDER
	# sh ./exec.sh true false false 500 $values $DEST_FOLDER
	# sh ./exec.sh true false false 1000 $values $DEST_FOLDER
	sh ./exec.sh true false false 10000 $values $DEST_FOLDER

	# Huff
	# sh ./exec.sh false true false 1 $values $DEST_FOLDER

	# RunL
	# sh ./exec.sh false false true 1 $values $DEST_FOLDER

#Huff e Runl
	# sh ./exec.sh false true true 1 $values $DEST_FOLDER
# BWT => Huff
	# sh ./exec.sh true true false 100 $values $DEST_FOLDER
	# sh ./exec.sh true true false 500 $values $DEST_FOLDER
	# sh ./exec.sh true true false 1000 $values $DEST_FOLDER
	sh ./exec.sh true true false 10000 $values $DEST_FOLDER
# BWT => RunL
	# sh ./exec.sh true false true 100 $values $DEST_FOLDER
	# sh ./exec.sh true false true 500 $values $DEST_FOLDER
	# sh ./exec.sh true false true 1000 $values $DEST_FOLDER
	sh ./exec.sh true false true 10000 $values $DEST_FOLDER
#Todos Juntos
	# sh ./exec.sh true true true 100 $values $DEST_FOLDER
	# sh ./exec.sh true true true 500 $values $DEST_FOLDER
	# sh ./exec.sh true true true 1000 $values $DEST_FOLDER
	sh ./exec.sh true true true 10000 $values $DEST_FOLDER
done