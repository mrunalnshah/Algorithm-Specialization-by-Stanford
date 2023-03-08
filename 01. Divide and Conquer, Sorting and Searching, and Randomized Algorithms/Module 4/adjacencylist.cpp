/*
 * File Name: adjacency list
 *
 * Credit : Course     : Divide and Conquer, Sorting and Searching, and Randomized Algorithms, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 17th Of FEBURARY, 2023
 *
 * Description:  An adjacency list represents a graph as an array of linked lists.
                 The index of the array represents a vertex and each element in its linked list represents the other vertices that form an
                 edge with the vertex.
 */

#include <bits/stdc++.h>
using namespace std;

// Add edge
void addEdge(vector<int> adj[], int s, int d) {
  adj[s].push_back(d);
  adj[d].push_back(s);
}

// Print the graph
void printGraph(vector<int> adj[], int V) {
  for (int d = 0; d < V; ++d) {
    cout << "\n Vertex "
       << d << ":";
    for (auto x : adj[d])
      cout << "-> " << x;
    printf("\n");
  }
}

int main() {
  int V = 5;

  // Create a graph
  vector<int> adj[V];

  // Add edges
  addEdge(adj, 0, 1);
  addEdge(adj, 0, 2);
  addEdge(adj, 0, 3);
  addEdge(adj, 1, 2);
  printGraph(adj, V);
}
