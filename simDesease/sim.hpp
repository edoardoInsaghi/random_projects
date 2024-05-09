
/************************************************************************
Script that implements a simple simulation of a disease spreading 
in a group of individuals. The population is represended by a matrix
in which each cell represent an individual. Each individual is found 
in one of either four states: vulnerable, exposed, infected or recovered.
In each iteration, for each vulnerable individual the number of infected
individuals around them (max is 8) are computed. Each of these vulnerable
individual has a probability of being infected given by 1 - (1 - p)^k 
where p is the probability of being infected by one subject and k is the 
number of infected neighbours. An exposed individual is assumed to be
non contagious.
*************************************************************************/

#ifndef SIMDESEASE_
#define SIMDESEASE_

#include <cmath>
#include <vector>
#include <cstdlib>

typedef unsigned int uint;

class Subject {
public:
    friend class Simulator;

    Subject(uint incubation_time, uint recovery_time, uint status = 0);

private:
    uint incubation_time;
    uint time_incubated = 0;
    uint recovery_time;
    uint time_recovered = 0;
    uint status; // 0=vulnerable, 1=exposed, 2=infected, 3=recovered
};


class Simulator {
public:
    std::pair<uint, uint> size;
    std::vector<Subject> population;
    std::vector<uint> vulnerable; // 0
    std::vector<uint> exposed;    // 1
    std::vector<uint> infected;   // 2
    std::vector<uint> recovered;  // 3
    double p;

    Simulator(std::pair<uint, uint> &size, std::pair<uint, uint> &position, double p);

    Simulator(const Simulator &other);

    Simulator &operator=(const Simulator &other);

    ~Simulator();

    // Method that counts the number of individuals belonging to each condition.
    void count();

    // Method that returns the number of infected individuals surrounding the position (i, j) in the population matrix.
    uint getNumInfected(uint i, uint j);

    // Method that updates the population given the current state and the probability of contagion.
    void updatePop();

    // Method that performs the simulation for a given number of iterations
    void Simulate(uint iters);
};

#endif