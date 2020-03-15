#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;
int wordFinder(ifstream& file, string word);
int hashFunction(string str);
//nodes used for chaining when collisions occur
class Node {
public:
	string key="";
	Node* next=NULL;
public: Node() {
	string key = "";
	Node* next = NULL;
}
public: Node(string k) {
	string key = k;
	Node* next = NULL;
}
};

int main() {

	string str;
	ifstream file;
	file.open("D:/DataStructures/Dictionary.txt");
	if (!file.is_open()) {
		cout << "File not opened" << endl;
	}
	cout << "Enter the word to look for." << endl;
	getline(cin, str);
	//finds word and returns 1 or doesn't find word and returns 0; 
	//also suggests words with same letters
	int found = wordFinder(file, str);
	if (found == 1) {
		cout << "True- Word Found" << endl;
	}
	else {
		cout << "False- Word Not Found" << endl;
	}
	return 0;

}
//computes index to place in hashtable using the ascii value of the first letter
//71 is an arbitrary prime number used to prevent as many collisions between keys as possible
int hashFunction(string str) {
	char first = str.at(0);
	int ascii = (int)first;
	return ascii%71;
}
//inserts keys into hash table using hashFunction() method, looks for the user-entered string word, and suggests words with same first two letters
//words grouped together in table by first letter, essentially reducing search time as you know the first place to look instead of searching the whole table
//returns 1 if word is found, 0 if not found
int wordFinder(ifstream& file, string word) {
	//200 is large enough number to be safeguard against overflow
	Node* Table[200];
	//fills the table with blank nodes to be used for keys
	for (int i = 0; i < 200; i++) {
		Node* n = new Node();
		Table[i] = n;
	}
	//inserts key into table
	//operation of inserting a key is O(1) because it only sets pointer values; 
	//inserting all keys is O(M) where M=number of keys to insert
	for (int i = 0; i < 200; i++) {
		string dictWord;
		getline(file, dictWord, '\n');
		int index = hashFunction(dictWord);
		//if there isn't a key in the hashed position, put the key into the table
		if (Table[index]->key.empty()) {
			Table[index]->key = dictWord;
		}
		//if collision occurs, add it to the beginning of the linked list at the hashed position
		else {
			Node* temp = new Node(dictWord);
			temp->key = dictWord;
			temp->next = NULL;
			temp->next = Table[index]->next;
			Table[index]->next = temp;
		}
		
	}
	/* outputs all values in hash table
	for (int i = 0; i < 200; i++) {
		if (!(Table[i]->key.empty())&&Table[i]->key!="") {
			cout<<Table[i]->key<<i<<endl;
			Node* temp = Table[i]->next;
			while (temp != NULL&&!(temp->key.empty())) {
				cout << temp->key <<i<< endl;
				temp = temp->next;
			}
		}
	}
	*/

	//Finds word looking for and suggests words based on it's letters 
	//best case for finding word: O(1)
	//worst case for finding word: O(N)
	//average case for finding word: O(1+N/M)
	//complexity for suggesting words is always O(N) where N=number of nodes in linked list chain
	int indexLook = hashFunction(word);
	//if first value is the word looking for; best case scenario
	if (Table[indexLook]->key == word) {
		//if the word being searched for is only one letter, suggest all words that start with that letter
		if (word.length() < 2) {
			Node* temp = Table[indexLook]->next;
			while (temp != NULL) {
				cout << "Suggested word: " << temp->key << endl;
				temp = temp->next;
			}
		}
		//if the word being searched for is more than one letter long
		else {
			Node* temp = Table[indexLook]->next;
			while (temp!=NULL) {
				//if key is only one letter, suggest it
				if (temp->key.length() < 2) {
					cout << "Suggested word: " << temp->key << endl;
						temp = temp->next;
				}
				//if key is more than one letter, make sure the second letter matches the second letter of the word 
				//being searched for (already know first letters match, since words are grouped by first letter)
				else {
					if (temp->key.at(1) == word.at(1)) {
						cout << "Suggested word: " << temp->key << endl;
						temp = temp->next;
					}
					//if key doesn't match, move on to the next node in the list
					else {
						temp = temp->next;
					}
				}
			}
		}
		return 1;
	}
	else {
		//word is not the first value in table; go through linked list
		Node* temp = Table[indexLook]->next;
		int foundIt = 0;
		//move through linked list until you find the word or you reach the end of the list worst case: O(N) N=number of nodes in list
		while (temp != NULL) {
			if (temp->key == word) {
				foundIt = 1;
				break;
			}
			else {
				temp = temp->next;
			}
		}
		//if you found the word, apply the same logic to suggest the word; approximately O(N), N=number of nodes in list
		if (foundIt == 1) {
			if (word.length() < 2) {
				Node* temp = Table[indexLook]->next;
				while (temp != NULL) {
					cout << "Suggested word: " << temp->key << endl;
					temp = temp->next;
				}
			}
			else {
				Node* temp = Table[indexLook]->next;
				while (temp != NULL) {
					if (temp->key.length() < 2) {
						cout << "Suggested word: " << temp->key << endl;
						temp = temp->next;
					}
					else {
						if (temp->key.at(1) == word.at(1)) {
							cout << "Suggested word: " << temp->key << endl;
							temp = temp->next;
						}
						else {
							temp = temp->next;
						}
					}
				}
			}
			return 1;
		}

	}

	return 0;
}
