#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;


class Node {
	long freq;
	Node *left;
	Node *right;

public:
	Node();
	~Node();
	
};


class Leaf: public Node {
	char symbol;
public:
	Leaf();
	~Leaf();
	
};


void printMap(map<char, long>& m) {
	cout << " >> Frequency Table:\n";
	for (map<char, long>::iterator it = m.begin(); it != m.end(); it++) {
		cout << " " << it->first << " -> " << it->second << "\n";
	}
}

int main(int argc, char const *argv[]) {
	/* Input  Parameters*/
	ifstream inFile;
	string inFileName, outFileName;
	char value;
	map<char, long> char_freq;

	inFileName = argv[1];

	cout << "trying to open " << inFileName << endl;
	inFile.open(inFileName.c_str());
	if (inFile.is_open()) {
		while ( inFile.get(value) ) {
			if (char_freq.find(value) == char_freq.end()) {
				char_freq[value] = 1;
			}
			else {
				char_freq[value]++;			
			}
		}
		printMap(char_freq);

	}
	

	return 0;
}