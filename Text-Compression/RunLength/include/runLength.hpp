#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


#ifndef __MY_RUN_LENGTH__
	#define __MY_RUN_LENGTH__
	std::string strToBinRunLength(std::istream &stream,long long int maxBits);
	std::string encodeRunLength(std::istream &inFile,long long  int *maxBits);
	std::string decodeRunLength(std::istream &inFile);
#endif