#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <chrono>

#include "model.hpp"
#include "pvm3.h"

std::vector<Image> read(std::string path);
void write(std::string path, const std::vector<Image>& result);

int main(int argc, char** argv) {
    int scale = atoi(argv[1]);
    std::vector<Image> images = read(argv[2]);
    int imagesCount = images.size();
    std::vector<Image> result;
    
    int tid[3];

    auto start = std::chrono::steady_clock::now();
    if(pvm_spawn(const_cast<char*>("first"), (char**)0, 0, nullptr, 1, &tid[0]) < 1) return -1;
    if(pvm_spawn(const_cast<char*>("second"), (char**)0, 0, nullptr, 1, &tid[1]) < 1) return -1;
    if(pvm_spawn(const_cast<char*>("third"), (char**)0, 0, nullptr, 1, &tid[2]) < 1) return -1;

    for(int i = 0; i < 3; ++i) {
        pvm_initsend(PvmDataDefault);
        pvm_pkint(tid, 3, 1);
        pvm_pkint(&imagesCount, 1, 1);
        pvm_send(tid[i], 0); 
    }  

    pvm_initsend(PvmDataDefault);
    pvm_pkint(&scale, 1, 1);
    pvm_send(tid[0], 1);
    for(size_t i = 0; i < images.size(); ++i) {
        pvm_initsend(PvmDataDefault);
        PackedImage packed = images.at(i).pack();
        pvm_pkint(&packed.size, 1, 1);
        for(int i = 0; i < packed.size; ++i) {
            pvm_pkint(packed.data[i], packed.size * 3, 1);
        }
        for(int i = 0; i < packed.size; ++i) {
            pvm_pkint(packed.rows[i], packed.size, 1);
        }
        for(int i = 0; i < packed.size; ++i) {
            pvm_pkint(packed.cols[i], packed.size, 1);
        }
        pvm_send(tid[0], i+2);
    }

    for(size_t i = 0; i < images.size(); ++i) {
        pvm_recv(tid[2], i+2);
        PackedImage packed; 
        pvm_upkint(&packed.size, 1, 1);
        for(int i = 0; i < packed.size; ++i) {
            pvm_upkint(packed.data[i], packed.size * 3, 1); 
        }
        for(int j = 0; j < packed.size; ++j) {
            pvm_upkint(packed.rows[j], packed.size, 1);
        }
        for(int j = 0; j < packed.size; ++j) {
            pvm_upkint(packed.cols[j], packed.size, 1);
        }
        result.push_back(Image(packed));
    }

    write(argv[3], result);

    auto end = std::chrono::steady_clock::now();
    std::cerr << "Time elapsed: " << std::chrono::duration<double>(end - start).count() << " sec" << std::endl;
    pvm_exit();
    return 0;
}

std::vector<Image> read(std::string path) {
    std::ifstream file(path);
    int size;
    file >> size;

    std::vector<Image> out;
    for(int i = 0; i < size; ++i) {
        Image temp; 
        file >> temp;
        out.push_back(temp);
    }

    return out;
}

void write(std::string path, const std::vector<Image>& result) {
    std::ofstream file(path);
    for(auto img : result) {
        file << img;
    }
}