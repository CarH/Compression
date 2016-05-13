#include "runLength.hpp"
#include "myIoBitStream.hpp"
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace std;

void putBitsOfNumber(MyIOBitStream &stream,long long int value,long long int maxBits){
	vector<bool> valueOfBits(maxBits);
	long long int mask = 1;//mascara para pegar o bit menos significativo
	for(long long int i=0;i<maxBits;i++){
		valueOfBits[i]=  value & mask;
		value>>=1;
	}
	for(long long int i=maxBits-1;i>=0;i--){
		stream.appendBit(valueOfBits[i]);
	}
}
long long int readSequenceLengthFromBitStream(MyIOBitStream &bitStream,long long int maxBits){
	long long int result=0;
	bool bitRead;
	for(long long int i=0;i<maxBits;i++){
		bitRead=bitStream.getNextBit();
		if(bitStream.eof()){//deveria conter os bits necessarios... erro grave
			cerr<<"\tERROR: Insufficient bits to read sequence length"<<endl;
			exit(EXIT_FAILURE);
		}
		result<<=1;
		result = result | bitRead;
	}
	return result;
}
string binToStrRunLength(iostream &inFile,long long int maxBits){
	MyIOBitStream bitStream(inFile);

	char currentChar;
	char previousChar;
	char trash;
	long long int sequenceLength;
	bool isFileEmpty=true;
	stringstream outResultStream;
	int currentPos;
	int counter=1;
	bool wasLastOperationARunLength=false;

	
	// inFile>>std::noskipws;
	previousChar=bitStream.getNextByte();
	while(bitStream.getBitsReadCounter()==8){ // enquanto tiver bytes completos para ler
		isFileEmpty=false;
		currentChar=bitStream.getNextByte();
		// cout<<" Current: "<<currentChar<<" previousChar: "<<previousChar<<endl;
		if(bitStream.getBitsReadCounter()==8){//se leu char completo
			outResultStream<<previousChar;
			if(previousChar==currentChar){
				counter++;
			}else{
				counter=1;
			}
			if(counter==3){
				sequenceLength=readSequenceLengthFromBitStream(bitStream,maxBits);
				// cout<<"\t Sequence Length: "<<sequenceLength<<endl;
				// inFile>>trash; //le o espaco vazio
				outResultStream<<currentChar;
				
				outResultStream<<sequenceLength<<" ";
				
				counter=1;
				previousChar=bitStream.getNextByte();
				wasLastOperationARunLength=true;
			}else{
				previousChar=currentChar;
				wasLastOperationARunLength=false;
			}
		}
		
	}
	if(!isFileEmpty
		&& !wasLastOperationARunLength
	){//se tiver um caracter so
		outResultStream<<previousChar; 
	}


	return outResultStream.str();

}

string strToBinRunLength(istream &inFile,long long int maxBits){
	char currentChar;
	char previousChar;
	char trash;
	long long int sequenceLength;
	bool isFileEmpty=true;
	MyIOBitStream outResultStream;
	int currentPos;
	int counter=1;
	bool wasLastOperationARunLength=false;

	
	inFile>>std::noskipws;
	inFile>>previousChar;
	while(!inFile.eof()){
		isFileEmpty=false;
		inFile>>currentChar;
		// cout<<" Current: "<<currentChar<<" previousChar: "<<previousChar<<endl;
		if(!inFile.eof()){
			outResultStream.appendByte(previousChar);
			if(previousChar==currentChar){
				counter++;
			}else{
				counter=1;
			}
			if(counter==3){
				inFile>>sequenceLength;
				// cout<<"\t Sequence Length: "<<sequenceLength<<endl;
				inFile>>trash; //le o espaco vazio
				outResultStream.appendByte(currentChar);
				
				putBitsOfNumber(outResultStream, sequenceLength,maxBits);
				
				counter=1;
				inFile>>previousChar;
				wasLastOperationARunLength=true;
			}else{
				wasLastOperationARunLength=false;
				previousChar=currentChar;
			}
		}
		
	}
	if(!isFileEmpty
		&& !wasLastOperationARunLength
	){//se tiver um caracter so
		outResultStream.appendByte(previousChar); 
	}


	return outResultStream.getString();
}

