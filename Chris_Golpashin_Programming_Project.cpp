// Chris Golpashin
// Coding and Information Theory
// Programming Project
// Problem #3. Huffman Code
// Due December 3 2019 | extended to December 10 2019

#include <iostream> 
#include <cstdlib>
#include <math.h> // Just using this for the pow function when printing the average word length.

using namespace std; 
  
#define MAX_TREE_HT 300

// Create global variable for average word size
double averageWordLength;
  
// Min Heap tree node 
struct MinHeapNode 
{ 
    // One of the input characters 
    char data; 
  
    // Probability of the character 
    unsigned prob; 
  
    // Left and right child of this node 
    struct MinHeapNode *left, *right; 
}; 
  
struct MinHeap 
{ 
    // Current size of min heap 
    unsigned size; 
  
    // capacity of min heap 
    unsigned capacity; 
  
    // Array of minheap node pointers 
    struct MinHeapNode** array; 
}; 

// Function to create a new 
// min heap node with a character 
// and probability of that character 
struct MinHeapNode* newNode(char data, unsigned prob) 
{ 
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode)); 
  
    temp->left = temp->right = NULL; 
    temp->data = data; 
    temp->prob = prob; 

    return temp; 
} 
  
// Function to create a min heap of the capacity 
struct MinHeap* createMinHeap(unsigned capacity)  
{ 
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap)); 
  
    // set size to 0 
    minHeap->size = 0; 
    
    // set size to capacity
    minHeap->capacity = capacity; 
  
    minHeap->array = (struct MinHeapNode**)malloc(minHeap-> capacity * sizeof(struct MinHeapNode*));

    return minHeap; 
} 
  
// Function to swap two min heap nodes 
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)  
{ 
    struct MinHeapNode* t = *a; 
    *a = *b; 
    *b = t; 
} 
  
void minHeapify(struct MinHeap* minHeap, int idx)  
{ 
    int smallest = idx; 
    int left = 2 * idx + 1; 
    int right = 2 * idx + 2; 
  
    if (left < minHeap -> size && minHeap -> array[left] -> prob < minHeap -> array[smallest] -> prob) { 
        smallest = left; 
    }

    if (right < minHeap -> size && minHeap -> array[right] -> prob < minHeap -> array[smallest] -> prob) {
        smallest = right; 
    }

    if (smallest != idx) { 
        swapMinHeapNode(&minHeap -> array[smallest], &minHeap -> array[idx]); 


        minHeapify(minHeap, smallest); 
    } 
} 
  
// Function to check if size of heap is 1 or not 
int isSizeOne(struct MinHeap* minHeap) 
{ 
    return (minHeap->size == 1); 
} 
  
// Function to extract minimum value node from heap 
struct MinHeapNode* extractMin(struct MinHeap* minHeap) 
{ 
    struct MinHeapNode* temp = minHeap->array[0]; 
    minHeap->array[0] = minHeap->array[minHeap->size - 1]; 
  
    --minHeap->size; 
    minHeapify(minHeap, 0); 
  
    return temp; 
} 
  
// Function to insert a new node to Min Heap 
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) 
{ 
    ++minHeap->size; 
    int i = minHeap->size - 1; 
  
    while (i && minHeapNode->prob < minHeap->array[(i - 1) / 2]->prob) { 
  
        minHeap->array[i] = minHeap->array[(i - 1) / 2]; 
        i = (i - 1) / 2; 
    } 
  
    minHeap->array[i] = minHeapNode; 
} 
  
// Function to build min heap 
void buildMinHeap(struct MinHeap* minHeap) 
{ 
    int n = minHeap->size - 1; 
  
    for (int i = (n - 1) / 2; i >= 0; --i)
    {
        minHeapify(minHeap, i); 
    }
} 
  
// Function to print array of size n 
void printArr(int arr[], int n) 
{ 
    for (int i = 0; i < n; ++i)
    {
        cout << arr[i]; 
    }

    cout << "\n"; 
}
  
