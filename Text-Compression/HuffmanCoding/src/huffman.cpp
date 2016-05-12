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
	
	Node* getLeft() {
		return left;
	}
	
	Node* getRight() {
		return right;
	}

	bool isLeaf() {
		return leaf;
	}
};

// The root of the Huffman's tree
Node huffmanTreeRoot;

// Code table to store char - code
std::map<char, vector<char> > char_codeVector;

// My 'garbage collector'
std::vector<Node*> collector;

// TODO: 
void freeDynamicMemory() {
	for (vector<Node*>::iterator it = collector.begin(); it != collector.end(); it++) {
		delete *it;
	}
}

struct NodeComparison {
	bool operator() (Node &a, Node &b) const {
		return a.getFrequency() > b.getFrequency();
	}
};

/**
 * Build the Huffman's tree based on the frequency of each character
 * @param map<char, long>	char_freq 	stores the frequency of each character
 */
void buildHuffmanTree(std::map<char, long long> &char_freq) {
	// Stores the nodes. The top node has the smallest frequency value
	priority_queue<Node, vector<Node>, NodeComparison> pq;

	// Inserting the leaves into the pq
	for (map<char, long long>::iterator it = char_freq.begin(); it != char_freq.end(); it++) {
		Node l(it->second, it->first);
		pq.push(l);
	}

	// Creating inner nodes of the Huffman tree
	if (pq.size() > 1) {
		while (pq.size() > 1) {
			Node a = pq.top(); pq.pop(); a.setCode(0);
			Node b = pq.top(); pq.pop(); b.setCode(1);
			Node *dynamicA = new Node(a);
			Node *dynamicB = new Node(b);
			Node parent( (a.getFrequency()+b.getFrequency()), dynamicA, dynamicB );
			pq.push(parent);
			collector.push_back(dynamicA);	collector.push_back(dynamicB); 
		}
	}
	else { // The tree has just one leaf: append it to the parent = root
		Node c = pq.top(); pq.pop(); c.setCode(0);
		Node *dynamicN = new Node(c);
		Node parent(c.getFrequency(), dynamicN, NULL);
		pq.push(parent);
	}
	huffmanTreeRoot = pq.top(); pq.pop();
}

/**
 * Traverse the Huffman Tree and build Huffman Code Table from it
 * @param Node 			no 			current node to process
 * @param vector<char> 	codeVector	vector to store the 'bits' of the code
 */
void createCodeTable(Node *no, std::vector<char> codeVector=vector<char>()) {
	if (no->isLeaf()) {
		char_codeVector[no->getChar()] = codeVector;
		return;
	}
	// Go to the left
	if (no->getLeft() != NULL) {
		codeVector.push_back(LEFT_BIT);
		createCodeTable(no->getLeft(), codeVector);
	}

	// Go to the right
	if (no->getRight() != NULL) {
		codeVector.pop_back(); codeVector.push_back(RIGHT_BIT);
		createCodeTable(no->getRight(), codeVector);
	}
}

/**
 * Calculate the frequency that a character occurs in the file
 * @param ifstream inFile input file
 */
map<char, long long> calculateFrequency(std::istream &inFile) {
	map<char, long long> char_freq;
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

void printBitString(std::string str) {
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


void writeHuffmanHeader(std::ofstream &outFile, std::map<char, long long> &char_freq, int validBitsLastByte) {
	long long length = char_freq.size(); // The number of pairs char-frequency that will appear
	
	cout << " validBitsLastByte: " << validBitsLastByte  << ", length: " << length<<endl;
	// Write H - Huffman Algorithm
	outFile << "H ";
	outFile << validBitsLastByte;
	outFile << " ";
	outFile << length;
	outFile << " ";

	for (map<char, long long>::iterator it = char_freq.begin(); it != char_freq.end(); it++) {
		outFile << it->first << " " << it->second << " ";
	}
	// Write the final delimiter
	outFile << (char)0; // TODO : DELETE THIS BITCH
}

std::map<char, long long> readHuffmanHeader(std::ifstream &inFile, int *validBitsLastByte) {
	long long length, frequency;
	char space, character, space2, algorithm;
	map<char, long long> char_freq;

	inFile >> algorithm;
	cout << " + algorithm: [" << algorithm <<"]" <<endl;
	inFile >> (*validBitsLastByte);
	cout << " + validBitsLastByte: " << *validBitsLastByte <<endl;
	inFile >> length;
	cout << " + length: " << length << endl;

	inFile.get(space);
	cout << " - It was suppose to be a space: [" << space << "]\n";

	for (int i = 0; i < length; i++) {
		inFile.get(character);
		inFile.get(space);
		inFile >> frequency;
		inFile.get(space2);
		cout << "character: " << character << ", space: "<< space << ", frequency: "<< frequency << ", space2: " << space2 <<endl;
		cout << "char_freq["<<character<<"] : " << frequency << endl;
		char_freq[character] = frequency;
	}
	inFile.get(character);
	cout << " - It was suppose to be a 0: [" << character << "]\n";
	return char_freq;
}


void huffmanEncode(std::istream &inFile, std::map<char, long long> &char_freq, std::string outFileName) {
	char value;
	int buffCnt;
	int validBitsLastByte = 0;
	ofstream outFile;
	stringstream ssFileEncoded;
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
					// outFile << bitStream.getString();
					ssFileEncoded << bitStream.getString();
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
			validBitsLastByte = bitStream.getBitsWriteCounter();
			cout << " Number of valid bits in the last byte: " << validBitsLastByte <<endl;
			ssFileEncoded << bitStream.getString();
			// outFile << bitStream.getString(); // TODO: Descomentar essa linha e remover as de cima!!
		}
		// Write the header
		writeHuffmanHeader(outFile, char_freq, validBitsLastByte);
		outFile << ssFileEncoded;
		outFile.close();
	}
	else {
		cerr << " ERROR: Could not open the file " << outFileName << "\n";
	}
}

