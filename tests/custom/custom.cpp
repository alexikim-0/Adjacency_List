#include "InterferenceGraph.hpp"
#include "CSVReader.hpp"
#include "IGWriter.hpp"
#include <string>
#include "register_allocation.hpp"
#include "catch_amalgamated.hpp"
#include "verifier.hpp"

namespace {
    TEST_CASE("addVertexaddEdge", "[ig-custom]") {
        InterferenceGraph<std::string> ig;
        ig.addVertex("a");
        ig.addVertex("b");
        ig.addEdge("a", "b");

        REQUIRE(ig.numEdges() == 1);
        REQUIRE(ig.numVertices() == 2);
    }

    TEST_CASE("removeEdge", "[ig-custom]") {
        InterferenceGraph<std::string> ig;
        ig.addVertex("a");
        ig.addVertex("b");
        ig.addVertex("c");

        ig.addEdge("a", "b");
        ig.addEdge("b", "c");
        ig.addEdge("a", "c");

        ig.removeEdge("b", "a");

        REQUIRE(ig.numEdges() == 2);
        REQUIRE(ig.numVertices() == 3);
    }

    TEST_CASE("removeVertex", "[ig-custom]") {
        InterferenceGraph<std::string> ig;
        ig.addVertex("a");
        ig.addVertex("b");
        ig.addVertex("c");

        ig.addEdge("a", "b");
        ig.addEdge("b", "c");
        ig.addEdge("a", "c");

        ig.removeVertex("b");

        REQUIRE(ig.numEdges() == 1);
        REQUIRE(ig.numVertices() == 2);
    }

    TEST_CASE("Vertices", "[ig-custom]") {
        InterferenceGraph<std::string> ig;
        ig.addVertex("a");
        ig.addVertex("b");
        ig.addVertex("c");

        ig.addEdge("a", "b");
        ig.addEdge("b", "c");
        ig.addEdge("a", "c");

        const std::unordered_set<Variable> &expected_neighbors = {"a", "c"};

        REQUIRE(ig.numEdges() == 3);
        REQUIRE(ig.numVertices() == 3);
        REQUIRE(ig.neighbors("b") == expected_neighbors);
    }

    TEST_CASE("interferes", "[ig-custom]") {
        InterferenceGraph<std::string> ig;
        ig.addVertex("a");
        ig.addVertex("b");
        ig.addVertex("c");
        ig.addVertex("d");

        ig.addEdge("a", "b");
        ig.addEdge("b", "c");
        ig.addEdge("a", "c");

        REQUIRE(ig.interferes("a", "b") == true);
        REQUIRE(ig.interferes("a", "d") == false);
    }

    TEST_CASE("degree", "[ig-custom]") {
        InterferenceGraph<std::string> ig;
        ig.addVertex("a");
        ig.addVertex("b");
        ig.addVertex("c");
        ig.addVertex("d");

        ig.addEdge("a", "b");
        ig.addEdge("b", "c");
        ig.addEdge("a", "c");
        ig.addEdge("a", "d");

        REQUIRE(ig.degree("a") == 3);
        REQUIRE(ig.degree("d") == 1);
    }

    TEST_CASE("CustomTest", "[ra-custom]") {
    const auto &GRAPH = "tests/graphs/complete_6.csv";
    const auto NUM_REGS = 7;

    const auto &allocation = assignRegisters(GRAPH, NUM_REGS);

    REQUIRE((verifyAllocation(GRAPH, NUM_REGS, allocation)));
    }
}