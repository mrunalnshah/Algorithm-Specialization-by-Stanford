/*
 * File Name: unimodal
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 16th Of FEBURARY, 2023
 *
 * Description: You are a given a _unimodal_ array of n distinct elements, meaning that its entries are in increasing order up until its maximum element, after which its elements are in decreasing order.
 * Give an algorithm to compute the maximum element that runs in O(log n) time.
*/
#include <iostream>

#include <vector>
#include <algorithm>

using namespace std;

#pragma region Utilities
bool verify(vector<int> const &v, int actual)
{
   vector<int> sorted = v;
   sort(sorted.begin(), sorted.end(), greater<int>());
   return actual == sorted[0];
}

ostream &operator<<(ostream &os, vector<int> const &v)
{
   os << "[";
   for (int i = 0; i < v.size(); ++i)
   {
      os << v[i];
      if (i != v.size() - 1)
         os << ", ";
   }
   os << "]";

   return os;
}
#pragma endregion

/**
 * To yield a logarithmic upper-bound i.e. O(log n), we can use the Master Method to find the required properties of our algorithm.
 * Concretely, since log(n) = n^0 * log(n), we know that i) the d constant MUST be 0 and ii) a must equal b^d.
 * If d = 0, then a HAS to be 1 since b^0 = 1.
 * In other words, we have to make a single recursive call per iteration and we have to halve the input size each time. That is, binary search is the name of the game!
 * The algorithm below honours the above constraints and thus achieves the target runtime upper-bound of O(log n).
 */
int findMaxFromUnimodal(vector<int> const &v, int s, int e) // s is included, e is excluded
{
   // We make the observation that in a unimodal array, the largest number has to be the "peak" number.
   // A peak number is one whose left and right values are smaller.
   // For example, given {1, 2, 10, 9, 8}, the peak number here is 10 since 10 > 2 and 10 > 9.
   // So, we simply perform binary search until we have reached a peak value.

   int n = e - s;

   if (n == 1)
      return v[s];

   int mid = s + n / 2;
   int left = mid - 1;
   int right = mid + 1;

   // (Note that only one recursive call is actually made due to the conditional branching)
   if (left >= 0 && v[left] > v[mid]) // slope is decreasing towards the right, so climb upwards to the left
      return findMaxFromUnimodal(v, s, mid);
   else if (right < v.size() && v[right] > v[mid]) // slope is increasing towards the right, so keep climbing upwards to the right
      return findMaxFromUnimodal(v, right, e);
   else // we are at the peak since both the left and the right values are smaller
      return v[mid];
}

int findMaxFromUnimodal(vector<int> const &v)
{
   return findMaxFromUnimodal(v, 0, v.size());
}

int main()
{
   vector<vector<int>> test_cases = {
       {10},
       {2, 10, -2},
       {1, 3, 4, 5, 7, 8, 10, 12, 13, 14, 10, 9, 6, 4, 3, 2},
       {7, 8, 10, 12, 13, 14, 10, 9, 6, 2},
       {10, 11, 12, 9, 2, -7, -16, -25, -250},
       {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
       {9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
   };

   for (auto &test_case : test_cases)
   {
      int result = findMaxFromUnimodal(test_case);
      bool success = verify(test_case, result);
      if (!success)
         cout << "(FAILED!) ";
      cout << "Result = " << result << ", Input = " << test_case;
      cout << endl;
   }

   return 0;
}
