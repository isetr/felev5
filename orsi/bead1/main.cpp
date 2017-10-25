#include <fstream>
#include <vector>
#include <future>
#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>

static const uint64_t CODE = 0x666;
static const uint64_t FF = 0xFF;
static const uint64_t MASK = 0x12345678;

std::vector<std::string> readLines(const std::string& filename);
std::vector<std::future<std::string>> hashLines(const std::vector<std::string>& lines);
std::vector<std::string> process(const std::vector<std::future<std::string>>& hashedLines);
void printResult(const std::vector<std::string>& result);

bool isPrime(const uint64_t& n);
uint64_t hash(const char& letter);
uint64_t hashWord(const std::string& word);
std::string hashLine(const std::string& line);

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

std::vector<std::string> readLines(const std::string& filename) {
    std::vector<std::string> out;
    std::ifstream file(filename);
    int size;
    
    file >> size;
    out.reserve(size);

    std::string line;
    while(std::getline(file, line)) {
        out.push_back(line);
    }

    return out;
}

std::vector<std::future<std::string>> hashLines(const std::vector<std::string>& lines) {
    std::vector<std::future<std::string>> result(lines.size());
    std::transform(
        lines.begin(),
        lines.end(),
        result.begin(),
        [](const std::string& line) {return std::async(std::launch::async, hashLine, line);}
    );
    return result;
}

std::vector<std::string> process(const std::vector<std::future<std::string>>& hashedLines) {
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

bool isPrime(const uint64_t& n) {
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

uint64_t hash(const char& letter) {
    uint64_t value = CODE;
    value <<= (letter % 2 == 0)?6:11;
    value ^= FF;
    return isPrime(value)?value|MASK:value&MASK;
}

uint64_t hashWord(const std::string& word) {
    return
        std::accumulate(
            word.begin(),
            word.end(),
            0,
            [](uint64_t state, char value) {return state + hash(value);}
        );
}

std::string hashLine(const std::string& line) {
    std::vector<std::string> words;
    std::string word;
    while(std::getline(line, word, ' ')) {
        words.push_back(word);
    }
    return
        std::accumulate(
            words.begin(),
            words.end(),
            "",
            [](std::string& state, const std::string& value){return state + " " + hashWord(value);}
        );
}
