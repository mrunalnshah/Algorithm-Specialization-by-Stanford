/*
 * File Name: Breadth First Search (BFS)
 *
 * Credit : Course     : Graph Search, Shortest Paths, and Data Structures, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 18th Of FEBURARY, 2023
 *
 * Description:  The breadth-first search (BFS) algorithm is used to search a tree or graph data structure for a node that meets a set of criteria.
 */

#include <bits/stdc++.h>
using namespace std;

void addEdge(vector<int> adj[], int s, int d) {
  adj[s].push_back(d);
}

void BFS(vector<int> adj[], int s, int V) {
  int arr[V] = {0};
  arr[s] = 1;
  queue<int> Q;
  Q.push(s);
  while (!Q.empty()) {
    int v = Q.front();
    cout << v << " ";
    Q.pop();
    for (auto x : adj[v]) {
      if (arr[x] == 0) {
        arr[x] = 1;
        Q.push(x);
      }
    }
  }
}

int main() {
  int V = 4;
  vector<int> adj[V];
  addEdge(adj, 0, 1);
  addEdge(adj, 0, 2);
  addEdge(adj, 1, 2);
  addEdge(adj, 2, 0);
  addEdge(adj, 2, 3);
  addEdge(adj, 3, 3);

  BFS(adj, 2, V);
}
