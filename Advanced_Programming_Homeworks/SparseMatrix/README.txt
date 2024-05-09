Advanced Programming, homework #1


Authors: Alessio Gaia, Barrasso Marco, Insaghi Edoardo.

The folder contains the files with the c++ code that handles sparse matrices, in both COO and CSR formats.
The build.sh file is used for compilation and execution of the main.cpp file, in which examples and test cases are provided to showcase the correctness of the code. 


Some notes: 

both of the formats are able to allocate a number in the matrix at a given position even if no number is present in that location (when the indexes are within the matrix bounds). The COO class is also able to manage the situation in which a zero is written into the matrix; the function to obtain the number of non zero values cleans any zeros found, keeping the matrix efficient (for this specific issue, at least). This is also why the getNNZ() function is declared as virtual in the parent class and then overridden in the child classes, rather than being implemented in the parent class and simply inherited from the child classes.

The work was evenly split between the members of the group, with the key concepts of the code, such as the logic behind the various function, being done together in presence. The work was split mostly for minor changes, such as code formatting, splitting between files and minor bug fixing.

While doing the homework several resources have been used:
* ChatGpt [https://chat.openai.com] & Phind [https://www.phind.com], used mostly for getting the syntax of specific tasks and sometimes for debugging.
* Extensive use of the course material, for everything from OOP concepts to syntax.
* Stack Overflow [https://stackoverflow.com], often used as a last resource when we were stuck. One example here is the overload of the () operator, in the const and non const cases.




