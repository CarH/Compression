#include <sstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#ifndef __MY_BW_TRANSFORM__
	#define __MY_BW_TRANSFORM__
	class BWTCompare{
		private:
			std::string str;
		public:
			BWTCompare(std::string str); 
			bool operator() (int i,int j); 
	};
	class BWTransform{
		private:
			

		public:
			BWTransform();
			~BWTransform();
			/**
			 * Iterates through a stream, separating it in blocks of size "blockSize". For each block, decodeBlock() is called in order to 
			 * decode each block. The results of decoding routines are concatenated and returned as a string
			 * @param  inFile    Input stream with original data
			 * @param  blockSize Size of block desired
			 * @param  indices   Vector from where the indices "a" of each block is read. The index "a" indicates which position in the encoded block corresponds to the original string
			 * @return           [description]
			 */
			static std::string decodeStream(std::istream &inFile,int blockSize,std::vector<int> &indices);
			/**
			 * Effectivelly decode a block of string encoded by BW Transform.
			 * @param  block Block to be decoded
			 * @param  a     Index of the encoded string that corresponds to the original.
			 * @return       Returns the original string.
			 */
			static std::string decodeBlock(std::string block,int a);
			/**
			 * Encode a given block using BW Transform. 
			 * @param  block block to be encoded
			 * @param  a     Output parameter. Represents the index of the encoded block that represents the original string
			 * @return       returns the encoded block
			 */
			static std::string encodeBlock(std::string block,int *a);
			/**
			 * Iterates through a stream, separating it in blocks of size "blockSize". For each block, encodeBlock() is called in order to 
			 * encode each block. The results of encoding routines are concatenated and returned as a string
			 * @param  inFile    Input stream
			 * @param  blockSize Desired block size
			 * @param  indices   Output Param. Returns the indices "a" of each block
			 * @return           Returns the encoded string
			 */
			static std::string encodeStream(std::istream &inFile,int blockSize,std::vector<int> &indices);			
			
		
	};
#endif 
