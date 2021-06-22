import random
import string
from math import log
from matplotlib import pyplot as plt
import numpy as np


def process_file(file_name):
    hist = {}
    file = open(file_name)
    for line in file:
        process_line(line, hist)
    return hist


def process_line(line, hist):
    line.replace('-', '')
    for word in line.split():
        word = word.strip(string.punctuation)
        word = word.lower()
        if word != '':
            try:
                float(word)
                continue
            except:
                if word not in hist:
                    hist[word] = 1
                else:
                    hist[word] += 1


def most_common(hist):
    t = []
    for key, value in hist.items():
        t.append((value, key))
    t.sort(reverse=True)
    return t


def total_words(hist):
    return sum(hist.values())


def unique_words_number(hist):
    return len(hist.values())


def print_most_common(hist, num=10):
    t = most_common(hist)
    print('\nThe most common words are:')
    for freq, word in t[:num]:
        print(word, freq, sep='\t\t')


def markov_analysis(file_name):
    file = open(file_name)
    words = []
    hist = {}
    for line in file:
        line.replace('-', '')
        for word in line.split():
            if not word.isupper():
                words.append(word)
    for i in range(1, len(words) - 1):
        if (words[i - 1], words[i]) not in hist:
            hist[(words[i - 1], words[i])] = [words[i + 1]]
        else:
            hist[(words[i - 1], words[i])] += [words[i + 1]]
    return hist


def auto_complete(hist, n=100, separator=' '):
    word1 = input('Insert the first word: ')
    word2 = input('Insert the second word: ')
    speech = [word1, word2]
    for i in range(1, n):
        try:
            word = random.choice(hist[(speech[i - 1], speech[i])])
            speech.append(word)
        except KeyError:
            word = input('Word not found, please enter a new one: ')

    return separator.join(speech)


def zipf_law(filename):
    file = open(filename)
    hist = {}
    t = []
    for line in file:
        line.replace('-', '')
        for word in line.split():
            if not word.isupper():
                if word != '':
                    word = word.strip(string.punctuation)
                    word = word.lower()
                    try: 
                        float(word)
                        continue
                    except:
                        if word not in hist:
                            hist[word] = 1
                        else:
                            hist[word] += 1

    for key, value in hist.items():
        t.append((value, key))
    t.sort(reverse=True)
    log_f = []
    log_r = []
    for i in range(len(t)):
        log_f.append(log(t[i][0]))
        log_r.append(log(i + 1))
    plt.ylabel('log_frequency')
    plt.xlabel('log_rank')
    plt.plot(log_r, log_f, color='black')
    plt.show()


def cumulative_distribution(filename):
    hist = process_file(filename)
    totw = total_words(process_file(filename))
    unqw = unique_words_number(process_file(filename))
    to_sort = np.arange(len(hist), dtype=float)
    i = -1
    for val in hist.values():
        i += 1
        to_sort[i] = val/totw
    np.sort(to_sort)
    distr = np.cumsum(to_sort)
    x = np.arange(len(distr), dtype=float)
    x /= unqw
    plt.plot(x, distr)
    plt.show()



# 'cumulative_distribution("mobydick.txt")
zipf_law("mobydick.txt")
