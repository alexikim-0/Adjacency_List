#include "register_allocation.hpp"
#include "CSVReader.hpp"
#include "InterferenceGraph.hpp"
#include <queue>
#include <deque>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace RA;
using namespace std;

// assignRegisters

struct compare_degrees {
  static InterferenceGraph<Variable> * graph;
  
  bool operator()(const Variable& a, const Variable& b) {
    return (graph->degree(a) < graph->degree(b));
  }
};

InterferenceGraph<Variable>* compare_degrees::graph = nullptr;

RegisterAssignment RA::assignRegisters(const std::string &path_to_graph, int num_registers) noexcept {
  InterferenceGraph<Variable> ig = CSVReader::load(path_to_graph);
  queue<Variable> uncolored_nodes;
  RegisterAssignment final_RA;
  
  std::unordered_set<Variable> vertices_set = ig.vertices();
  deque<Variable> vertices_deque{vertices_set.begin(), vertices_set.end()};
  compare_degrees::graph = &ig;
  make_heap(vertices_deque.begin(), vertices_deque.end(), compare_degrees{});
  
  // queue is now greatest to lowest
  for (auto e : vertices_deque) {
    uncolored_nodes.push(e);
  }

  if (unsigned(num_registers) < (ig.degree(uncolored_nodes.front()) + 1)) {
    return {};
  }

  while (final_RA.size() != ig.numVertices()) {
    Variable most_popular = uncolored_nodes.front();

    unordered_set<Variable> these_neighbors = ig.neighbors(most_popular);
    unordered_set<Register> connected_colors{};
    for (auto e : these_neighbors) {
      if (final_RA.find(e) != final_RA.end()) {
        connected_colors.insert(final_RA[e]);
      }
    }
    for (int i = 1; i <= num_registers; ++i) {
      if (connected_colors.find(i) != connected_colors.end()) {
        continue;
      } else {
        final_RA[most_popular] = i;
        break;
      }
    }
    uncolored_nodes.pop();
  }
  return final_RA;
}