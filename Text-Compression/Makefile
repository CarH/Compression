all: clean compile
compileRunLength: compileMyIoBitStream
	g++ -g -c -I./MyIOBitStream/include/ -I./RunLength/include/ -o ./bin/runLength.o ./RunLength/src/runLength.cpp
compile: compileEncode compileDecode
compileEncode:	compileRunLength compileBWT compileCompressionConfig compileHuffman
	g++ -g -I./CompressionConfig/include/ -I./MyIOBitStream/include/ -I./BWTransform/include/ -I./RunLength/include/ -I./HuffmanCoding/include/ -o ./bin/encode ./src/encode.cpp ./bin/runLength.o ./bin/huffman.o ./bin/myIoBitStream.o ./bin/CompressionConfig.o ./bin/BWTransform.o
compileDecode:	compileRunLength compileBWT compileCompressionConfig compileHuffman
	g++ -g -I./CompressionConfig/include/ -I./BWTransform/include/ -I./RunLength/include/ -I./HuffmanCoding/include/ -o ./bin/decode ./src/decode.cpp ./bin/runLength.o ./bin/huffman.o ./bin/myIoBitStream.o ./bin/CompressionConfig.o ./bin/BWTransform.o
compileMyIoBitStream:
	g++ -g -c -I./CompressionConfig/include/ -I./MyIOBitStream/include/ -o ./bin/myIoBitStream.o ./MyIOBitStream/src/myIoBitStream.cpp
compileBWT:
	g++ -g -c -I./BWTransform/include/ -o ./bin/BWTransform.o ./BWTransform/src/BWTransform.cpp
compileCompressionConfig: compileHuffman
	g++ -g -c -I./CompressionConfig/include/ -I./HuffmanCoding/include/ -o ./bin/CompressionConfig.o ./bin/huffman.o ./CompressionConfig/src/CompressionConfig.cpp
compileHuffman:	compileMyIoBitStream 
	g++ -g -c -I./HuffmanCoding/include/ -I./MyIOBitStream/include/ -o ./bin/huffman.o ./HuffmanCoding/src/huffman.cpp
clean:
	rm -f -r ./bin
	mkdir ./bin
tests: 
	rm outCoded.bin resultDecoded.txt
	./bin/encode -i in.in -o outCoded.bin --txtblck=1024 --bwt=false --runl=false --huffman=true
	./bin/decode -i outCoded.bin -o resultDecoded.txt