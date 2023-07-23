import numpy as np
import matplotlib.pyplot as plt
import random


# 37 = Fair roulette
# 38 = European roulette
# 39 = American roulette

class Roulette:
    def __init__(self, numbers=[], win=0, plays=0):
        self.numbers = numbers
        for i in range(1, 37):
            self.numbers.append(i)
        self.win = win
        self.plays = plays
        self.walk = []

    def play(self, throw=None):
        self.win -= 1
        self.plays += 1
        random.shuffle(self.numbers)
        if throw is None:
            throw = np.random.randint(1, 37)
        result = random.choice(self.numbers)
        if throw == result:
            self.win += 36
        self.walk.append(self.win)

    def game(self, number, throw=None):
        while self.plays < number:
            self.play(throw)
        print(f"Winning percentage after {number} plays: \t\t{round((self.win / self.plays) * 100, 3)}")
        self.win = 0
        self.plays = 0

    def plot_walks(self, number):
        x = np.arange(number)
        plt.plot(x, self.walk)
        plt.show()


# Example
# my_roulette = Roulette()
# my_roulette.game(100000)
# my_roulette.plot_walks(100000)
