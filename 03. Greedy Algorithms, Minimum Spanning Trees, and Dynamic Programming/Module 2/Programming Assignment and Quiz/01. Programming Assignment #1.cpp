/*
 * File Name: Programming Assignment #1
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 5th Of MARCH, 2023
 *
 * Description: Your task in this problem is to run the clustering algorithm from lecture on this data set, where the target number kk of clusters is set to 4.  What is the maximum spacing of a 4-clustering?
 *
 * Dependencies: This Code requires a file named "01.clustering1.txt" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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

bool compareTwoEdges(vector<int> e1, vector<int> e2){
    // return true for lower cost
    return e1[2] < e2[2];
}

class KruskalMST{
public:
    int clustering(int k, int n, vector<vector<int> > E) {
        // sort edges in order of increasing distance
        sort(E.begin(), E.end(), compareTwoEdges);

        // initialize each node as a separate cluster
        UF uf(n);

        // merge two closest clusters until only k clusters
        int j = 0;
        while(uf.count()>k){
            while(uf.connected(E[j][0], E[j][1]))
                j++;

            uf.merge(E[j][0], E[j][1]);
        }

        // find the next closest clusters
        while(uf.connected(E[j][0], E[j][1]))
            j++;
        return E[j][2];
    }
};

int main()
{
    ifstream myfile;
    myfile.open("01.clustering1.txt");

    int n;              // number of nodes
    myfile >> n;

    int m = n*(n-1)/2;  // number of edges
    vector<vector<int> > E(m, vector<int>(3));

    for(int i=0; i<m; ++i)
    {
        myfile >> E[i][0];
        myfile >> E[i][1];
        myfile >> E[i][2];
	E[i][0]--;
	E[i][1]--;
    }

    KruskalMST kmst;
    int max_space = kmst.clustering(4, n, E);

    cout << max_space << endl;
    return 0;
}
