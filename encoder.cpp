/*
 *  C++ Program to Implement D-ary-Heap
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
using namespace std;


//Minheap Node Structure
class HeapNode{
    string data;  //One of the input characters
    int freq;  //Frequency of the character
    HeapNode *left, *right; //Left and right child of this node
    public:
        HeapNode(string data, int freq, HeapNode *left=NULL, HeapNode *right=NULL){
            this->data = data;
            this->freq = freq;
            this->left = left;
            this->right = right;
        }

        // Utility function to check if node's child present
        int get_Freq(){
            return this->freq;
        }

        // Utility function to check if node's child present
        bool is_Child(int child){
            if(child == 0 and this->left){
                return true;
            } else if(child == 1 and this->right){
                return true;
            }
            return false;
        }

        // Utility function to return node's child (left or right)
        HeapNode* get_Child(int child){
            if(child == 0){
                return this->left;
            } else{
                return this->right;
            }
        }
        // Utility function to return node's data
        string get_Data(){
            return this->data;
        }
        // Utility function to check if this node is leaf
        int is_Leaf(){
            return !(this->left) && !(this->right);
        }

};


/*
 *  Fway Heap Class
 */
class FwayHeap{
    private:
        int currentSize;
        int size;
        HeapNode **array;
    public:
        //Constructor
        FwayHeap(int capacity){
            currentSize = 3;
            size = capacity + 4;
            array = new HeapNode*[capacity + 4];
            for (int i = 0 ; i < capacity + 4; i++)
                array[i] = NULL;
        }
        //returns true if heap empty
        bool is_Empty(){
            return currentSize < 4;
        }
        //returns true if heap full
        bool is_Full(){
            return currentSize == size;
        }
        //returns index of parent of i
        int parent(int i){
            return i/ 4 + 2;
        }
        //returns index of kth child of i
        int kth_Child(int i, int k){
            return 4 * (i-2) + (k-1);
        }
        //Inserts heap node into
        void insert(HeapNode* temp){
            if (is_Full()){
                cout<<"Array Out of Bounds"<<endl;
                return;
            }
            int hole = currentSize;
            currentSize++;
            array[hole] = temp;
            perc_Up(hole);
        }

        //deletes and returns the deleted min element
        HeapNode* deleteMin(){
            if (is_Empty()){
                cout<<"Array Underflow"<<endl;
                return 0;
            }
            HeapNode* keyItem = array[3];
            array[3] = array[currentSize - 1];
            currentSize--;
            perc_Down(3);
            return keyItem;
        }


        void Heapify(){
            for (int i = currentSize - 1 ; i >= 3; i--)
                perc_Down(i);
        }

        // downward percolation of hole
        void perc_Down(int hole){
            int child;
            HeapNode* tmp = array[hole];
            while(kth_Child(hole, 1) < currentSize){
                child = smallest_Child(hole);
                if (array[child]->get_Freq() < tmp->get_Freq()) {
                    array[hole] = array[child];
                } else {
                    break;
                }
                hole = child;
            }
            array[hole] = tmp;
        }

         //Function to get smallest child from all children
        int smallest_Child(int hole){
            int bestChildYet = kth_Child(hole, 1);
            int k = 2;
            int candidateChild = kth_Child(hole, k);
            while ((k <= 4) && (candidateChild < currentSize)){
                if (array[candidateChild]->get_Freq() < array[bestChildYet]->get_Freq()) {
                    bestChildYet = candidateChild;
                }
                k++;
                candidateChild = kth_Child(hole, k);
            }
            return bestChildYet;
        }

        //upward percolation of hole
        void perc_Up(int hole){
            HeapNode* tmp = array[hole];
            while(hole > 3 && tmp->get_Freq() < array[parent(hole)]->get_Freq()){
                array[hole] = array[parent(hole)];
                hole = parent(hole);
            }
            array[hole] = tmp;
        }

        void set_Array(int i, string data, int freq){
            this->array[i] = new HeapNode(data, freq);
        }

        void set_Size(int size){
            this->currentSize = 3 + size;
        }

        // A utility function to check if size of heap is 1 or not
        int is_One(){
            return (this->currentSize == 4);
        }
};

string int_to_string(int value){
    ostringstream os;
    os << value;
    return os.str();
}

