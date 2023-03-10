/*
 * File Name: Programming Assignment #1
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 5th Of MARCH, 2023
 *
 * Description: Your task in this problem is to run the Huffman coding algorithm from lecture on this data set. What is the maximum length of a codeword in the resulting Huffman code?
 *
 * Dependencies: This Code requires a file named "huffman.txt" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

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

        // sort W in non-decreasing order
        sort(W.begin(), W.end());

        // use a queue to store the weights of symbol
        queue<node*> q1;
        for(int i=0; i<n; ++i){
            q1.push(new node(W[i]));
        }

        // use another queue to store merged weights
        queue<node*> q2;

        // create Huffman tree
        for(int i=0; i<n-1; ++i){
            // remove 1
            node* x;
            if(q2.empty()){
                x = q1.front();
                q1.pop();
            }else if(q1.empty()){
                x = q2.front();
                q2.pop();
            }else if(q1.front()->key <= q2.front()->key){
                x = q1.front();
                q1.pop();
            }else{
                x = q2.front();
                q2.pop();
            }

            // remove 2
            node* y;
            if(q2.empty()){
                y = q1.front();
                q1.pop();
            }else if(q1.empty()){
                y = q2.front();
                q2.pop();
            }else if(q1.front()->key <= q2.front()->key){
                y = q1.front();
                q1.pop();
            }else{
                y = q2.front();
                q2.pop();
            }

            // merge and re-insert
            node* z = new node(x->key + y->key);
            z->left = x;
            z->right = y;
            q2.push(z);
        }

        return maxDepth(q2.front())-1;
    }

        // compute the max depth of a tree
    int maxDepth(node* x){
        if(!x)
            return 0;
        else
            return max(maxDepth(x->left)+1, maxDepth(x->right)+1);
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
    int max_length = huff.coding(n, W);

    cout << max_length << endl;
    return 0;
}
