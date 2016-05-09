#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "runLength.hpp"

using namespace std;

unsigned char* strToBinRunLength(){

}

void parseHeader(istream &inFile,long long int *maxBits){
	char encodingType;
	long long int localMaxBits;
	inFile>>noskipws;
	inFile>>encodingType;
	if(encodingType!='R'){
		cerr<<"Not encoded by RunLength"<<endl;
		exit(EXIT_FAILURE);
	}
	inFile.read((char*)&localMaxBits,sizeof(localMaxBits));
	inFile>>encodingType;
	if(encodingType==0){
		cout<<"End of Run Length header detected properly"<<endl;
	}else{
		cerr<<"ERROR: Run Length Header. End not detected"<<endl;
		exit(EXIT_FAILURE);
	}
	*maxBits=localMaxBits;
	
}

int main(int argc, char const *argv[])
{
	string filenameIn;
	string filenameOut;
	fstream inFile;
	fstream outFile;
	string decodedString;
	



	if(argc<3){
		cerr<<"usage: decode <file_name> <out_file>"<<endl;
		exit(EXIT_FAILURE);
	}
	filenameIn=argv[1]; 
	filenameOut=argv[2]; 
	inFile.open(filenameIn.c_str(),ios_base::in|ios_base::binary);
	long long int maxBits;
	parseHeader(inFile,&maxBits);
	string encodedString = binToStrRunLength(inFile,maxBits);
	stringstream encodedStringStream(encodedString);
	decodedString= decodeRunLength(encodedStringStream);
	inFile.close();

	outFile.open(filenameOut.c_str(),ios_base::out);
	outFile<<decodedString;
	outFile.close();

	// cout<<decodedString;

	return 0;
}