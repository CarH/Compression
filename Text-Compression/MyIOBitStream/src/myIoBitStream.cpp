#include "myIoBitStream.hpp"

using namespace std;

MyIOBitStream::MyIOBitStream(){
	this->bitCounterWrite=0;
	this->bitCounterRead=0;
	this->charBuff=(char)0;
	this->wasStreamDeclaredInternally=true;
	this->stream = new stringstream();
	(*(this->stream))>>noskipws;
}

MyIOBitStream::MyIOBitStream(iostream &s){

	this->bitCounterWrite=0;
	this->bitCounterRead=0;
	this->charBuff=(char)0;
	this->stream = &s;
	this->wasStreamDeclaredInternally=false;
	(*(this->stream))>>noskipws;
}
MyIOBitStream::~MyIOBitStream(){
	if(this->wasStreamDeclaredInternally){
		delete this->stream;
	}
}
bool MyIOBitStream::appendBit(bool b){
	(this->bitCounterWrite)++;
	this->charBuff=this->charBuff<<1;
	this->charBuff = this->charBuff | b; //adiciona bit no final
	// cout<<"BOOL ADDING: "<<(int)b<<endl;
	if(this->bitCounterWrite==8){ //se completou um byte
		// cout<<"\t\tCHAR COPLETED"<<endl;
		this->bitCounterWrite=0;
		(*(this->stream))<<this->charBuff;
		this->charBuff=(char)0;
	}

}
bool MyIOBitStream::appendByte(char byte){
	bool bitValues[8];
	char returnValue = (char)0;
	char auxChar;
	char mask = 0x80; //mascara para capturar o primeiro bit . 10000000
	for(int i=0;i<8;i++){
		auxChar = (byte) & mask;//recupera o primeiro bit
		auxChar = auxChar>>7; // realiza shift para que o bit fique na posicao menos significativa
		if((int)auxChar==0){
			this->appendBit(false);
		}else{
			this->appendBit(true);
		}
		byte<<=1;
	}
	return true;

}
bool MyIOBitStream::getNextBit(){
	
	char auxChar;
	char mask = 0x80; //mascara para capturar o primeiro bit . 10000000
	if(this->bitCounterRead==0){//se buffer vazio, entao le o proximo byte
		
		(*(this->stream))>>(this->charBuff);
		// cout<<"Leu um char da stream: "<<this->charBuff<<endl;
		this->bitCounterRead=8;
	}
	auxChar = (this->charBuff) & mask;//recupera o primeiro bit
	auxChar = auxChar>>7; // realiza shift para que o bit fique na posicao menos significativa
	(this->bitCounterRead)--;//decrementa indicando que ja leu um bit
	(this->charBuff)=(this->charBuff)<<1;
	// cout<<"Result char: |"<<(int )auxChar<<"|"<<endl;
	// cout<<"Mask: |"<<(char )mask<<"|"<<endl;
	if((int)auxChar==0){
		return false;
	}else{
		return true;
	}
}
char MyIOBitStream::getNextByte(){
	bool bitValues[8];
	char returnValue = (char)0;
	for(int i=0;i<8;i++){
		bitValues[i]=this->getNextBit();
		cout<<(int)bitValues[i];
		returnValue=returnValue<<1;
		returnValue=returnValue | bitValues[i];
	}
	// for(int i=0;i<8;i++){
		
	// }
	return returnValue;

}
string MyIOBitStream::getString(){
	stringstream strStream;
	char auxChar;
	(*(this->stream))>>auxChar;
	// int debugCounter=0;
	while(!(*(this->stream)).eof()) {
		strStream<<auxChar;
		// debugCounter++;
		(*(this->stream))>>auxChar;
	}
	// cout<<"DEBUG COUNTER GET STRING: "<<debugCounter<<endl;
	if(this->bitCounterWrite >0){
		int shiftsNeeded = 8-this->bitCounterWrite;
		this->charBuff = (this->charBuff) <<shiftsNeeded;
		strStream<<(this->charBuff);
		this->bitCounterWrite=0;
		this->charBuff = (char)0;
	}
	this->stream->clear();
	// cout<<strStream.str()<<endl;
	return strStream.str();

}
