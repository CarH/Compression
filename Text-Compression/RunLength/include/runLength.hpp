#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


#ifndef __MY_RUN_LENGTH__
	#define __MY_RUN_LENGTH__
	#define RL_MINIMUM_SEQUENCE_NEDDED_TO_ENCODE 3
	/**
	 * Converts a sequence of bytes representing data encoded by Run Length algorithm in binary format to ASCII format.
	 * @param  inFile  A input stream containing the original data in binary format.
	 * @param  maxBits The max number of bits used to encode the length of the longest run.
	 * @return         Returns a string containing the data in "ASCII" format
	 */
	std::string binToStrRunLength(std::iostream &inFile,long long int maxBits);
	/**
	 * Converts a sequence of bytes representing data encoded by Run Length algorithm in ASCII format to binary format.
	 * @param  stream  A input Stream containing the original data in ASCII format
	 * @param  maxBits The max number of bits to be used to encode the length of the longest run.
	 * @return         Returns a string containing the data in binary format
	 */
	std::string strToBinRunLength(std::istream &stream,long long int maxBits);
	/**
	 * Encodes a stream of bytes using the Run Length Algorithm. The output is given in ASCII format.
	 * The string is encoded in the following way:
	 * The run length encoding is applyed only if the character was repeated at least 3 times. If so, the repetition is
	 * replaced with the character repeated three time, followed by a number of extra repetitions and then a blank space.
	 * Example:
	 * aaab => a0 b
	 * abbbbbc=> abbb2 c
	 * @param  inFile  The input stream containing the original data to be encoded.
	 * @param  maxBits Output parameter. The minimum number of bits needed to represent the longest run is returned in this parameter.
	 * @return         Returns the encoded string in ASCII format
	 */
	std::string encodeRunLength(std::istream &inFile,long long  int *maxBits);
	/**
	 * Decodes a stream of bytes that was previously encoded using Run Length. The input is expected to be in ASCII format, and the output is also given in ASCII format
	 * @param  inFile The input stream containing the data incoded by Run Length in ASCII format
	 * @return        Returns the decoded string in ASCII format.
	 */
	std::string decodeRunLength(std::istream &inFile);
#endif