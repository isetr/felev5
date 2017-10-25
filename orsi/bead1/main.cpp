#include <fstream>
#include <vector>
#include <future>
#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <chrono>

static const uint32_t CODE = 0x666;
static const uint32_t FF = 0xFF;
static const uint32_t MASK = 0x12345678;

std::vector<std::string> readLines(int argc, char** argv);
std::vector<std::future<std::string>> hashLines(std::vector<std::string>& lines);
std::vector<std::string> process(std::vector<std::future<std::string>>& hashedLines);
void printResult(const std::vector<std::string>& result);

bool isPrime(const uint32_t& n);
uint32_t hash(const char& letter);
uint32_t hashWord(const std::string& word);
std::string hashLine(const std::string& line);

int main (int argc, char** argv) {
    std::vector<std::string> lines(readLines(argc, argv));

    auto start = std::chrono::steady_clock::now();

    std::vector<std::future<std::string>> hashedData(hashLines(lines));
    std::vector<std::string> result(process(hashedData));

    auto end = std::chrono::steady_clock::now();
    std::cerr << "Time elapsed: " << std::chrono::duration<double>(end - start).count() << " sec" << std::endl;

    printResult(result);
    return 0;
}

std::vector<std::string> readLines(int argc, char** argv) {
    std::vector<std::string> out;
    std::ifstream file;
    int size;
    
    if (argc == 2) {
        file.open(argv[1]);
    } else {
        file.open("input.txt");
    }

    file >> size;
    std::getline(file, line); // first empty line after the number
    
    out.reserve(size);
    
    std::string line;
    while(std::getline(file, line)) {
        out.push_back(line);
    }

    return out;
}

std::vector<std::future<std::string>> hashLines(std::vector<std::string>& lines) {
    std::vector<std::future<std::string>> result(lines.size());
    std::transform(
        lines.begin(),
        lines.end(),
        result.begin(),
        [](std::string& line) {return std::async(std::launch::async, hashLine, line);}
    );
    return result;
}

std::vector<std::string> process(std::vector<std::future<std::string>>& hashedLines) {
    std::vector<std::string> result(hashedLines.size());
    std::transform(
        hashedLines.begin(),
        hashedLines.end(),
        result.begin(),
        [](std::future<std::string>& line) {return line.get();}
    );
    return result;
}

void printResult(const std::vector<std::string>& result) {
    std::ofstream file("output.txt");
    for(auto& line : result) {
        file << line << " " << std::endl;
    }
}

bool isPrime(const uint32_t& n) {
    if (n<=1) {
        return false;
    } else if (n<=3) {
        return true;
    } else if (n%2==0||n%3==0) {
        return false;
    }
    int i=5;
    while(i*i < n) {
        if (n%i==0||n%(i+2)==0)
            return false;
        i=i+6;
    }
    return true;
}

uint32_t hash(const char& letter) {
    uint32_t value = CODE;
    value <<= (letter % 2 == 0)?6:11;
    value ^= letter & FF;
    return isPrime(value)?value|MASK:value&MASK;
}

uint32_t hashWord(const std::string& word) {
    return
        std::accumulate(
            word.begin(),
            word.end(),
            0,
            [](uint32_t state, char value) {return state + hash(value);}
        );
}

std::string hashLine(const std::string& line) {
    std::stringstream words(line);
    std::stringstream output;
    std::string word;
    while(words >> word) {
        output << hashWord(word) << " ";
    }
    return output.str();
}
