/*
 * File Name: Local Min
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 * Acknowledgements:
 *          I got some good hints from
 *              - https://www.coursera.org/learn/algorithms-divide-conquer/discussions/forums/-HCDbbpyEeayJw6eqJ0T8g/threads/y36Cyu0jQIe-gsrtI6CHGg
 *              - https://www.coursera.org/learn/algorithms-divide-conquer/discussions/forums/-HCDbbpyEeayJw6eqJ0T8g/threads/hegaBqHfQfCoGgah38HwTg
 *      The following served as good thinking aids to fully reason out the proof of correctness as well as the running time analysis
 *              - https://stackoverflow.com/a/24461101
 *              - http://courses.csail.mit.edu/6.006/spring11/lectures/lec02.pdf
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 16th Of FEBURARY, 2023
 *
 * Description:  You are given an n by n grid of distinct numbers. A number is a local minimum if it is smaller than all of its neighbors.
                (A neighbor of a number is one immediately above, below, to the left, or the right. Most numbers have four neighbors; numbers on the side have three; the four corners have two.)
                Use the divide-and-conquer algorithm design paradigm to compute a local minimum with only O(n) comparisons between pairs of numbers.
                (Note: since there are n^2 numbers in the input, you cannot afford to look at all of them.Hint: Think about what types of recurrences would give you the desired upper bound.)
 * Dependency : You need a folder named "./localmin_testcases" and can add all txt test cases in there.
*/
#include <iostream>
#include <fstream>
#include <dirent.h> // UNIX only; for Windows, need to get it elsewhere, see https://stackoverflow.com/a/612176/3477043
#include <cmath>
#include <set>

#include <limits>
#include <vector>
#include <array>
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
template <typename T>
ostream &operator<<(ostream &os, set<T> const &s)
{
   vector<T> v;
   copy(s.begin(), s.end(), back_inserter(v));
   os << v;

   return os;
}

set<int> verify(vector<int> const &matrix, int actual)
{
   // As a verification control, we implement the O(n^2) brute-force algorithm for finding a local minimum and compare it with the given result
   set<int> expected;
   int n = sqrt(matrix.size());
   for (int i = 0; i < n; ++i)
   {
      for (int j = 0; j < n; ++j)
      {
         int j_left = j - 1;
         int j_right = j + 1;
         int i_up = i - 1;
         int i_down = i + 1;

         int current = matrix[i*n + j];

         if (j_left >= 0 && matrix[i*n + j_left] < current)
            continue;
         if (j_right < n && matrix[i*n + j_right] < current)
            continue;
         if (i_up >= 0 && matrix[i_up*n + j] < current)
            continue;
         if (i_down < n && matrix[i_down*n + j] < current)
            continue;

         expected.insert(current);
      }
   }

   return expected;
}
#pragma endregion

#pragma region Helpers for the algorithm
struct Cell
{
   int row;
   int col;
   int min;

   bool operator< (Cell const& other) const
   {
      return min < other.min;
   }
};

// Convert a 2D index to a 1D index
int index(int row, int col, int n)
{
   return row*n + col;
}

Cell computeGlobalMinimum(int row_start, int row_end, int col_start, int col_end, vector<int> const& matrix, int matrix_dimension)
{
   Cell result;
   result.row = row_start;
   result.col = col_start;
   result.min = numeric_limits<int>::max();
   for (int i = row_start; i <= row_end; ++i)
   {
      if (i < 0 || i >= matrix_dimension)
         continue;

      for (int j = col_start; j <= col_end; ++j)
      {
         if (j < 0 || j >= matrix_dimension)
            continue;

         int cell = matrix[index(i, j, matrix_dimension)];
         if (cell < result.min)
         {
            result.row = i;
            result.col = j;
            result.min = cell;
         }
      }
   }
   return result;
}

bool isWithinRange(Cell const& cell, int row_start, int row_end, int col_start, int col_end)
{
   int row = cell.row;
   int col = cell.col;
   return row_start <= row && row <= row_end && col_start <= col && col <= col_end;
}
#pragma endregion

