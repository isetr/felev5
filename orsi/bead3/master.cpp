#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <chrono>

#include "model.hpp"
#include "pvm3.h"

std::vector<Image> read(std::string path);
void write(std::string path, std::vector<Image> result);

int main(int argc, char** argv) {
    debug << "master: start\n";
    debug.flush();

    int scale = atoi(argv[1]);
    std::vector<Image> images = read(argv[2]);
    int imagesCount = images.size();
    std::vector<Image> result;

    debug << "master: images size is " << images.size() << "\n";
    debug.flush();

    result.resize(images.size());
    int tid[3];

    debug << "master: init variables\n";
    debug.flush();

    auto start = std::chrono::steady_clock::now();
    if(pvm_spawn(const_cast<char*>("first"), (char**)0, 0, nullptr, 1, &tid[0]) < 1) return -1;
    if(pvm_spawn(const_cast<char*>("second"), (char**)0, 0, nullptr, 1, &tid[1]) < 1) return -1;
    if(pvm_spawn(const_cast<char*>("third"), (char**)0, 0, nullptr, 1, &tid[2]) < 1) return -1;

    debug << "master: 3 children spawned\n";
    debug.flush();

    pvm_initsend(PvmDataDefault);
    pvm_pkint(tid, 3, 1);
    pvm_pkint(&imagesCount, 1, 1);
    pvm_send(tid[0], 0);
    pvm_send(tid[1], 0);
    pvm_send(tid[2], 0);

    debug << "master: tids and count sent to everyone\n";
    debug.flush();

    pvm_initsend(PvmDataDefault);
    pvm_pkint(&scale, 1, 1);
    for(size_t i = 0; i < images.size(); ++i) {
        pvm_initsend(PvmDataDefault);
        debug << "master: packing image #" << i << "\n";
        debug.flush();
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
        pvm_send(tid[0], 0);
        debug << "master: image #" << i << " sent to first\n";
        debug.flush();
    }

    debug << "master: waiting for a processed picture\n";
    debug.flush();

    for(size_t i = 0; i < images.size(); ++i) {
        debug << "master: waiting for image #" << i << "\n";
        debug.flush();
        pvm_recv(tid[2], 0);
        debug << "master: got image #" << i << "\n";
        debug.flush();
        PackedImage packed;
        pvm_upkint(&packed.size, 1, 1);
        for(int i = 0; i < packed.size; ++i) {
            pvm_upkint(packed.data[i], packed.size * 3, 1);
        }
        for(int i = 0; i < packed.size; ++i) {
            pvm_upkint(packed.rows[i], packed.size, 1);
        }
        for(int i = 0; i < packed.size; ++i) {
            pvm_upkint(packed.cols[i], packed.size, 1);
        }
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
    int size;
    file >> size;

    debug << "master-read: size " << size << "\n";
    debug.flush();
    
    std::vector<Image> out;
    for(int i = 0; i < size; ++i) {
        debug << "master-read: " << i << "\n";
        debug.flush();
        Image temp;
        file >> temp;
        out.push_back(temp);

    }

    return out;
}

void write(std::string path, std::vector<Image> result) {
    std::ofstream file(path);
    for(auto img : result) {
        file << img;
    }
}