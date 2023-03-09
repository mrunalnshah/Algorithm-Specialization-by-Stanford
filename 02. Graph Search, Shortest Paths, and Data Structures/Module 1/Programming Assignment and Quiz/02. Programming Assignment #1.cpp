/*
 * File Name: Programming Assignment #1
 *
 * Credit : Course     : Graph Search, Shortest Paths, and Data Structures, Stanford University on Coursera.
 *          Instructor : Tim Roughgarden, Professor
 *
 * Author: Mrunal Nirajkumar Shah
 * Date: 20th Of FEBURARY, 2023
 *
 * Description:  code up the algorithm from the video lectures for computing strongly connected components (SCCs), and to run this algorithm on the given graph.
 *
 * Dependencies: This Code requires a folder named "./02. testcases" and you can add all the test cases there. Add SCC.txt in the folder or just change the name of folder to file. and add SCC.txt in the folder containing code.
 *
 *      IT IS REALLY HARD DUE TO MEMORY MISMANAGEMENT KINDLY LOOK THE WAY ITS DONE, IT MAYBE HELPFUL.
 */

#include <iostream>
#include <fstream>
#include <dirent.h> // It is for UNIX only but for Windows : use msys2 compiler and code::blocks(i used for windows)
#include <string>

#include <cassert>
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
#pragma endregion

class DirectedGraph
{
   class Node
   {
      int m_key; // can only be set upon construction

   public:
      int key() const { return m_key; }

      int value;
      unordered_set<Node*> out_neighbors;
      unordered_set<Node*> in_neighbors; // helpful for reverse edge traversal

      Node(int const _key, int const _value) : m_key(_key), value(_value)
      { /* nothing */ }
   };

   unordered_map<int, unique_ptr<Node>> nodes;

   // Creates a new node in the graph with a guaranteed unique id/key (uniqueness is only local to the graph)
   Node* MakeNode(int const value, int const key)
   {
      auto p = new Node(key, value);
      nodes.insert(make_pair(key, unique_ptr<Node>(p)));
      return p;
   }

   void AddEdge(Node* from, Node* to)
   {
      if (from == nullptr || to == nullptr)
         return;

      // we go OUT from `from` to `to`
      from->out_neighbors.insert(to);

      // we come IN to `to` from `from`
      to->in_neighbors.insert(from);
   }

   void DFS(bool is_first_pass, Node* node, vector<bool>& visited, vector<Node*> *finishing_order, vector<int> *scc)
   {
      if (node == nullptr)
         return;

      // Init
      vector<bool> finished(N(), false); // tracks whether the node has completed recording its finishing order (used only during first pass)
      vector<Node*> stack;
      stack.push_back(node);

      // Begin
      while (!stack.empty())
      {
         // Peek and process
         Node* node = stack[stack.size() - 1];
         int key = node->key();

         // Check neighbors if unexplored
         if (!visited[key])
         {
            // Mark as explored
            visited[key] = true;

            // Associate with SCC, only on second pass
            if (!is_first_pass && scc != nullptr)
            {
               scc->push_back(node->key());
            }

            // First pass DFS is done on the graph with reverse edges
            // This is equivalent to exploring the node's incoming edges
            // Second pass DFS is performed normally i.e. following outgoing edges of the node
            unordered_set<Node*> *neighbors = is_first_pass ? &(node->in_neighbors) : &(node->out_neighbors);

            // Visit neighors
            for (Node* neighbor : *neighbors)
            {
               if (visited[neighbor->key()]) // already seen
                  continue;

               stack.push_back(neighbor);
            }
         }
         // Otherwise it means that all neighbors have been processed, so we can finish processing this one
         else
         {
            // "Record" finishing time, only on first pass
            // Note that due to the nature of the DFS stack-based implementation, the already visited node can
            // appear more than once on the stack. But we only want to push into `finishing_order` exactly once
            // in order for the algorithm to be correct. We use the `finished` cache to deduplicate this.
            if (is_first_pass && finishing_order != nullptr && !finished[key])
            {
               finished[key] = true;
               finishing_order->push_back(node);
            }

            stack.pop_back();
         }
      }
   }

public:
   DirectedGraph(vector<pair<int, int>> const& edge_data)
   {
      // We interpret the data such that each value of the node of the edge is the same is its key
      // For ex: the value 1 refers to the node whose key is 1 and whose value is 1
      for (auto const& datum : edge_data)
      {
         int first = datum.first;
         int second = datum.second;

         Node *from(nullptr), *to(nullptr);

         auto it_from = nodes.find(first);
         if (it_from == nodes.end())
         {
            from = MakeNode(first, first);
         }
         else
         {
            from = it_from->second.get();
         }

         auto it_to = nodes.find(second);
         if (it_to == nodes.end())
         {
            to = MakeNode(second, second);
         }
         else
         {
            to = it_to->second.get();
         }

         AddEdge(from, to);
      }
   }

