/*
 * File Name: Programming Assignment #1
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 6th Of MARCH, 2023
 *
 * Description: code up the knapsack algorithm
 * Dependencies: This Code requires a file named "knapsack1.txt" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Knapsack{
public:
    int optV(vector<int> v, vector<int> w, int W){
        int n = v.size();
        vector<vector<int> > V(n+1, vector<int>(W+1, 0));

        for(int i = 1; i <= n; ++i)
        {
            for(int x = 1; x <= W; ++x)
            {
                if(w[i-1] > x)
                    V[i][x] = V[i-1][x];
                else
                    V[i][x] = max(V[i-1][x], V[i-1][x-w[i-1]] + v[i-1]);
            }
        }

        return V[n][W];
    }
};

int main()
{
    ifstream myfile;
    myfile.open("knapsack1.txt");

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
    int optValue = ks.optV(v, w, W);

    cout << optValue << endl;
    return 0;
}
