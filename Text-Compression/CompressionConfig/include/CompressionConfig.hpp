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
	class CompressionConfig{
		private:
			void parseHeaderBWT(std::istream &inFile,int *blockSize,std::vector<int> &aVector);
			void writeHeaderBWT(std::ostream &outFile,int blockSize,std::vector<int> &aVector);

			void parseHeaderRunLength(std::istream &inFile,long long int *maxBits);
			void writeHeaderRunLength(std::ostream &outFile,long long int maxBits);
		public:
			BWTConfig bwtConfig;
			RunLengthConfig rlConfig;
			void parseHeader(std::istream &headerFile);
			void writeHeader(std::ostream &outFile);
			
	};
#endif 