string encodeRunLength(istream &inFile,long long int *maxBits){
	char nextCharToProcess;
	char currentChar;
	long long int sequenceLength;
	bool isFileEmpty=true;
	stringstream outResultStream;
	int currentPos;
	long long int maxSequenceLength=-1;

	

	inFile>>std::noskipws;
	
	inFile>>currentChar;
	nextCharToProcess=currentChar; //iguala para dar certo a logica do if apos o while
	inFile>>nextCharToProcess;
	sequenceLength=1;
	while(!inFile.eof()){
		// cout<<currentChar;
		isFileEmpty=false;
		// cout<<"Current : "<<currentChar<<" NExt: "<<nextCharToProcess<<endl;
		if(nextCharToProcess==currentChar){
			sequenceLength++;
		}else{
			//somente aplica o runlength se for sequencia maior que 3
			
			//imprime o caracter X vezes, indo no maximo ate 3
			for(long long int i=0;i<3 && i<sequenceLength;i++){
				outResultStream<<currentChar;
			}
			sequenceLength-=3;
			//se tiver mais que 3, coloca na frente quantos a mais que tem
			//seguido obrigatoriamente por um espaco
			if(sequenceLength>=0){
				outResultStream<<sequenceLength<<" ";
				maxSequenceLength = max(maxSequenceLength,sequenceLength);
			}
			sequenceLength=1;
		}
		currentChar=nextCharToProcess;
		inFile>>nextCharToProcess;
	}
	// currentPos = inFile.tellg();
	// cout<<L"CurrentPos: "<<currentPos<<endl;
	if(	!isFileEmpty //se arquivo nao vazio 
		&& currentChar==nextCharToProcess //OBS: aparentemente, essa condicao sera sempre verdadeira
		){
		
		for(long long int i=0;i<3 && i<sequenceLength;i++){
			outResultStream<<currentChar;
		}	
		sequenceLength-=3;
		//se tiver mais que 3, coloca na frente quantos a mais que tem
		//seguido obrigatoriamente por um espaco
		if(sequenceLength>=0){
			outResultStream<<sequenceLength<<" ";
			maxSequenceLength = max(maxSequenceLength,sequenceLength);
		}

	}
	*maxBits=0;
	while(maxSequenceLength!=0){
		maxSequenceLength>>=1;
		// cout<<"maxSequenceLength: "<<maxSequenceLength<<endl;
		(*maxBits)++;
	} 
	return outResultStream.str();
}

//primeira versao
// string encodeRunLength(fstream &inFile){
// 	char nextCharToProcess;
// 	char currentChar;
// 	long long int sequenceLength;
// 	bool isFileEmpty=true;
// 	stringstream outResultStream;
// 	int currentPos;


// 	if(inFile.is_open()){

// 		inFile>>std::noskipws;
		
// 		inFile>>currentChar;
// 		nextCharToProcess=currentChar; //iguala para dar certo a logica do if apos o while
// 		inFile>>nextCharToProcess;
// 		sequenceLength=1;
// 		while(!inFile.eof()){
// 			isFileEmpty=false;
// 			// cout<<"Current : "<<currentChar<<" NExt: "<<nextCharToProcess<<endl;
// 			if(nextCharToProcess==currentChar){
// 				sequenceLength++;
// 			}else{
// 				outResultStream<<currentChar;
// 				outResultStream<<sequenceLength<<" ";
// 				sequenceLength=1;
// 			}
// 			currentChar=nextCharToProcess;
// 			inFile>>nextCharToProcess;
// 		}
// 		// currentPos = inFile.tellg();
// 		// cout<<L"CurrentPos: "<<currentPos<<endl;
// 		if(	!isFileEmpty //se arquivo nao vazio 
// 			&& currentChar==nextCharToProcess
// 			){
			
// 			outResultStream<<currentChar;
// 			outResultStream<<sequenceLength<<" ";

// 		}
// 	}
// 	return outResultStream.str();
// }


string decodeRunLength(istream &inFile){
	char currentChar;
	char previousChar;
	char trash;
	long long int sequenceLength;
	bool isFileEmpty=true;
	stringstream outResultStream;
	int currentPos;
	int counter=1;
	bool wasLastOperationARunLength=false;

	
	inFile>>std::noskipws;
	inFile>>previousChar;
	while(!inFile.eof()){
		isFileEmpty=false;
		inFile>>currentChar;
		cout<<" Current: "<<currentChar<<" previousChar: "<<previousChar<<endl;
		if(!inFile.eof()){
			outResultStream<<previousChar;
			if(previousChar==currentChar){
				counter++;
			}else{
				counter=1;
			}
			if(counter==3){
				inFile>>sequenceLength;
				cout<<"\t Sequence Length: "<<sequenceLength<<endl;
				inFile>>trash; //le o espaco vazio
				outResultStream<<currentChar;
				for(long long int i=0;i<sequenceLength ;i++){
					outResultStream<<currentChar;
				}
				counter=1;
				inFile>>previousChar;
				wasLastOperationARunLength=true;
			}else{
				wasLastOperationARunLength=false;
				previousChar=currentChar;
			}
		}
		
	}
	if(!isFileEmpty
		&& !wasLastOperationARunLength
		){//se tiver um caracter so
		outResultStream<<previousChar; 
	}


	return outResultStream.str();
}


//primeira versao
// string decodeRunLength(fstream &inFile){
// 	char currentChar;
// 	char trash;
// 	long long int sequenceLength;
// 	bool isFileEmpty;
// 	stringstream outResultStream;
// 	int currentPos;


// 	if(inFile.is_open()){

// 		inFile>>std::noskipws;
// 		while(!inFile.eof()){
// 			inFile>>currentChar;
// 			// inFile>>trash; //le o espaco //espaco removido no encode
// 			inFile>>sequenceLength;
// 			inFile>>trash;
// 			// cout<<"Char: "<<currentChar<<" Length: "<<sequenceLength<<endl;
// 			if(!inFile.eof()){
// 				for(long long int i=0;i<sequenceLength ;i++){
// 					outResultStream<<currentChar;
// 				}
// 			}
			
// 		}
	
// 	}
// 	return outResultStream.str();
// }
