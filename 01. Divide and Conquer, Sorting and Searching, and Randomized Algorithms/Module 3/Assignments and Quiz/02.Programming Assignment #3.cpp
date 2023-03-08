/*
 * File Name: Programming Assignment #3
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 16th Of FEBURARY, 2023
 *
 * Description: compute the total number of comparisons used to sort the given input file by QuickSort.
 *
 * Dependencies: This Code requires a txt file named "02.QuickSort.txt". (you can change the numbers too)
 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Sort{
public:
    long long QuickSort(vector<int>& X, int low, int high, int flag=1)
    {
        if(low >= high){                              // 0- or 1-element subarray
            return 0;
        }


        int p;
        if(flag==1){
            p = low;
        }                              // choose 1st element as pivot point
        else if(flag==2){
            p = high;                   // choose last element as pivot point
        }
        else if(flag==3){
            p = Median(X, low, high);                // choose the median among 1st, n/2, last as pivot point
        }
        swap(X[low], X[p]);                       // make pivot first

        int j = Partition(X, low, high);
        long long c1 = high-low;                             // number of comparision on a subarray with length r-l+1
        long long c2 = QuickSort(X, low, j-1, flag);      // recurse on first part
        long long c3 = QuickSort(X, j+1, high, flag);      // recurse on second part

        return c1+c2+c3;
    }

    int Partition(vector<int>& X, int low, int high)
    {
        int p = X[low];   // pivot point
        int i = low+1;    // elements between p and i are less than pivot
        int j = low+1;    // elements between i and j are greater than pivot

        for(; j<=high; j++)
        {
            if(X[j] < p)            // if A[j] > p, do nothing
            {
                swap(X[j], X[i]);
                i++;                // restore invariant
            }
        }

        swap(X[low], X[i-1]);         // place pivot correctly
        return i-1;
    }

    int Median(vector<int> X, int low, int high){
        int mid = low + (high-low)/2;
        bool b1 = X[high] > X[mid];
        bool b2 = X[mid] > X[low];
        bool b3 = X[low] > X[high];

        if(b1 && b2 || !b1 && !b2)
            return mid;
        else if(b1 && b3 || !b1 && !b3)
            return high;
        else if(b2 && b3 || !b2 && !b3)
            return low;
    }
};

int main(){

    ifstream myfile("02.QuickSort.txt");
    if(!myfile.is_open())
    {
        cout << "File failed to open" << endl;
        return 0;
    }

    vector<int> X;
    int x;

    while(myfile >> x)
        X.push_back(x);

    Sort qs;
    vector<int> X1(X.begin(), X.end());
    vector<int> X2(X.begin(), X.end());
    long long c1 = qs.QuickSort(X1, 0, X.size()-1, 1);
    long long c2 = qs.QuickSort(X2, 0, X.size()-1, 2);
    long long c3 = qs.QuickSort(X, 0, X.size()-1, 3);
    cout << c1 << endl << c2 << endl << c3 << endl;

    return 0;
}
