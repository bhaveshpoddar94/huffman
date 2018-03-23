#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class HeapNode{
	public:
	    string data;  //One of the input characters
	    HeapNode *left, *right; //Left and right child of this node
			HeapNode(string data){
				this->data = data;
				left = NULL;
				right = NULL;
			}
};

vector<int> read_file(char* filename){
	ifstream f(filename, ios::binary | ios::in);
	vector<int> v;
	char c;
	int bit;
	while (f.get(c)){
	    for(int i = 0; i < 8; i++){
	    	bit = (c >> i) & 1;
	        v.push_back(bit);
	    }
	}
	return v;
}

map<string, string> read_code_table(char* filename){
	map<string, string> m;
	//feed code table to hashmap
	ifstream the_file(filename);
    if (!the_file.is_open())
        cout<<"Could not open file\n";
    else{
        string key, value;
        while (the_file >> key >> value){
        	m[key] = value;
	    }

	}
	return m;
}


HeapNode *build_huffman(map<string, string> m){
	HeapNode* root = new HeapNode("$");
	HeapNode* head = root;
	HeapNode* temp = root;

	//build huffman from code table
	for (map<string,string>::iterator it=m.begin(); it!=m.end(); ++it){
		root = temp;
    	for (int i=0; i < it->second.size(); i++){
	        if(it->second[i]=='0'){
	        	if(!root->left)
	        		root->left = new HeapNode("$");
	        	root = root->left;
	        }
	        else{
	        	if(!root->right)
	        		root->right = new HeapNode("$");
	        	root = root->right;
	        }
    	}
    	root->data = it->first;
    }
    return head;
}


void decode(HeapNode* root, vector<int> v){
	HeapNode* head = root;
	ofstream myfile("decoded.txt");
    int i;
    if (!myfile.is_open())
        cout<<"Could not open file\n";
    else{
	    for(i = 0; i<v.size(); i++){
	    	if(v[i])
	            root = root->right;
	        else
	            root = root->left;

	        if(!root->left && !root->right){
	            myfile << root->data << endl;
	            root = head;
	        }
	    }
	}
	myfile.close();
}


int main(int argc, char *argv[]){
	if(argc != 3)
    	cout<<"usage: "<< argv[0] <<" <binary filename> " << "<code table filename> " << endl;
  	else{
		vector<int> v = read_file(argv[1]);
		map<string, string> m = read_code_table(argv[2]);
		HeapNode* head = build_huffman(m);
		//generates a decoded file named decoded.txt
		decode(head, v);
	}
	return 0;
}
