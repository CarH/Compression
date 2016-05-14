#include "BWTransform.hpp"
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
// TODO: melhorar comparacao...
bool BWTCompare::operator() (int i,int j){
	// return (i<j);
	// 
	string str1;
	string str2;
	int k=0;
	//compara as strings caracter a caracter.
	//Variaveis i e j correspondem a posicoes na string original (armazenada em this->str)
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
	return false;
}
BWTCompare::BWTCompare(string str){
	this->str = str;
}
string BWTransform::decodeStream(std::istream &inFile,int blockSize,std::vector<int> &indices){
	string decodedString;
	stringstream outStream;
	char symbol;
	inFile>>noskipws;
	string blockToDecode; // string que contera o bloco a ser decodificado
	//O vetor "indices" contem o indice "a" necessario para decodificacao. Cada posicao
	//	corresponde ao indice de um bloco. Portanto, precisa-se iterar sobre esse vetor a cada bloco 
	vector<int>::iterator indicesIT = indices.begin();
	inFile>>symbol;//le um caracter
	while(!inFile.eof()){ 
		blockToDecode+=symbol;// concatena o simbolo lido de forma a ir montando um bloco
		if(blockToDecode.size() == blockSize){//se atingiu o tamanho do bloco
			decodedString= BWTransform::decodeBlock(blockToDecode,*indicesIT);//
			indicesIT++;
			// cout<<"\tBlock Encoded: "<<encodedString<<endl;
			blockToDecode=""; //limpa a variavel para construir o proximo bloco
			outStream<<decodedString;
		}
		inFile>>symbol;
	}
	//Na maneira como eh definida, o ultimo bloco pode ser de tamanho menor que os demais
	//caso isso ocorra, o if de dentro do laco acima nao sera atingido para o ultimo bloco
	//Entao, realizar  a decodificacao aqui
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
	
	//Fonte: pseudo-codigo presente no material disponibilizado no tidia
	int maxNumberOfSymbols = 256; //numero inicial que sera modificado depois
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
	//insere cada caracter em uma estrutura de map. Dessa maneira, simbolos iguais so serao inseridos uma unica vez
	////desse modo, eh possivel contabilizar a quantidade de simbolos diferentes presentes no bloco
	for(int i=0;i<blockSize;i++){
		checkingSymbolQnt[block[i]]=0;
	}
	//seta a quantidade de simbolos com a contagem de characteres diferentes realizada acima
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
	vector<int> orderedIndexes;//vetor que armazenara os indices da string ordenada
	//inicializa o vetor
	for(int i=0;i<blockSize;i++){
		orderedIndexes.push_back(i);
	}
	
	// cout<<"ENCODING BLOCK "<<block<<endl;
	BWTCompare myComp = BWTCompare(block);
	//ordena a string, armazenando os indices do inicio de cada umadas rotacoes no vetor orderedIndexes
	sort(orderedIndexes.begin(),orderedIndexes.end(),myComp);
	int localA;
	int indexOfCode;
	for(int i=0;i<blockSize;i++){
		if(orderedIndexes[i]==0){
			localA=i;//indica onde esta a string que representa o bloco iriginal
		}
		//os caracteres do bloco codificado sao os ultimos caracteres das strings ordenadas
		//estes se encontram no indice imedeatametne anterior ao indice inicial
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
	string blockToEncode; //buffer sobre o qual vai send contruindo os blocos
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
	/*
		Logica do if: vide decodeBlock
	 */
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