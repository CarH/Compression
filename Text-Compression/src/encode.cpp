#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include "huffman.hpp"
#include "runLength.hpp"
#include "BWTransform.hpp"
#include "CompressionConfig.hpp"


#define BUFFER_SIZE 4096

using namespace std;


/**
 * It performs the calls necessary to encode the inputFile according to the BWT algorithm.
 * @param inFile  Input data
 * @param outFile Output stream where the encoded data must be written 
 * @param config  Configuration parameters of BWT algorithm
 */
void mainEncodeBWT(istream &inFile,ostream &outFile,BWTConfig &config);
/**
 * It performs the calls necessary to encode the inputFile according to the Run Length algorithm.
 * The Run Length algorithm may encode the sequence length as ASCII or as binary. 
 * @param inFile  Input data
 * @param outFile Output stream where the encoded data must be written
 * @param config  Configuration parameters of Run Length algorithm
 * @param binary  Indicates if the sequence length must be encoded as binary or as ASCII numbers
 */
void mainEncodeRunLength(istream &inFile,ostream &outFile,RunLengthConfig &config,bool binary);
void mainEncodeHuffman(istream &inFile,ostream &outFile,HuffmanConfig &config);
/**
 * ParseArgs verifies the arguments passed to this program. It is expected that all 8 params is specified, setting
 * it to false in case it is not desired. The txtblck param must be equal to an positive integer number,
 * even if it is not used. The following params are expected:
 * --btw=[true|false] --txtblck=<int_number> --huffman=[true|false] --runl=[true|false] -i <input_file_name> -o <output_file_name>
 * @param  argc        Quantity of params,including the program name
 * @param  argv        Params to be parsed
 * @param  huffman     Ouput param. It is set to true if the huffman algorithm was requested
 * @param  bwt         Ouput param. It is set to true if the BWT algorithm was requested
 * @param  rl          Ouput param. It is set to true if the Run Length algorithm was requested
 * @param  blockSize   Ouput param. It is set to the size of the block requested
 * @param  inFileName  Ouput param. It is set to name of the input file read from the params
 * @param  outFileName Ouput param. It is set to name of the output file read from the params
 * @return             Returns true if no errors occurred during parsing. Returns false otherwise.
 */
bool parseArgs(int argc,const char *argv[],bool *huffman,bool *bwt,bool *rl,int *blockSize,string &inFileName,string &outFileName);

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



	int blockSize;
	bool huffman;
	bool runl;
	bool bwt;
	char *inFileNameChar;
	char *outFileNameChar;

	//realiza a verficacao dos parametros
	if(!parseArgs(argc,argv,&huffman,&bwt,&runl,&blockSize,filenameIn,filenameOut)){
		cerr<<"usage: encode -i <input_file_name> -o <output_file_name> --btw=[true|false] --txtblck=<int_number> --huffman=[true|false] --runl=[true|false]"<<endl;
		exit(EXIT_FAILURE);
	}
	// filenameIn=inFileNameChar; 
	// filenameOut=outFileNameChar; 

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

	cout<<"Encoding"<<endl;
	//verifica as flags booleanas retornadas pelo parse e chama as rotinas de codificacao adequadamente
	//Caso mais de um algoritmo seja requisitado, foi escolhida a seguinte ordem:
	//	BWT => Run Length => Huffman
	//Caso Run length e Huffman sejam especificados juntos, o Run Length eh utilizado no modo ASCII.
	//	Caso contrario, o run length eh utilizado no modo binario.
	//	Essa decisao foi tomada pois o Huffman codifica a frequencia de cada byte. Se usar Run Length
	//	no modo binario um byte nao significaria mais um character.
	if(huffman && bwt && runl){
		cout<<"\tEncoding type detected: BWT => Run Length => Huffman"<<endl;
		stringstream intermediateStringStream;
		stringstream intermediateStringStream2;
		compConfig.bwtConfig.blockSize=blockSize;
		compConfig.huffConfig.isValid = true;
		// cout<<"Block Size: "<<compConfig.bwtConfig.blockSize<<endl;
		cout<<"\tEncoding BWT"<<endl;
		mainEncodeBWT(inFile,intermediateStringStream,compConfig.bwtConfig);
		inFile.close();
		cout<<"\tEncoding Run Length"<<endl;
		//usa Run Length no modo ASCII
		mainEncodeRunLength(intermediateStringStream,intermediateStringStream2,compConfig.rlConfig,false);
		intermediateStringStream.str("");//forcing freeing of data (or trying to)
		cout<<"\tEncoding Huffman"<<endl;
		mainEncodeHuffman(intermediateStringStream2,outStrStream,compConfig.huffConfig);
	}else if(huffman && runl){
		stringstream intermediateStringStream;
		cout<<"\tEncoding type detected: Run Length => Huffman"<<endl;
		cout<<"\tEncoding Run Length"<<endl;
		compConfig.huffConfig.isValid = true;
		mainEncodeRunLength(inFile,intermediateStringStream,compConfig.rlConfig,false);
		// cout<<"intermediteStringStream(E): "<<intermediateStringStream.str()<<endl;
		inFile.close();
		cout<<"\tEncoding Huffman"<<endl;
		mainEncodeHuffman(intermediateStringStream,outStrStream,compConfig.huffConfig);
	}else if(huffman && bwt){
		cout<<"\tEncoding type detected: BWT => Huffman"<<endl;
		stringstream intermediateStringStream;
		compConfig.bwtConfig.blockSize=blockSize;
		// cout<<"Block Size: "<<compConfig.bwtConfig.blockSize<<endl;
		cout<<"\tEncoding BWT"<<endl;
		compConfig.huffConfig.isValid = true;
		mainEncodeBWT(inFile,intermediateStringStream,compConfig.bwtConfig);
		inFile.close();
		cout<<"\tEncoding Huffman"<<endl;
		mainEncodeHuffman(intermediateStringStream,outStrStream,compConfig.huffConfig);
	}else if(bwt && runl){
		cout<<"\tEncoding type detected: BWT => Run Length"<<endl;
		stringstream intermediateStringStream;
		compConfig.bwtConfig.blockSize=blockSize;
		// cout<<"Block Size: "<<compConfig.bwtConfig.blockSize<<endl;
		cout<<"\tEncoding BWT"<<endl;
		mainEncodeBWT(inFile,intermediateStringStream,compConfig.bwtConfig);
		inFile.close();
		cout<<"\tEncoding Run Length"<<endl;
		mainEncodeRunLength(intermediateStringStream,outStrStream,compConfig.rlConfig,true);
	}else if(bwt){
		cout<<"\tEncoding type detected: BWT"<<endl;
		compConfig.bwtConfig.blockSize=blockSize;
		cout<<"\tEncoding BWT"<<endl;
		mainEncodeBWT(inFile,outStrStream,compConfig.bwtConfig);
		inFile.close();
	}else if(runl){
		cout<<"\tEncoding type detected: Run Length"<<endl;
		cout<<"\tEncoding Run Length"<<endl;
		mainEncodeRunLength(inFile,outStrStream,compConfig.rlConfig,true);
		inFile.close();
	}else if(huffman){
		cout<<"\tEncoding type detected: Huffman"<<endl;
		cout<<"\tEncoding Huffman"<<endl;
		compConfig.huffConfig.isValid = true;
		mainEncodeHuffman(inFile,outStrStream,compConfig.huffConfig);
		inFile.close();
	}else{
		cout<<"No algorithms detected. Exiting"<<endl;
		inFile.close();
		outFile.close();
		exit(EXIT_FAILURE);
	}
	
	cout<<"Writing to File"<<endl;
	compConfig.writeHeader(outFile);
	outFile<<outStrStream.str();

	
	outFile.close();

	return 0;
}

