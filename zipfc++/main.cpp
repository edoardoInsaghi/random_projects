#include"zipf.hpp"


int main(){
  
    const std::string input_file_path = "alice_in_wonderland.txt";
    const std::string output_file_path = "output.txt";

    textZipf processor(input_file_path);
    processor.elab(output_file_path);

    return 0;
}