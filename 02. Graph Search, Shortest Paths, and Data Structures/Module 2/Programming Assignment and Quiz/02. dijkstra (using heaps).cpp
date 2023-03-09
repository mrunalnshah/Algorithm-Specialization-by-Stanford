/*
 * File Name: Programming Assignment #2
 *
 * Credit : Course     : Graph Search, Shortest Paths, and Data Structures, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 20th Of FEBURARY, 2023
 *
 * Description:  run Dijkstra's shortest-path algorithm on this graph, using 1 (the first vertex) as the source vertex, and to compute the shortest-path distances between 1 and every other vertex of the graph. If there is no path between a vertex vv and vertex 1, we'll define the shortest-path distance between 1 and vv to be 1000000.
 *
 * Dependencies: This Code requires a folder named "./02. testcases" and you can add all the test cases there.
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <regex>

#include <cassert>
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

template <typename Key, typename Value, typename ComparisonOperator>
class Heap
{
   typedef pair<Key, Value> KeyValuePair_t;
   vector<KeyValuePair_t> elements;
   static ComparisonOperator compare;

   template <typename K, typename V, typename C> friend ostream& operator<<(ostream& os, Heap<K, V, C> const& heap);

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

      return compare(elements[i].first, elements[left_index].first) && (right_index == -1 || compare(elements[i].first, elements[right_index].first));
   }

   void BubbleUp(int from_index)
   {
      // Bubble-up the element until it finds its proper place
      for (int i = from_index; i > 0; /**/)
      {
         int parent_index = GetParentIndex(i);

         if (SatisfiesHeapProperty(parent_index))
            break;

         // Swap with the parent and keep going
         swap(elements[i], elements[parent_index]);
         i = parent_index;
      }
   }

   void BubbleDown(int from_index)
   {
      int n = elements.size();

      // Bubble-down the element until it finds its proper place
      for (int i = from_index; i < n && !SatisfiesHeapProperty(i); /**/)
      {
         int left_index = GetLeftChildIndex(i);
         int right_index = GetRightChildIndex(i);

         // Swap with the smallest of the two children and try again
         int swap_index = right_index == -1 ? left_index : (compare(elements[left_index].first, elements[right_index].first) ? left_index : right_index);
         swap(elements[i], elements[swap_index]);
         i = swap_index;
      }
   }

public:
   typedef KeyValuePair_t KeyValuePair;

   size_t size() const
   {
      return elements.size();
   }

   bool empty() const
   {
      return size() == 0;
   }

   void Insert(Key const& key, Value const& value)
   {
      // Append the new element to the end of the array, meaning that it is on the last level of the heap tree
      elements.push_back(make_pair(key, value));

      // Bubble-up the new element until it finds its proper place
      BubbleUp(elements.size()-1);
   }

   KeyValuePair const& Peek() const
   {
      assert(size() > 0);
      return elements[0];
   }

   KeyValuePair Extract()
   {
      int n = elements.size();
      assert(n > 0);

      // Swap root (min element) with last element in the last level of the heap tree
      swap(elements[0], elements[n-1]);

      // Delete the min (now the last element) from the tree, keep it aside to return
      KeyValuePair minimum = move(elements[n-1]);
      elements.pop_back();
      --n;

      // Bubble-down the new illegitimate root until it finds its proper place
      BubbleDown(0);

      // Just return the min now
      return move(minimum);
   }

   /// Remove the first entry in the heap whose value matches the given input
   KeyValuePair Remove(Value const& value)
   {
      int n = elements.size();
      assert(n > 0);

      int found = -1;
      for (int i = 0; i < n; ++i)
      {
         if (elements[i].second == value)
         {
            found = i;
            break;
         }
      }
      assert(found > -1);

      // Case 1: Root, this is exactly the same as minimum extraction
      if (found == 0)
         return Extract();

      // Case 2: Anywhere else, handle every similarly to the Root case except that we might either have to bubble up or bubble down

      // Swap element to be deleted with last element in the last level of the heap tree
      swap(elements[found], elements[n-1]);

      // Delete the element (now the last element) from the tree, keep it aside to return
      KeyValuePair removed = move(elements[n-1]);
      elements.pop_back();

      // Case 2: Absolute last element, simply return what was removed
      if (found == n-1)
         return removed;

      // Else, proceed with the heap rebalancing. Bubble up or down until the swapped element has found its correct place
      // Tip: just run both, whichever one needs to be done will be done
      BubbleUp(found);
      BubbleDown(found);

      return move(removed);
   }
};

template<typename K, typename V, typename C>
C Heap<K, V, C>::compare;

template<typename K, typename V, typename C>
ostream& operator<<(ostream& os, Heap<K, V, C> const& heap)
{
   os << "[";
   for (int i = 0; i < heap.elements.size(); ++i)
   {
      os << heap.elements[i];
      if (i != heap.elements.size() - 1)
         os << ", ";
   }
   os << "]";

   return os;
}

template<typename Key, typename Value>
using MinHeap = Heap<Key, Value, std::less<Key>>;

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
   unordered_map<int, vector<Edge>> in_edges_map; // tells you what edges are incident to a given vertex (the key)

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

      auto it2 = in_edges_map.find(edge.to);
      if (it2 == in_edges_map.end())
      {
         in_edges_map[edge.to] = {edge};
      }
      else
      {
         it2->second.push_back(edge);
      }
   }
   cout << "Total Edges = " << M << endl;
   cout << "Total Vertices = " << vertices.size() << endl;
   cout << "Total iterations = " << M * vertices.size() << endl;

   // Initialize X with the source vertex
   unordered_set<int> processed;
   processed.insert(source);

   // Initialize distances
   vector<int> distances(vertices.size(), numeric_limits<int>::max());
   distances[source - 1] = 0;

   // Initialize ye ol' heap
   MinHeap<int, int> heap;
   for (auto v : vertices)
   {
      if (v == source)
         continue;

      auto it = in_edges_map.find(v);
      if (it == in_edges_map.end())
         continue;

      int smallest_key = numeric_limits<int>::max();
      for (auto const& edge : it->second)
      {
         // At this init phase, the only edges whose "from" vertex has a valid distance are edges which start from the source vertex, meaning that these are by definition edges crossing the frontier
         if (distances[edge.from - 1] == numeric_limits<int>::max())
            continue;

         int key = distances[edge.from - 1] + edge.weight;
         if (key < smallest_key)
            smallest_key = key;
      }

      heap.Insert(smallest_key, v);
   }

   // Let's go!
   while (!heap.empty())
   {
      // First, extract the minimum vertex v
      auto minimum = heap.Extract();
      int distance = minimum.first;
      auto v = minimum.second;

      // Update distances
      distances[v - 1] = distance;

      // Mark the selected one as processed
      processed.insert(v);

      // Lastly, update the keys in the heap of the vertices reachable from v
      auto it = out_edges_map.find(v);
      if (it != out_edges_map.end())
      {
         for (auto const& edge : it->second)
         {
            // Ignore vertices that are in X
            if (processed.find(edge.to) != processed.end())
               continue;

            // Extract the element in the heap corresponding to the target vertex
            auto element = heap.Remove(edge.to);

            // Compute the new key
            int key_candidate = distances[v - 1] + edge.weight;
            int key_new = key_candidate < element.first ? key_candidate : element.first;

            // Re-insert into the heap
            heap.Insert(key_new, element.second);
         }
      }
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
