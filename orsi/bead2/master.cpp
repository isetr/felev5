#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

#include "pvm3.h"

std::vector<int> readFile(std::string path);
void writeFile(std::string path, bool result);

int main(int argc, char** argv) {
    int sum = atoi(argv[1]);
    std::vector<int> set = readFile(argv[2]);
    
    int tid;
    pvm_spawn(const_cast<char*>("child"), (char**)0, 0, const_cast<char*>(""), 1, tid);
    
    pvm_initsend(PvmDataDefault);
    pvm_pkint(set.size(), 1, 1);
    pvm_pkint(&set[0], set.size(), 1);
    pvm_pkint(sum, 1, 1);
    pvm_send(tid, 0);

    pvm_recv(tid, -1);

    int result;
    pvm_upkint(&result, 1, 1);

    writeFile(argv[3], result);

    pvm_exit();
    return 0;
}

std::vector<int> readFile(std::string path) {
    std::ifstream file(path);

    int count;
    file >> count;

    std::vector<int> out;
    out.reserve(count);

    int tmp;
    for(int i = 0; i < count; ++i) {
        file >> tmp;
        out.push_back(tmp);
    }
}

void writeFile(std::string path, bool result) {
    std::ofstream file(path);

    if(result) {
        file.write("May the subset be with you!");
    } else {
        file.write("I find your lack of subset disturbing!");
    }
}