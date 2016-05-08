all: clean compile
compileRunLength: compileMyIoBitStream
	g++ -g -c -I./MyIOBitStream/include/ -I./RunLength/include/ -o ./bin/runLength.o ./RunLength/src/runLength.cpp
compileHuffman: compileMyIoBitStream
	g++ -g -c -I./MyIOBitStream/include/ -o ./bin/HuffmanCoding.o ./HuffmanCoding/src/huffman.cpp
compile: compileEncode compileDecode 
compileEncode:	compileRunLength compileHuffman
	g++ -g -I./RunLength/include/ -o ./bin/encode ./src/encode.cpp ./bin/runLength.o
compileDecode:	compileRunLength
	g++ -g -I./RunLength/include/ -o ./bin/decode ./src/decode.cpp ./bin/runLength.o
compileMyIoBitStream:
	g++ -g -c -I./MyIOBitStream/include/ -o ./bin/myIoBitStream.o ./MyIOBitStream/src/myIoBitStream.cpp
clean:
	rm -f ./bin/*
tests: all
	g++ -g -I./MyIOBitStream/include/ -I./RunLength/include/ -o ./bin/testBitStream ./src/testBitStream.cpp ./bin/runLength.o ./bin/myIoBitStream.o
