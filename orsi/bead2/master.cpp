#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <chrono>

#include "pvm3.h"

std::vector<int> readFile(std::string path, int& count);
void writeFile(std::string path, int result);

int main(int argc, char** argv) {
    int sum = atoi(argv[1]);
    int setsize = 0;
    std::vector<int> set = readFile(argv[2], setsize);
    int result = 0;
    
    auto start = std::chrono::steady_clock::now();
    if(setsize == 0 || sum == 0) {
        writeFile(argv[3], sum == 0);
    } else {
        int tid;
        if(pvm_spawn(const_cast<char*>("child"), (char**)0, 0, nullptr, 1, &tid) < 1) return -1;
        
        pvm_initsend(PvmDataDefault);
        pvm_pkint(&setsize, 1, 1);

        int send[setsize];
        for(int i = 0; i < setsize; ++i) {
            send[i] = set.at(i);
        }
        pvm_pkint(send, setsize, 1);
        pvm_pkint(&sum, 1, 1);
        pvm_send(tid, 0);

        pvm_recv(tid, 0);

        pvm_upkint(&result, 1, 1);

        writeFile(argv[3], result);
    }
    auto end = std::chrono::steady_clock::now();
    std::cerr << "Time elapsed: " << std::chrono::duration<double>(end - start).count() << " sec" << std::endl;
    pvm_exit();
    return 0;
}

std::vector<int> readFile(std::string path, int& count) {
    std::ifstream file(path);
    file >> count;
    std::vector<int> out;
    out.resize(count);
    for(int i = 0; i < count; ++i) {
        file >> out[i];
    }
    return out;
}

void writeFile(std::string path, int result) {
    std::ofstream file(path);

    if(result) {
        file << "May the subset be with You!";
    } else {
        file << "I find your lack of subset disturbing!";
    }

    file.close();
}