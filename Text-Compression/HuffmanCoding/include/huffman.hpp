#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#define SIZEBUFFER 1024 		// ! It must be M(8)
#define LEFT_BIT 0				// used to traverse the Huffman Tree
#define RIGHT_BIT 1				// used to traverse the Huffman Tree

#define DEBUG false				// Debug Flag

#ifndef __HUFFMAN_HPP__
	#define __HUFFMAN_HPP__
	
	// Node class is the basic structure of the Huffman's Tree
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

	// My 'garbage collector' to clean up the dynamic memory allocated
	extern std::vector<Node*> collector;

	// Comparison function used to build the Huffman Tree. 
	// Mantain the nodes sorted in the priority queue so that the top element always be the
	// one with the lowest frequency.
	class NodeComparison {
		public:
			bool operator() (Node &a, Node &b) const;
	};

	/**
	 * Delete the dynamic memory allocated during the Huffman Tree constuction
	 */
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
	 * Calculates the frequency of each character of the input stream inFile
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

	/**
	 * Reads the Huffman's header from the input file stream inFile.
	 * @param  inFile            	input stream to be encoded
	 * @param  validBitsLastByte 	number of valid bits in the last byte
	 * @return 						the map char-frequency build based on the header
	 */
	std::map<char, long long> readHuffmanHeader(std::istream &inFile, int *validBitsLastByte);

	/**
	 * Takes an input stream inFile and encode it using the Huffman code associated with each character
	 * that is stored into the char_freq map. In the last byte of the stream only the valid bits (validBitsLastByte)
	 * are taking into account.
	 * @param  inFile            input stream to be encoded
	 * @param  char_freq         maps each char to its correspondent Huffman code
	 * @param  validBitsLastByte number of valid bits in the last byte
	 * @return                   a string that contains the inFile stream encoded
	 */
	std::string huffmanEncode(std::istream &inFile, std::map<char, long long> &char_freq, int *validBitsLastByte);

	/**
	 * Takes an input stream encoded with Huffman encode algorithm and decodes it properly. In the last byte
	 * of the input stream only valid bits (validBitsLastByte) are taking into account.
	 * @param inFile            Huffman encoded input file stream 
	 * @param outFile           output stream used to write the decoded stream
	 * @param validBitsLastByte number of valid bits in the last byte
	 */
	void huffmanDecode(std::istream &inFile, std::ostream &outFile, int validBitsLastByte);


	/*==========================================
	=            Auxilary Functions            =
	==========================================*/
	void printCodeTable();

	void printHuffmanTree(Node *no);

	void printMap(std::map<char, long long>& m);
	/*=====  End of Auxilary Functions  ======*/

#endif