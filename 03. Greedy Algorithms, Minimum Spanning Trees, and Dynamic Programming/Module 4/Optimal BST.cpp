/*
 * File Name: Optimal BST
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 5th Of MARCH, 2023
 *
 * Description: An Optimal Binary Search Tree (OBST), also known as a Weighted Binary Search Tree, is a binary search tree that minimizes the expected search cost. In a binary search tree, the search cost is the number of comparisons required to search for a given key.
 */

#include<iostream>
#include<vector>

using namespace std;


class OptimalBST{
public:
    double bst(vector<double> p){
        int n = p.size();
        vector<vector<double> > A(n+1, vector<double>(n, 0));   // optimal search time for item i to j
        vector<vector<double> > P(n, vector<double>(n, 0));     // sum of probabilities from i to j
        vector<vector<int> > r(n, vector<int>(n, 0));           // root of optimal subtree for item i to j

        // initialize A, P, r
        for(int i = 0; i < n; ++i)
        {
            P[i][i] = p[i];
            A[i][i] = p[i];
            r[i][i] = i;
        }

        // recurrence
        for(int s = 1; s < n; ++s)
        {
            for(int i = 0; i+s < n; ++i)
            {
                P[i][i+s] = p[i] + P[i+1][i+s];

                // due to Knuth, 1971
                double temp_min = 100000.0;
                for(int k = r[i][i+s-1]; k <= r[i+1][i+s]; ++k){
                    double temp = A[i][k-1] + A[k+1][i+s];
                    if(temp < temp_min)
                    {
                        temp_min = temp;
                        r[i][i+s] = k;
                    }
                }

                A[i][i+s] = P[i][i+s] + temp_min;
            }
        }

        // print A
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n; ++j){
                cout << A[i][j] << "\t";
            }
            cout << endl;
        }

        return A[0][n-1];
    }
};

int main()
{
    double mydata[] = { 0.05, 0.4, 0.08, 0.04, 0.1, 0.1, 0.23 };
    //	double mydata[] = { .2, .05, .17, .1, .2, .03, .25 };	// final exam
    vector<double> p(mydata, mydata + sizeof(mydata) / sizeof(double));

    OptimalBST optBST;
    double y = optBST.bst(p);

    cout<< y << endl;
    return 0;
}