/**
 * Our target is O(n) comparisons. Plugging this into the Master Method, we would need a < b and d = 1. So if a = 2, then as long as b > 3, we will meet our target.
 * Our best-case scenario though would be a = 1 and b = 2. Let's try to meet the target with these parameters.
 * Constraints: maximum 1 recursive call, the input size is halved every time, and we do O(n) comparisons per subproblem.
 *
 * To preserve a square matrix in the divide step, we will need to work on n/2 x n/2 submatrices. Meaning we split into 4 submatrices but recurse into only 1.
 * For the algorithm to work correctly, we need to maintain the following invariants:
 * - The submatrix is guaranteed to contain a local minimum of the main matrix.
 * - The subset of elements examined within a submatrix is guaranteed to either contain the local minimum or reliably lead one to the next candidate submatrix.
 *
 * With that said, here's the proposed algorithm:
 * - Base case: if we have a 1 x 1 submatrix (a single number), we have found the local minimum of the whole matrix, so return it.
 * - Compute the global minimum element in the "window", i.e. find the smallest element in the "window".
 *    - The "window" is defined as the n/2th column, the n/2th row and the boundary elements around the submatrix. If the boundary element is out-of-bounds, then either discard it or assume it is infinity.
 * - If this minimum is a local minimum of the submatrix, we are done, return it.
 * - Else, by definition of this minimum, the only neighbours that need to be checked are the ones that are NOT within the window. There should be either 1 or 2 neighbours to be checked.
 * - Pick the smallest neighbour and recurse into the quadrant submatrix that it belongs to.
 *    - Note that this submatrix will be of size floor(n/2) x floor(n/2), so when partitioning the matrix, if its dimension is even, then all of the quadrants won't necessarily be square. Count this by padding it with infinity until it becomes a square. This extra padding is negligible in the time complexity analysis because it only modifies n by a constant amount of 1.
 *
 * The correctness of the algorithm is proven as follows:
 * 1. The boundary elements are either positive infinity OR are members of the window, as per the definition.
 * 2. Because we are always choosing a neighbour that is smaller than the window's global minimum element, the neighbour will be smaller than all of the boundary elements as well. So when we keep recursing, we are always finding neighbours that are smaller than the boundary elements. Thus, we are guaranteed to reach a local minimum of the whole matrix. If we reach the 1x1 base case, we have guaranteed that this lone element is smaller than the entire boundary and thus by definition is a local minimum of the whole matrix.
 *
 * Comparison count analysis:
 * - We are making only 1 recursive call each iteration. So a is truly 1.
 * - The input size n is halving for each subproblem (from n x n matrix to n/2 x n/2 matrix). So b is truly 2.
 * - Each subproblem is of order O(n).
 *    - There are 4n + 4 boundary elements.
 *    - The cross-section formed by the middle row and column has 2n - 1 elements.
 *    - So total number of elements in the window (which are the only elements considered in each subproblem) = 4n + 4 + 2n - 1 = 6n + 3 = O(n)
 *    - This means that d = 1.
 * - We have thus satisfied the Master Method constraint and so the time complexity is O(n) and by definition of max computing, the total number of comparisons is also O(n).
 *
 * Alternate way of counting comparisons without the Master Method:
 * - From before, each subproblem involves 6n + 3 comparisons. We know that 6n + 3 < 12n
 * - And again, the subproblem size is halved each time.
 * - So, total number of comparisons < 12*n + 12*n/2 + 12*n/4 + 12*n/8 + ... 12*1 <= 24n = O(n)
 */
