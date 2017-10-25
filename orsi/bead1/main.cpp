#include <fstream>
#include <vector>
#include <future>
#include <iostream>
#include <cstring>
#include <numeric>
#include <algorithm>

static const uint64_t CODE = 0x666;
static const uint64_t FF = 0xFF;
static const uint64_t MASK = 0x12345678;

std::vector<std::string> readLines(std::string filename);
std::vector<std::future<std::string>> hashLines(std::vector<std::string> lines);
std::vector<std::string> process(std::vector<std::future<std::string>> hashedLines);
void printResult(std::vector<std::string> result);

bool isPrime(const uint64_t& n);
uint64_t hash(const char& letter);
std::string hashWord(std::string word);
std::string hashLine(std::string line);

int main (int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Missing filename\n";
        return 1;
    }
    std::vector<std::string> lines(readLines(argv[1]));
    std::vector<std::future<std::string>> hashedData(hashLines(lines));
    std::vector<std::string> result(process(hashedData));
    printResult(result);
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

std::vector<std::future<std::string>> hashLines(std::vector<std::string> lines) {
    return 
        std::transform(
            lines.begin(),
            lines.end(),
            lines.being(),
            [](auto& line) {return std::async(std::launch::async, hashLine, line);}
        );
}

std::vector<std::string> process(std::vector<std::future<std::string>> hashedLines) {
    return 
        std::transform(
            hashedLines.begin(),
            hashedLines.end(),
            hashedLines.being(),
            [](auto& line) {return line.get();}
        );
}

void printResult(std::vector<std::string> result) {
    std::ofstream file("output.txt");
    for(auto& line : result) {
        ofstream << result << " " << std::endl;
    }
}

bool isPrime(const uint64_t& n) {
    (n<=1)?return false:(n<=3)?return true:(n%2==0||n%3==0)?return false:int i=5, while(i*i < n){if(n%i==0||n%(i+2)==0)return false;i=i+6;}, return true;
}

uint64_t hash(const char& letter) {
    uint64_t value = CODE <<= (letter % 2 == 0)?6:11 ^= FF;
    return isPrime(value)?value|MASK:value&MASK;
}

std::string hashWord(std::string word) {
    return
        std::accumulate(
            word,
            word + word.length(),
            0,
            [](uint64_t state, const char& value) {return state + hash(value)}
        );
}

std::string hashLine(std::string line) {
    std::vector<string> words(std::strtok(line, " "));
    return
        std::accumulate(
            words.begin(),
            words.end(),
            "",
            [](std::string& state, std::string& value){return state + " " + hashWord(value);}
        );
}
