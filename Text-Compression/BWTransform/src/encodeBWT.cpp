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


void writeHeader(ostream &outFile,int blockSize,vector<int> &aVector){
	outFile<<'B';
	char nullChar = 0;
	int *aArray =(int*) malloc(sizeof(int)*aVector.size());
	int aSize = aVector.size();
	
	for(int i=0;i<aSize;i++){
		aArray[i]=aVector[i];//copies the aVector to an Array in order to write ir properly in a file
	}
	outFile.write((char*)&blockSize,sizeof(int));
	outFile.write((char*)&aSize,sizeof(int));//stores the size of aVector
	outFile.write((char*)aArray,sizeof(int)*aSize);//stores the aValues
	outFile.write(&nullChar,sizeof(char));
	free(aArray);
}




int main(int argc, char const *argv[])
{
	// cout<<"Oi"<<endl;
	string filenameIn;
	string filenameOut;
	fstream inFile;
	fstream outFile;
	string encodedString;



	int blockSize;
	vector<int> as;	



	if(argc<4){
		cerr<<"usage: encode <file_name> <out_file> <block_size>"<<endl;
		exit(EXIT_FAILURE);
	}

	filenameIn=argv[1]; 
	filenameOut=argv[2]; 
	blockSize=atoi(argv[3]);
	inFile.open(filenameIn.c_str(),ios_base::in);
	if(!inFile.is_open()){
		cerr<<"ERROR: Input File not Found: "<<filenameIn<<endl;
		exit(EXIT_FAILURE);
	}
	// inFile.imbue(locale(inFile.getloc(), new codecvt_utf8<char, 0x10ffff, consume_header>()));
	cout<<"Encoding"<<endl;

	encodedString= BWTransform::encodeStream(inFile,blockSize,as);
	inFile.close();
	//Writing to File
	

	outFile.open(filenameOut.c_str(),ios_base::out | ios_base::binary );
	writeHeader(outFile,blockSize,as);
	outFile<<encodedString;
	outFile.close();
	// cout<<"DEBUG: "<<encodedString<<endl;
	for(int i=0;i<as.size();i++){
		// cout<<"\t"<<as[i]<<endl;
	}

	// cout<<encodedString;

	return 0;
}