void mainEncodeBWT(istream &inFile,ostream &outFile,BWTConfig &config){
	// inFile.imbue(locale(inFile.getloc(), new codecvt_utf8<char, 0x10ffff, consume_header>()));
	string encodedString;
	encodedString= BWTransform::encodeStream(inFile,config.blockSize,config.aVector);
	//Writing to File
	outFile<<encodedString;
	// cout<<"DEBUG: "<<encodedString<<endl;
	// cout<<encodedString;

}

void mainEncodeRunLength(istream &inFile,ostream &outFile,RunLengthConfig &config,bool binary){
	string encodedString;
	if(binary){
		encodedString= encodeRunLength(inFile,&(config.maxBits));
		//Writing to File
		stringstream encodedStringStream(encodedString);
		string binaryEncodedString = strToBinRunLength(encodedStringStream,config.maxBits);
		outFile<<binaryEncodedString;
	}else{
		encodedString= encodeRunLength(inFile,&(config.maxBits));
		outFile<<encodedString;
	}
}
void mainEncodeHuffman(istream &inFile,ostream &outFile,HuffmanConfig &config){	
	string encodedString;

	config.char_freq = calculateFrequency(inFile);

	printMap(config.char_freq); // TODO DELETE
	
	buildHuffmanTree(config.char_freq);

	createCodeTable(&huffmanTreeRoot);
	
	printCodeTable(); // TODO DELETE

	encodedString = huffmanEncode(inFile, config.char_freq, &config.validBitsLastByte);
	// cout << "encodedString Huffman: "<<encodedString<<endl;
	outFile << encodedString;

	freeDynamicMemory();
}


bool parseArgs(int argc,const char *argv[],bool *huffman,bool *bwt,bool *rl,int *blockSize,string &inFileName,string &outFileName){
	
	if(argc<9){
		cerr<<"ERROR: Unexpected numberof argumets. Expected 8. Found: "<<argc-1<<endl;
		return false;
	}
	*rl=false;
	*huffman=false;
	*bwt=false;
	*blockSize=0;
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
		}else if(argRead=="--bwt=true"){
			*bwt=true;
		}else if(argRead=="--bwt=false"){
			*bwt=false;
		}else if(argRead=="--huffman=true"){
			*huffman=true;
		}else if(argRead=="--huffman=false"){
			*huffman=false;
		}else if(argRead=="--runl=true"){
			*rl=true;
		}else if(argRead=="--runl=false"){
			*rl=false;
		}else{
			//testar o --txtblck=X
			int equalSignPos=argRead.find_first_of("=");
			if(equalSignPos!=string::npos && equalSignPos<argRead.length()){//se achou
				string command = argRead.substr(0,equalSignPos);
				string number = argRead.substr(equalSignPos+1,string::npos);
				int  numberInt = atoi(number.c_str());
				if(command=="--txtblck" && numberInt>0){
					*blockSize = numberInt;
					continue;
				}

			}
			cerr<<"ERROR: Unexpected argument found: "<<argv[i]<<endl;
			return false;
		}
	}
	if(*bwt && *blockSize<=0){
		cerr<<"ERROR: BWT requested but invalid block size read: "<<blockSize<<endl;
		return false;
	}
	return true;
}