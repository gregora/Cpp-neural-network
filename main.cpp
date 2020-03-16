#include "network.h"

int randNum(double r){
  if(r >= 0.5){
    return 1;
  }else{
    return 0;
  }
}


int main(){


  std::vector<Network> nvec;
  std::vector<double> ncost;

  double array[3];

  //set up the first network
  for(int x = 0; x < 50; x++){
    Network n(3, 1, 3, 3);
    nvec.push_back(n);

    double allcost = 0;

    for(int y = 0; y < 20; y++){

      array[0] = randNum(n.random());
      array[1] = randNum(n.random());
      array[2] = randNum(n.random());

      n.set_input_nodes(array);
      n.forward_propagate();
      allcost += n.cost(array);

    }

    double avgcost = allcost / 20;

    ncost.push_back(avgcost);

  }

  int itnum = 0;

  double avgcost = 1;

  int minposition = -1;
  double minvalue = 10;

  while(minvalue > 0.1 && itnum < 1000000){

    minposition = -1;
    minvalue = 10;

    itnum += 1;

    double ancost = 0;

    for(int x = 0; x < nvec.size(); x++){
      ancost += ncost[x];
      if(ncost[x] < minvalue){
        minvalue = ncost[x];
        minposition = x;
      }
      //std::cout << ncost[x] << std::endl;
    }

    avgcost = ancost / nvec.size();

    if(itnum % 100 == 0){

      std::cout << "All networks average cost: " << avgcost << " at " << itnum << " iteration" << std::endl;
      std::cout << "nvec size: " << nvec.size() << std::endl;
      std::cout <<  "\033[32mMin cost: " << minvalue << "\033[0m" << std::endl;

      if(itnum % 10000 == 0){
        std::string savefile = "th_network_saved.AI";
        savefile = "savefiles/"+std::to_string(itnum) + savefile;
        nvec[minposition].save(savefile);
        std::cout << "\033[35mSaved!\033[0m" << std::endl;
      }
    }


    int size = nvec.size();

    //delete all the useless cases
    for(int x = 0; x < size; x++){

      if(ncost[x] > avgcost && nvec.size() > 10){
        nvec.erase(nvec.begin() + x);
        ncost.erase(ncost.begin() + x);
        size = size - 1;
        x = x - 1;
      }else if(nvec.size() < 50){ // population cap
        Network n2(3, 1, 3, 3);

        if(n2.random() > 0.7){
          n2 = nvec[x];
          n2.mutate(20);
        }

        double allcost2 = 0;

        for(int y = 0; y < 20; y++){

          array[0] = randNum(n2.random());
          array[1] = randNum(n2.random());
          array[2] = randNum(n2.random());

          //std::cout << array[0] << array[1] << array[2] << std::endl;

          n2.set_input_nodes(array);
          n2.forward_propagate();

          allcost2 += n2.cost(array);
        }

        double avgcost2 = allcost2 / 20;

        ncost.push_back(avgcost2);
        nvec.push_back(n2);

      }

    }

  }

  std::cout << "\033[35Ending cost:  " << minvalue << "\033[0m" << std::endl;

  std::cout << "========= TEST ==========" << std::endl;




  Network n2(3, 1, 3, 3);

  n2 = nvec[minposition];

  double allcost2 = 0;
  double testarray[3];


  for(int y = 0; y < 10; y++){

    std::cout << "----------- Test case ----------" << std::endl;

    testarray[0] = randNum(n2.random());
    testarray[1] = randNum(n2.random());
    testarray[2] = randNum(n2.random());

    n2.set_input_nodes(testarray);
    n2.forward_propagate();
    n2.output_all_nodes();
    std::cout << "Cost: " << n2.cost(testarray) << std::endl;

    allcost2 += n2.cost(testarray);

  }

  double avgcost2 = allcost2 / 10;

  std::cout << "Average cost: " << avgcost2 << std::endl;

  n2.save("savefiles/network_saved.AI");

  std::cout <<  randNum(n2.random()) << std::endl;
  std::cout <<  randNum(n2.random()) << std::endl;
  std::cout <<  randNum(n2.random()) << std::endl;
  std::cout <<  randNum(n2.random()) << std::endl;
  std::cout <<  randNum(n2.random()) << std::endl;
  std::cout <<  randNum(n2.random()) << std::endl;

}
