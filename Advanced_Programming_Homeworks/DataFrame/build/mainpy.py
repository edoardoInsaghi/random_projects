import sys
import os
import argparse
import libstatsModPy
import libintegrModPy
from libintegrModPy import integrMod
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import seaborn as sns
import numpy as np
import matplotlib.patches as mpatches
import time
import math


if not os.path.exists("./plots"):
    os.makedirs("./plots")

# Part 1: Statistics Module
print("\n")
print("///////////////////////////////")
print("// Part 1: Statistics Module //")
print("///////////////////////////////")

# Allowing the user to give a custom input file path.
# In order to pass a file path, the flag -i must precede the user input.
parser = argparse.ArgumentParser(description="Homework 3 Alessio Barrasso Insaghi")
parser.add_argument('-i', '--input', type=str, help="Input file path")
parser.add_argument('-o', '--output', type=str, help="Output file path")

args = parser.parse_args()

# Default file to be analyzed if none is provided by the user.
input_file = "../data/CountryData.csv"
if args.input:
    input_file = args.input

# Default file for output if none is provided by the user.
out_file = "../data/output.csv"
if args.output:
    out_file = args.output

# Building the object.
df = libstatsModPy.statsMod(input_file)

# Removing rows with at least one NA value (if any are present).
df.dropNA()

# Obtaining the number of columns of the data-set.
print("\n")
print("-------------------------------------------------------------------------------------------------------------------------------------------------\n")
print("Number of columns stored in the object:", df.ncol, "\n")
print("-------------------------------------------------------------------------------------------------------------------------------------------------\n")

# Printing the name of the columns.
print("Name of the variables:")
df.colnames()
print("\n-------------------------------------------------------------------------------------------------------------------------------------------------\n")

# The data-set contains many columns, which makes it hard to visualize it neatly.
# In this case, we select a subset of the columns to showcase the functioning
# of the statistics module. The object lends itself to be sliced both on the columns
# and the rows (more on that later) by simply calling the operator() with a vector
# of the columns the user wants to keep.
sub_vars = ["Country","Region","Happiness Score","Economy (GDP per Capita)","Family"]
sub_df = df(sub_vars)

# Getting a brief description of the data_set"
print("Brief summary of the data.")
sub_df.summary()
print("-------------------------------------------------------------------------------------------------------------------------------------------------\n")

# Each of the previous functions can be called specifying any numerical column stored in the
# data-set and two indexes. The function will use only the rows in between said indexes as
# inputs. Here we call the mean of Happiness Score and Family, for the observation
# between row 100 and 150.
vars_reduced = ["Happiness Score", "Family"]
means_reduced = df.mean(vars_reduced, 100, 150)
print("Means of the variables Happiness Score and Family for the observation between rows 100 and 150:")
for key, value in means_reduced.items():
    print(f"{key}: {value}")
print()

print("-------------------------------------------------------------------------------------------------------------------------------------------------\n")

# Obtaining the correlation between pair of numerical variables.
print("Correlations:")
print("Corr(Happiness Score, Family) =", sub_df.corr(['Happiness Score', 'Family']))
print("-------------------------------------------------------------------------------------------------------------------------------------------------\n")

# Obtaining the subset of the rows that contain a specific value in a given column.
# Obtaining only the observations for the people who come from Western Europe.
cash_df = sub_df.find("Region", "Western Europe")
print("Subset of the people from Western Europe")
cash_df.head()

# Overloading the operator() also allows slicing the data-set on the rows, calling
# df(i, j) returns a statsMod object containing the rows between indexes i and j.
sub_df_small = sub_df(1, 100)
print("\n")
print("-------------------------------------------------------------------------------------------------------------------------------------------------\n")
print("Number of rows of the small data-set:", sub_df_small.nrow, "\n")
print("-------------------------------------------------------------------------------------------------------------------------------------------------\n")

