#include <sstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#ifndef __MY_COMPRESSION_CONFIG__
	#define __MY_COMPRESSION_CONFIG__
	class BWTConfig{
		public:	
			int blockSize;
			std::vector<int> aVector;
			BWTConfig();
			~BWTConfig();
	};
	class RunLengthConfig{
		public:	
			long long int maxBits;
			RunLengthConfig();
			~RunLengthConfig();
	};
	class HuffmanConfig{
		public:	
			bool isValid;
			HuffmanConfig();
			~HuffmanConfig();
	};
	class CompressionConfig{
		private:
			void parseHeaderBWT(std::istream &inFile,int *blockSize,std::vector<int> &aVector);
			void writeHeaderBWT(std::ostream &outFile,int blockSize,std::vector<int> &aVector);

			void parseHeaderRunLength(std::istream &inFile,long long int *maxBits);
			void writeHeaderRunLength(std::ostream &outFile,long long int maxBits);

			void parseHeaderHuffman(std::istream &inFile);
			void writeHeaderHuffman(std::ostream &outFile);
		public:
			BWTConfig bwtConfig;
			RunLengthConfig rlConfig;
			HuffmanConfig huffConfig;
			void parseHeader(std::istream &headerFile);
			void writeHeader(std::ostream &outFile);
			
	};
#endif 
