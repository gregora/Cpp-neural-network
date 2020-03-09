#include "network.h"

//seed for Network.random()
double randseed = 0;

//constructor
Network::Network(int ils, int hln, int hls, int ols){

  pils = ils;
  phln = hln;
  phls = hls;
  pols = ols;

  reset_nodes();
  reset_edges();

}

//"truly" random number between 0 and 1
double Network::random(){
  srand((unsigned) time(0) + randseed);
  randseed = (double) (rand() % 10000) * (double) (rand() % 10000);
  return (double) (rand() % 1000) / 1000;
}

//sigmoid function approximation
double Network::sigmoid(double in){
  return in / (1 + abs(in));
}

//set input nodes
void Network::set_input_nodes(double nodes[]){
  for(int x = 0; x < pils; x++){
    input_nodes[x] = nodes[x];
  }
}

//sets all nodes values to 0
void Network::reset_nodes(){

  input_nodes.clear();
  hidden_nodes.clear();
  output_nodes.clear();

  for(int x = 0; x < pils; x++){
    input_nodes.push_back(0);
  }
  for(int x = 0; x < phln; x++){

    hidden_nodes.push_back({});
    for(int y = 0; y < phls; y++){
      hidden_nodes[x].push_back(0);
    }
  }

  for(int x = 0; x < pols; x++){
    output_nodes.push_back(0);;
  }

}

///sets all edges to random
void Network::reset_edges(){

  input_edges.clear();
  hidden_edges.clear();
  output_edges.clear();

  //set input edges
  for(int x = 0; x < pils; x++){
    input_edges.push_back({});
    for(int y = 0; y < phls; y++){
      input_edges[x].push_back(random());
    }

  }

  //set hidden edges
  for(int x = 0; x < phln - 1; x++){
    hidden_edges.push_back({});
    for(int y = 0; y < phls; y++){
      hidden_edges[x].push_back({});
      for(int z = 0; z < phls; z++){
        hidden_edges[x][y].push_back(random());
      }
    }
  }

  //set output edges
  for(int x = 0; x < phls; x++){
    output_edges.push_back({});
    for(int y = 0; y < pols; y++){
      output_edges[x].push_back(random());
    }
  }


}

//randomly change edges (used in genetic algorithms)
void Network::mutate(int amount){

  //this part approximates what are the odds of mutation happening on any layer (input, hidden or output)
  double hidden_edges_num = 0;
  if(hidden_edges.size() > 0){
    hidden_edges_num = hidden_edges.size() * hidden_edges[0].size() * hidden_edges[0][0].size();
  }
  double input_edges_num = input_edges.size() * input_edges[0].size();
  double output_edges_num = output_edges.size() * output_edges[0].size();

  //number of all edges
  double all_edges_num = hidden_edges_num + input_edges_num + output_edges_num;

  double rn = random();

  for(int x = 0; x < amount; x++){
    //odds of mutation on any edge should be equal
    if(rn < input_edges_num / all_edges_num){

      //choose random edge
      input_edges[(input_edges.size() - 1) * random()][(input_edges[0].size() - 1) * random()] = random();

    }else if(rn < (input_edges_num + hidden_edges_num) / all_edges_num){
      //choose random edge
      hidden_edges[(hidden_edges.size() - 1) * random()][(hidden_edges[0].size() - 1) * random()][(hidden_edges[0][0].size() - 1) * random()] = random();

    }else{

      //choose random edge
      output_edges[(output_edges.size() - 1) * random()][(output_edges[0].size() - 1) * random()] = random();
    }
  }
}

//get cost after propagation, provided the expected output
double Network::cost(double expected_output[]){
  double c = 0;

  for(int x = 0; x < output_nodes.size(); x++){
    c += (output_nodes[x] - expected_output[x]) * (output_nodes[x] - expected_output[x]);
  }

  return c;

}

//propagate forward
void Network::forward_propagate(){

  //input nodes to hidden (first layer)
  for(int x = 0;  x < input_nodes.size(); x++){

    for(int y = 0; y < hidden_nodes[0].size(); y++){

      double insert = input_nodes[x] * input_edges[x][y];
      hidden_nodes[0][y] += insert;

    }

  }

//forward propagate through hidden layers
for(int x = 0; x < hidden_nodes.size() - 1; x++){//for each layer

  for(int y = 0; y < hidden_nodes[x].size(); y++){//for each node in layer

    hidden_nodes[x][y] = sigmoid(hidden_nodes[x][y]);

    for(int z = 0; z < hidden_nodes[x + 1].size(); z++){//for each node in next layer

      double insert = hidden_nodes[x][y] * hidden_edges[x][y][z];
      hidden_nodes[x + 1][z] += insert;
    }

  }
}


//hidden nodes (last layer) to output
for(int x = 0;  x < hidden_nodes[hidden_nodes.size() - 1].size(); x++){

  hidden_nodes[hidden_nodes.size() - 1][x] = sigmoid(hidden_nodes[hidden_nodes.size() - 1][x]);

  for(int y = 0; y < output_nodes.size(); y++){

    double insert = hidden_nodes[hidden_nodes.size() - 1][x] * output_edges[x][y];

    output_nodes[y] += insert;

  }

}

//run output layer through sigmoid

for(int x = 0; x < output_nodes.size(); x++){
  output_nodes[x] = sigmoid(output_nodes[x]);
}


}

//output values for all nodes (mainly for debuging)
void Network::output_all_nodes(){

std::cout << "Input layer: " << "\n";

for(int x = 0; x < input_nodes.size(); x++){
  std::cout << input_nodes[x] << "  ";
}

std::cout << "\n" << "Hidden layers: " << "\n";

for(int x = 0; x < hidden_nodes.size(); x++){

  for(int y = 0; y < hidden_nodes[x].size(); y++){
    std::cout << hidden_nodes[x][y] << "  ";
  }

  std::cout << "\n";
}

std::cout << "Output layer: " << "\n";

for(int x = 0; x < output_nodes.size(); x++){
  std::cout << output_nodes[x] << "  ";
}

std::cout << "\n";

}
