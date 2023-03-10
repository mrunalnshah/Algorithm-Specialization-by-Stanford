/*
 * File Name: Programming Assignment #2
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 5th Of MARCH, 2023
 *
 * Description: Your task now is to run the greedy algorithm that schedules jobs (optimally) in decreasing order of the ratio (weight/length).  In this algorithm, it does not matter how you break ties.  You should report the sum of weighted completion times of the resulting schedule --- a positive integer
 *
 * Dependencies: This Code requires a folder named "jobs.txt" and you can add all the test cases there.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
  vector < tuple <double, double, double> > v;
  int jobs;
  ifstream file("jobs.txt");
  file >> jobs;
  double weight, length;
  while (file >> weight >> length) {
    double ratio = weight / length;
    v.push_back(make_tuple(ratio, weight, length));
  }

  // Sort jobs in decreasing order of difference (weight - length)
  // If difference is the same, schedule job with higher weight
  sort(v.begin(), v.end());
  long long sumLength = 0;
  long long weightedSum = 0;

  for (int i = v.size() - 1; i >= 0; i--) {
    sumLength += get<2>(v[i]);
    weightedSum += sumLength * get<1>(v[i]);
  }

  cout << weightedSum << endl;
}
