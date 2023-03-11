/*
 * File Name: Programming Assignment #2
 *
 * Credit : Course     : Shortest Paths Revisited, NP-Complete Problems and What To Do About Them
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 7th Of MARCH, 2023
 *
 * Description:  implement one or more algorithms for the traveling salesman problem, such as the dynamic programming algorithm covered in the video lectures.  Here is a data file describing a TSP instance.
 * Dependencies: This Code require files named "tsp.txt" one file compiled at a time.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>        // for sqrt()
#include <algorithm>    // for prev_permutation()

using namespace std;

typedef pair<double, double> pi;

class TSP{
public:
    int MinCostDP(int n, vector<pi> G)
    {
        // compute the distance between vertices
        vector<vector<double>> D(n, vector<double>(n,0));   // d_ij is the distance between i and j vertices
        double dx, dy;
        for(int i=0; i<n; ++i)
        {
            for(int j=i+1; j<n; ++j)
            {
                dx = G[i].first - G[j].first;
                dy = G[i].second - G[j].second;
                D[i][j] = sqrt(dx*dx + dy*dy);
                D[j][i] = D[i][j];                  // due to symmetry
                //cout << "d_" << i << j << " = " << D[i][j] << endl;
            }
        }

        // DP initialization
        int pow2n = 1;
        for(int i=0; i<n-1; ++i)
            pow2n *= 2;
        vector<vector<double>> A(pow2n, vector<double>(n, 10000000.0));
        A[0][0] = 0;

        // DP iteration
        for(int m=1; m<n; ++m)                  // m = subproblem size (excluding vertex 0)
        {
            cout << "m = " << m+1 << endl;
            vector<bool> S(n-1);                // S is the indicator of {1,2,...,n-1} of size n-1
            fill(S.begin(), S.begin()+m, 1);    // pick m elements from n-1 elements.

            do{
                int i_S = pos(S);               // map bitset to int
                for(int j=1; j<n; ++j)
                {
                    if(S[j-1])                  // if vertex j is picked
                    {
                        int i_Sj = pos_i(S,j-1);
                        double A_min = A[i_Sj][0] + D[0][j];    // k=0
                        for(int k=1; k<n; ++k)
                        {
                            if(S[k-1] && k!=j)
                            {
                                double temp = A[i_Sj][k] + D[k][j];
                                if(A_min > temp)
                                    A_min = temp;
                            }
                        }
                        A[i_S][j] = A_min;
                    }
                }
            } while(prev_permutation(S.begin(), S.end()));
        }

        // DP min loop cost
        double cost_min = 10000000.0;
        for(int j=1; j<n; ++j)
        {
            double cost = A[pow2n-1][j] + D[j][0];
            if(cost_min > cost)
                cost_min = cost;
        }

        return floor(cost_min);
    }

    void combi(int n, int r)
    {
        // pick r elements in a set {1,2,...,n}
        vector<bool> S(n);
        fill(S.begin(), S.begin()+r, 1);

        do {
            for(int i=0; i<n; ++i)
            {
                cout << S[i];
            }
            cout << " -> " << pos(S) << endl;
        } while (prev_permutation(S.begin(), S.end()));
    }

    int pos(vector<bool> S)
    {
        // converte array S as integer by assuming S as a binary variable
        int base = 1;
        int num = 0;

        int n = S.size();
        for(int i=n-1; i>=0; --i)
        {
            num += S[i]*base;
            base *= 2;
        }

        return num;
    }

    int pos_i(vector<bool> S, int i)
    {
        // compute S as integer if i-th element is set to 0
        S[i] = 0;
        return pos(S);
    }
};

int main(){

    ifstream myfile("tsp.txt");
    if(!myfile.is_open())
    {
        cout << "File failed to open." << endl;
        return 0;
    }

    int n;              // number of vertices
    myfile >> n;
    vector<pi> G(n);    // corrdinate of vertices

    for(int i=0; i<n; ++i)
    {
        myfile >> G[i].first;
        myfile >> G[i].second;
    }
    myfile.close();

    TSP tsp;
    //tsp.combi(n-1,2);
    int cost = tsp.MinCostDP(n, G);
    cout << "Min cost of TSP = " << cost << endl;

    return 0;
}
