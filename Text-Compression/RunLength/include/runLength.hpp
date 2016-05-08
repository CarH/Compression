#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


#ifndef __MY_RUN_LENGTH__
	#define __MY_RUN_LENGTH__
	std::string strToBinRunLength(std::istream &stream);
	std::string encodeRunLength(std::istream &inFile);
	std::string decodeRunLength(std::istream &inFile);
#endif