import numpy as np
import math as m
import random
import matplotlib.pyplot as plt


class Individual:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.fitness = None


def make_individual(k):
    a = Individual(random.uniform(-k, k), random.uniform(-k, k))
    return a


def make_population(n, k):
    population = []
    for i in range(n):
        population.append(make_individual(k))
    return population


def fit(Individual):
    X = Individual.x
    Y = Individual.y
    Individual.fitness = X**2 + Y**2 - 23*X - 7*Y - 23


def tweak(Indiv, k):
    a = Individual(Indiv.x + m.sqrt(k) * np.random.randn(), Indiv.y + m.sqrt(k) * np.random.randn())
    return a


def plot(population):
    X = []
    y = []
    z = []
    for i in range(len(population)):
        X.append(population[i].x)
        y.append(population[i].y)
        z.append(population[i].fitness)

    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    ax.scatter(X, y, z)
    plt.show()


def evolution(first_sample_size, generations, limit_value, elite):

    population = make_population(first_sample_size, limit_value)

    best = None

    for i in range(generations):

        current_best = []

        for j in range(len(population)):
            if population[j].fitness is None:
                fit(population[j])

        for k in range(len(population)):
            if best is None:
                best = population[k]
            if population[k].fitness < best.fitness:
                best = population[k]

        to_sort = []
        for w in range(len(population)):
            to_sort.append((population[w].fitness, population[w]))
        to_sort.sort()

        for z in range(elite):
            current_best.append(to_sort[z][1])

        for l in range(len(current_best)):
            population.append(tweak(current_best[l], limit_value))

    for j in range(len(population)):
        if population[j].fitness is None:
            fit(population[j])

    return population, best


population, best = evolution(200, 50, 10000, 30)
print(f"({round(best.x, 3)}, {round(best.y, 3)})", "\t", round(best.fitness, 2))
plot(population)