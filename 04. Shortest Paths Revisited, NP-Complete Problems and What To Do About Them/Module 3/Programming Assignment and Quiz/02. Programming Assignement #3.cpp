/*
 * File Name: Programming Assignment #3
 *
 * Credit : Course     : Shortest Paths Revisited, NP-Complete Problems and What To Do About Them
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 8th Of MARCH, 2023
 *
 * Description: implement the nearest neighbor heuristic
 * Dependencies: This Code require files named "nn.txt" one file compiled at a time.
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    ifstream file("nn.txt");
    file >> n;

    bool visited[n] = {0};

    int index;
    double x, y;
    vector<pair <double, double>> v;
    while (file >> index >> x >> y) {
        v.push_back(make_pair(x, y));
    }
    double total_dist = 0.0;
    int src = 0;
    int num_visited = 1;
    int dest;
    visited[0] = true;
    double min_dist = DBL_MAX;
    while (num_visited < n) {
        for (int i = 1; i < n; i++) {
            if (!visited[i]) {
                double dist = sqrt((v[src].first - v[i].first) * (v[src].first - v[i].first) + (v[src].second - v[i].second) * (v[src].second - v[i].second));
                if (dist < min_dist) {
                    min_dist = dist;
                    dest = i;
                }
            }
        }
        total_dist += min_dist;
        visited[dest] = true;
        src = dest;
        min_dist = DBL_MAX;
        num_visited++;
        if (num_visited % 100 == 0) cout << "visited  " << num_visited << endl;
    }
    total_dist += sqrt((v[src].first - v[0].first) * (v[src].first - v[0].first) + (v[src].second - v[0].second) * (v[src].second - v[0].second));

    cout << total_dist << endl;
    cout << fixed;
    cout << total_dist << endl;
}
