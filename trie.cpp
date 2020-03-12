#include<bits/stdc++.h> 
using namespace std; 
#define char_int(c) ((int)c - (int)'a') 
#define SIZE (26) 

struct Node { 
	Node *alphabet[SIZE]; 
	bool isWord; 
}; 
/*
	return the new empy node
*/
Node *getNode() { 
	Node * newNode = new Node; 
	newNode->isWord = false; 
	for (int i =0 ; i< SIZE ; i++) newNode->alphabet[i] = NULL; 
	return newNode; 
} 
/*
	takes string as an argument and puts it into tree of the words
*/
void insert(Node *root, char* newStr) { 
	Node * temp = root; 
	for (int i=0; i<strlen(newStr); i++) { 
		int index = char_int(newStr[i]); 
		if (temp->alphabet[index] == NULL) 
			temp->alphabet[index] = getNode(); 
		temp = temp->alphabet[index]; 
	} 
	temp->isWord = true; 
} 
bool checkBorder(int i, int j, vector<vector<bool>> &visited) { 
	return (i >=0 && i < visited.size() && j >=0 && j < visited.size() && !visited[i][j]); 
} 
/*
	checks if the word is valid and saves it into set.
	then for all neighbours does the same thing 
	until visits all the points
*/
void search(Node *root,vector<vector<char>> &boggle,int i,int j,vector<vector<bool>> &visited,string str,set<string> &s) { 
	if (root->isWord == true) s.insert(str);
	if(!checkBorder(i,j,visited)) return;
	visited[i][j] = true; 
	for (int k =0; k < SIZE; k++) {
		if(root->alphabet[k]==NULL) continue; 
		char ch = (char)k + (char)'a' ; 
		if (checkBorder(i+1,j+1,visited)&&boggle[i+1][j+1]==ch) search(root->alphabet[k],boggle,i+1,j+1,visited,str+ch,s); 
		if (checkBorder(i, j+1,visited)&&boggle[i][j+1]==ch) search(root->alphabet[k],boggle,i, j+1,visited,str+ch,s); 
		if (checkBorder(i-1,j+1,visited)&&boggle[i-1][j+1]==ch) search(root->alphabet[k],boggle,i-1, j+1,visited,str+ch,s); 
		if (checkBorder(i+1,j, visited)&&boggle[i+1][j]==ch) search(root->alphabet[k],boggle,i+1, j,visited,str+ch,s); 
		if (checkBorder(i+1,j-1,visited)&&boggle[i+1][j-1]==ch) search(root->alphabet[k],boggle,i+1, j-1,visited,str+ch,s); 
		if (checkBorder(i, j-1,visited)&&boggle[i][j-1]==ch) search(root->alphabet[k],boggle,i,j-1,visited,str+ch,s); 
		if (checkBorder(i-1,j-1,visited)&&boggle[i-1][j-1]==ch) search(root->alphabet[k],boggle,i-1, j-1,visited,str+ch,s); 
		if (checkBorder(i-1, j,visited)&&boggle[i-1][j]==ch) search(root->alphabet[k],boggle,i-1, j, visited,str+ch,s);  
	} 
	visited[i][j] = false;  
} 
/*
	fill the matrix with given bool variable 
*/
void defaultMatrix(vector<vector<bool>> &visited,bool value,int size){
	visited.resize(size, vector<bool> (size)); 
	for(int i=0; i<visited.size(); i++){
		for(int j=0; j<visited.size(); j++){
			visited[i][j] = value;
		}
	}
}

void findWords(vector<vector<char>> &boggle, Node *root,set<string> &s) { 
	vector<vector<bool>> visited;
	defaultMatrix(visited,false,boggle.size());
	Node *tempNode = root ; 
	for (int i = 0 ; i < boggle.size(); i++) { 
		for (int j = 0 ; j < boggle.size() ; j++) { 
			if (tempNode->alphabet[char_int(boggle[i][j])] ) { 
				string temp(1,boggle[i][j]); 
				search(tempNode->alphabet[char_int(boggle[i][j])],boggle, i, j, visited, temp,s); 
			} 
		} 
	} 
} 
/*
	reads file and creates matrix and dictionaries from this one
*/
void readFile(ifstream &myfile,vector<string> &dictionary,vector<vector<char>> &boggle){
	assert(myfile.is_open());
	string line;
    getline(myfile,line);
    int gridSize = stoi(line);
	boggle.resize(gridSize , vector<char> (gridSize));
    int row = 0, col = 0;
    for(int i=0; i<gridSize; i++){
        getline(myfile,line);
        for(int k=0; k<line.length(); k+=2){
            boggle[row][col] = line[k];
            col++;
        }
        row++;
        col = 0;
    }
    while (getline(myfile,line)) dictionary.push_back(line);
}
/*
	iterates all the answers' files and converts them into set of words
*/
set<string> readOutput(int number){
	string temp;
	temp = "testTrie/answers/puzzle"+to_string(number)+"Output.txt";
	ifstream myfile (temp);
	assert(myfile.is_open());
	string line;
    getline(myfile,line);
	istringstream ss(line);
	set<string> results((istream_iterator<string>(ss)),istream_iterator<string>());
	return results;
}
/*	
	this method iterates all the input files and for each one
	creates set of all the possible words and compares to the anwers' files 
*/
void testPuzzle(){
	cout<<"this will take several minutes"<<endl;
	string puzzle = "testTrie/input/puzzle";
	int count = 0;
	for(int i=0; i<=20; i++){
		string temp = puzzle+to_string(i)+".txt";
		ifstream myfile (temp);
		vector<string> dictionary;
		vector<vector<char>> boggle;
		readFile(myfile,dictionary,boggle);
		set<string> allStrings;
		Node *root = getNode();
		for (int i=0; i<dictionary.size(); i++) insert(root, &dictionary[i][0]); 
		findWords(boggle, root,allStrings);
		set<string> output = readOutput(i);
		if(output==allStrings){
			cout<<to_string(i)<<" passed"<<endl;
			count++;
		}else cout<<to_string(i)<<" failed"<<endl;
	}
	cout<<"passed "<<count<<" test"<<endl;
}
int main() { 
    testPuzzle();
	return 0; 
} 

