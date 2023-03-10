/*
 * File Name: Programming Assignment #2
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 6th Of MARCH, 2023
 *
 * Description:solve a knapsack instance, but a much bigger one.
 * Dependencies: This Code requires a file named "knapsack_big.txt" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

struct HASH{
    size_t operator() (const pair<int, int>&x) const{
        return hash<long long>() ( ((long long)x.first) ^ (((long long)x.second)<<32) );
    }
};

class Knapsack{
    // hash table to store results
    unordered_map<pair<int,int>, long long, HASH> Vmap;
public:
    long long optV(vector<int> v, vector<int> w, int W){
        int n = v.size();

        return V(n, W, v, w);
    }

    // recursive implementation
    long long V(int i, int x, vector<int> v, vector<int> w){
        if(i == 0 || x == 0)
            return 0;

        pair<int,int> key = make_pair(i, x);
        pair<int,int> key1 = make_pair(i-1, x);

        // if V[i-1, x] has not been calculated, call function V
        if(Vmap.find(key1) == Vmap.end())
            Vmap.emplace(key1, V(i-1, x, v, w));

        if(w[i-1] > x)
            Vmap.emplace(key, Vmap[key1]);
        else
        {
            pair<int,int> key2 = make_pair(i-1, x-w[i-1]);
            // if V[i-1, x-w[i-1]] has not been calculated, call function V
            if(Vmap.find(key2) == Vmap.end())
                Vmap.emplace(key2, V(i-1, x-w[i-1], v, w));

            Vmap.emplace(key, max(Vmap[key1], Vmap[key2]+v[i-1]));
        }

        return Vmap[key];
    }
};

int main()
{
    ifstream myfile;
    myfile.open("knapsack_big.txt");

    int W;              // knapsack size
    myfile >> W;

    int n;              // number of items
    myfile >> n;

    vector<int> v(n);   // value
    vector<int> w(n);   // weight

    for(int i=0; i<n; ++i){
        myfile >> v[i];
        myfile >> w[i];
    }

    Knapsack ks;
    long long optValue = ks.optV(v, w, W);

    cout << optValue << endl;
    return 0;
}
