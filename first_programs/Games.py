import random
import numpy as np
import matplotlib.pyplot as plt


# Every unit of food can be disputed by at most two contenders
class Food:

    def __init__(self):
        self.contenders = []

    def fight_for_food(self, population):

        if len(self.contenders) == 1:
            population.append(self.contenders[0])
            population.append(Player(status=self.contenders[0].status))

        elif self.contenders[0].status == 1 and self.contenders[1].status == 1:
            population.append(self.contenders[0])
            population.append(self.contenders[1])

        elif self.contenders[0].status == 2 and self.contenders[1].status == 2:
            r1 = np.random.rand()
            r2 = np.random.rand()
            if r1 > 0.75:
                population.append(self.contenders[0])
            if r2 > 0.75:
                population.append(self.contenders[1])

        elif self.contenders[0].status == 1 and self.contenders[1].status == 2:
            population.append(self.contenders[1])
            r1 = np.random.rand()
            r2 = np.random.rand()
            if r1 > 0.5:
                population.append(self.contenders[0])
            if r2 > 0.5:
                population.append(Player(status=self.contenders[1].status))

        elif self.contenders[0].status == 2 and self.contenders[1].status == 1:
            population.append(self.contenders[0])
            r1 = np.random.rand()
            r2 = np.random.rand()
            if r1 > 0.5:
                population.append(self.contenders[1])
            if r2 > 0.5:
                population.append(Player(status=self.contenders[0].status))


class Player:
    
    def __init__(self, status):
        self.status = status
        # 1 = dove
        # 2 = hawk
        
        
def show_evolution(evolution):

    doves = []
    hawks = []
    ratios = []
    total_population = []
    time = np.arange(len(evolution))

    for i in range(len(evolution)):
        doves.append(evolution[i][1])
        hawks.append(evolution[i][2])
        ratios.append(doves[i] / (hawks[i] + doves[i]))
        total_population.append(hawks[i] + doves[i])

    fig, axs = plt.subplots(3)
    axs[0].plot(time, doves, color='blue', label='doves')
    axs[0].plot(time, hawks, color='red', label='hawks')
    axs[0].legend()
    axs[1].plot(time, ratios, color='black', label='ratio doves/population')
    axs[1].legend()
    axs[2].plot(time, total_population, color='black', label='total population')
    axs[2].legend()
    plt.show()


def game(pop_size, food_size, game_len):

    evolution = []

    population = [Player(random.randint(1, 2)) for _ in range(pop_size)]

    total_hawks = 0
    total_doves = 0

    for m in range(len(population)):
        if population[m].status == 1:
            total_doves += 1
        elif population[m].status == 2:
            total_hawks += 1

    evolution.append((0, total_doves, total_hawks))

    for i in range(game_len):

        food = [Food() for _ in range(food_size)]
        new_population = []
        eaten = []
        random.shuffle(population)

        while len(population) > 0:

            if len(food) == 0:
                while len(population) > 0:
                    population.remove(population[0])
                    break

            else:
                this_food = random.choice(food)
                this_food.contenders.append(population[0])

                if len(this_food.contenders) == 2:
                    eaten.append(this_food)
                    food.remove(this_food)

                population.remove(population[0])

        for p in range(len(food)):
            if len(food[p].contenders) > 0:
                eaten.append(food[p])

        for k in range(len(eaten)):
            eaten[k].fight_for_food(population=new_population)

        population = [player for player in new_population]

        total_hawks = 0
        total_doves = 0

        for m in range(len(population)):
            if population[m].status == 1:
                total_doves += 1
            elif population[m].status == 2:
                total_hawks += 1

        evolution.append((i+1, total_doves, total_hawks))

    return evolution

# Example
evolution = game(10, 2500, 100)
show_evolution(evolution)
