#include"sim.hpp"

typedef unsigned int uint;

// Subject class
Subject::Subject(uint incubation_time, uint recovery_time, uint status)
    : incubation_time(incubation_time), recovery_time(recovery_time), status(status) {}


// Simulator class
Simulator::Simulator(std::pair<uint, uint> &size, std::pair<uint, uint> &position, double p)
    : size(size), p(p) {
    for (uint i = 0; i < size.first * size.second; i++) {
        population.push_back(Subject{(uint)rand() % 2 + 1, (uint)rand() % 10 + 5, 0});
    }

    population[position.first * size.second + position.second] = Subject{(uint)rand() % 5 + 1, (uint)rand() % 10 + 5, 2};
}

Simulator::Simulator(const Simulator &other)
    : size(other.size), population(other.population), vulnerable(other.vulnerable),
      infected(other.infected), recovered(other.recovered), p(other.p) {}

Simulator &Simulator::operator=(const Simulator &other) {
    if (this != &other) {
        this->size = other.size;
        this->population = other.population;
        this->vulnerable = other.vulnerable;
        this->infected = other.infected;
        this->recovered = other.recovered;
        this->p = other.p;
    }
    return *this;
}

Simulator::~Simulator() {}

void Simulator::count() {
    std::vector<uint> counts(4, 0);

    for (uint i = 0; i < size.first * size.second; i++) {
        counts[population[i].status]++;
    }

    vulnerable.push_back(counts[0]);
    exposed.push_back(counts[1]);
    infected.push_back(counts[2]);
    recovered.push_back(counts[3]);
}

uint Simulator::getNumInfected(uint i, uint j) {
    uint count = 0;
    int ni, nj;

    for (int u = -1; u < 2; u++) {
        for (int v = -1; v < 2; v++) {
            ni = (int)i + u;
            nj = (int)j + v;

            if (ni >= 0 && ni < size.first && nj >= 0 && nj < size.second) {
                count = (population[ni * size.first + nj].status == 2) ? ++count : count;
            }
        }
    }

    return count;
}

void Simulator::updatePop() {
    std::vector<Subject> population_copy = population;

    for (uint i = 0; i < size.first; i++) {
        for (uint j = 0; j < size.second; j++) {
            Subject &subject = population_copy[i * size.first + j];

            switch (subject.status) {
                case 0: // subject is vulnerable
                {
                    uint infected_neighbors = getNumInfected(i, j);
                    double infection_prob = 1 - std::pow((1 - p), infected_neighbors);
                    if (rand() / (double)RAND_MAX < infection_prob)
                        subject.status = 1;
                    break;
                }

                case 1: // subject is incubating the disease
                {
                    subject.time_incubated++;
                    if (subject.time_incubated >= subject.incubation_time)
                        subject.status = 2;
                    break;
                }

                case 2: // subject is infected
                {
                    subject.time_recovered++;
                    if (subject.time_recovered >= subject.recovery_time)
                        subject.status = 3;
                    break;
                }

                case 3: // subject has recovered and is now immune
                {
                    break;
                }
            }
        }
    }

    population = population_copy;
}

void Simulator::Simulate(uint iters) {
    count();

    uint i = 0;
    while (i < iters) {
        updatePop();
        count();

        if (recovered.back() == size.first * size.second)
            break;
        i++;
    }
}
