/*
 * File Name: Programming Assignment #3
 *
 * Credit : Course     : Graph Search, Shortest Paths, and Data Structures, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
            Website    : https://stackoverflow.com/questions/58593657/implementation-of-median-maintenance
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 23th Of FEBURARY, 2023
 *
 * Description:  implement the "Median Maintenance" algorithm
 *
 * Dependencies: This Code requires a folder named "./02. testcases" and you can add all the test cases there.
 */
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include <cassert>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#pragma region Utilities
template <typename T>
ostream &operator<<(ostream &os, vector<T> const &v)
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

template <typename T, typename ComparisonOperator=std::less<T>>
bool verify(vector<T> const &sorted, vector<T> const &original)
{
   vector<T> correctAnswer = original;
   sort(correctAnswer.begin(), correctAnswer.end(), ComparisonOperator());

   if (sorted.size() != correctAnswer.size())
      return false;

   for (int i = 0; i < sorted.size(); ++i)
   {
      if (sorted[i] != correctAnswer[i])
      {
         cout << "(FAILED!)" << endl;
         cout << "\tOriginal = " << original << endl;
         cout << "\tActual = " << sorted << endl;
         cout << "\tExpected = " << correctAnswer << endl;
         return false;
      }
   }

   return true;
}
#pragma endregion

template <typename T, typename ComparisonOperator>
class Heap
{
   vector<T> elements;
   static ComparisonOperator compare;

   template <typename U, typename C> friend ostream& operator<<(ostream& os, Heap<U, C> const& heap);

   int GetParentIndex(int i) const
   {
      return i == 0 ? -1 : ((i+1) / 2)-1;
   }

   int GetLeftChildIndex(int i) const
   {
      int x = 2*i + 1;
      return x < elements.size() ? x : -1;
   }

   int GetRightChildIndex(int i) const
   {
      int x = 2*i + 2;
      return x < elements.size() ? x : -1;
   }

   bool SatisfiesHeapProperty(int i) const
   {
      int n = elements.size();
      if (i < 0 || i >= n)
         return false;

      int left_index = GetLeftChildIndex(i);

      // If not even the left child exists, then it follows that the right child also does not exist, meaning we are at a leaf node
      if (left_index == -1)
         return true;
      // After this point, we are guaranteed that at least the left child exists

      int right_index = GetRightChildIndex(i);

      return compare(elements[i], elements[left_index]) && (right_index == -1 || compare(elements[i], elements[right_index]));
   }

public:
   size_t size() const
   {
      return elements.size();
   }

   void Insert(T const& value)
   {
      // Append the new element to the end of the array, meaning that it is on the last level of the heap tree
      elements.push_back(value);

      // Bubble-up the element until it finds its proper place
      for (int i = elements.size()-1; i > 0; /**/)
      {
         int parent_index = GetParentIndex(i);

         if (SatisfiesHeapProperty(parent_index))
            break;

         // Swap with the parent and keep going
         T temp = elements[i];
         elements[i] = elements[parent_index];
         elements[parent_index] = temp;
         i = parent_index;
      }
   }

   T const& Peek() const
   {
      assert(size() > 0);
      return elements[0];
   }

   T Extract()
   {
      int n = elements.size();
      assert(n > 0);

      // Swap root (min element) with last element in the last level of the heap tree
      T temp = elements[0];
      elements[0] = elements[n-1];
      elements[n-1] = temp;

      // Delete the min (now the last element) from the tree, keep it aside to return
      T minimum = elements[n-1];
      elements.pop_back();
      --n;

      // Bubble-down the new illegitimate root until it finds its proper place
      for (int i = 0; i < n && !SatisfiesHeapProperty(i); /**/)
      {
         int left_index = GetLeftChildIndex(i);
         int right_index = GetRightChildIndex(i);

         // Swap with the smallest of the two children and try again
         int swap_index = right_index == -1 ? left_index : (compare(elements[left_index], elements[right_index]) ? left_index : right_index);
         T temp = elements[i];
         elements[i] = elements[swap_index];
         elements[swap_index] = temp;
         i = swap_index;
      }

      // Just return the min now
      return minimum;
   }
};

template<typename T, typename C>
C Heap<T, C>::compare;

template<typename T, typename C>
ostream& operator<<(ostream& os, Heap<T, C> const& heap)
{
   os << "[";
   for (int i = 0; i < heap.elements.size(); ++i)
   {
      os << heap.elements[i];
      if (i != heap.elements.size() - 1)
         os << ", ";
   }
   os << "]";

   return os;
}

template<typename T>
using MinHeap = Heap<T, std::less<T>>;
template<typename T>
using MaxHeap = Heap<T, std::greater<T>>;

template<typename T>
class MedianMaintainer
{
   MaxHeap<T> lower; // lower half
   MinHeap<T> upper; // upper half

   bool HalvesAreBalanced() const
   {
      int n_lower = lower.size();
      int n_upper = upper.size();
      return std::abs(n_lower - n_upper) <= 1;
   }

public:
   size_t size() const
   {
      return upper.size() + lower.size();
   }

   void Add(T const& value)
   {
      int n = size();

      // Expect the Invariant to hold
      assert(HalvesAreBalanced());

      if (n == 0)
         lower.Insert(value);
      else if (value > lower.Peek())
         upper.Insert(value);
      else
         lower.Insert(value);

      // The expected Invariant could now be broken, so fix it by "balancing" the two halves before returning
      if (!HalvesAreBalanced())
      {
         if (lower.size() > upper.size())
            upper.Insert(lower.Extract());
         else
            lower.Insert(upper.Extract());
      }
   }

   T GetCurrentMedian() const
   {
      int n = size();
      assert(n > 0);

      int dt = lower.size() - upper.size();
      return dt < 0 ? upper.Peek() : lower.Peek();
   }
};


int main()
{
   // Find all test case files (courtesy of https://stackoverflow.com/a/612176)
   vector<filesystem::path> test_case_files;
   string const TESTDIR = "./02.testcases";
   for (const auto & entry : filesystem::directory_iterator(TESTDIR))
      test_case_files.push_back(entry.path());

   // Sort them
   sort(test_case_files.begin(), test_case_files.end());

   int const n_start = 1, n = test_case_files.size();
   for (auto it = test_case_files.begin() + n_start - 1; it != test_case_files.begin() + n_start - 1 + n; ++it)
   {
      string const &filename = it->string();
      cout << filename << ":" << endl;

      MedianMaintainer<int> mm;
      long long median_sum = 0;
      vector<int> testcase;

      ifstream file(*it);
      int x;
      while (file >> x)
      {
         testcase.push_back(x);

         mm.Add(x);
         int median = mm.GetCurrentMedian();
         // {
         //    // Dev-time assert: This is truly the median
         //    int n = testcase.size();
         //    int median_index = n/2 - (1 - (n % 2));
         //    auto clone = testcase;
         //    sort(clone.begin(), clone.end());
         //    cout << "Expected = " << clone[median_index] << ", actual = " << median << endl;
         //    assert(clone[median_index] == median);
         // }
         median_sum += (long long)median;
      }

      cout << "\tInput = " << testcase << endl;
      cout << "Sum of Medians = " << median_sum << endl;
      cout << "Last 4 digits of Sum of Medians = " << median_sum % 10000LL << endl;
   }

   return 0;
}
