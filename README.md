Interference Graph

The key idea from the paper is that of an interference graph. An interference graph is an undirected graph where the nodes are variables and edges between them represent overlapping lifetimes. A variable lifetime is the duration for which the value stored in a variable must live.


Each line in the interference graph represents either an edge or vertex in the graph. Thus, each row will have either one or two "columns". If it is a single column row, that represents a lone vertex. If it is a two column row that represents an edge. There is a pre-written function load() in CSVReader which will parse the CSV file and add the nodes to your graph structure. The addVertex and addEdge functions which it calls will be added by you.

The difficulty of this part will depend on the data structures you use to store the nodes and edges of the graph. I recommend thinking deeply about the data structures studied in the course, and having a plan before starting. This part should be relatively easy compared to other data structures implemented in this course.

Runtime Expectations

The implementation should be able to load a Complete Graph with 1000 vertices in under 10 seconds

A test case which explores this would look like the following:

TEST_CASE(“RuntimeTest“, “[ig-complete_1000]”) {
  const auto &GRAPH = "tests/graphs/complete_1000.csv";
  InterferenceGraph<Variable> ig = CSVReader::load(GRAPH);
}

Register Allocation

In this part you will be solving the register allocation problem using your new InterferenceGraph structure. You will be given the pseudocode needed to solve the graph coloring problem, but you are responsible for mapping it to the register allocation problem illustrated above in the introduction. Mapping solutions from one problem to another is a very useful skill that will serve you well in your programming career. This is sometimes called a "reduction". For example, you'll often hear the register allocation "reduces" to graph coloring.

src/register_allocation.cpp

RegisterAssignment assignRegisters(const std::string &path_to_graph, int num_registers) noexcept


Input

path_to_graph: 

A string path to the graph we will be allocating registers for. The first thing your implementation should do is load this string into an InterferenceGraph using CSVReader::load(). This has already been done for you. Note that there are some restrictions on the input graph:

 There will not be any self-edges. In other words, no entry in the CSV like `x,x`.
 There may be lone vertices in the graph.
 The graph will not be empty.
 There will be equal to or fewer than 250 vertices.
 There will not be duplicate edges. In other words, there can only be a maximum of 1 edge between any two vertices.

num_registers: 

The maximum number of registers you have _available_ to use. Think of this like the number of registers on your processor. Thus, your register allocation algorithm should use some number of registers in the range [1, num_registers] inclusive. In all private test cases this value will be equal to or less than 500. Note, there are further restrictions given on the number of registers a _correct_ solution will give in the Requirements section based on how the input graph looks.

Output

RegisterAssignment: 

An unordered_map<string, int> type which is the mapping of each variable in the graph to a register in the range [1, num_registers] that solves the register allocation problem mentioned above. 

You can only use d(G) + 1 unique registers where d(G) is defined as the largest degree in the graph G (here G is the interference graph provided via the path_to_graph parameter). Your assignment should meet the requirements mentioned in the "Register Allocation Requirements" section.

As stated previously, you will be basing your implementation off of a graph coloring algorithm. The algorithm you will be adjusting is the Welsh-Powell Algorithm from 1967 (!!) [3]. Isn't it cool that an algorithm developed for something completely different can be used up to 20 years later in a new domain? 
Implementation

Welsh-Powell Graph Coloring

All vertices should be sorted in descending order according to their degree. (Warning: std::sort not allowed per Implementation Restrictions section below).
Colors should be sorted in a list C.
The first non-colored vertex v in V is colored with the first available color in C. "Available" means a color that was not previously used by the algorithm.
The remaining part of the ordered list V is traversed and the same color is allocated to every vertex for which no adjacent vertex has the same color.
Steps 3 and 4 are applied iteratively until all the vertices have been colored.

Each variable must be mapped to a register in the range [1, num_registers] inclusive.
If a variable v is a neighbor of variable w in the interference graph they cannot share the same register.
You may only use at most d(G) + 1 registers, even if you are given more via num_registers. Here, d(G) is defined as the highest degree in the graph G (the interference graph provided). Note, the provided algorithm accomplishes this.
Each register allocation test must run in under 20 seconds. The worst case scenario can be derived from the input restrictions. The developed (suboptimal) solution was able to perform the worst case register allocation in 1 second on a machine with a AMD Ryzen 9 3900X 12-Core processor and 16 GB RAM. The released test cases clearly do not include the worst case scenario. You will have to create this test yourself using the input restrictions listed in the *Input* section above. You are explicitly allowed to ADD interfaces to the InterferenceGraph class if you need them to make your register allocation implementation faster. However, do not *remove* any interfaces (public functions, etc.) because we will use those for the InterferenceGraph tests. The same rules about posting to EdStem from the Interference Graph section apply here.
If the interference graph cannot be allocated, you should return an empty map. Note, this is equivalent to checking if the number of provided registers is less than d(G) + 1. You do not need to worry about the optimal number of registers. For example, if the interference graph has two vertices with an edge between them, but there is only one register available.

TEST(“Simple”, “[ra-public]”) {
  const auto &GRAPH = "tests/graphs/simple.csv";
  const auto NUM_REGS = 3;

  const auto &allocation = assignRegisters(GRAPH, NUM_REGS);
  IGWriter::write(CSVReader::load(GRAPH), "tests/graphs/simple_success.dot",
              	allocation);
  REQUIRE((verifyAllocation(GRAPH, NUM_REGS, allocation)));
}



This tests calls assignRegisters (which you will implement) on the graph "tests/graphs/simple.csv", which looks like the following:

x,y
z,x
z,y


The colors assigned to fillcolor are derived from the register allocation that are passed in through the allocation parameter. You are free to look at the implementation of IGWriter::write and play around with different colors if you want. As of now, it only supports 8 different colors (register 1 through 8).

You can visualize your dot file using the following tool: https://dreampuf.github.io/GraphvizOnline/ 
