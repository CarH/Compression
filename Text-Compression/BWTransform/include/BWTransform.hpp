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
			static std::string decodeStream(std::istream &inFile,int blockSize,std::vector<int> &indices);			
			static std::string decodeBlock(std::string block,int a);
			static std::string encodeBlock(std::string block,int *a);
			static std::string encodeStream(std::istream &inFile,int blockSize,std::vector<int> &indices);			
			
		
	};
#endif 
