#ifndef __INTERFERENCE_GRAPH__HPP
#define __INTERFERENCE_GRAPH__HPP

#include <exception>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>
using namespace std;

class UnknownVertexException : public std::runtime_error {
public:
  UnknownVertexException(const std::string &v)
      : std::runtime_error("Unknown vertex " + v) {}
};

class UnknownEdgeException : public std::runtime_error {
public:
  UnknownEdgeException(const std::string &v, const std::string &w)
      : std::runtime_error("Unknown edge " + v + " - " + w) {}
};

// InterferenceGraph
//
// This is a class representing an interference graph
// as described in "Part 1: Interference Graph" of the README.md
// file. Though this class is templated, because of the usage of exceptions
// UnknownVertexException and UnknownEdgeException it will
// ONLY be tested with strings.
template <typename T> class InterferenceGraph {
public:
  // Custom type used to represent edges. This is mainly
  // used in the utility function for reading and writing
  // the graph structure to/from files. You don't need to use it.
  using EdgeTy = std::pair<T, T>;

  InterferenceGraph();

  ~InterferenceGraph();

  void addEdge(const T &v, const T &w);

  void addVertex(const T &vertex);

  void removeEdge(const T &v, const T &w);

  void removeVertex(const T &vertex);

  std::unordered_set<T> vertices() const noexcept;

  std::unordered_set<T> neighbors(const T &vertex) const;

  unsigned numVertices() const noexcept;

  unsigned numEdges() const noexcept;

  bool interferes(const T &v, const T &w) const;

  unsigned degree(const T &v) const;

private:
  unordered_map< T, unordered_set<T> > aj;
  unsigned num_edges;
};

template <typename T> InterferenceGraph<T>::InterferenceGraph() 
  : num_edges (0)
{}

template <typename T> InterferenceGraph<T>::~InterferenceGraph() {
  num_edges = 0;
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T &vertex) const {
  if (aj.find(vertex) == aj.end()) {
    throw UnknownVertexException(vertex);
  } else {
    return aj.at(vertex);
  }
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {
  unordered_set<T> all_vertices;
  for (auto e : aj) {
    all_vertices.insert(e.first);
  }
  return all_vertices;
}

template <typename T>
unsigned InterferenceGraph<T>::numVertices() const noexcept {
  return aj.size();
}

template <typename T> unsigned InterferenceGraph<T>::numEdges() const noexcept {
  return num_edges;
}

template <typename T>
void InterferenceGraph<T>::addEdge(const T &v, const T &w) {
  if (aj.find(v) == aj.end() || aj.find(w) == aj.end()) {
    throw UnknownVertexException(v);
  }
  aj[v].insert(w);
  aj[w].insert(v);
  ++ num_edges;
}

template <typename T>
void InterferenceGraph<T>::removeEdge(const T &v, const T &w) {
  if (aj.find(v) == aj.end() || aj.find(w) == aj.end()) {
    throw UnknownVertexException(v);
  }
  if (aj[v].find(w) == aj[v].end() || aj[w].find(v) == aj[w].end()) {
    throw UnknownEdgeException(v, w);
  }
  aj[v].erase(w);
  aj[w].erase(v);
  -- num_edges;
}

template <typename T>
void InterferenceGraph<T>::addVertex(const T &vertex) {
  if (aj.find(vertex) == aj.end()) {
    aj[vertex] = unordered_set<T>();
  }
}

template <typename T>
void InterferenceGraph<T>::removeVertex(const T &vertex) {
  if (aj.find(vertex) == aj.end()) {
    throw UnknownVertexException(vertex);
  }
  num_edges -= aj[vertex].size();
  for (auto e : aj[vertex]) {
    aj[e].erase(vertex);
  }
  aj.erase(vertex);
}

template <typename T>
bool InterferenceGraph<T>::interferes(const T &v, const T &w) const {
  if (aj.find(v) == aj.end() || aj.find(w) == aj.end()) {
    throw UnknownVertexException(v);
  }
  for (auto e : aj) {
    if (e.first == v) {
      for (auto c : e.second) {
        if (c == w) {
          return true;
        }
      }
      return false;
    }
    if (e.first == w) {
      for (auto c : e.second) {
        if (c == v) {
          return true;
        }
      }
      return false;
    }
  }
  return false;
}

template <typename T> unsigned InterferenceGraph<T>::degree(const T &v) const {
  if (aj.find(v) == aj.end()) {
    throw UnknownVertexException("Vertex not found");
  }
  return aj.at(v).size();
}

#endif