void huffmanDecode(std::istream &inFile, std::string outFileName, int validBitsLastByte) {
	/**
		TODO:
		- SET the number of valid bits BEFORE CALL THIS FUNCTION!!
	 */
	char byte;
	char bit;
	char mask = 0x80;
	long long iniOffset, 
			inLength, 
			byteCounter = 0; // Used to know when the last byte was reached
	ofstream outFile;
	string buffer = "";

	// Get the initial offset of the inFile
	iniOffset = inFile.tellg();
	cout << " iniOffset: " << iniOffset<<endl;

	// Get the length of the inFile
	inFile.seekg(0, inFile.end);
	inLength = inFile.tellg();
	inFile.seekg(iniOffset, inFile.beg);
	byteCounter = inLength - iniOffset;

	cout << " byteCounter: "<<byteCounter<<endl;

	Node *currNode = &huffmanTreeRoot;
	outFile.open(outFileName.c_str(), ios::out | ios::trunc);
	while (!inFile.eof() && byteCounter--) {
		inFile.get(byte);
		for (int i = 0; i < 8; i++) {
			// The last byte approach: just the valid bits must be considered
			if (byteCounter == 0 && i >= validBitsLastByte)
				break;
			bit = (byte & mask);
			byte <<= 1;
			if (bit == LEFT_BIT) {
				currNode = currNode->getLeft();
			}
			else {
				currNode = currNode->getRight();
			}
			if (currNode->isLeaf()) {
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

/**
 * DELETE
 * @param no [description]
 */
void printHuffmanTree(Node *no) {
	if (no->isLeaf()) {
		cout << "\t- (LEAF) char: " << no->getChar() << ", frequency: " << no->getFrequency()  << ", code: ";
		if (no->getCode() & 1) {
			cout << "1\n";
		}
		else {
			cout << "0\n"; 
		}
		return;
	}
	cout << "  - (INNER) frequency: " << no->getFrequency() << ", code: " << (int)no->getCode() << "\n";
	
	if (no->getLeft() != NULL)
		printHuffmanTree(no->getLeft());
	if (no->getRight() != NULL)
		printHuffmanTree(no->getRight());
}


void printMap(std::map<char, long long>& m) {
	cout << " >> Frequency Table:\n";
	for (map<char, long long>::iterator it = m.begin(); it != m.end(); it++) {
		cout << " " << it->first << " -> " << it->second << "\n";
	}
}

int main(int argc, char const *argv[]) {
	/* Input  Parameters*/
	ifstream inFile;
	ofstream outFile;
	map<char, long long> char_freq;
	string inFileName, outFileName;
	int validBitsLastByte;
	char value;

	inFileName  = argv[1];
	outFileName = argv[2];

	inFile.open(inFileName.c_str());
	if (inFile.is_open()) {
		char_freq = calculateFrequency(inFile);

		printMap(char_freq);
		
		buildHuffmanTree(char_freq);

		// printHuffmanTree(huffmanTreeRoot);
		
		createCodeTable(&huffmanTreeRoot);
		
		printCodeTable();
		
		huffmanEncode(inFile, char_freq, outFileName);

		inFile.close();
		freeDynamicMemory();

		//// Decode test
		inFile.clear(); 
		inFile.open(outFileName.c_str(), ios::in | ios::binary);
		char_freq = readHuffmanHeader(inFile, &validBitsLastByte);
		
		cout << " >>> char_freq after read Header:"<<endl;
		printMap(char_freq);


		cout << " >>> Building new Huffman tree"<<endl;
		buildHuffmanTree(char_freq);
		
		createCodeTable(&huffmanTreeRoot);
		printCodeTable();
		
		outFileName = "inhuffmanDecode.txt";
		huffmanDecode(inFile, outFileName, validBitsLastByte);
		inFile.close();
	}

	return 0;
}