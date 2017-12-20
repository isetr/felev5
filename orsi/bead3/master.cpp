#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <chrono>

#include "model.hpp"
#include "pvm3.h"

std::vector<Image> read(std::string path);
void write(std::string path, std::vector<Images> result);

int main(int argc, char** argv) {
    int scale = atoi(argv[1]);
    std::vector<Image> images = read(argv[2]);
    int imagesCount = images.size();
    std::vector<Image> result;
    result.resize(images.size());
    int tid[3];

    auto start = std::chrono::steady_clock::now();
    if(pvm_spawn(const_cast<char*>("first"), char(**)0, 0, nullptr, 1, tid[0]) < 1) return -1;
    if(pvm_spawn(const_cast<char*>("second"), char(**)0, 0, nullptr, 1, tid[1]) < 1) return -1;
    if(pvm_spawn(const_cast<char*>("third"), char(**)0, 0, nullptr, 1, tid[2]) < 1) return -1;

    pvm_initsend(PvmDataDefault);
    pvm_pkint(tid, 3, 1);
    pvm_pkint(&imagesCount, 1, 1);
    pvm_send(-1, 0);

    pvm_pkint(&scale, 1, 1);  
    for(size_t i = 0; i < images.size(); ++i) {
        PackedImage packed = images.at(i).pack();
        pvm_pkint(&packed.size, 1, 1);
        pvm_pkint(packed.data, packed.size * packed.size * 3, 1);
        pvm_pkint(packed.rows, packed.size * packed.size, 1);
        pvm_pkint(packed.cols, packed.size * packed.size, 1);
        pvm_send(tid[0], 0);
    }

    for(size_t i = 0; i < images.size(); ++i) {
        pvm_recv(tid[2]);
        PackedImage packed;
        pvm_upkint(&packed.size, 1, 1);
        pvm_upkint(packed.data, packed.size * packed.size * 3, 1);
        pvm_upkint(packed.rows, packed.size * packed.size, 1);
        pvm_upkint(packed.cols, packed.size * packed.size, 1);
        result.at(i) = Image(packed);
    }

    write(argv[3], result);

    auto end = std::chrono::steady_clock::now();
    std::cerr << "Time elapsed: " << std::chrono::duration<double>(end - start).count() << " sec" << std::endl;
    pvm_exit();
    return 0;
}

std::vector<Image> read(std::string path) {
    std::ifstream file(path);
    std::vector<Image> out;
    int size;
    
    file >> size;
    out.resize(size);
    for(int i = 0; i < size; ++i) {
        file >> out.at(i);
    }

    return out;
}

void write(std::string path, std::vector<Images> result) {
    std::ofstream file(path);

    for(auto img : result) {
        file << img;
    }
}