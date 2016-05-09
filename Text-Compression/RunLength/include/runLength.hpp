#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


#ifndef __MY_RUN_LENGTH__
	#define __MY_RUN_LENGTH__
	#define RL_MINIMUM_SEQUENCE_NEDDED_TO_ENCODE 3
	std::string binToStrRunLength(std::iostream &inFile,long long int maxBits);
	std::string strToBinRunLength(std::istream &stream,long long int maxBits);
	std::string encodeRunLength(std::istream &inFile,long long  int *maxBits);
	std::string decodeRunLength(std::istream &inFile);
#endif