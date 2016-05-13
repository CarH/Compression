#include "huffman.hpp"
#include "myIoBitStream.hpp"
using namespace std;

Node huffmanTreeRoot;
std::map<char, std::vector<char> > char_codeVector;
std::vector<Node*> collector;

Node::Node() {}
Node::Node(long f, Node *l, Node *r) {
	freq 	= f;
	left 	= l;
	right 	= r;
	leaf 	= false;
}
Node::Node(long f, char c) {
	freq 	= f;
	symbol 	= c;
	left 	= NULL;
	right 	= NULL;
	leaf	= true;
}
Node::~Node() {}

void Node::setCode(char value) {
	code = value;
} 

long Node::getFrequency() {
	return freq;
}

char Node::getChar() {
	return symbol;
}

char Node::getCode() {
	return code;
}

Node* Node::getLeft() {
	return left;
}

Node* Node::getRight() {
	return right;
}

bool Node::isLeaf() {
	return leaf;
}

void freeDynamicMemory() {
	for (vector<Node*>::iterator it = collector.begin(); it != collector.end(); it++) {
		delete *it;
	}
}

bool NodeComparison::operator() (Node &a, Node &b) const {
	return a.getFrequency() > b.getFrequency();
}

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
	else { // If the tree has just one leaf: append it to a parent node = root
		Node c = pq.top(); pq.pop(); c.setCode(0);
		Node *dynamicN = new Node(c);
		Node parent(c.getFrequency(), dynamicN, NULL);
		pq.push(parent);
	}
	huffmanTreeRoot = pq.top(); pq.pop();
}

void createCodeTable(Node *no, std::vector<char> codeVector) {
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

void writeHuffmanHeader(std::ostream &outFile, std::map<char, long long> &char_freq, int validBitsLastByte) {
	long long length = char_freq.size(); // The number of pairs char-frequency that will appear
	
	cout << " validBitsLastByte: " << validBitsLastByte  << ", length: " << length<<endl;
	
	outFile << "H ";					// H - Huffman Algorithm
	outFile << validBitsLastByte;		// valid bits of the last byte
	outFile << " ";						// space separator
	outFile << length;					// size of the char - frequency list
	outFile << " ";						// space separator

	for (map<char, long long>::iterator it = char_freq.begin(); it != char_freq.end(); it++) {
		outFile << it->first << " " << it->second << " ";
	}
}

std::map<char, long long> readHuffmanHeader(std::istream &inFile, int *validBitsLastByte) {
	long long length, frequency;
	char space, character, space2, algorithm;
	map<char, long long> char_freq;
	inFile >> skipws;
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
	return char_freq;
}


std::string huffmanEncode(std::istream &inFile, std::map<char, long long> &char_freq, int *validBitsLastByte) {
	char value;
	int buffCnt;
	stringstream ssFileEncoded;
	vector<char> codeVector;
	MyIOBitStream bitStream;

	inFile.clear(); inFile.seekg(0, inFile.beg);
	buffCnt = 0;
	while (inFile.get(value)) {
		codeVector = char_codeVector[value];
		// for each bit of the code
		for (int i = 0; i < codeVector.size(); i++) {
			if (codeVector[i] & 1) {
				bitStream.appendBit(true);
			}
			else {
				bitStream.appendBit(false);
			}
			buffCnt++;
			if (buffCnt == SIZEBUFFER) { // Record the buffer into the output file
				ssFileEncoded << bitStream.getString();
				buffCnt = 0;
			}
		}
	}
	if (buffCnt > 0) {
		*validBitsLastByte = bitStream.getBitsWriteCounter();
		cout << " Number of valid bits in the last byte: " << *validBitsLastByte <<endl;
		ssFileEncoded << bitStream.getString();
	}
	
	return ssFileEncoded.str();
}

void huffmanDecode(std::istream &inFile, std::ostream &outFile, int validBitsLastByte) {
	char byte;
	char bit;
	char mask = 0x80;
	long long iniOffset, 
			inLength, 
			byteCounter = 0; // Used to know when the last byte was reached
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
	while (!inFile.eof() && byteCounter--) {
		inFile.get(byte);
		for (int i = 0; i < 8; i++) {
			// If it is the last byte : just the valid bits must be considered
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
}

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