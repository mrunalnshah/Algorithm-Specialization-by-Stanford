/*
 * File Name: max2
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 * Acknowledgements:
 *      Got good hints from
 *          - https://www.coursera.org/learn/algorithms-divide-conquer/discussions/forums/-HCDbbpyEeayJw6eqJ0T8g/threads/hegaBqHfQfCoGgah38HwTg
 *          - https://www.coursera.org/learn/algorithms-divide-conquer/discussions/weeks/2/threads/a1Vay5IPEeeZQBKMOoXdvg/replies/5ifieSO7EeiMeA6rxdO7VA/comments/PA_EViQrEeiaqg5F6uM_rA
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 16th Of FEBURARY, 2023
 *
 * Description:  You are given as input an unsorted array of n distinct numbers, where n is a power of 2. Give an algorithm that identifies the
                second-largest number in the array, and that uses at most n + lg(n) - 2n comparisons.
*/
#include <iostream>

#include <limits>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#pragma region Utilities
bool verify(vector<int> const &v, int actual)
{
   int n = v.size();

   if (n < 2)
      return actual == v[0];

   vector<int> sorted = v;
   sort(sorted.begin(), sorted.end());
   return actual == sorted[n - 2];
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
 * Number of comparisons analysis when n is a power of 2:
 * - Every single subproblem performs exactly 1 comparison.
 * - Since the base case kicks in when n = 2, the total number of subproblems is equal to the total number of nodes in the perfect problem division binary tree of height less than 1, i.e. 2^lg(n) - 1 subproblems. And 2^lg(n) = n, so we have n - 1 subproblems * 1 = n - 1 comparisons.
 * - After the above divide-and-conquer algorithm, the remaining procedure is to compute the max among the second-largest candidates. This can be done using k - 1 comparisons, where k is the total number of candidates.
 * - Note that in every non-base case of the divide-and-conquer algorithm, we added 1 to the winning candidates.
 * - This can be expressed by the recurrence k(n) = k(n/2) + 1 when n > 2, with k(2) = 1. From a bottom-up perspective, as we double n, we add 1. This is nothing other than lg(n).
 * - So, k = lg(n). Thus, the procedure has k - 1 = lg(n) - 1 comparisons.
 * - Putting it all together, the total number of comparisons = n - 1 + lg(n) - 1 = n + lg(n) - 2.
 */

struct MaxAndCandidates
{
   int max;
   vector<int> candidates;
};

void findLargestAndSecondLargestCandidates(vector<int> const &v, int s, int e, MaxAndCandidates &result) // Note: the range is [s, e), that is inclusive of s, but exclusive of e
{
   int n = e - s;
   if (n == 1)
   {
      result.max = v[s];
   }
   else if (n == 2)
   {
      // Base case: with two numbers, the highest and the second highest numbers are trivially determined
      int first = v[s];
      int second = v[s + 1];
      if (first > second)
      {
         result.max = first;
         result.candidates.push_back(second);
      }
      else
      {
         result.max = second;
         result.candidates.push_back(first);
      }
   }
   else
   {
      // Divide
      int mid = s + n / 2;
      MaxAndCandidates result_half1, result_half2;
      findLargestAndSecondLargestCandidates(v, s, mid, result_half1);
      findLargestAndSecondLargestCandidates(v, mid, e, result_half2);

      // Conquer
      // We can determine the highest number of the candidates trivially
      // We CAN'T yet determine the second-highest, but we DO know which are the 2 candidates based on which one is the highest one
      MaxAndCandidates *winner, *loser;
      if (result_half1.max > result_half2.max)
      {
         winner = &result_half1;
         loser = &result_half2;
      }
      else
      {
         winner = &result_half2;
         loser = &result_half1;
      }

      // The max is determined trivially
      result.max = winner->max;

      // Now, to consider the second-highest candidates:
      // In the case of the loser, the only valid candidate is the loser's max. We can completely ignore the loser's candidates because by definition they CAN'T be valid candidates for the second-largest number if the loser's max isn't the second-largest
      result.candidates.push_back(loser->max);
      // In the case of the winner, we know that all of its candidates are valid
      copy(winner->candidates.begin(), winner->candidates.end(), back_inserter(result.candidates));
   }
}

int findSecondLargest(vector<int> const &v)
{
   int n = v.size();

   if (n < 2)
      return v[0];

   MaxAndCandidates result;
   findLargestAndSecondLargestCandidates(v, 0, n, result);

   int secondHighest = result.candidates[0];
   for (int i = 1; i < result.candidates.size(); ++i)
   {
      int candidate = result.candidates[i];
      if (candidate > secondHighest)
         secondHighest = candidate;
   }

   return secondHighest;
}

int main()
{
   vector<vector<int>> test_cases = {
       {8, 4, 5, 4, 9, 2, 0, 1},
       {9, 1, 90, 20, 23},
       {9},
       {-5, 4, 14, 6, 17, -2, 3, -11, 20, -12},
       {-1, 20, 19, 15, -13, -11, -15, 4, -18, 9},
       {-15, -1, -14, 14, -10, -9, -11, 16, -13, 1},
       {20, -9, -19, 9, 15, -12, 16, -16, -10, 10},
       {-1, -17, 19, 10, -8, -15, 4, -3, -18, -10},
       {17, 18, -6, 7, -9, -15, -1, -2, 16, 1},
       {9, 13, -4, -18, -8, -16, 1, 18, 14, 15},
       {-12, 9, -13, -2, -15, -1, 1, -6, 4, -14},
       {-10, -19, -8, 13, -6, 0, 10, -20, -16, -1},
       {10, 7, 4, 5, -6, -18, -13, 11, -12, 9},
   };

   for (auto &test_case : test_cases)
   {
      int result = findSecondLargest(test_case);
      bool success = verify(test_case, result);
      if (!success)
         cout << "(FAILED!) ";
      cout << "Result = " << result << ", Input = " << test_case;
      cout << endl;
   }

   return 0;
}
