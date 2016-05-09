all: clean compile tests

compile: compileHuffman
	g++ ./bin/huffman.o ./bin/myIoBitStream.o -o p
compileHuffman:	compileMyIoBitStream 
	g++ -g -c -I./MyIOBitStream/include/ -o ./bin/huffman.o ./HuffmanCoding/src/huffman.cpp
compileMyIoBitStream:
	g++ -g -c -I./MyIOBitStream/include/ -o ./bin/myIoBitStream.o ./MyIOBitStream/src/myIoBitStream.cpp
clean:
	rm -rf ./bin p outHuffmanCoded.bin
	mkdir bin
tests:
	./p ./HuffmanCoding/src/in.in outHuffmanCoded.bin