/*
 * File Name: MergeSort.cpp
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 15th Of FEBURARY, 2023
 *
 * Description: The code implements the Merge Sort.
 */

#include <bits/stdc++.h>

using namespace std;

// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int temp_array[], int low, int mid, int high) {
    int i = low;
    int j = mid;
    int k = low;
    while (i < mid && j <= high) {
        if (arr[i] < arr[j]) {
            temp_array[k] = arr[i];
            i++;
            k++;
        } else {
            temp_array[k] = arr[j];
            j++;
            k++;
        }
    }
    while (i < mid) {
        temp_array[k] = arr[i];
        i++;
        k++;
    }
    while (j <= high) {
        temp_array[k] = arr[j];
        j++;
        k++;
    }
    for (int i = low; i <= high; i++) {
        arr[i] = temp_array[i];
    }
}

void mergeSort(int arr[], int temp_array[], int low, int high) {
    if (low >= high){
        return;
    }else{
        int mid = (low+high)/2;
        mergeSort(arr, temp_array, low, mid);
        mergeSort(arr, temp_array, mid+1, high);
        merge(arr, temp_array, low, mid+1, high);
    }
}

int main() {
    int sizeofarray;
    cout << "Enter Size of Array : ";
    cin >> sizeofarray;
    int arr[sizeofarray];
    int temp_array[sizeofarray];

    cout << endl;
    for(int i = 0; i < sizeofarray ; i++){
        cout << "Input Array[" << i << "]";
        cin >> arr[i];
    }
    mergeSort(arr, temp_array, 0, sizeofarray-1);
    for (int i = 0; i < sizeofarray; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}
