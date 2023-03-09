/*
 * File Name: Programming Assignment #4
 *
 * Credit : Course     : Graph Search, Shortest Paths, and Data Structures, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 22th Of FEBURARY, 2023
 *
 * Description:compute the number of target values tt in the interval [-10000,10000] (inclusive) such that there are distinct numbers x,yx,y in the input file that satisfy x+y=tx+y=t. (NOTE: ensuring distinctness requires a one-line addition to the algorithm from lecture.)
 *
 * Dependencies: This Code requires a folder named "./02. testcases" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include <cassert>
#include <vector>
#include <unordered_set>
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
#pragma endregion


typedef long long num_t;

class HasTargetAs2Sum
{
   vector<num_t> const& m_numbers;
   unordered_set<num_t> m_numbers_set;

public:
   HasTargetAs2Sum(vector<num_t> const& numbers)
      : m_numbers(numbers)
   {
      copy(numbers.begin(), numbers.end(), inserter(m_numbers_set, m_numbers_set.begin()));
   }

   bool CheckForTarget(int target) const
   {
      for (num_t x : m_numbers)
      {
         num_t y = static_cast<num_t>(target) - x;
         // TODO: This approach is taking way too long. Think of a better alternative
         if (x != y && m_numbers_set.find(y) != m_numbers_set.end())
            return true;
      }
      return false;
   }
};

int main()
{
   // Find all test case files (courtesy of https://stackoverflow.com/a/612176)
   vector<filesystem::path> test_case_files;
   string const TESTDIR = "./02. testcases";
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
      vector<num_t> testcase;
      num_t x;
      while (file >> x)
         testcase.push_back(x);

      HasTargetAs2Sum checker(testcase);
      int count = 0;
      for (int t = -10000; t <= 10000; ++t)
         if (checker.CheckForTarget(t))
            count++;
      cout << "Count = " << count << endl;

      cout << endl;
   }

   return 0;
}
