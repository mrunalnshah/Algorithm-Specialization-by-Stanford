/*
 * File Name: Programming Assignment #1
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 5th Of MARCH, 2023
 *
 * Description: what is the largest value of kk such that there is a kk-clustering with spacing at least 3?  That is, how many clusters are needed to ensure that no pair of nodes with all but 2 bits in common get split into different clusters?
 *
 * Dependencies: This Code requires a file named "jobs.txt" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class UF {
    int N, cnt, *id, *size;
public:

        // create an empty union-find data structure with N separate clusters
    UF(int n){
        N = n;
        cnt = n;
        id = new int[n];
        size = new int[n];

        for(int i=0; i<n; ++i){
            id[i] = i;
            size[i] = 1;
        }
    }

        // return the leader of the group that point p belongs to
    int find(int p){
        return id[p];
    }

        // replace clusters containing x and y with their union
    void merge(int x, int y){
        int i = find(x);
        int j = find(y);
        if(i == j)
            return;

        // update the leader of the smaller group
        if(size[i] < size[j]){
            for(int k=0; k<N; ++k){
                if(id[k] == i)
                    id[k] = j;
            }
            size[j] += size[i];
        } else {
            for(int k=0; k<N; ++k){
                if(id[k] == j)
                    id[k] = i;
            }
            size[i] += size[j];
        }
        cnt--;
    }

    bool connected(int x, int y){
        return find(x) == find(y);
    }

    int count(){
        return cnt;
    }
};

class KruskalMST{
public:
    int clustering_big(vector<string> V){
        int n = V.size();       // number of nodes
        int l = V[0].size();    // number of bits for each node's label

        // generate a hash table to store binary code and the vertex number
        unordered_map<string, int> Vmap;
        for(int i=0; i<n; ++i){
            Vmap.emplace(V[i], i);
        }

        // initialize each node as a separate cluster
        UF uf(n);

        for(int i=0; i<n; ++i)
        {
            string key = V[i];

            // merge node 0 unit apart from key
            if((Vmap.find(key) != Vmap.end() & (Vmap[key] != i)))
                uf.merge(Vmap[key], i);

            for(int j=0; j<l; ++j)
            {
                string key1 = key;
                if(key1[j] == '0')
                    key1[j] = '1';
                else
                    key1[j] = '0';

                // merge node 1 unit apart from key
                if(Vmap.find(key1) != Vmap.end())
                    uf.merge(Vmap[key1], i);

                for(int k=j+1; k<l; ++k)
                {
                    string key2 = key1;
                    if(key2[k] == '0')
                        key2[k] = '1';
                    else
                        key2[k] = '0';

                    // merge node 2 unit apart from key
                    if(Vmap.find(key2) != Vmap.end())
                        uf.merge(Vmap[key2], i);
                }
            }
        }

        return uf.count();
    }
};

int main()
{
    ifstream myfile;
    myfile.open("02.clustering_big.txt");

    int n;              // number of nodes
    int l;              // number of bits for each node's label
    myfile >> n;
    myfile >> l;

    vector<string> V(n);
    char c;

    for(int i=0; i<n; ++i)
    {
        string str;
        for(int j=0; j<l; ++j){
            myfile >> c;
            str.push_back(c);
        }
        V[i] = str;
    }

    KruskalMST kmst;
    int k = kmst.clustering_big(V);

    cout << "max number of clusters: " << k << endl;
    return 0;
}
