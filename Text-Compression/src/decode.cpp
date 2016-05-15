#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "huffman.hpp"
#include "runLength.hpp"
#include "BWTransform.hpp"
#include "CompressionConfig.hpp"


using namespace std;

#define BUFFER_SIZE 4096
/*
	Funcoes e logicas analogas as presentes em encode.cpp. Para maiores detalhes,
	favor consultar os comentarios em encode.cpp, assim como a documentacao de CompressionConfig
 */

void mainDecodeBWT(istream &inFile,ostream &outFile,BWTConfig &config);
void mainDecodeRunLength(iostream &inFile,ostream &outFile,RunLengthConfig &config,bool binary);
void mainDecodeHuffman(iostream &inFile,ostream &outFile,HuffmanConfig &config);
bool parseArgs(int argc,const char *argv[],string &inFileName,string &outFileName);

int main(int argc, char const *argv[])
{
	
	char bufferIn[BUFFER_SIZE];
	char bufferOut[BUFFER_SIZE];
	string filenameIn;
	string filenameOut;
	fstream inFile;
	fstream outFile;
	string encodedString;
	stringstream outStrStream;
	CompressionConfig compConfig;


	if(!parseArgs(argc,argv,filenameIn,filenameOut)){
		cerr<<"usage: encode -i <file_name> -o <out_file>"<<endl;
		exit(EXIT_FAILURE);
	}
	// filenameIn=argv[1]; 
	// filenameOut=argv[2]; 

	// inFile.rdbuf()->pubsetbuf(bufferIn,BUFFER_SIZE);
	// outFile.rdbuf()->pubsetbuf(bufferOut,BUFFER_SIZE);

	inFile.open(filenameIn.c_str(),ios_base::in);
	if(!inFile.is_open()){
		cerr<<"ERROR: Input File not Found: "<<filenameIn<<endl;
		exit(EXIT_FAILURE);
	}

	outFile.open(filenameOut.c_str(),ios_base::out | ios_base::binary );
	if(!outFile.is_open()){
		cerr<<"ERROR: Output File could not be created: "<<filenameOut<<endl;
		exit(EXIT_FAILURE);
	}
	cout<<"Decoding"<<endl;
	//Realiza a verificacao do Header e seta adequadamente os parametros em compConfig
	compConfig.parseHeader(inFile);
	/*
		Verifica quais algoritmos foram lidos do header. Para isso, verifica as configuracoes em 
		compConfig
			Se BWT foi detectado, entao bwtConfig.blockSize>0 
			Se Run Length foi detectado,entao compConfig.rlConfig.maxBits>=0
			Se Huffman Detectado, entao compConfig.huffConfig.isValid ==true
	 */

	if(compConfig.bwtConfig.blockSize>0 
		&& compConfig.rlConfig.maxBits>=0
		&& compConfig.huffConfig.isValid
		){
		cout<<"\tEncoding type detected: BWT => Run Length => Huffman"<<endl;
		stringstream intermediteStringStream;
		stringstream intermediteStringStream2;
		cout<<"\tDecoding Huffman"<<endl;
		mainDecodeHuffman(inFile,intermediteStringStream,compConfig.huffConfig);
		cout<<"\tDecoding Run Length"<<endl;
		mainDecodeRunLength(intermediteStringStream,intermediteStringStream2,compConfig.rlConfig,false);
		intermediteStringStream.str("");//forcing freeing of data (or trying to)
		cout<<"\tDecoding BWT"<<endl;
		mainDecodeBWT(intermediteStringStream2,outStrStream,compConfig.bwtConfig);
	}else if(compConfig.huffConfig.isValid 
		&& compConfig.rlConfig.maxBits>=0
		){
		stringstream intermediteStringStream;
		cout<<"\tEncoding type detected: Run Length => Huffman"<<endl;
		cout<<"\tDecoding Huffman"<<endl;
		mainDecodeHuffman(inFile,intermediteStringStream,compConfig.huffConfig);
		cout<<"intermediteStringStream: "<<intermediteStringStream.str()<<endl;
		cout<<"\tDecoding Run Length"<<endl;
		mainDecodeRunLength(intermediteStringStream,outStrStream,compConfig.rlConfig,false);
	}else if(compConfig.huffConfig.isValid 
		&& compConfig.bwtConfig.blockSize>0 
		){
		stringstream intermediteStringStream;
		cout<<"\tEncoding type detected: BWT => Huffman"<<endl;
		cout<<"\tDecoding Huffman"<<endl;
		mainDecodeHuffman(inFile,intermediteStringStream,compConfig.huffConfig);
		cout<<"\tDecoding BWT"<<endl;
		mainDecodeBWT(intermediteStringStream,outStrStream,compConfig.bwtConfig);
	}else if(compConfig.bwtConfig.blockSize>0 && compConfig.rlConfig.maxBits>=0){
		//It has been encoded aplying first BWT and then RunLength
		cout<<"\tEncoding type detected: BWT => Run Length"<<endl;
		stringstream intermediteStringStream;
		cout<<"\tDecoding Run Length"<<endl;
		mainDecodeRunLength(inFile,intermediteStringStream,compConfig.rlConfig,true);
		cout<<"\tDecoding BWT"<<endl;
		mainDecodeBWT(intermediteStringStream,outStrStream,compConfig.bwtConfig);
	}else if(compConfig.bwtConfig.blockSize>0){
		//It has been encoded only using bwt
		cout<<"\tEncoding type detected: BWT"<<endl;
		cout<<"\tDecoding BWT"<<endl;
		mainDecodeBWT(inFile,outStrStream,compConfig.bwtConfig);
	}else if(compConfig.rlConfig.maxBits>=0){
		//It has been encoded only using run length
		cout<<"\tEncoding type detected: Run Length"<<endl;
		cout<<"\tDecoding Run Length"<<endl;
		mainDecodeRunLength(inFile,outStrStream,compConfig.rlConfig,true);
	}else if(compConfig.huffConfig.isValid){
		stringstream intermediteStringStream;
		cout<<"\tEncoding type detected: Huffman"<<endl;
		cout<<"\tDecoding Huffman"<<endl;
		mainDecodeHuffman(inFile,outStrStream,compConfig.huffConfig);
	}else{
		cout<<"No algorithms detected. Exiting"<<endl;
		inFile.close();
		outFile.close();
		exit(EXIT_FAILURE);
	}
	inFile.close();
	cout<<"Writing to File"<<endl;
	outFile<<outStrStream.str();
	outFile.close();
	return 0;
}