   int N() const { return nodes.size(); }

   // Returns list of SCCs, first element of each SCC is the leader node
   vector<vector<int>> GetStronglyConnectedComponents()
   {
      // 1. Perform first-pass DFS on the graph with REVERSED edges, computing the "finishing times" label for each node.
      vector<Node*> finishing_order;
      {
         vector<bool> visited(N(), false);
         for (auto & node : nodes)
         {
            if (!visited[node.second.get()->key()]) // not yet explored
            {
               DFS(true, node.second.get(), visited, &finishing_order, nullptr);
            }
         }
      }

      // 2. Perform second-pass DFS on the graph with original edges, this time grouping nodes into their corresponding SCC.
      vector<vector<int>> sccs;
      {
         // set<Node*> visited;
         vector<bool> visited(N(), false);
         for (int i = finishing_order.size(); i > 0; --i)
         {
            vector<int> scc;
            Node* node = finishing_order[i - 1];
            if (!visited[node->key()]) // not yet explored
            {
               DFS(false, node, visited, nullptr, &scc);
            }
            if (!scc.empty())
               sccs.push_back(scc);
         }
      }

      return move(sccs);
   }
};

array<int, 5> GetTop5SCCsSizesDescending(vector<pair<int, int>> const& edge_data)
{
   DirectedGraph graph(edge_data);

   auto sccs = graph.GetStronglyConnectedComponents();

   vector<int> scc_sizes;
   transform(sccs.begin(), sccs.end(), back_inserter(scc_sizes), [&](vector<int> const& scc) { return scc.size(); });
   sort(scc_sizes.begin(), scc_sizes.end(), [](int const a, int const b) { return a > b; });

   // Sanity check: The sum of all sizes should be the total number of nodes in the graph;
   {
      int sum = 0;
      for (int n : scc_sizes)
         sum += n;
      assert(sum == graph.N());
   }

   array<int, 5> result = {0, 0, 0, 0, 0};
   for (int i = 0; i < scc_sizes.size() && i < 5; ++i)
      result[i] = scc_sizes[i];

   return move(result);
}

int main()
{
   // Find all test case files
   vector<string> test_case_files;
   DIR *dir;
   struct dirent *ent;
   string const TESTDIR = "./02. testcases";
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

   int const n_start = 1, n = test_case_files.size();
   // int const n_start = 1, n = 4; // TODO: REMOVE WHEN READY
   for (auto it = test_case_files.begin() + n_start - 1; it != test_case_files.begin() + n_start - 1 + n; ++it)
   {
      string const &filename = *it;
      string const filepath = TESTDIR + "/" + filename;
      cout << filename << ":" << endl;

      ifstream file(filepath);
      vector<pair<int, int>> edge_data;
      array<int, 2> line;
      int x;
      for (int i = 0; file >> x; i = (i + 1) % 2)
      {
         line[i] = x;

         if (i % 2 == 1)
            edge_data.push_back(make_pair(line[0], line[1]));
      }
      auto top5 = GetTop5SCCsSizesDescending(edge_data);
      cout << "Top 5 SCC sizes: " << top5 << endl;
      cout << endl;
   }

   return 0;
}
