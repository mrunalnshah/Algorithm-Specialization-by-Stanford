/*
 * File Name: Programming Assignment #2
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 5th Of MARCH, 2023
 *
 * Description: Your task in this problem is to run the clustering algorithm from lecture on this data set, where the target number kk of clusters is set to 4.  What is the maximum spacing of a 4-clustering?
 *           --> Continuing the previous problem, what is the minimum length of a codeword in your Huffman code?

 * Dependencies: This Code requires a file named "huffman.txt" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct node
{
    long long key;
    node *left;
    node *right;
    node(long long x) : key(x), left(NULL), right(NULL) {}
};

class compareNodes{
public:
    bool operator()(node* a, node* b)
    {
        // return "true" if "b" is ordered at top
        return (a->key > b->key);
    }
};

class Huffman{
public:
    int coding(int n, vector<int> W){
        if(n == 0)
            return 0;
        else if(n <= 2)
            return 1;

        // use a min-priority queue
        priority_queue<node*, vector<node*>, compareNodes> mHeap;
        for(int i=0; i<n; ++i){
            mHeap.push(new node(W[i]));
        }

        // create Huffman tree
        for(int i=0; i<n-1; ++i){
            // remove 1
            node* x = mHeap.top();
            mHeap.pop();

            // remove 2
            node* y = mHeap.top();
            mHeap.pop();

            // merge and re-insert
            node* z = new node(x->key + y->key);
            z->left = x;
            z->right = y;
            mHeap.push(z);
        }

        return minDepth(mHeap.top())-1;
    }

        // compute the min depth of a tree
    int minDepth(node* x){
        if(!x)
            return 0;
        else
            return min(minDepth(x->left)+1, minDepth(x->right)+1);
    }
};

int main()
{
    ifstream myfile;
    myfile.open("huffman.txt");

    int n;              // number of symbols
    myfile >> n;

    vector<int> W(n);

    for(int i=0; i<n; ++i)
        myfile >> W[i];

    Huffman huff;
    int min_length = huff.coding(n, W);

    cout << min_length << endl;
    return 0;
}
