/*
 * File Name: Programming Assignment #2
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 16th Of FEBURARY, 2023
 *
 * Description: compute the number of inversions in the file given, where the ith row of the file indicates the ith entry of an array.
 *
 * Dependencies: This Code requires a txt file named "02.IntegerArray.txt". (you can change the numbers too)
 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class DAC{
public:
    long long SortCountInv(vector<int>& Arr, int low, int high) {
        if(low > high){
            cout << "Error!(l is greater than r.)" << endl;
            return 0;
        }

        if(high==low){      // base case, n=1
            return 0;
        }

        int mid = low + (high-low)/2;    // same as (l+r)/2, but avoid overflow for large l and r
        long long leftInv = SortCountInv(Arr, low, mid);
        long long rightInv = SortCountInv(Arr, mid+1, high);
        long long splitInv = MergeCountSplitInv(Arr, low, mid, high);

        return leftInv + rightInv + splitInv;
    }

    long long MergeCountSplitInv(vector<int>& Arr, int low, int mid, int high) {
        int i, j, k;
        int sizeofarray_1 = mid - low + 1;
        int sizeofarray_2 = high - mid;

        vector<int> temp_array_1(sizeofarray_1), temp_array_2(sizeofarray_2);
        // copy data to temp arrays temp_array_1 and temp_array_2
        for(i=0; i<sizeofarray_1; ++i){
            temp_array_1[i] = Arr[low+i];
        }
        for(j=0; j<sizeofarray_2; ++j){
            temp_array_2[j] = Arr[mid+1+j];
        }

        i=0;
        j=0;
        k=low;
        long long splitInv = 0;
        // merge temp arrays back into Arr[low,..high]
        while(i != sizeofarray_1 && j != sizeofarray_2){
            if(temp_array_1[i] < temp_array_2[j]){
                Arr[k] = temp_array_1[i];
                i++;
            }
            else{
                Arr[k] = temp_array_2[j];
                j++;
                splitInv += sizeofarray_1 - i;
            }
            k++;
        }

        // copy the remaining elements of temp_array_1, if any
        while(i != sizeofarray_1){
            Arr[k] = temp_array_1[i];
            i++;
            k++;
        }

        // copy the remaining elements of temp_array_2, if any
        while(j != sizeofarray_2){
            Arr[k] = temp_array_2[j];
            j++;
            k++;
        }

        return splitInv;
    }
};

int main(){
    ifstream myfile("02.IntegerArray.txt");
    if(!myfile.is_open())
    {
        cout << "File failed to open" << endl;
        return 0;
    }

    vector<int> X;
    int integer;

    while(myfile >> integer)
        X.push_back(integer);

    DAC dac;
    long long count = dac.SortCountInv(X, 0, X.size()-1);
    cout << count << endl;

    return 0;
}
