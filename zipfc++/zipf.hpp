#ifndef ZIPF__
#define ZIPF__


#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>

/************************************************************************
Simple script that takes a text file as input and returns a dictionary
counting the number of occurrences for each word. 
*************************************************************************/

typedef unsigned int uint;

class textZipf {
public:

    textZipf(const std::string& filePath);

    ~textZipf();

    std::map<std::string, uint> elab(const std::string& saveFilePath);

private:
    std::ifstream file;
};

#endif