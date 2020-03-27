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
  std::vector<double> nloss;

  double array[3];

  //set up the first network
  for(int x = 0; x < 100; x++){
    Network n(3, 1, 3, 3);
    nvec.push_back(n);

    double allloss = 0;

    for(int y = 0; y < 100; y++){

      array[0] = randNum(n.random());
      array[1] = randNum(n.random());
      array[2] = randNum(n.random());

      n.set_input_nodes(array);
      n.forward_propagate();
      allloss += n.loss(array);

    }

    double avgloss = allloss / 100;

    nloss.push_back(avgloss);

  }

  int itnum = 0;

  double avgloss = 1;

  int minposition = -1;
  double minvalue = 10;

  while(minvalue > 0.005 && itnum < 1000000){

    minposition = -1;
    minvalue = 10;

    itnum += 1;

    double anloss = 0;

    for(int x = 0; x < nvec.size(); x++){
      anloss += nloss[x];
      if(nloss[x] < minvalue){
        minvalue = nloss[x];
        minposition = x;
      }
    }

    avgloss = anloss / nvec.size();

    if(itnum % 100 == 0){

      std::cout << "All networks average loss: " << avgloss << " at " << itnum << " iteration" << std::endl;
      std::cout << "nvec size: " << nvec.size() << std::endl;
      std::cout <<  "\033[32mMin loss: " << minvalue << "\033[0m" << std::endl;

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

      if(nloss[x] > avgloss && nvec.size() > 10){
        nvec.erase(nvec.begin() + x);
        nloss.erase(nloss.begin() + x);
        size = size - 1;
        x = x - 1;
      }else if(nvec.size() < 100){ // population cap
        Network n2(3, 1, 3, 3);

        if(n2.random() >= 0.1){ //a fraction of new population should start clean
          n2 = nvec[x];
        }

        n2.mutate(20);

        double allloss2 = 0;

        for(int y = 0; y < 100; y++){

          array[0] = randNum(n2.random());
          array[1] = randNum(n2.random());
          array[2] = randNum(n2.random());

          n2.set_input_nodes(array);
          n2.forward_propagate();

          allloss2 += n2.loss(array);
        }

        double avgloss2 = allloss2 / 100;

        nloss.push_back(avgloss2);
        nvec.push_back(n2);

      }

    }

  }

  std::cout << "\033[35mEnding loss:  " << minvalue << "\033[0m" << std::endl;

  std::cout << "========= TEST ==========" << std::endl;


  Network n2(3, 1, 3, 3);

  n2 = nvec[minposition];

  double allloss2 = 0;
  double testarray[3];

  // 20 test cases
  for(int y = 0; y < 20; y++){

    std::cout << "----------- Test case ----------" << std::endl;

    testarray[0] = randNum(n2.random());
    testarray[1] = randNum(n2.random());
    testarray[2] = randNum(n2.random());

    n2.set_input_nodes(testarray);
    n2.forward_propagate();
    n2.output_all_nodes();
    std::cout << "loss: " << n2.loss(testarray) << std::endl;

    allloss2 += n2.loss(testarray);

  }

  double avgloss2 = allloss2 / 20;

  std::cout << "Average loss: " << avgloss2 << std::endl;

  n2.save("savefiles/network_saved.AI");


}
