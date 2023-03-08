/*
 * File Name: QuickSort
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 16th Of FEBURARY, 2023
 *
 * Description: Sort the integers using QuickSort Algorithm
 */

#include <bits/stdc++.h>

using namespace std;

void swap(int arr[], int a, int b) {
    int temp = arr[b];
    arr[b] = arr[a];
    arr[a] = temp;
}

int findMedian(int arr[], int low, int high) {
    int n = high-low+1;
    int pos = 0;
    if (n % 2 == 0) {
        pos = n/2 - 1 + low;
    } else {
        pos = n/2 + low;
    }
    int a = arr[low];
    int b = arr[pos];
    int c = arr[high];
    int maxi = max({a, b, c});
    int mini = min({a, b, c});

    if (a != maxi && a != mini){
        return low;
    }else if (b != maxi && b != mini){
        return pos;
    }else{
        return high;
    }
}

int partition(int arr[], int low, int high, int pi) {
    if (pi != 0){
        swap(arr, low, pi);
    }
    int pivot = arr[low];
    int i = low+1;
    for (int j = low+1; j <= high; j++) {
        if (arr[j] < pivot) {
            swap(arr, i, j);
            i++;
        }
    }
    swap(arr, low, i-1);
    return i-1;
}

void quickSort(int arr[], int low, int high) {
    if (high <= low){
        return;
    }
    // first element of the array is the pivot element:
    // int position = partition(arr, low, high, 0);
    // last element of the array is the pivot element:
    // int position = partition(arr, low, high, high);
    // median element is the pivot element:
    int position = partition(arr, low, high, findMedian(arr, low, high));
    quickSort(arr, low, position-1);
    quickSort(arr, position+1, high);
}

int main() {
    int sizeofarray;
    cout << "Enter Size of array :  ";
    cin >> sizeofarray;
    int arr[sizeofarray];
    cout << endl;

    for(int i = 0; i < sizeofarray; i++){
        cout << "Input Array[" << i << "] : ";
        cin >> arr[i];
    }
    cout << endl;
    quickSort(arr, 0, sizeofarray-1);
    for (int i = 0; i < sizeofarray; i++) {
        cout << arr[i] << " ";
    }
}
