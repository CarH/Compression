#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include "BWTransform.hpp"

using namespace std;


void parseHeader(istream &inFile,int *blockSize,vector<int> &aVector){
	char encodingType;
	char nullChar = 1;
	int aSize;
	int *aArray ;
	inFile>>noskipws;
	inFile.read(&encodingType,sizeof(char));
	int localBlockSize;
	if(encodingType=='B'){
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
		inFile.read(&nullChar,sizeof(char));
		if(nullChar!=0){
			cerr<<"ERROR: Unexpected ending of BWT header"<<endl;
			exit(EXIT_FAILURE);
		}
	}
	
	
}




int main(int argc, char const *argv[])
{
	// cout<<"Oi"<<endl;
	string filenameIn;
	string filenameOut;
	fstream inFile;
	fstream outFile;
	string decodedString;



	int blockSize;
	vector<int> as;	



	if(argc<3){
		cerr<<"usage: decode <file_name> <out_file>"<<endl;
		exit(EXIT_FAILURE);
	}

	filenameIn=argv[1]; 
	filenameOut=argv[2]; 
	
	inFile.open(filenameIn.c_str(),ios_base::in);
	if(!inFile.is_open()){
		cerr<<"ERROR: Input File not Found: "<<filenameIn<<endl;
		exit(EXIT_FAILURE);
	}
	// inFile.imbue(locale(inFile.getloc(), new codecvt_utf8<char, 0x10ffff, consume_header>()));
	cout<<"Decoding"<<endl;
	parseHeader(inFile,&blockSize,as);

	decodedString= BWTransform::decodeStream(inFile,blockSize,as);
	inFile.close();
	//Writing to File
	

	outFile.open(filenameOut.c_str(),ios_base::out );
	outFile<<decodedString;
	outFile.close();
	// cout<<"DEBUG: "<<decodedString<<endl;

	// cout<<encodedString;

	return 0;
}