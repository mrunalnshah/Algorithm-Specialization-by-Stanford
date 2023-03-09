/*
 * File Name: Heap Sort
 *
 * Credit : Course     : Graph Search, Shortest Paths, and Data Structures, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
            Website    : https://www.geeksforgeeks.org/heap-sort/
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 21th Of FEBURARY, 2023
 *
 * Description: Heap sort is a comparison-based sorting technique based on Binary Heap data structure. It is similar to the selection sort where we first find the minimum element and place the minimum element at the beginning. Repeat the same process for the remaining elements.
 *
 * Dependencies: This Code requires a folder named "./heapsort_testcases" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include <cassert>
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

bool verify(vector<int> const &sorted, vector<int> const &original)
{
   vector<int> correctAnswer = original;
   sort(correctAnswer.begin(), correctAnswer.end());

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

template <typename T>
class MinHeap
{
   vector<T> elements;

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

      return elements[i] <= elements[left_index] &&
            (right_index == -1 || elements[i] <= elements[right_index]);
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

   T ExtractMin()
   {
      int n = elements.size();
      assert(n > 0);

      // Dev time assert: Root is the true minimum
      {
         int minimum = elements[0];
         for (int i = 1; i < elements.size(); ++i)
            if (elements[i] < minimum)
               minimum = elements[i];

         assert(elements[0] == minimum);
      }

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
         int swap_index = right_index == -1 ? left_index : (elements[left_index] < elements[right_index] ? left_index : right_index);
         T temp = elements[i];
         elements[i] = elements[swap_index];
         elements[swap_index] = temp;
         i = swap_index;
      }

      // Just return the min now
      return minimum;
   }
};

void heapSort(vector<int> &numbers)
{
   MinHeap<int> heap;

   for (auto x : numbers)
      heap.Insert(x);

   int n = numbers.size();
   for (int i = 0; heap.size() > 0; ++i)
      numbers[i] = heap.ExtractMin();
}

int main()
{
   // Find all test case files (courtesy of https://stackoverflow.com/a/612176)
   vector<filesystem::path> test_case_files;
   string const TESTDIR = "./heapsort_testcases";
   for (const auto & entry : filesystem::directory_iterator(TESTDIR))
      test_case_files.push_back(entry.path());

   // Sort them
   sort(test_case_files.begin(), test_case_files.end());

   int const n_start = 1, n = test_case_files.size();
   for (auto it = test_case_files.begin() + n_start - 1; it != test_case_files.begin() + n_start - 1 + n; ++it)
   {
      string const &filename = it->string();
      cout << filename << ":" << endl;

      ifstream file(*it);
      vector<int> testcase;
      int x;
      while (file >> x)
         testcase.push_back(x);

      auto original = testcase; // make a copy
      heapSort(testcase);
      verify(testcase, original);
      cout << endl;
   }

   return 0;
}
