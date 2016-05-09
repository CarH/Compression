#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include "runLength.hpp"
#include "myIoBitStream.hpp"

using namespace std;

unsigned char* strToBinRunLength(){

}

void writeHeader(ostream &outFile,long long int maxBits){
	outFile<<'R';
	char nullChar = 0;
	outFile.write((char*)&maxBits,sizeof(long long int));
	outFile.write(&nullChar,sizeof(char));
}




int main(int argc, char const *argv[])
{
	// cout<<"Oi"<<endl;
	string filenameIn;
	string filenameOut;
	fstream inFile;
	fstream outFile;
	string encodedString;
	string encodedStringBinary;

	stringstream myStrStream;
	MyIOBitStream myBitStream;
	



	if(argc<3){
		cerr<<"usage: encode <file_name> <out_file>"<<endl;
		exit(EXIT_FAILURE);
	}
	filenameIn=argv[1]; 
	filenameOut=argv[2]; 
	inFile.open(filenameIn.c_str(),ios_base::in);
	// inFile.imbue(locale(inFile.getloc(), new codecvt_utf8<char, 0x10ffff, consume_header>()));
	cout<<"Encoding"<<endl;
	long long int maxBits;
	encodedString= encodeRunLength(inFile,&maxBits);
	inFile.close();
	//Writing to File
	stringstream encodedStringStream(encodedString);
	string binaryEncodedString = strToBinRunLength(encodedStringStream,maxBits);

	outFile.open(filenameOut.c_str(),ios_base::out | ios_base::binary );
	writeHeader(outFile,maxBits);
	outFile<<binaryEncodedString;
	outFile.close();

	// cout<<encodedString;

	return 0;
}