# This way of slicing the data-set, which returns another instance of the same class
# containing the subset of interest of the data, allows for complex instruction via concatenation
# of simpler ones. With the next line of code, we obtain the subset of the observations between
# line 7 and line 100 who come from Region "Australia and New Zealand" and Country "Australia".
special_df = sub_df(7, 100).find("Region", "Australia and New Zealand").find("Country", "Australia")
print("Subset of the observations between line 10 and 20, where the region is Australia and New Zealand and the country is Australia. ")
special_df.head()
print("\n-------------------------------------------------------------------------------------------------------------------------------------------------\n")

#Histogram for numerical variables 
numerical_vars = sub_df.numCols
if numerical_vars:
    for i, var in enumerate(numerical_vars):
        plt.figure(figsize=(10, 6))
        
        data = [x for x in sub_df.data[var] if x is not None]
        
        n, bins, patches = plt.hist(data, bins=20, color='skyblue', edgecolor='black')  

        plt.title(f'Histogram for {var}')
        plot_filename = f"./plots/histogram{i}.png"
        plt.savefig(plot_filename)

# Bar Chart for Categorical Variable "Region"
var = "Region"
values = [x for x in sub_df.data[var] if x is not None]
labels, counts = np.unique(values, return_counts=True)

colors = plt.cm.rainbow(np.linspace(0, 1, len(labels)))

plt.figure(figsize=(10, 6))
plt.bar(labels, counts, color=colors)
plt.xticks([])
patches = [mpatches.Patch(color=colors[i], label=labels[i]) for i in range(len(labels))]
plt.legend(handles=patches)

plt.title(f'Bar chart for {var}')
plot_filename = "./plots/Bar_chart.png"
plt.savefig(plot_filename)

# Scatterplot: Happiness Score vs Economy (GDP per Capita) with color variable set to Region
x_variable = "Economy (GDP per Capita)"
y_variable = "Happiness Score"
color_variable = "Region"

x_data = [x for x in sub_df.data[x_variable] if x is not None]
y_data = [x for x in sub_df.data[y_variable] if x is not None]
color_data = [x for x in sub_df.data[color_variable] if x is not None]

categories = np.unique(color_data)
colors = plt.cm.rainbow(np.linspace(0, 1, len(categories)))
color_map = dict(zip(categories, colors))

plt.figure(figsize=(10, 6))
for category in categories:
    indices = [i for i, x in enumerate(color_data) if x == category]
    plt.scatter(np.array(x_data)[indices], np.array(y_data)[indices], color=color_map[category], alpha=0.5, edgecolors='w', linewidth=0.5, label=category)

plt.xlabel(x_variable)
plt.ylabel(y_variable)
plt.legend()

plt.title(f'Scatterplot: {y_variable} vs {x_variable}')
plot_filename = "./plots/scatterplot.png"
plt.savefig(plot_filename)


# Iterating over the rows of the data-set. Sometimes this causes a segfault when running the 
# program for the first time, we could not find the reason for this behaviour.
#for i, row in enumerate(sub_df):
#    if i > 10:
#        break
#    sub_df.printRow(row)
#print("\n-------------------------------------------------------------------------------------------------------------------------------------------------\n")

special_df.writeToFile(out_file)
print("\n-------------------------------------------------------------------------------------------------------------------------------------------------\n")

# Part 2: Integration Module
print("\n")
print("///////////////////////////////")
print("// Part 2: Integration Module //")
print("///////////////////////////////")

# Function to calculate and show accuracy, efficency and order of convergence for the methods in our class.
# This function, which is the one for the methods in the class that calculate the integral with a chosen
# number of subintervals, takes in input a method with that specific signature, a function (the one we want to integrate),
# the left and right limit of the integration and the exact value of that integral.
# It calculates, for different numbers of intervals, the value of the integral, time taken by the method, relative error
# and the order of convergence. 

