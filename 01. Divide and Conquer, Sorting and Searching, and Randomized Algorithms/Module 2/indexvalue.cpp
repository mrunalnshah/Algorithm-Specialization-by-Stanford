/*
 * File Name: Index Value
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 16th Of FEBURARY, 2023
 *
 * Description:  You are given a sorted (from smallest to largest) array A of n distinct integers which can be positive, negative, or zero.
                You want to decide whether or not there is an index i such that A[i] = i. Design the fastest algorithm that you can for solving this problem.
*/
#include <iostream>

#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#pragma region Utilities
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
 * The fastest algorithm would be logarithmic, so let's see if we can design a O(log n) algorithm.
 * We want the SORTED array A to satisfy the relation A[i] = i where i >= 0 for at least 1 i.
 * Key observation: By the definition of the relation AND the fact that A is sorted, there can only be a single contiguous sequence of elements for which the relation is true. This sequence is of size >= 0. We simply want to find this "target" sequence.
 * By the above observation, the scenario is ripe for a binary search approach.
 * The algorithm below implements this and yields the fastest time, i.e. O(log n).
 */
bool decideWhetherThereExistsIndexMatchingValue(vector<int> const &v, int s, int e)
{
   // In order to binary search successfully, we need a correct way to decide whether to search in the left or right half.
   // Note that when A[i] > i, the target sequence is on the left; and when A[i] < i, this means that the target sequence is on the right.
   // We've got a valid binary search!

   int n = e - s;

   if (n <= 0) // we've searched through the entire array and still not found the target sequence
      return false;

   int mid = s + n / 2;
   if (v[mid] > mid)
      return decideWhetherThereExistsIndexMatchingValue(v, s, mid);
   else if (v[mid] < mid)
      return decideWhetherThereExistsIndexMatchingValue(v, mid + 1, e);
   else // found an element of the target sequence
      return true;
}

bool decideWhetherThereExistsIndexMatchingValue(vector<int> const &v)
{
   return decideWhetherThereExistsIndexMatchingValue(v, 0, v.size());
}

int main()
{
    //Test cases
   vector<pair<vector<int>, bool>> test_cases = {
       {{0}, true},
       {{1}, false},
       {{0, 1}, true},
       {{1, 2}, false},
       {{0, 1, 2, 3, 4, 5}, true},
       {{-2, -1, 0, 1, 2, 3}, false},
       {{-2, 0, 1, 4, 10}, false},
       {{-30, -20, -21, 3, 4, 5}, true},
       {{4, 5, 9}, false},
       {{-100, -50, 0, 50, 100, 1000, 2000, 3000}, false},
       {{-100, -50, -49, -48, -47, -10, -20, 7, 10}, true},
       {{-100, -50, -49, -48, -47, -10, -20, 7, 9, 10}, true},
   };

   for (auto &test_case : test_cases)
   {
      int result = decideWhetherThereExistsIndexMatchingValue(test_case.first);
      bool success = result == test_case.second;
      if (!success)
         cout << "(FAILED!) ";
      cout << "Result = " << (result ? "true" : "false") << ", Input = " << test_case.first;
      cout << endl;
   }

   return 0;
}
