#include "network.h"

int main(){
  Network network(3, 1, 1, 3);

  double array[] = {1, 0, 1};
  network.set_input_nodes(array);
  network.forward_propagate();
  network.output_all_nodes();
  
}