// A utility function to print an array of size n
string printCodes(int arr[], int n){
    int i;
    string temp = "";
    for (i = 0; i < n; ++i)
        temp = temp + int_to_string(arr[i]);
    return temp;
}

map<string, string> code_table;
// Prints huffman codes from the root of Huffman Tree. It uses arr[] to
// store codes
void gen_Codes(HeapNode* root, int arr[], int top){
    // Assign 0 to left edge and recur
    if (root->is_Child(0)){
        arr[top] = 0;
        gen_Codes(root->get_Child(0), arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->is_Child(1)){
        arr[top] = 1;
        gen_Codes(root->get_Child(1), arr, top + 1);
    }

    // If this is a leaf node, then it contains one of the input
    // characters, print the character and its code from arr[]
    if (root->is_Leaf()){
        code_table[root->get_Data()] = printCodes(arr, top);
    }
}


void generate_code_table(HeapNode* node, int size){
    int *arr = new int[size];
    int head = 0;
    gen_Codes(node, arr, head);
    ofstream out("code_table.txt");
    if (out.is_open()){
        for(map<string,string>::iterator iter=code_table.begin(); iter!=code_table.end(); ++iter){
            out << iter->first << " " << iter->second << endl;
        }
        out.close();
    }
    else cout << "Unable to open file" << endl;
}



FwayHeap *init_Heap(map<string, int> m){
    FwayHeap* MinHeap = new FwayHeap(m.size());
    int i = 3;
    for(map<string,int>::iterator iter=m.begin(); iter!=m.end(); ++iter){
        MinHeap->set_Array(i, iter->first, iter->second);
        i++;
    }
    MinHeap->set_Size(m.size());
    MinHeap->Heapify();
    return MinHeap;
}


//The main function that builds Huffman tree
void buildHuffman(map<string, int> m){
    HeapNode *left, *right, *top;
    // Step 1: Initialise a min heap of capacity equal to size.
    // Initially nodes are equal to size.
    FwayHeap *heap = init_Heap(m);

    // Iterate until heap becomes size 1
    while(!heap->is_One()){
        //Extract the two minimum freq items from min heap
        left = heap->deleteMin();
        right = heap->deleteMin();

        //Insert a new minheapnode with freq = left.freq + right.freq
        //and left child = left and right child = right into the heap
        top = new HeapNode("$", left->get_Freq() + right->get_Freq(), left, right);
        heap->insert(top);
    }

    // Step 4: The remaining node is the root node and the tree is complete.
    generate_code_table(heap->deleteMin(), m.size());
}


void generate_encoded_data(vector<string> v){
    FILE *myfile;
    int bit;
    int current_bit = 0;
    unsigned char bit_buffer;

    //convert data into a string of bits
    string result = "";
    for(int i=0; i<v.size(); i++)
        result += code_table[v[i]];

    //write bits in binary file
    myfile = fopen ("encoded.bin", "wb");
    for(int j=0; j<result.size(); j++){
        bit = result[j] - '0';
        if(bit)
            bit_buffer |= (1<<current_bit);
        current_bit++;
        if (current_bit == 8){
            fwrite(&bit_buffer, 1, 1, myfile);
            current_bit = 0;
            bit_buffer = 0;
        }
    }
    fclose(myfile);
}


struct datafreq{
    map<string, int> m;
    vector<string> v;
};

//reads file input
struct datafreq read_file(char *filename){
    ifstream the_file(filename);
    map<string, int> m;
    vector<string> v;
    struct datafreq temp;
    if (!the_file.is_open())
        cout<<"Could not open file\n";
    else{
        string line;
        while (getline(the_file, line)){
            // Process line
            if(line!=""){
                v.push_back(line);
                if (m.find(line) != m.end()){
                    m[line]++;
                }
                else{
                    m[line]=1;
                }
            }
        }
        the_file.close();
    }
    temp.m = m;
    temp.v = v;
    return temp;
}


int main(int argc, char *argv[]){
    clock_t start_time;
    if (argc != 2) // argc should be 2 for correct execution
        // We print argv[0] assuming it is the program name
        cout<<"usage: "<< argv[0] <<" <filename>\n";
    else{
        struct datafreq input = read_file(argv[1]);
        buildHuffman(input.m);
        generate_encoded_data(input.v);
    }
}
