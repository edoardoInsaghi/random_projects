## Advanced Programming, Homework 3


### Notes for homework 3
Since the third homework is a continuation of the second we thought of adding what is new about the new project in the same README file. We provided the python bindings with pybind11 for the two libraries which are buildable as two separate python modules. We then provided a python script to show that the bindings work as intended for both modules, the file is named ```mainpy.py``` and can be found inside the ```/build``` and ```/examples``` directory and can be called with ```python3 mainpy.py``` directly from ```/build``` after running ```cmake ..``` and ```make```. In this file we provided some plots to showcase some functionalities of the matplotlib and seaborn libraries, these plots serve to both better understand the data managed by the statistics library and the time taken by the integration module, and they are put in a ```/plot``` folder by the ```mainpy.py``` script. With respect to the last homework we also implemented iterators for the statsMod dataframe, but while they work smoothly in c++, they rarely result in a segfault in the python module. We tried debugging with lldb but we could not resolve the issue. The few lines of code for the iterators in ```mainpy.py``` have therefore been commented out because even if they almost always work, we do not want to force manual removal in the worst case scenario (we are sorry for the inconvenience).

This is the updated structure of the proejct: 
```
.
├── CMakeLists.txt
├── README.MD
├── bindings
│   ├── integrMod_bindings.cpp
│   └── statsMod_bindings.cpp
├── build
│   └── mainpy.py
├── data
│   ├── CountryData.csv
│   ├── output.csv
│   └── shopping_trends_updated.csv
├── examples
│   ├── mainc.cpp
│   └── mainpy.py
├── libs
│   ├── cxxopts-master.zip
│   └── gsl-2.7.1.zip
└── src
    ├── integrMod.cpp
    ├── integrMod.hpp
    ├── statsMod.cpp
    └── statsMod.hpp
```

### Authors: 
- Gaia Alessio, gaia-alessio@hotmail.com
- Marco Barrasso, marcobarrasso@gmail.com
- Edoardo Insaghi, edoardo.insaghi@gmail.com


### Implemented Modules:
- Statistics Module (statsMod)
- Integration Module (integrMod)


### statsMod
Our group decided to represent the data as a standard map, in which the names of the variables are the keys that map to the respective column in the data-frame. Each column is stored in a standard vector of standard optional of standard variants. These structures allow to manage NA values as well as the possible different data types contained in a csv file.

Our object possesses a number of methods that calculate the mean, median, variance, standard deviation, correlation and frequency count for variables where these operations are applicable. The user may also select a subset of the rows on which these operations will be applied. The user may specify a subset of columns stored in the data-set when calling one of these method, and if none is selected then all suitable variables will be used. The function use the standard library when possible for efficiency purposes, with custom lambda functions that handle optional and variant data types.

The object also possesses some utility methods that are able to drop NA values, print the name of the columns and the first n lines of the data-set, as well as providing a brief summary of the data and output the latter to a user given csv file.

Other features are given by the functions that allow for slicing the data-set, both on the rows and the columns. Said functions return a new instance of the same class that stores only the data of interest. Each time one of these function is called, a new object is instantiated, and the data is copied to it. This approach is not the most efficient due to the resources needed to copy and rewrite the data, but on the other hand it allows to modify the new object without messing with the old one, which might be of interest. The other approach would require the use of pointers to store the data, and the smart way would be using shared pointers so that different instances of the class can safely point to the same data.

Some of the features of the class were inspired by other popular frameworks, such as the R data-frame.


### integrMod
Our group decided to create a single class containing the integration rules as methods. All of them are made static since within the class we don't store anything and we thought it was a good idea to call the method without instantiating an object of the class. Every integration method has in common the fact that it takes in input a function,the left and the right limit of the integration.

Midpoint, Trapezoid and Simpson rule have two methods each because we wanted to be more flexible and give the user the possibility to pass either the number of subintervals that he wanted to use for the integration or the error tolerance. In the second case the number of subintervals is calculated internally in order not to exceed the threshold. GaussQuadrature rule has been implemented using the GNU Scientific Library and has only one method because the function from the Scientific Library used inside does not accept absolute or relative error bounds. Since we used the Scientific Library we also decided to implement an adaptive integration method for the class.

Regarding the results we built a function in the main that calculates the value of the integral, time taken by the method, relative error and the order of convergence as the numbers of intervals and error threshold change. From what we can see from the results for the Midpoint, Trapezoid and Simpson as the precision increases the time taken increases too and the relative error decreases, which is reasonable. The convergence is quadratic for the Midpoint and Trapezoid methods while it is of the fourth order for the Simpson rule. The Gauss Quadrature method has some problems in calculating the order of convergence, which are probably due to the fact that the calculation of the relative error and the order of convergence work with really small numbers which can cause errors with floating point operations. For the adaptive rule the errors remain constant due to the nature of the method itself.


### Project Structure
Our project has the following structure:  
```
.
├── CMakeLists.txt
├── build
├── data
│   ├── output.csv
│   └── shopping_trends_updated.csv
├── examples
│   └── main.cpp
├── libs
│   ├── cxxopts-master.zip
│   └── gsl-2.7.1.zip
├── readme.MD
└── src
    ├── integrMod.cpp
    ├── integrMod.hpp
    ├── statsMod.cpp
    └── statsMod.hpp
```
```/data``` contains the default files for showcasing the libraries, ```/libs``` has the zipped files with the external libraries needed throughout the project, and ```/src``` contains the source files and the header files of the two modules.


### External libraries and Build
In order to easily read inputs from the command line we used the cxxopts library (https://github.com/jarro2783/cxxopts), while the integration module relies on the GSL (https://www.gnu.org/software/gsl/) for the gaussian quadrature and the adaptive methods. To build the libraries we wrote a CMake file  which handles everything, from locating the external libraries on the machine to the compilation of the executable and library linking (from our understanding of how the CMake file works). To build it it is sufficient to run ```cmake ..``` and ```make``` from the build/ directory. The example file already contains default values for the paths of the input and output files, if the user wants to specify custom files they will have to run it with ```./main -i path/to/input/file -o path/to/output/file```.


### Work Subdivision
The core ideas and methods were decided together, with only minor implementations and various fixes being dealt with individually at times, but they were always split equally among group members.


### Resources
While doing the homework several resources have been used:
- ChatGpt [https://chat.openai.com] & Phind [https://www.phind.com], used mostly for understanding how to use the libraries and work with files and the CMake, and sometimes for debugging.
- Stack Overflow [https://stackoverflow.com], especially to deal with standard variants.
- The material of the course was consulted many times, especially for the CMake file.
- Several websites, mostly for trying to understand the CMake and the overall structure of our project.