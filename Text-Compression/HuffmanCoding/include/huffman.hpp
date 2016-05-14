#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#define SIZEBUFFER 1024 		/* ! It must be M(8) */
#define LEFT_BIT 0
#define RIGHT_BIT 1

#ifndef __HUFFMAN_HPP__
	#define __HUFFMAN_HPP__
	// Node structure of the Huffman's Tree
	class Node {
		 private:
			long freq;
			char symbol;
			char code;
			Node *left;
			Node *right;
			bool leaf;
		 public:
		 	Node();
			Node(long f, Node *l, Node *r);
			Node(long f, char c);
			~Node();
			void setCode(char value);
			long getFrequency();
			char getChar();
			char getCode();
			Node* getLeft();
			Node* getRight();
			bool isLeaf();
	};

	// The root of the Huffman's tree
	extern Node huffmanTreeRoot;

	// Code table to store char - code
	extern std::map<char, std::vector<char> > char_codeVector;

	// My 'garbage collector'
	extern std::vector<Node*> collector;

	class NodeComparison {
		public:
			bool operator() (Node &a, Node &b) const;
	};

	void freeDynamicMemory();

	/**
	 * Build the Huffman's tree based on the frequency of each character
	 * @param map<char, long>	char_freq 	stores the frequency of each character
	 */
	void buildHuffmanTree(std::map<char, long long> &char_freq);

	/**
	 * Traverse the Huffman Tree and build Huffman Code Table from it
	 * @param Node 			no 			current node to process
	 * @param vector<char> 	codeVector	vector to store the 'bits' of the code
	 */
	void createCodeTable(Node *no, std::vector<char> codeVector=std::vector<char>());

	/**
	 *
	 * @param 	inFile the input stream related to the 
	 * @return 	the char - frequency map for each character in the inFile
	 */
	std::map<char, long long> calculateFrequency(std::istream &inFile);

	/**
	 * Write the Huffman header into the outFile in the current possition
	 * @param outFile           the output file that will store the encoded file
	 * @param char_freq         the frequency of each character encoded
	 * @param validBitsLastByte number of valid bits in the last byte
	 */
	void writeHuffmanHeader(std::ostream &outFile, std::map<char, long long> &char_freq, int validBitsLastByte);

	std::map<char, long long> readHuffmanHeader(std::istream &inFile, int *validBitsLastByte);

	std::string huffmanEncode(std::istream &inFile, std::map<char, long long> &char_freq, int *validBitsLastByte);

	void huffmanDecode(std::istream &inFile, std::ostream &outFile, int validBitsLastByte);


	/*==========================================
	=            Auxilary Functions            =
	==========================================*/
	void printCodeTable();

	void printHuffmanTree(Node *no);

	void printMap(std::map<char, long long>& m);
	/*=====  End of Auxilary Functions  ======*/

#endif