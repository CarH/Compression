#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include "myIoBitStream.hpp"
#define SIZEBUFFER 1024 // It must be M(8)
#define LEFT_BIT 0
#define RIGHT_BIT 1
using namespace std;

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
 	Node(){}
	Node(long f, Node *l, Node *r) {
		freq 	= f;
		left 	= l;
		right 	= r;
		leaf 	= false;
	}
	Node(long f, char c) {
		freq 	= f;
		symbol 	= c;
		left 	= NULL;
		right 	= NULL;
		leaf	= true;
	}
	~Node() {}

	void setCode(char value) {
		code = value;
	} 

	long getFrequency() {
		return freq;
	}

	char getChar() {
		return symbol;
	}

	char getCode() {
		return code;
	}

	Node getLeft() {
		return *left;
	}
	
	Node* leftPtr() {
		return left;
	}
	
	Node getRight() {
		return *right;
	}

	Node* rightPtr() {
		return right;
	}

	bool isLeaf() {
		return leaf;
	}
};

// The root of the Huffman's tree
Node huffmanTreeRoot;
// Code table to store char - code
map<char, vector<char> > char_codeVector;

struct NodeComparison {
	bool operator() (Node &a, Node &b) const {
		return a.getFrequency() > b.getFrequency();
	}
};

/**
 * Build the Huffman's tree
 * @param map<char, long>  char_freq
 */
void buildHuffmanTree(map<char, long> &char_freq) {
	// Stores the nodes. The top node has the smallest frequency value
	priority_queue<Node, vector<Node>, NodeComparison> pq;

	// Inserting the leaves into the pq
	for (map<char, long>::iterator it = char_freq.begin(); it != char_freq.end(); it++) {
		Node l(it->second, it->first);
		pq.push(l);
	}

	// Creating inner nodes of the Huffman tree
	while (pq.size() > 1) {
		Node a = pq.top(); pq.pop(); a.setCode(0);
		Node b = pq.top(); pq.pop(); b.setCode(1);
		Node parent( (a.getFrequency()+b.getFrequency()), new Node(a), new Node(b) );
		pq.push(parent);
	}
	huffmanTreeRoot = pq.top(); pq.pop();
}

/**
 * Traverse the Huffman Tree and build Huffman Code Table from it
 * @param Node 			no 			current node to process
 * @param vector<char> 	codeVector	vector to store the 'bits' of the code
 */
void createCodeTable(Node no, vector<char> codeVector=vector<char>()) {
	if (no.isLeaf()) {
		char_codeVector[no.getChar()] = codeVector;
		return;
	}
	// Go to the left
	codeVector.push_back(LEFT_BIT);
	createCodeTable(no.getLeft(), codeVector);

	// Go to the right
	codeVector.pop_back(); codeVector.push_back(RIGHT_BIT);
	createCodeTable(no.getRight(), codeVector);
}

/**
 * Calculate the frequency that a character occurs in the file
 * @param ifstream inFile input file
 */
map<char, long> calculateFrequency(ifstream &inFile) {
	map<char, long> char_freq;
	char value;
	while (inFile.get(value)) {
		if (char_freq.find(value) == char_freq.end()) {
			char_freq[value] = 1;
		}
		else {
			char_freq[value]++;
		}
	}
	return char_freq;
}

void printBitString(string str) {
	char aux;
	char mask = 0x80;
	for (int i = 0; i < str.length(); i++) {
		aux = str[i];
		for (int j = 0; j < 8; j++) {
			cout << ((aux & mask) ? "1" : "0");
			aux = aux << 1;
		}
	}
}

void huffmanEncode(ifstream &inFile, string outFileName) {
	char value;
	int buffCnt;
	ofstream outFile;
	vector<char> codeVector;
	MyIOBitStream bitStream;

	inFile.clear(); inFile.seekg(0, inFile.beg);
	outFile.open(outFileName.c_str(), ios::binary | ios::in | ios::trunc);
	if (outFile.is_open()) {
		buffCnt = 0;
		while (inFile.get(value)) {
			codeVector = char_codeVector[value];
			// cerr << " > Recording into output file [" << value <<"]: ";
			// for each bit of the code
			for (int i = 0; i < codeVector.size(); i++) {
				if (codeVector[i] & 1) {
					bitStream.appendBit(true);
					// cerr << "1";
				}
				else {
					bitStream.appendBit(false);
					// cerr << "0";
				}
				buffCnt++;
				if (buffCnt == SIZEBUFFER) { // Record the buffer into the output file
					///////////////////////////////////////////////////
					// Test
					// cout << "Atingiu SIZEBUFFER!\n";
					// string str1 = bitStream.getString();
					// printBitString(str1);
					// outFile << str1;
					//////////////////////////////////////////
					outFile << bitStream.getString();
					buffCnt = 0;
				}
			}
			// cerr << "\n";
		}
		if (buffCnt > 0) {
			///////////////////////////////////////////////////
			// Test
			// string str = bitStream.getString();
			// outFile << str;
			// printBitString(str);
			///////////////////////////////////////////////////
			outFile << bitStream.getString();
		}
		outFile.close();
	}
	else {
		cerr << " ERROR: Could not open the file " << outFileName << "\n";
	}
}

