#include "runLength.hpp"
#include "myIoBitStream.hpp"
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace std;
/**
 * Writes the bits corresponding to parameter value into the stream, using a fixed number of bits, indicated by maxBits. If maxBits is less than the minimum required, the data is truncated, using only the maxBits first bits. If maxBits is greater than the minimum required, zeros are added to the most significant digits.
 * @param stream  Output stream to be written to.
 * @param value   Value that must be written to stream.
 * @param maxBits Number of bits to be used
 */
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
/**
 * Reads the next maxBits from bitStream and interprets it as a number.
 * @param  bitStream Input stream to be read from.
 * @param  maxBits   Number of bits to be read.
 * @return           Number represented by the bits read.
 */
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

	char currentChar; //ultimo caracter lido
	char previousChar; //caracter anterior. Este sera escrito no resultado final a cada iteracao
	char trash;
	long long int sequenceLength;
	bool isFileEmpty=true;
	stringstream outResultStream;
	int currentPos;
	int counter=1;
	bool wasLastOperationARunLength=false;

	/*
		Logica implementada: mantem 2 caracteres na memoria, escrevendo sempre o caracter mais antigo 
		no arquivo
	 */
	// inFile>>std::noskipws;
	previousChar=bitStream.getNextByte(); //le um caracter
	while(bitStream.getBitsReadCounter()==8){ // enquanto a ultima leitura ler corretamente 1 byte completo
		isFileEmpty=false;
		currentChar=bitStream.getNextByte();//le o proximo
		// cout<<" Current: "<<currentChar<<" previousChar: "<<previousChar<<endl;
		//se a operacao de leitura leu um byte completo(o que deve sempre acontecer)
		if(bitStream.getBitsReadCounter()==8){
			outResultStream<<previousChar; //escreve o caracter mais antigo na stream de saida
			if(previousChar==currentChar){
				//se houve uma repeticao
				counter++;
			}else{
				counter=1;
			}
			if(counter==3){
				/* se houve tres repeticoes,
				// entao os proximos bits representam o numero extras de repeticoes
				*/
				sequenceLength=readSequenceLengthFromBitStream(bitStream,maxBits);
				// cout<<"\t Sequence Length: "<<sequenceLength<<endl;
				// inFile>>trash; //le o espaco vazio
				outResultStream<<currentChar; //escreve a terceira repeticao
				
				outResultStream<<sequenceLength<<" ";//escreve o numero de repeticoes, seguido de expaco
				
				counter=1;
				previousChar=bitStream.getNextByte(); //continua a leitura
				wasLastOperationARunLength=true;//seta booleano para o teste apos o laco
			}else{
				previousChar=currentChar; //continua a leitura
				wasLastOperationARunLength=false;//seta booleano para o teste apos o laco
			}
		}
		
	}
	/*
		caso o ultimo caracter lido tenha correspondido a um Run, entao ele ja foi escrito completamente 
		na saida. No entanto, caso o ultimo caracter nao fosse um run, ele ainda nao foi escrito na saida, devido
		a logica implementada. Entao, deve existir essa verificacao para escreve-lo corretamente.
		Caso semelhante ocorre se o arquivo tiver apenas um caracter. Neste caso, a condicao abaixo
		tambem sera satisfeita devido ao valor de inicializacao da variavel wasLastOperationARunLength
	*/
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


	//mesma logica de leitura implementada em binToStrRunLength
	
	inFile>>std::noskipws;
	inFile>>previousChar;
	while(!inFile.eof()){ //enquanto a ultima leitura do byte for bem sucedida
		isFileEmpty=false;
		inFile>>currentChar;
		// cout<<" Current: "<<currentChar<<" previousChar: "<<previousChar<<endl;
		if(!inFile.eof()){ //se leu corretamente currentChar
			outResultStream.appendByte(previousChar); //escreve o caracter mais antigo
			if(previousChar==currentChar){
				//se houve repeticao, contabiliza
				counter++;
			}else{
				counter=1;
			}
			if(counter==3){
				//se houve repeticao de pelo menos 3
				inFile>>sequenceLength; //le o tamanho de repeticoes extras
				// cout<<"\t Sequence Length: "<<sequenceLength<<endl;
				inFile>>trash; //le o espaco vazio
				outResultStream.appendByte(currentChar);//escreve a terceira repeticao
				
				putBitsOfNumber(outResultStream, sequenceLength,maxBits); //escreve a quantidade de repeticoes extras
				
				counter=1;
				inFile>>previousChar;
				wasLastOperationARunLength=true;
			}else{
				wasLastOperationARunLength=false;
				previousChar=currentChar;
			}
		}
		
	}
	/*
		Logica deste condicional: vide binToStrRunLength
	 */
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
	
	/*
		Logica de leitura implementada: mantem dois caracteres lidos, um que esta sendo atualmente processado
		e o proximo caracter
	 */

	if(inFile>>currentChar){//le o primeiro caractesr
		//seta indicativo de que arquivo possui ao menos 1 byte de dado
		isFileEmpty=false;
	}
	nextCharToProcess=currentChar; //iguala para dar certo a logica do if apos o while
	inFile>>nextCharToProcess;
	sequenceLength=1;
	while(!inFile.eof()){ //enquanto houver um caracter a frente do processado atualmente:
		// cout<<currentChar;
		
		// cout<<"Current : "<<currentChar<<" NExt: "<<nextCharToProcess<<endl;
		if(nextCharToProcess==currentChar){ //se for repeticao, contabiliza
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
	/*
		Nessa logica, o ultimo caracter nunca eh escrito na saida pelo laco acima... entao, se 
		o arquivo nao for vazio, eh preciso escrever o ultimo caracter na saida
		(seja ele uma sequencia ou nao)
	 */
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
	//calculo do numero minimo de bits necessario para representar a maior sequencia.
	*maxBits=0;
	if(maxSequenceLength>0){ //caso nao haja nenhuma sequencia noa arquivo,maxsequence se mantem em seu valor inicial, -1.
		while(maxSequenceLength!=0){
			maxSequenceLength>>=1; 
			// cout<<"maxSequenceLength: "<<maxSequenceLength<<endl;
			(*maxBits)++;
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
	bool wasLastOperationARunLength=false;

	
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
				wasLastOperationARunLength=true;
			}else{
				wasLastOperationARunLength=false;
				previousChar=currentChar;
			}
		}
		
	}
	/*
		Logica deste condicional: vide binToStrRunLength
	 */
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
