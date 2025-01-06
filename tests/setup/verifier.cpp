/**
   verifier.cpp

   You do NOT need to modify this file as part of your project. This is simply
   a utility for verifying that your register allocation is correct for a given
   graph.

*/

#include "verifier.hpp"
#include "CSVReader.hpp"
#include "InterferenceGraph.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace RA;

bool verifyAllocation(const std::string &path_to_graph, int num_registers,
                      const RegisterAssignment &mapping) {

  std::string line;
  std::ifstream file_stream(path_to_graph);
  std::unordered_map<Variable, unsigned> degrees;
  std::unordered_set<Variable> variables;
  std::vector<std::pair<Variable, Variable>> interferences;

  while (std::getline(file_stream, line)) {
    const auto &row = CSVReader::readRow(line);
    for (const auto &v : row) {
      variables.insert(v);
      if (degrees.find(v) == degrees.end()) {
        degrees[v] = 0;
      }
    }
    if (row.size() == 2) {
      degrees[row[0]]++;
      degrees[row[1]]++;
      interferences.push_back(std::make_pair(row[0], row[1]));
    }
  }

  for (const auto &v : variables) {
    if (mapping.find(v) == mapping.end()) {
      throw std::runtime_error(
          std::string("Variable ") + v +
          std::string(" did not get mapped to a register!"));
    }

    if (mapping.at(v) < 1 || mapping.at(v) > num_registers)
      throw std::runtime_error(std::string("Variable ") +
       v +
       std::string(" mapped to register ") + 
       std::to_string(mapping.at(v)) +
       std::string(" which is out of range [") + 
       std::to_string(1) + 
       std::string(",") + 
       std::to_string(num_registers) +
       std::string("]"));
  }

  for (const auto &interference : interferences) {
    if (mapping.at(interference.first) == mapping.at(interference.second))
      throw std::runtime_error(
             std::string("Variables ") + 
             interference.first +
             std::string(" and ") +
             interference.second + 
             std::string(" were mapped to the same register: ") +
             std::to_string(mapping.at(interference.first)));
  }

  if (variables.empty()) {
    return true;
  }

  const auto highest_degree =
      std::max_element(
          std::begin(degrees), std::end(degrees),
          [](const auto &p1, const auto &p2) { return p1.second < p2.second; })
          ->second;

  std::unordered_set<Register> unique_registers;
  for (const auto &e : mapping)
    unique_registers.insert(e.second);

  const auto MAX_ALLOWED_REGS = highest_degree + 1;
  const auto NUM_USED_REGS = unique_registers.size();
  if (NUM_USED_REGS > MAX_ALLOWED_REGS) {
    throw std::runtime_error("Too many registers were used!");
  }

  return true;
}