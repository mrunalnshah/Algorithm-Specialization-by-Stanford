/*
 * File Name: Programming Assignment #1
 *
 * Credit : Course     : Greedy Algorithms, Minimum Spanning Trees, and Dynamic Programming
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 5th Of MARCH, 2023
 *
 * Description: Your task is to run the greedy algorithm that schedules jobs in decreasing order of the difference (weight - length).  Recall from lecture that this algorithm is not always optimal.  IMPORTANT: if two jobs have equal difference (weight - length), you should schedule the job with higher weight first.  Beware: if you break ties in a different way, you are likely to get the wrong answer.  You should report the sum of weighted completion times of the resulting schedule --- a positive integer
 *
 * Dependencies: This Code requires a file named "jobs.txt" and you can add all the test cases there.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
  vector < tuple <int, int, int> > v;
  int jobs;
  ifstream file("jobs.txt");
  file >> jobs;
  int weight, length;
  while (file >> weight >> length) {
    int difference = weight - length;
    v.push_back(make_tuple(difference, weight, length));
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
