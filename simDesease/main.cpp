#include<iostream>
#include<fstream>
#include"sim.hpp"

typedef unsigned int uint;

int main(){

    std::pair<uint, uint> size = {100, 100};
    std::pair<uint, uint> pos = {50, 50};

    Simulator simu{size, pos, 0.4};
    simu.Simulate(1000);

    std::ofstream file("results.csv");
    file << "i,vulnerable,exposed,infected,recovered\n";
    for(uint i=0; i<simu.vulnerable.size(); i++){
        file << i+1 << "," << simu.vulnerable[i] << "," << simu.exposed[i] << "," << simu.infected[i] << "," << simu.recovered[i] << "\n" << std::endl;
    }
    file.close();

    return 0;
}