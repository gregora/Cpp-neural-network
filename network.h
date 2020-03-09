#include <iostream>
#include <vector>
#include <math.h>

class Network {

  public:

    //constructor
    Network(int ils, int hln, int hls, int ols);

    //all nodes
    std::vector<double> input_nodes;
    std::vector<std::vector<double>> hidden_nodes;
    std::vector<double> output_nodes;

    //all edges (connections between nodes)
    std::vector<std::vector<double>> input_edges;
    std::vector<std::vector<std::vector<double>>> hidden_edges;
    std::vector<std::vector<double>> output_edges;

    //public input layer size, public hidden layers number, public hidden layer size, public output layer size
    int pils, phln, phls, pols;


    double random();
    double sigmoid(double in);
    void set_input_nodes(double nodes[]);
    void reset_nodes();
    void reset_edges();
    void mutate(int amount);
    double cost(double expected_output[]);
    void forward_propagate();
    void output_all_nodes();
};