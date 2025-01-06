#include "CSVReader.hpp"
#include "IGWriter.hpp"
#include "InterferenceGraph.hpp"
#include "catch_amalgamated.hpp"

namespace {
    TEST_CASE("SimpleAddEdgesAndVertices", "[ig-public]") {
    const auto &GRAPH = "tests/graphs/simple.csv";

    const InterferenceGraph<Variable> &ig = CSVReader::load(GRAPH);

    REQUIRE(ig.numEdges() == 3);
    REQUIRE(ig.numVertices() == 3);
    }

    TEST_CASE("Neighbors", "[ig-public]") {
    const auto &GRAPH = "tests/graphs/simple.csv";

    const InterferenceGraph<Variable> &ig = CSVReader::load(GRAPH);
    const std::unordered_set<Variable> &expected_neighbors = {"y", "z"};

    REQUIRE(ig.neighbors("x") == expected_neighbors);
    }
}