void huffmanDecode(ifstream &inFile, string outFileName) {
	/**
		TODO:
		- get the number of valid bits on the last byte and use it to check
	 */
	char byte;
	char bit;
	char mask = 0x80;
	ofstream outFile;
	string buffer;

	Node *currNode = &huffmanTreeRoot;
	outFile.open(outFileName.c_str(), ios::out | ios::trunc);
	while (!inFile.eof()) {
		inFile.get(byte);
		for (int i = 0; i < 8; i++) {
			bit = (byte & mask);
			byte <<= 1;
			if (bit == LEFT_BIT) {
				currNode = currNode->leftPtr();
			}
			else {
				currNode = currNode->rightPtr();
			}
			if (currNode->isLeaf()) {
				// cout << currNode->getChar();
				buffer += currNode->getChar();
				if (buffer.length() == SIZEBUFFER) {
					outFile << buffer;
					buffer.clear();
				}
				currNode = &huffmanTreeRoot;
			}
		}
	}
	if (buffer.length() > 0)
		outFile << buffer;
	outFile.close();
}



void printCodeTable() {
	cout << "--------------------------\n + Code Table:\n";
	for (map<char, vector<char> >::iterator it = char_codeVector.begin(); it != char_codeVector.end(); it++) {
		cout << "  [" << it->first << "] : ";
		for (vector<char>::iterator v = it->second.begin(); v != it->second.end(); v++) {
			cout << (((*v) & 1) ? "1" : "0"); 
		}
		cout << "\n";
	}
}

void printHuffmanTree(Node &no) {
	if (no.isLeaf()) {
		cout << "\t- (LEAF) char: " << no.getChar() << ", frequency: " << no.getFrequency()  << ", code: ";
		if (no.getCode() & 1) {
			cout << "1\n";
		}
		else {
			cout << "0\n"; 
		}
		return;
	}
	cout << "  - (INNER) frequency: " << no.getFrequency() << ", code: " << (int)no.getCode() << "\n";
	Node n = no.getLeft();
	printHuffmanTree(n);
	n = no.getRight();
	printHuffmanTree(n);
}

void printMap(map<char, long>& m) {
	cout << " >> Frequency Table:\n";
	for (map<char, long>::iterator it = m.begin(); it != m.end(); it++) {
		cout << " " << it->first << " -> " << it->second << "\n";
	}
}

int main(int argc, char const *argv[]) {
	/* Input  Parameters*/
	ifstream inFile;
	ofstream outFile;
	map<char, long> char_freq;
	string inFileName, outFileName;
	char value;

	inFileName  = argv[1];
	outFileName = argv[2];


	// MyIOBitStream bitStream;
	// bitStream.appendBit(false);
	// bitStream.appendBit(true);
	// bitStream.appendBit(false);
	// bitStream.appendBit(false);
	// bitStream.appendBit(false);
	// bitStream.appendBit(false);
	// bitStream.appendBit(false);
	// bitStream.appendBit(false);

	inFile.open(inFileName.c_str());
	if (inFile.is_open()) {
		char_freq = calculateFrequency(inFile);

		printMap(char_freq);
		
		buildHuffmanTree(char_freq);

		// printHuffmanTree(huffmanTreeRoot);
		
		createCodeTable(huffmanTreeRoot);
		
		printCodeTable();
		
		huffmanEncode(inFile, outFileName);

		inFile.close();
		inFile.clear(); inFile.open(outFileName.c_str(), ios::in | ios::binary);
		
		outFileName = "text50huffmanDecode.txt";
		huffmanDecode(inFile, outFileName);
		inFile.close();
	}

	return 0;
}