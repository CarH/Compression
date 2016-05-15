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
	rm -f outCoded* resultDecoded*
	./bin/encode -i in.in -o outCodedHuff.bin --txtblck=1024 --bwt=false --runl=false --huffman=true
	./bin/encode -i in.in -o outCodedRL.bin --txtblck=1024 --bwt=false --runl=true --huffman=false
	./bin/encode -i in.in -o outCodedBTW1024.bin --txtblck=1024 --bwt=true --runl=false --huffman=false
	./bin/encode -i in.in -o outCodedHuffRL.bin --txtblck=1024 --bwt=false --runl=true --huffman=true
	./bin/encode -i in.in -o outCodedHuffBTW1024.bin --txtblck=1024 --bwt=true --runl=false --huffman=true
	./bin/encode -i in.in -o outCodedHuffRLBTW1024.bin --txtblck=1024 --bwt=true --runl=true --huffman=true
	./bin/encode -i in.in -o outCodedRLBTW1024.bin --txtblck=1024 --bwt=true --runl=true --huffman=false
	./bin/decode -i outCodedHuff.bin -o resultDecodedHuff.txt
	./bin/decode -i outCodedRL.bin -o resultDecodedRL.txt
	./bin/decode -i outCodedBTW1024.bin -o resultDecodedBTW1024.txt
	./bin/decode -i outCodedHuffRL.bin -o resultDecodedHuffRL.txt
	./bin/decode -i outCodedHuffBTW1024.bin -o resultDecodedHuffBTW1024.txt
	./bin/decode -i outCodedHuffRLBTW1024.bin -o resultDecodedHuffRLBTW1024.txt
	./bin/decode -i outCodedRLBTW1024.bin -o resultDecodedRLBTW1024.txt
	diff resultDecodedHuff.txt in.in
	diff resultDecodedRL.txt in.in
	diff resultDecodedBTW1024.txt in.in
	diff resultDecodedHuffRL.txt in.in
	diff resultDecodedHuffBTW1024.txt in.in
	diff resultDecodedHuffRLBTW1024.txt in.in
	diff resultDecodedRLBTW1024.txt in.in