void mainDecodeBWT(istream &inFile,ostream &outFile,BWTConfig &config){
	string decodedString;
	decodedString= BWTransform::decodeStream(inFile,config.blockSize,config.aVector);	
	//Writing to File
	outFile<<decodedString;
}

void mainDecodeRunLength(iostream &inFile,ostream &outFile,RunLengthConfig &config,bool binary){
	if(binary){
		string encodedString = binToStrRunLength(inFile,config.maxBits);
		stringstream encodedStringStream(encodedString);
		string decodedString= decodeRunLength(encodedStringStream);

		outFile<<decodedString;
	}else{

		string decodedString= decodeRunLength(inFile);

		outFile<<decodedString;
	}
}

void mainDecodeHuffman(iostream &inFile,ostream &outFile,HuffmanConfig &config){
	huffmanDecode(inFile, outFile, config.validBitsLastByte);
}


bool parseArgs(int argc,const char *argv[],string &inFileName,string &outFileName){
	if(argc<5){
		cerr<<"ERROR: Unexpected numberof argumets. Expected 4. Found: "<<argc-1<<endl;
		return false;
	}
	string argRead;
	for(int i=1;i<argc;i++){
		argRead = argv[i];
		if(argRead=="-i"){
			if(i+1>=argc){
				cerr<<"Name of file Expected after -i"<<endl;
				return false;
			}
			inFileName=argv[i+1];
			i++;
		}else if(argRead=="-o"){
			if(i+1>=argc){
				cerr<<"Name of file Expected after -o"<<endl;
				return false;
			}
			outFileName=argv[i+1];
			i++;
		}else{
			cerr<<"ERROR: Unexpected argument found: "<<argv[i]<<endl;
			return false;
		}
	}
	return true;
}