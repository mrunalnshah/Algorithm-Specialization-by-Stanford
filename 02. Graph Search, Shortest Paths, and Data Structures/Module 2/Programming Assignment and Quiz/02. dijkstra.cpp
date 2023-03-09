/*
 * File Name: Programming Assignment #2 USING HEAPS
 *
 * Credit : Course     : Graph Search, Shortest Paths, and Data Structures, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 21th Of FEBURARY, 2023
 *
 * Description:  run Dijkstra's shortest-path algorithm using heaps on this graph, using 1 (the first vertex) as the source vertex, and to compute the shortest-path distances between 1 and every other vertex of the graph. If there is no path between a vertex vv and vertex 1, we'll define the shortest-path distance between 1 and vv to be 1000000.
 *
 * Dependencies: This Code requires a folder named "./02. testcases" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <regex>

#include <limits>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

#pragma region Utilities
template <typename T, typename U>
ostream &operator<<(ostream &os, pair<T, U> const &p)
{
   os << "(" << p.first << "," << p.second << ")";
   return os;
}

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
ostream &operator<<(ostream &os, unordered_set<T> const &v)
{
   os << "[";
   int n = v.size();
   for (auto it = v.begin(); it != v.end(); ++it, --n)
   {
      os << *it;
      if (n > 1)
         os << ", ";
   }
   os << "]";

   return os;
}

template <typename K, typename V>
ostream &operator<<(ostream &os, unordered_map<K, V> const &v)
{
   os << "[";
   int n = v.size();
   for (auto it = v.begin(); it != v.end(); ++it, --n)
   {
      os << *it;
      if (n > 1)
         os << ", ";
   }
   os << "]";

   return os;
}

template <typename T, size_t N>
ostream &operator<<(ostream &os, array<T, N> const &a)
{
   os << "[";
   for (int i = 0; i < N; ++i)
   {
      os << a[i];
      if (i != N - 1)
         os << ",";
   }
   os << "]";

   return os;
}

// Courtesy of https://stackoverflow.com/a/9437426
std::vector<std::string> split(const string& input, const string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}
#pragma endregion

typedef vector<pair<int, pair<int, int>>> EdgeData; // each entry is a weighted edge (u,v)

struct Edge
{
   int from;
   int to;
   int weight;
};

ostream& operator<< (ostream& os, Edge const& e)
{
   os << "(" << e.from << "," << e.to << "|" << e.weight << ")";
   return os;
}

vector<int> ComputeShortestPathDistancesFrom(int source, EdgeData const& edge_data)
{
   unordered_set<int> vertices;
   unordered_map<int, vector<Edge>> out_edges_map; // tells you what edges are out-going edges for a given vertex (the key)

   // Build the graph
   int M = 0;
   for (auto const& datum : edge_data)
   {
      int source = datum.first;
      vertices.insert(source);

      auto const& dest = datum.second;
      Edge edge = {source, dest.first, dest.second};
      M++;

      auto it = out_edges_map.find(source);
      if (it == out_edges_map.end())
      {
         out_edges_map[source] = {edge};
      }
      else
      {
         it->second.push_back(edge);
      }
   }
   cout << "Total Edges = " << M << endl;
   cout << "Total Vertices = " << vertices.size() << endl;
   cout << "Total iterations = " << M * vertices.size() << endl;

   // Initialize pending vertices, i.e. vertices that are NOT in X
   unordered_set<int> unprocessed = vertices;

   // Initialize X with the source vertex
   unordered_set<int> processed;
   processed.insert(source);
   unprocessed.erase(source);

   // Initialize distances
   vector<int> distances(vertices.size(), numeric_limits<int>::max());
   distances[source - 1] = 0;

   // Let's go!
   while (!unprocessed.empty())
   {
      // Find all edges crossing the frontier, i.e. going out from X
      vector<Edge> frontier;
      for (int from : processed)
      {
         auto it = out_edges_map.find(from);
         auto const& out_edges = it->second;
         copy_if(out_edges.begin(), out_edges.end(), back_inserter(frontier), [&](Edge const& e) {
            return processed.find(e.to) == processed.end(); // ignore already processed vertices
         });
      }

      // Find the edge with the smallest dijkstra cost
      pair<Edge, int> smallest = {frontier[0], numeric_limits<int>::max()}; // first -> edge, second -> smallest cost so far
      for (auto const& edge : frontier)
      {
         int cost = distances[edge.from - 1] + edge.weight;

         if (cost < smallest.second)
         {
            smallest.first = edge;
            smallest.second = cost;
         }
      }

      auto const& choice = smallest.first;

      // Update distances
      distances[choice.to - 1] = smallest.second;

      // Mark the selected one as processed
      processed.insert(choice.to);
      unprocessed.erase(choice.to);
   }

   return distances;
}

vector<int> OrderedDistances (vector<int> const& distances, vector<int> const& order)
{
   vector<int> ordered;
   for (int v : order)
      if (v > distances.size())
         continue;
      else
         ordered.push_back(distances[v - 1]);
   return ordered;
}

int main()
{
   // Find all test case files (courtesy of https://stackoverflow.com/a/612176)
   vector<filesystem::path> test_case_files;
   string const TESTDIR = "./testcases";
   for (const auto & entry : filesystem::directory_iterator(TESTDIR))
      test_case_files.push_back(entry.path());

   // Sort them
   sort(test_case_files.begin(), test_case_files.end());

   int const n_start = 1, n = test_case_files.size();
   // int const n_start = 1, n = 1; // TODO: REMOVE WHEN READY
   for (auto it = test_case_files.begin() + n_start - 1; it != test_case_files.begin() + n_start - 1 + n; ++it)
   {
      string const &filename = it->string();
      cout << filename << ":" << endl;

      ifstream file(*it);
      vector<string> lines;
      string line;
      while (getline(file, line))
         lines.push_back(line);

      EdgeData edge_data;
      for (auto const& line : lines)
      {
         auto tokens = split(line, "\\s+");

         int source;
         stringstream ss(tokens[0]);
         ss >> source;

         for (auto it = tokens.begin() + 1; it != tokens.end(); ++it)
         {
            auto ctokens = split(*it, ",");

            int dest, weight;
            {
               stringstream ss(ctokens[0]);
               ss >> dest;
            }
            {
               stringstream ss(ctokens[1]);
               ss >> weight;
            }

            // STRONG assumption: each line gives you edge data for the source vertex starting from 1 all the way to N in order where N is the total number of lines in the file
            edge_data.push_back(make_pair(source, make_pair(dest, weight)));
         }
      }

      auto distances = ComputeShortestPathDistancesFrom(1, edge_data);
      cout << "Distances = " << distances << endl;
      cout << "Ordered Distances = " << OrderedDistances(distances, {7,37,59,82,99,115,133,165,188,197}) << endl;
      cout << ";" << endl;
   }

   return 0;
}
