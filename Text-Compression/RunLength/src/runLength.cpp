#include "runLength.hpp"
#include "myIoBitStream.hpp"

using namespace std;

string strToBinRunLength(istream &inFile,long long int maxBits){
	// char currentChar;
	// char previousChar;
	// char trash;
	// long long int sequenceLength;
	// bool isFileEmpty=true;
	// stringstream outResultStream;
	// int currentPos;
	// int counter=1;

	
	// inFile>>std::noskipws;
	// inFile>>previousChar;
	// while(!inFile.eof()){
	// 	isFileEmpty=false;
	// 	inFile>>currentChar;
	// 	// cout<<" Current: "<<currentChar<<" previousChar: "<<previousChar<<endl;
	// 	if(!inFile.eof()){
	// 		outResultStream<<previousChar;
	// 		if(previousChar==currentChar){
	// 			counter++;
	// 		}else{
	// 			counter=1;
	// 		}
	// 		if(counter==3){
	// 			inFile>>sequenceLength;
	// 			// cout<<"\t Sequence Length: "<<sequenceLength<<endl;
	// 			inFile>>trash; //le o espaco vazio
	// 			outResultStream<<currentChar;
	// 			for(long long int i=0;i<sequenceLength ;i++){
	// 				outResultStream<<currentChar;
	// 			}
	// 			counter=1;
	// 			inFile>>previousChar;
	// 		}else{
	// 			previousChar=currentChar;
	// 		}
	// 	}
		
	// }
	// if(!isFileEmpty){//se tiver um caracter so
	// 	outResultStream<<previousChar; 
	// }


	// return outResultStream.str();
}

string encodeRunLength(istream &inFile){
	char nextCharToProcess;
	char currentChar;
	long long int sequenceLength;
	bool isFileEmpty=true;
	stringstream outResultStream;
	int currentPos;


	

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
			}
			sequenceLength=1;
		}
		currentChar=nextCharToProcess;
		inFile>>nextCharToProcess;
	}
	// currentPos = inFile.tellg();
	// cout<<L"CurrentPos: "<<currentPos<<endl;
	if(	!isFileEmpty //se arquivo nao vazio 
		&& currentChar==nextCharToProcess
		){
		
		for(long long int i=0;i<3 && i<sequenceLength;i++){
			outResultStream<<currentChar;
		}	
		sequenceLength-=3;
		//se tiver mais que 3, coloca na frente quantos a mais que tem
		//seguido obrigatoriamente por um espaco
		if(sequenceLength>=0){
			outResultStream<<sequenceLength<<" ";
		}

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

	
	inFile>>std::noskipws;
	inFile>>previousChar;
	while(!inFile.eof()){
		isFileEmpty=false;
		inFile>>currentChar;
		// cout<<" Current: "<<currentChar<<" previousChar: "<<previousChar<<endl;
		if(!inFile.eof()){
			outResultStream<<previousChar;
			if(previousChar==currentChar){
				counter++;
			}else{
				counter=1;
			}
			if(counter==3){
				inFile>>sequenceLength;
				// cout<<"\t Sequence Length: "<<sequenceLength<<endl;
				inFile>>trash; //le o espaco vazio
				outResultStream<<currentChar;
				for(long long int i=0;i<sequenceLength ;i++){
					outResultStream<<currentChar;
				}
				counter=1;
				inFile>>previousChar;
			}else{
				previousChar=currentChar;
			}
		}
		
	}
	if(!isFileEmpty){//se tiver um caracter so
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
