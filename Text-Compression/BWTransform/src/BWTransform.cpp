#include "BWTransform.hpp"
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
// TODO: melhorar comparacao...
bool BWTCompare::operator() (int i,int j){
	// return (i<j);
	string str1;
	string str2;
	int k=0;
	//Testing the influence of compare complexity
	// return i<j;
	while(k<(this->str).size()){
		if(j==(this->str).size()){
			j=0;
		}
		if(i==(this->str).size()){
			i=0;
		}
		
		if((this->str)[i]<(this->str)[j]){
			return true; 
		}else if ((this->str)[i]>(this->str)[j]){
			return false;
		}
		k++;
		j++;
		i++;
	}

	// for(int k=i;k<(this->str).length();k++){
	// 	str1+=(this->str)[k];
	// }
	// for(int k=0;k<i;k++){
	// 	str1+=(this->str)[k];
	// }

	// for(int k=j;k<(this->str).length();k++){
	// 	str2+=(this->str)[k];
	// }
	// for(int k=0;k<j;k++){
	// 	str2+=(this->str)[k];
	// }
	// for(int k=0;k<(this->str).size();k++){
	// 	if(str1[k]<str2[k]){
	// 		return true; 
	// 	}else if (str1[k]>str2[k]){
	// 		return false;
	// 	}
	// }
	return true;
}
BWTCompare::BWTCompare(string str){
	this->str = str;
}
string BWTransform::decodeStream(std::istream &inFile,int blockSize,std::vector<int> &indices){
	string decodedString;
	stringstream outStream;
	char symbol;
	inFile>>noskipws;
	inFile>>symbol;
	string blockToDecode;
	
	vector<int>::iterator indicesIT = indices.begin();
	while(!inFile.eof()){
		blockToDecode+=symbol;//concatenate the char to the block to encode
		if(blockToDecode.size() == blockSize){//if it has reached the max block size
			decodedString= BWTransform::decodeBlock(blockToDecode,*indicesIT);//
			indicesIT++;
			// cout<<"\tBlock Encoded: "<<encodedString<<endl;
			blockToDecode=""; //clear the block
			outStream<<decodedString;
		}
		inFile>>symbol;
	}
	if(blockToDecode.size()>0){
		decodedString= BWTransform::decodeBlock(blockToDecode,*indicesIT);//
		indicesIT++;
		// cout<<"\tBlock Encoded: "<<encodedString<<endl;
		blockToDecode=""; //clear the block
		outStream<<decodedString;
	}
	return outStream.str();
}

string BWTransform::decodeBlock(string block,int a){
	// in this implementation, we are encoding bytes, then we have at most 256 symbls
	//Fonte: material disponibilizado no tidia
	int maxNumberOfSymbols = 256; 
	int blockSize;
	int sum;
	map<char,int> k;
	vector<int> c;
	map<char,int> m ;
	// cout<<"DECODING BLOCK: "<<block<<endl;
	stringstream resultStream;
	vector<char> Q;
	map<char,int> checkingSymbolQnt;
	blockSize=block.size();

	for(int i=0;i<blockSize;i++){
		checkingSymbolQnt[block[i]]=0;
	}

	maxNumberOfSymbols=checkingSymbolQnt.size();
	// for(int i=0;i<maxNumberOfSymbols;i++){
	// 	k[i]=0;
	// }
	for(map<char,int>::iterator 
			it= checkingSymbolQnt.begin()
		;it!=checkingSymbolQnt.end()
		;it++){
		k[it->first]=0;
	}
	for(int i=0;i<blockSize;i++){
		c.push_back(k[block[i]]);
		k[block[i]]=k[block[i]]+1;
	}
	sum=0;
	// for(int i=0;i<maxNumberOfSymbols;i++){
	// 	m[i]=sum;
	// 	sum=sum+k[i];
	// }
	for(map<char,int>::iterator 
			it= checkingSymbolQnt.begin()
		;it!=checkingSymbolQnt.end()
		;it++){

		m[it->first]=sum;
		sum=sum+k[it->first];
	}
	int y=a;
	for(int j=blockSize-1;j>=0;j--){
		Q.push_back(block[y]);
		y=c[y]+m[block[y]];
	}

	for(int i=blockSize-1;i>=0;i--){
		resultStream<<Q[i];
	}
	return resultStream.str();
}
string BWTransform::encodeBlock(string block,int *a){
	string result;
	int blockSize = block.length();
	vector<int> orderedIndexes(blockSize);
	for(int i=0;i<blockSize;i++){
		orderedIndexes[i]=i;
	}
	// cout<<"ENCODING BLOCK "<<block<<endl;
	BWTCompare myComp = BWTCompare(block);
	sort(orderedIndexes.begin(),orderedIndexes.end(),myComp);
	int localA;
	int indexOfCode;
	for(int i=0;i<blockSize;i++){
		if(orderedIndexes[i]==0){
			localA=i;
		}
		if(orderedIndexes[i]==0){
			indexOfCode=block.size()-1;
		}else{

			indexOfCode=orderedIndexes[i]-1;
		}
		result+=block[indexOfCode];
	}
	*a=localA;
	return result;


}
string BWTransform::encodeStream(std::istream &inFile,int blockSize,vector<int> &indices){
	string blockToEncode;
	char symbol;
	inFile>>noskipws;
	
	int a;
	string encodedString;

	stringstream outStream;

	indices.clear();
	inFile>>symbol;//read from file
	while(!inFile.eof()){//if reading attempt did not fail
		blockToEncode+=symbol;//concatenate the char to the block to encode
		if(blockToEncode.size() == blockSize){//if it has reached the max block size
			encodedString= BWTransform::encodeBlock(blockToEncode,&a);//
			// cout<<"\tBlock Encoded: "<<encodedString<<endl;
			indices.push_back(a);//store the value of A
			blockToEncode=""; //clear the block
			outStream<<encodedString;
		}
		inFile>>symbol;

	}
	if(blockToEncode.size()>0){//if the stream ended and the block was not complete
		encodedString= BWTransform::encodeBlock(blockToEncode,&a);//
		// cout<<"\tBlock Encoded: "<<encodedString<<endl;
		// cout<<"\tA = "<<a<<endl;
		indices.push_back(a);//store the value of A
		blockToEncode=""; //clear the block
		outStream<<encodedString;
	}
	return outStream.str();
}