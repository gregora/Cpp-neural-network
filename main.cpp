#include "network.h"

using namespace std;

int main(){


  Network n(3, 2, 3, 3);
  double input[3] = {1, 2, 3};

  for(int i = 0; i < 10000; i++){
    n.mutate(10);
    n.set_input_nodes(input);
    n.forward_propagate();
    n.output_all_nodes();
    cout << "=======================\n";
  }

  n.save("savefiles/n.AI");

}