// Function to check if this node is leaf 
int isLeaf(struct MinHeapNode* root)  
{ 
    return !(root->left) && !(root->right); 
} 
  
// Creates a min heap of capacity 
// equal to size and inserts all character of 
// data[] in min heap. Initially size of 
// min heap is equal to capacity 
struct MinHeap* createAndBuildMinHeap(char data[], int prob[], int size) 
{ 
    struct MinHeap* minHeap = createMinHeap(size); 
  
    for (int i = 0; i < size; ++i)
    {
        minHeap->array[i] = newNode(data[i], prob[i]); 
    }
    
    minHeap->size = size; 
    buildMinHeap(minHeap); 
  
    return minHeap; 
} 
  
// Main function that builds Huffman tree 
struct MinHeapNode* buildHuffmanTree(char data[], int prob[], int size) 
{ 
    struct MinHeapNode *left, *right, *top; 
  
    // Step 1: Create a min heap of capacity 
    // equal to size. In the start there are 
    // nodes equal to the size. 
    struct MinHeap* minHeap = createAndBuildMinHeap(data, prob, size); 
  
    // Loop while size of the heap isn't 1 
    while (!isSizeOne(minHeap)) { 
  
        // Step 2: Pull out two of the lowest probabilitys from min heap 
        left = extractMin(minHeap); 
        right = extractMin(minHeap);
  
        // Step 3: Create a new internal 
        // node with a sum of the two nodes probabilities. 
        // And take the two summed probabiltys and make them. 
        //  the left and right of the new node. Add this node to the min heap 
        // Ignore # since its just a dummy placeholder. 
        top = newNode('#', left->prob + right->prob); 

        // sums up the average word size as the entire program's functions loop through.
        averageWordLength += (left->prob + right->prob);

        top->left = left; 
        top->right = right; 
  
        insertMinHeap(minHeap, top); 
    } 
  
    // Step 4: What ever node, that is left, is the new root. 
    return extractMin(minHeap); 
} 
  
// Assigns 1s, 0s recursively until it determines if it is a leaf,
// which it will then print the character from data then arr[]
// which contains the codes of that character. 
void printCodes(struct MinHeapNode* root, int arr[], int top)  
{ 
    // Assign 1 to left and call self again
    if (root -> left) { 
       
        arr[top] = 1; 
        printCodes(root->left, arr, top + 1); 
    } 
  
    // Assign 0 to right and call self again 
    if (root -> right) { 
        
        arr[top] = 0; 
        printCodes(root->right, arr, top + 1); 
    } 
  
    // Print the character and its code from arr[] 
    if (isLeaf(root)) { 
  
        cout << root -> data << ": "; 
        printArr(arr, top); 
    } 
}

// Prints out the AVG word size
// but also corrects for the actual
// decimal point since whole numbers
// were used for the probabilitys.
void printAverageWord(double prob)
{
    cout << endl;
    cout << "Average word length: " << averageWordLength * (1.0* pow(10,-2)) << endl;
}
  
void HuffmanCodes(char data[], int prob[], int size) 
{ 
    // Build Min Heap Tree 
    struct MinHeapNode* root = buildHuffmanTree(data, prob, size); 
  
    // Print Huffman codes
    int arr[MAX_TREE_HT], top = 0; 
  
    printCodes(root, arr, top); 
} 

int main() 
{ 
    char arr[] = {'a', 'b', 'c', 'd', 'e'}; // these letters take the placeholder of s1, s2, ..., sn symbols
    int prob[] = {10, 20, 20, 20, 30}; // 20 == .2, and probabilities must add up to 100 == 1.0
  
    int size = sizeof(arr) / sizeof(arr[0]); // 
  
    HuffmanCodes(arr, prob, size);

    printAverageWord(averageWordLength); 
  
    return 0; 
} 