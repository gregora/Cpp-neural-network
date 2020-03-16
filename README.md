# Cpp-neural-network
A neural network class, written in C++

## How to use this network
Create your own programm and include "network.h" header file with "network.cpp" file. To create an instance of class run `Network network(x, y, z, n)`. X here represents the number of input nodes, y number of hidden layers, z number of nodes in each hidden layer and n number of output nodes.

## Functions
* `random()` returns a double (a random number between 0 and 1)
* `sigmoid(double in)` is an approximation to sigmoid function. Accepts double argument and squishes it to a number between -1 and 1
* `set_input_nodes(double nodes[])` accepts an array of values, that input nodes should be set to. This function sets all the other nodes to 0
* `reset_nodes()` sets all the nodes to 0
* `reset_edges()` sets all the edges to a random number from 0 to 1
* `mutate(int amount)` changes a random edge to a random number from 0 to [chosen] amount
* `cost(double expected_output[])` accepts and array of what output nodes should be like and compares it to the actual values and returns the cost
* `forward_propagate()` propagates forward and changes all the values of input_nodes
* `output_all_nodes()` outputs all node values (manly used for debugging)
* `save(std::string file_path)` saves the network to a file
* `load(std::string file_path)` loads a network from a file (note: this only changes edges and not nodes)
