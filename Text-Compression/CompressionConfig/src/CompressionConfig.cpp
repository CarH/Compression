#include "CompressionConfig.hpp"
#include "huffman.hpp"
#include <cstdlib>
#include <cstdio>
using namespace std;

BWTConfig::BWTConfig(){
	this->blockSize=-1;
}
BWTConfig::~BWTConfig(){

}

RunLengthConfig::RunLengthConfig(){
	this->maxBits=-1;
}
RunLengthConfig::~RunLengthConfig(){

}

HuffmanConfig::HuffmanConfig(){
	this->isValid=false;
}
HuffmanConfig::~HuffmanConfig(){

}

//==============CompressionConfig

	//======Main
void CompressionConfig::parseHeader(istream &inFile){
	char encodingType;
	char nullChar = 1;
	inFile>>noskipws;
	inFile.read(&encodingType,sizeof(char));
	while(encodingType!=0){ //le os identificadores de algoritmos ate achar o fim 
		switch (encodingType){
			case 'B':{
				this->parseHeaderBWT(inFile,&((this->bwtConfig).blockSize),bwtConfig.aVector);
				break;
			}
			case 'R':{
				this->parseHeaderRunLength(inFile,&((this->rlConfig).maxBits));
				break;
			}
			case 'H':{
				huffConfig.isValid = true;
				this->parseHeaderHuffman(inFile);
				break;
			}
			default:{
				cerr<<"ERROR: Unexpected Algorithm In Header: "<<encodingType<<endl;
				exit(EXIT_FAILURE);
			}

		}
		
		inFile.read(&encodingType,sizeof(char));
	}

}
void CompressionConfig::writeHeader(ostream &outFile){
	char nullChar = 0;
	//verifica as variaveis de configuracao para saber qual algoritmo foi usado, chamando
	//o respectivo headerWriter
	if(this->bwtConfig.blockSize>0){//if there is a BWT config set in this object
		this->writeHeaderBWT(outFile,this->bwtConfig.blockSize,this->bwtConfig.aVector);
	}
	if(this->rlConfig.maxBits>=0){
		this->writeHeaderRunLength(outFile,this->rlConfig.maxBits);
	}
	if(this->huffConfig.isValid){
		this->writeHeaderHuffman(outFile);
	}
	//padronizou-se que o header finalizar sempre com um byte 0;
	outFile.write(&nullChar,sizeof(char));
}


//== BWT
void CompressionConfig::writeHeaderBWT(ostream &outFile,int blockSize,vector<int> &aVector){
	outFile<<'B';
	
	int *aArray =(int*) malloc(sizeof(int)*aVector.size());
	int aSize = aVector.size();
	
	for(int i=0;i<aSize;i++){
		aArray[i]=aVector[i];//copies the aVector to an Array in order to write ir properly in a file
	}
	outFile.write((char*)&blockSize,sizeof(int));
	outFile.write((char*)&aSize,sizeof(int));//stores the size of aVector
	outFile.write((char*)aArray,sizeof(int)*aSize);//stores the aValues
	free(aArray);
}

void CompressionConfig::parseHeaderBWT(istream &inFile,int *blockSize,vector<int> &aVector){
	
	int aSize;
	int *aArray ;
	int localBlockSize;


	aVector.clear();
	inFile.read((char*)&localBlockSize,sizeof(int));
	*blockSize=localBlockSize;
	inFile.read((char*)&aSize,sizeof(int));
	aArray =(int*) malloc(sizeof(int)*aSize);
	inFile.read((char*)aArray,sizeof(int)*aSize);
	for(int i=0;i<aSize;i++){
		//read the values of A from file
		aVector.push_back(aArray[i]);
	}

	free(aArray);
	
	
}
//==RUN LENGTH

void CompressionConfig::parseHeaderRunLength(istream &inFile,long long int *maxBits){
	
	long long int localMaxBits;
	inFile>>noskipws;
	inFile.read((char*)&localMaxBits,sizeof(long long int));
	*maxBits=localMaxBits;
	
}

void CompressionConfig::writeHeaderRunLength(ostream &outFile,long long int maxBits){
	outFile<<'R';
	outFile.write((char*)&maxBits,sizeof(long long int));
}


//================= HUFFMAN
void CompressionConfig::writeHeaderHuffman(ostream &outFile){
	// Write the header
	writeHuffmanHeader(outFile, huffConfig.char_freq, huffConfig.validBitsLastByte);
}
void CompressionConfig::parseHeaderHuffman(istream &inFile){
	huffConfig.char_freq = readHuffmanHeader(inFile, &huffConfig.validBitsLastByte);
	buildHuffmanTree(huffConfig.char_freq);
}
