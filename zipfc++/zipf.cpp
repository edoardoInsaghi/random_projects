#include"zipf.hpp"

typedef unsigned int uint;


textZipf::textZipf(const std::string& filePath){

    file.open(filePath);
    if(!file.is_open()){
        throw std::exception();
    }
}


textZipf::~textZipf(){
    file.close();
}



std::map<std::string, uint> textZipf::elab(const std::string& saveFilePath){

    std::map<std::string, uint> wordFreq;

    std::string line;
    while(getline(file, line)){

        std::istringstream iss(line);
        std::string word;

        while(iss >> word){

            std::transform(word.begin(), word.end(), word.begin(),
                           [] (char ch) -> char 
                           {if (ispunct(ch)) return '\0'; return tolower(ch);});


            wordFreq[word]++;
        }
    }

    std::vector<std::pair<std::string, uint>> wordFreqVec(wordFreq.begin(),
                                                          wordFreq.end());
    
    std::sort(wordFreqVec.begin(), wordFreqVec.end(),
              [](const auto &a, const auto &b){
              return (a.second > b.second || (a.second == b.second && a.first < b.first));
              });

    std::ofstream saveFile(saveFilePath);
    saveFile << "word, count \n";
    for(const auto &[word, frequency] : wordFreqVec){
        saveFile << word << ", " << frequency << std::endl;
    }

    saveFile.close();

    return wordFreq;
}