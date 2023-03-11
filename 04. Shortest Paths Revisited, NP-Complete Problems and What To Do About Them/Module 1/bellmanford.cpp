/*
 * File Name: Bellman Ford Algorithm
 *
 * Credit : Course     : Shortest Paths Revisited, NP-Complete Problems and What To Do About Them
 *          Instructor : Tim Roughgarden, Professor
            Website    : https://www.geeksforgeeks.org/bellman-ford-algorithm-dp-23/
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 7th Of MARCH, 2023
 *
 * Description: The Bellman–Ford algorithm is an algorithm that computes shortest paths from a single source vertex to all of
                the other vertices in a weighted digraph.
 */

#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int src, dst, wt;
};
int V, E; //V-> Number of vertices, E-> Number of edges

void bellmanFord(vector<Edge>& Edges) {
    vector<int> value(V, INT_MAX);

    value[0] = 0;

    bool updated;


    for (int i = 0; i < V-1; i++) {
        updated = false;

        for (int j = 0; j < E; j++) {
            int src = Edges[j].src;
            int dst = Edges[j].dst;
            int wt = Edges[j].wt;

            if (value[src]!=INT_MAX and value[src]+wt < value[dst]) {
                value[dst] = value[src]+wt;
                updated = true;
            }
        }

        if (!updated) break;
    }

    for (int j = 0; j < E and updated; j++) {
        int src = Edges[j].src;
        int dst = Edges[j].dst;
        int wt = Edges[j].wt;

        if (value[src]!=INT_MAX and value[src]+wt < value[dst]) {
            cout << "Graph has negative edge cycle" << endl;
            return;
        }
    }
    for (int i = 0; i < V; i++) {
        cout << "Distance of " << i << " from source = " << value[i] << endl;
    }
}

int main() {
    cout << "Enter Values for Vertices and Edge : " << endl;
    cin >> V >> E;
    vector<Edge> Edges(E);

    int src, dst, wt;
    for (int i = 0; i < E; i++) {
        cout << "Input src,dst and wt for E = " << i << " : ";
        cin >> src >> dst >> wt;
        Edges[i].src = src;
        Edges[i].dst = dst;
        Edges[i].wt = wt;
    }

    bellmanFord(Edges);
}
