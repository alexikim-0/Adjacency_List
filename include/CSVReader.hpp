/**
   CSVReader.hpp
*/

#ifndef CSV_READER_H
#define CSV_READER_H

#include "InterferenceGraph.hpp"
#include "register_allocation.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace RA;

class CSVReader {
public:

  static std::vector<std::string> readRow(std::string &csv_line);

  // This function iterates through the file at `graph_path`
  // line by line, adding each edge to this InterferenceGraph.
  // See the README for an example.
  static InterferenceGraph<Variable> load(const std::string &graph_path);
};

#endif