def printInfoN(method, func, a, b, real_integral):

    # Initialization of the vectors where we will store the absolute error, step size and the order of convergence 
    # for each iteration. 
    errors = []
    step_size = []
    result = []
    intervals = [10, 100, 1000]

    print("\n" + "-" * 92)
    print("{:<20} {:<20} {:<20} {:<20}".format("Intervals", "Time taken (ms)", "Result", "Relative error"))

    for n in intervals:
        # List where we will save the time to invoke a method.
        # We invoke it 10 times and then take the median.
        times = []
        for i in range(10):
            start_time = time.time()
            approx_integral = method(func, a, b, n)
            end_time = time.time()
            times.append((end_time - start_time) * 1000)
        median_time = np.median(times)

        abs_error = abs(real_integral - approx_integral)
        print("{:<20} {:<20} {:<20} {:<20}".format(n, median_time, approx_integral, abs_error / real_integral))

        h = (b - a) / n
        errors.append(abs_error)
        step_size.append(h)

    print("-" * 92)

    # Loop to calculate the order of convergence.
    # We calculate it for each iteration and then take the mean.
    for i in range(len(intervals) - 1):
        if errors[i] < 1e-10 or errors[i+1] < 1e-10 or step_size[i] < 1e-10 or step_size[i+1] < 1e-10:
            continue

        error_change = errors[i+1] / errors[i]
        step_change = step_size[i+1] / step_size[i]

        if not np.isfinite(error_change) or not np.isfinite(step_change):
            continue

        # Calculation for the order of convergence for each ietarion.
        result.append(math.log(error_change) / math.log(step_change))

    # If the result is not empty it means that we calculate the order of convergence for some iteration
    # and so we take the mean. If it is empty it means that the errors or step size were to small to do the calculation
    # so we print an error message.
    if result:
        print(f"The order of convergence is: {np.mean(result)}")
    else:
        print("The order of convergence is not defined")

# Very similar function to the one above, but in this case we change the error threshold and not the 
# number of subintervals.
# Since the methods are the same but with a different input and the order of convergence has been calulated
# with the function above, here it is omitted.
# The order of convergence for the adaptive method is not calculated beacuse we need the step size or
# the number of intervals but the GNU Scientific Library, for that function, gives only the possibility to pass
# the absolute and relative error.
def printInfoErr(method, func, a, b, real_integral):
    thresholds = [1e-2, 1e-3, 1e-4]

    print("\n" + "-" * 92)
    print("{:<20} {:<20} {:<20} {:<20}".format("Threshold", "Time taken (ms)", "Result", "Relative error"))

    for threshold in thresholds:
        times = []
        for i in range(10):
            start_time = time.time()
            approx_integral = method(func, a, b, threshold)
            end_time = time.time()
            times.append((end_time - start_time) * 1000)
        median_time = np.median(times)

        abs_error = abs(real_integral - approx_integral)
        print("{:<20} {:<20} {:<20} {:<20}".format(threshold, median_time, approx_integral, abs_error / real_integral))

    print("-" * 92)


# Function to plot the absolute error against the number of intervals.
def showConvergence(methods, func, a, b, intervals, real_integral):

    plt.figure()

    # Dynamically choosing the colors of the plots
    num_methods = len(methods)
    colors = cm.rainbow(np.linspace(0, 1, num_methods))

    # For each method passed in input we calculate the error for each number of intervals and then
    # make a plot using matplotlib.
    for i, method in enumerate(methods):
        errors = []
        for n in intervals:
            approx_integral = method(func, a, b, n)
            error = abs(real_integral - approx_integral)
            errors.append(error)
        
        plt.plot(intervals, errors, color=colors[i], label=str(method.__name__))
    
    plt.xlabel("Number of intervals")
    plt.ylabel("Abs errors")
    plt.xticks(intervals)
    plt.legend()

    # Save plot to the directory
    plot_filename = "./plots/convergence_plot.png"
    plt.savefig(plot_filename)

