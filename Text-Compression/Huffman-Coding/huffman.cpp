#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

// Node structure of the Huffman's Tree
class Node {
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

	Node getRight() {
		return *right;
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
 * buildHuffmanTree
 *  Build the Huffman's tree
 * @param char_freq a map whose key is a char and the value is the frequency that it occurs
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

void createCodeTable(Node no, vector<char> mStack=vector<char>()) {
	if (no.isLeaf()) {
		char_codeVector[no.getChar()] = mStack;
		return;
	}
	// Go to the left
	Node n = no.getLeft();
	mStack.push_back(0);
	createCodeTable(n, mStack);

	// Go to the right
	mStack.pop_back(); mStack.push_back(1);
	n = no.getRight();
	createCodeTable(n, mStack);
}

void huffmanEncode(Node &huffmanTreeRoot, string outFileName) {
	ofstream outFile;
	outFile.open(outFileName.c_str(), ios_base::binary);

	if (outFile.is_open()) {

	}
	else {
		cerr << " ERROR: Could not open the file " << outFileName << "\n";
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
	string inFileName, outFileName;
	char value;
	map<char, long> char_freq;

	inFileName = argv[1];

	cout << "trying to open " << inFileName << endl;
	inFile.open(inFileName.c_str());
	if (inFile.is_open()) {
		while (inFile.get(value)) {
			cout << value;
			if (char_freq.find(value) == char_freq.end()) {
				char_freq[value] = 1;
			}
			else {
				char_freq[value]++;
			}
		}
		cout << endl;
		printMap(char_freq);
		cout << "chamou buildHuffmanTree!\n"; cout.flush();
		buildHuffmanTree(char_freq);

		printHuffmanTree(huffmanTreeRoot);
		
		createCodeTable(huffmanTreeRoot);
		printCodeTable();

		inFile.close();
	}

	return 0;
}