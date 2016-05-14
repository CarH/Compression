#include <sstream>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <vector>
#ifndef __MY_COMPRESSION_CONFIG__
	#define __MY_COMPRESSION_CONFIG__
	/**
	 * BWTConfig holds the configuration parameters to encode and decode using BWT algorithm.
	 * If blockSize <=0, it indicates that the information in this object is invalid, meaning that
	 * 	the BWT algorithm was not used
	 */
	class BWTConfig{
		public:	
			int blockSize;
			std::vector<int> aVector;
			BWTConfig();
			~BWTConfig();
	};
	/**
	 * RunLengthConfig holds the configuration parameters to encode and decode using Run Length algorithm.
	 * If maxBits <0, it indicates that the information in this object is invalid, meaning that
	 * 	the run length algorithm was not used
	 */
	class RunLengthConfig{
		public:	
			long long int maxBits;
			RunLengthConfig();
			~RunLengthConfig();
	};
	/**
	 * HuffmanConfig holds the configuration parameters to encode and decode using Huffman algorithm.
	 * If isValid is false, it indicates that the information in this object is invalid, meaning that
	 * 	the huffman algorithm was not used
	 */
	class HuffmanConfig{
		public:	
			std::map<char, long long> char_freq;
			int validBitsLastByte; 
			bool isValid;
			HuffmanConfig();
			~HuffmanConfig();
	};
	class CompressionConfig{
		private:
			/**
			 * Reads the data inside the BWT header. BWT header expects two numbers and one array
			 * 	The first number corresponds to the block size. The second one corresponds to the array size
			 * @param inFile    Input Stream where to read the header from.
			 * @param blockSize Output Param. Returns the block size read from the header
			 * @param aVector   Output Param. Returns the array of indices
			 */
			void parseHeaderBWT(std::istream &inFile,int *blockSize,std::vector<int> &aVector);
			/**
			 * Writes the header to the output stream in the correct order.
			 * It writes the identifier 'B' to indicate where the header starts
			 * Then it writes the block size, followed by the array size.
			 * At last, it writes the vecto 
			 * @param outFile   Ouput Stream 
			 * @param blockSize Size of block to write 
			 * @param aVector   Indices "a" to write
			 */
			void writeHeaderBWT(std::ostream &outFile,int blockSize,std::vector<int> &aVector);
			/**
			 * Reads the data inside the Run Length header. 
			 *  RL header expects a number indicating the quantity of bits used to encode the sequence lengths.
			 * @param inFile  Stream containing the header
			 * @param maxBits Output param. Returns teh quantity of bits used to encode the sequence length
			 */
			void parseHeaderRunLength(std::istream &inFile,long long int *maxBits);
			
			void writeHeaderRunLength(std::ostream &outFile,long long int maxBits);

			void parseHeaderHuffman(std::istream &inFile);
			void writeHeaderHuffman(std::ostream &outFile);
		public:
			BWTConfig bwtConfig; //
			RunLengthConfig rlConfig;
			HuffmanConfig huffConfig;
			/**
			 * Method responsible for reading the header and set the variable bwtConfig, rlConfig, huffConfig
			 * accordingly.
			 * @param headerFile Input file where header must be read from.
			 */
			void parseHeader(std::istream &headerFile);
			/**
			 * Method that verifies the variables bwtConfig, rlConfig, huffConfig in order to detect
			 * which algorithms were used. When one of them is detected, the coresponding
			 * header writer is called. 
			 * @param outFile Output Stream where header muust be written
			 */
			void writeHeader(std::ostream &outFile);
			
	};
#endif 