# Funtion to plot the time taken by each methods for different element of a sequence(in this case the 
# sequence could be a list number of intevals or a list of threshold) against the elements of the sequence.

def showTimeTaken(methods, func, a, b, sequence, real_integral):

    plt.figure()

    # Dynamically choosing the colors of the plots
    num_methods = len(methods)
    colors = cm.rainbow(np.linspace(0, 1, num_methods))

    # For each method passed in input we calculate the time taken to invoke the method 
    # on each element in the sequence and then make a plot using matplotlib.
    for i, method in enumerate(methods):
        median_time = []
        for n in sequence:
            times = []
            for j in range(10):
                start_time = time.time()
                approx_integral = method(func, a, b, n)
                end_time = time.time()
                times.append((end_time - start_time) * 1000)
            median_time.append(np.median(times))

        plt.plot(sequence, median_time, color=colors[i], label=str(method.__name__))
    
    plt.xlabel("Sequence")
    plt.ylabel("Time taken (ms)")
    plt.legend()

    # Save plot to the directory
    plot_filename = "./plots/timetaken_plot.png"
    plt.savefig(plot_filename)


# Adding the functions to the integrMod class.
libintegrModPy.integrMod.showConvergence = showConvergence
libintegrModPy.integrMod.showTimeTaken = showTimeTaken
       

# Defining e^x + 3x^2+ 10 as a lambda function.
f = lambda x: math.exp(x) + 3 * x**2 + 10

# We decided to integrate the f function in the interval [0,10].
# Exact value of the integral.
real_integral = 1099 + math.exp(10)


# Calling the functions to test the performance of Midpoint methods of the class.
print("\n")
print("///////////////////")
print("// Midpoint rule //")
print("///////////////////")

printInfoN(integrMod.Midpoint, f, 0, 10, real_integral)
printInfoErr(integrMod.Midpoint, f, 0, 10, real_integral)

# Calling the functions to test the performance of Trapezoid methods of the class.
print("\n\n")
print("////////////////////")
print("// Trapezoid rule //")
print("////////////////////")

printInfoN(libintegrModPy.integrMod.Trapezoid, f, 0, 10, real_integral)
printInfoErr(libintegrModPy.integrMod.Trapezoid, f, 0, 10, real_integral)

# Calling the functions to test the performance of Trapezoid methods of the class.
print("\n\n")
print("//////////////////")
print("// Simpson rule //")
print("//////////////////")

printInfoN(libintegrModPy.integrMod.Simpson, f, 0, 10, real_integral)
printInfoErr(libintegrModPy.integrMod.Simpson, f, 0, 10, real_integral)

# Calling the functions to test the performance of GaussianQuadrature method of the class.
print("\n\n")
print("///////////////////")
print("// Gaussian rule //")
print("///////////////////")

printInfoN(libintegrModPy.integrMod.GaussQuadrature, f, 0, 10, real_integral)

# Calling the functions to test the performance of Adaptive method of the class.
print("\n\n")
print("///////////////////")
print("// Adaptive rule //")
print("///////////////////")

printInfoErr(libintegrModPy.integrMod.Adaptive, f, 0, 10, real_integral)

# Just an example of how the Integrate method can be used.
print("\nThe result of the integral with Trapezoid rule using the Integrate method is: ",
      libintegrModPy.integrMod.Integrate("Trapezoid", f, 0, 10, 1e-4),"\n") 

intervals = [i for i in range(2, 18, 2)]

start = 0.001
end = 0.1
step = 0.005
sequence = np.arange(start, end + step, step)

methods = [integrMod.Midpoint, integrMod.Trapezoid, integrMod.Simpson, integrMod.GaussQuadrature]

# Calling the methods to generate plot of the results.
integrMod.showConvergence(methods, f, 0, 10, intervals, real_integral)
integrMod.showTimeTaken(methods[:-1], f, 0, 10, sequence, real_integral)