int computeLocalMinimum(int row_start, int row_end, int col_start, int col_end, vector<int> const& matrix, int matrix_dimension) // starts and ends are all INCLUSIVE
{
   int submatrix_row_start = row_start + 1;
   int submatrix_row_end = row_end - 1;
   int submatrix_col_start = col_start + 1;
   int submatrix_col_end = col_end - 1;

   int n = submatrix_row_end - submatrix_row_start + 1;

   // Base case
   if (n == 1)
   {
      int i = row_start + 1;
      int j = col_start + 1;
      return matrix[index(i, j, matrix_dimension)];
   }
   // Else, Conquer first, then divide

   // Compute global minimum element of the "window"
   vector<Cell> window;
   // Boundary surrounding the (sub)matrix
   window.push_back(computeGlobalMinimum(row_start, row_end, col_start, col_start, matrix, matrix_dimension));
   window.push_back(computeGlobalMinimum(row_start, row_end, col_end, col_end, matrix, matrix_dimension));
   window.push_back(computeGlobalMinimum(row_start, row_start, col_start, col_end, matrix, matrix_dimension));
   window.push_back(computeGlobalMinimum(row_end, row_end, col_start, col_end, matrix, matrix_dimension));
   // Cross-section of the middle row and column
   int row_mid = row_start + 1 + n/2;
   int col_mid = col_start + 1 + n/2;
   window.push_back(computeGlobalMinimum(row_start, row_end, row_mid, row_mid, matrix, matrix_dimension));
   window.push_back(computeGlobalMinimum(row_mid, row_mid, col_start, col_end, matrix, matrix_dimension));

   auto min_result = window[0];
   for (int k = 1; k < window.size(); ++k)
   {
      if (window[k] < min_result)
         min_result = window[k];
   }

   // Find the smallest neighbor (some repeated work, but to keep the code simple, just blindly check all neighbors even though in some cases you DON'T need to check some of them due to the definition of the global minimum)
   vector<Cell> neighbors;
   Cell smallestNeighbor = min_result;
   int row_up = min_result.row - 1;
   int row_down = min_result.row + 1;
   int col_left = min_result.col - 1;
   int col_right = min_result.col + 1;
   if (row_up >= submatrix_row_start)
      neighbors.push_back({ row_up, smallestNeighbor.col, matrix[index(row_up, smallestNeighbor.col, matrix_dimension)] });
   if (row_down <= submatrix_row_end)
      neighbors.push_back({ row_down, smallestNeighbor.col, matrix[index(row_down, smallestNeighbor.col, matrix_dimension)] });
   if (col_left >= submatrix_col_start)
      neighbors.push_back({ smallestNeighbor.row, col_left, matrix[index(smallestNeighbor.row, col_left, matrix_dimension)] });
   if (col_right <= submatrix_col_end)
      neighbors.push_back({ smallestNeighbor.row, col_right, matrix[index(smallestNeighbor.row, col_right, matrix_dimension)] });

   bool foundLocalMinimum = true;
   for (auto const& neighbor : neighbors)
   {
      if (neighbor < smallestNeighbor)
      {
         foundLocalMinimum = false;
         smallestNeighbor = neighbor;
      }
   }

   // All clear, we are at a local minimum of the submatrix
   if (foundLocalMinimum)
      return min_result.min;

   // Otherwise, recurse into the quadrant submatrix that the smallest neighbor belongs to
   int n_halved = n/2;
   // Top Left
   int row_top_left_start = row_start;
   int row_top_left_end = row_start + n_halved + 1;
   int col_top_left_start = col_start;
   int col_top_left_end = col_top_left_start + n_halved + 1;
   // Top Right
   int row_top_right_start = row_top_left_start;
   int row_top_right_end = row_top_left_end;
   int col_top_right_start = col_start + n_halved + 1;
   int col_top_right_end = col_top_right_start + n_halved + 1;
   // Bottom Right
   int row_bottom_right_start = col_top_right_start;
   int row_bottom_right_end = col_top_right_end;
   int col_bottom_right_start = col_top_right_start;
   int col_bottom_right_end = col_top_right_end;
   // Bottom Left
   int row_bottom_left_start =  row_bottom_right_start;
   int row_bottom_left_end = row_bottom_right_end;
   int col_bottom_left_start = col_top_left_start;
   int col_bottom_left_end = col_top_left_end;

   // Find the right one and jump in
   int row_final_start = row_top_left_start;
   int row_final_end = row_top_left_end;
   int col_final_start = col_top_left_start;
   int col_final_end = col_top_left_end;
   if (isWithinRange(smallestNeighbor, row_top_right_start, row_top_right_end, col_top_right_start, col_top_right_end))
   {
      row_final_start = row_top_right_start;
      row_final_end = row_top_right_end;
      col_final_start = col_top_right_start;
      col_final_end = col_top_right_end;
   }
   else if (isWithinRange(smallestNeighbor, row_bottom_right_start, row_bottom_right_end, col_bottom_right_start, col_bottom_right_end))
   {
      row_final_start = row_bottom_right_start;
      row_final_end = row_bottom_right_end;
      col_final_start = col_bottom_right_start;
      col_final_end = col_bottom_right_end;
   }
   else if (isWithinRange(smallestNeighbor, row_bottom_left_start, row_bottom_left_end, col_bottom_left_start, col_bottom_left_end))
   {
      row_final_start = row_bottom_left_start;
      row_final_end = row_bottom_left_end;
      col_final_start = col_bottom_left_start;
      col_final_end = col_bottom_left_end;
   }

   return computeLocalMinimum(row_final_start, row_final_end, col_final_start, col_final_end, matrix, matrix_dimension);
}

// Assumes that the given 1D vector consists of distinct numbers that form a square matrix
int computeLocalMinimum(vector<int> const& matrix)
{
   int n = sqrt(matrix.size());
   return computeLocalMinimum(-1, n, -1, n, matrix, n);
}

int main()
{
   // Find all test case files (courtesy of https://stackoverflow.com/a/612176/3477043)
   vector<string> test_case_files;
   DIR *dir;
   struct dirent *ent;
   string const TESTDIR = "./localmin_testcases";
   if ((dir = opendir(TESTDIR.c_str())) != nullptr)
   {
      while ((ent = readdir(dir)) != nullptr)
         if (ent->d_name[0] != '.')
            test_case_files.push_back(ent->d_name); // assumes that the file is not a dir
      closedir(dir);
   }
   else
   {
      // Could not open directory
      perror("");
      return EXIT_FAILURE;
   }

   // Sort them
   sort(test_case_files.begin(), test_case_files.end());

   // For each case, read the file and compute a local minimum
   int const n_start = 1, n = test_case_files.size();
   // int const n_start = 1, n = 3;
   for (auto it = test_case_files.begin() + n_start - 1; it != test_case_files.begin() + n_start - 1 + n; ++it)
   {
      string const &filename = *it;
      string const filepath = TESTDIR + "/" + filename;
      vector<int> test_case;
      ifstream file(filepath);
      int x;
      while (file >> x)
         test_case.push_back(x);

      int actual = computeLocalMinimum(test_case);
      auto expected = verify(test_case, actual);
      if (expected.find(actual) == expected.end())
         cout << "(FAILED! Expected to be one of " << expected << ") ";
      cout << "Found = " << actual;
      cout << "; " << filename;
      cout << "; " << endl;
   }

   return 0;
}
