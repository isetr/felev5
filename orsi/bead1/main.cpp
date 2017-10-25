#include <fstream>
#include <vector>
#include <future>
#include <iostream>

std::vector<std::string> readLines(std::string filename);
std::vector<std::future<std::string>> hashLines(std::vector<std::string> lines);
void process(std::vector<std::future<std::string>> hashedLines);

int main (int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Missing filename\n";
        return 1;
    }
    std::vector<std::string> lines(readLines(argv[1]));
    std::vector<std::future<std::string>> hashedData(hashLines(lines));
    process(hashedData);
    return 0;
}

std::vector<std::string> readLines(std::string filename) {
    std::vector<std::string> out;
    std::ifstream file(filename);
    int size;
    
    file >> size;
    out.reserve(size);

    std::string line;
    while(getline(file, line)) {
        out.push_back(line);
    }

    return out;
}

std::vector<std::future<std::string>> hashLines(std::vector<std::string>> lines) {
    std::vector<std::future<std::string>> out;
    return out;
}

void process(std::vector<std::future<std::string>> hashedLines) {

}