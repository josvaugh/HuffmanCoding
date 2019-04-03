/*
	Joseph Vaughn Greedy Algorithms
*/

#include<iostream>
#include<queue>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

//create dynamic arrays (my preference) 
vector<char> letters;
vector<int> frequencies;
vector<struct Encoding*> encode;

struct Node {
	Node(char data, int freq) : 
		left(nullptr), right(nullptr), 
		data(data), freq(freq) {}
	char data;
	int freq;
	Node *left, *right;
};

struct Comparator {
	bool operator() (Node* left, Node* right) {
		return (left->freq > right->freq);
	}
};

struct Encoding {
	Encoding(char data, string enc) : data(data), enc(enc) {}
	char data;
	string enc;
};

struct sortAscii
{
	inline bool operator() (const Encoding* enc1, const Encoding* enc2)
	{
		return (enc1->data < enc2->data);
	}
};

void openText(string myFile) {
	ifstream input(myFile);
	string line;
	if (input.fail()) {
		cout << "Error: Cannot Open File, Try again. " << endl;
		exit(1);
	}
	while (!input.eof()) {
		getline(input, line);
		string letter = line.substr(0, line.find(' '));
		string frequency = line.substr(line.find(' ') + 1);
		if (letter != "LF") {
			letters.push_back(letter[0]);
		}
		else {
			letters.push_back('\n');
		}
		frequencies.push_back(stoi(frequency));
	}
}

struct Node* createTree() {
	struct Node *left, *right, *top = nullptr;
	//Create a min heap to store Huffman nodes
	priority_queue<Node*, vector<Node*>, Comparator> minHeap;

	for (int i = 0; i < frequencies.size(); i++) {
		minHeap.push(new Node(letters[i], frequencies[i]));
	}
	//Iterate until we have a single node in our min heap which is for our tree
	while (minHeap.size() != 1) {
		//Obtain the first 2 values from the minHeap
		left = minHeap.top();
		minHeap.pop();
		right = minHeap.top();
		minHeap.pop();

		//Create a new node then sum the two frequencies and set the data with a '*'
		top = new Node('*', left->freq + right->freq);
		top->left = left;
		top->right = right;

		//Push the new internal node onto the min heap
		minHeap.push(top);
	}
	//return the tree
	return top;
}

void getCodes(struct Node* root, string str) {
	//base case
	if (!root) return;

	//store non internal nodes
	if (root->data != '*') {
		encode.push_back(new Encoding(root->data, str));
	}

	/*
	recursively traverse the tree,
		so if we go left we will add a 0
		and right, we add 1
	*/
	getCodes(root->left, str + "0");
	getCodes(root->right, str + "1");
}

void getResults(string myFile) {

	//sort the encodingss by ASCII value of data
	sort(encode.begin(), encode.end(), sortAscii());

	ofstream output;
	output.open(myFile);
	for (int i = 0; i < encode.size(); i++) {
		if (encode[i]->data != '\n') {
			cout << encode[i]->data << ":" << encode[i]->enc << endl;
			output << encode[i]->data << ":" << encode[i]->enc << endl;
		}
		else {
			cout << "LF:" << encode[i]->enc << endl;
			output << "LF:" << encode[i]->enc << endl;
		}

	}
	output.close();
	cout << "\nNow go to the codetable.txt! "<< endl;

}

int main() {
	openText("freq.txt");
	getCodes(createTree(), "");
	getResults("codetable.txt");
	cin.get();//pause
	return 0;
}