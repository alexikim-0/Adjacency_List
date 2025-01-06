#include "CSVReader.hpp"
#include "IGWriter.hpp"
#include "InterferenceGraph.hpp"
#include "catch_amalgamated.hpp"
#include "register_allocation.hpp"
#include "verifier.hpp"

namespace {
TEST_CASE("SimpleSucess", "[ra-public]") {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "tests/graphs/simple.csv";
  const auto NUM_REGS = 3;

  // Provide 3 registers to the register allocation algorithm.
  // Note, the highest degree in this graph is two, so all three
  // will be needed.
  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  IGWriter::write(CSVReader::load(GRAPH), "tests/graphs/simple_success.dot",
                  allocation);

  REQUIRE((verifyAllocation(GRAPH, NUM_REGS, allocation)));
}

TEST_CASE("SimpleFail", "[ra-public]") {
  // Load the graph pointed to by simple.csv
  const auto &GRAPH = "tests/graphs/simple.csv";
  const auto NUM_REGS = 2;

  // Provide 2 registers to the register allocation algorithm.
  // Note, the highest degree in this graph is two, so the allocation
  // cannot be completed.
  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  REQUIRE((allocation.empty()));
}
} // namespace