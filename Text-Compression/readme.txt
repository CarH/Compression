									  PROJETO PARTE 1
									COMPRESSÃO DE TEXTO
									MULTIMÍDIA (SCC0261)
										1o Sem/2016
	________________________________________________________________________entrega:_15/05/2015_____________
	
	- ALGORITMOS IMPLEMENTADOS
		. Transformada de Burrows-Wheeler (BWT) com otimização espacial (sugerida na bibliografia)
		. Run Length
		. Huffman

	- GRUPO
		ALUNO: Carlos Humberto S. Baqueta	NUSP.: 7987456
		ALUNO: Eric Freire Antunes			NUSP.: 
		
	- Instruções (para Terminal de um sistema linux)
	 	Para compilar: 
	 		$ make

	 	Para codificar:
	 		$ encode -i arquivo_original.txt -o arquivo_binario.bin --bwt=X --txtblck=X --huffman=X –-runl=X
	 	
	 	Para decodificar:
	 		$ decode -i arquivo_binario.bin -o arquivo_descomprimido.txt