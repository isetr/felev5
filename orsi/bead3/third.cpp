#include <future>

#include "model.hpp"
#include "pvm3.h"

std::vector<int> calcRows(int i, Image img);
std::vector<int> calcCols(int i, Image img);

int main(int argc, char** argv) {
    int ptid = pvm_parent();
    int tid[3];
    int imagesCount;

    pvm_recv(ptid, 0);
    pvm_upkint(tid, 3, 1);
    pvm_upkint(&imagesCount, 1, 1);   

    for(int i = 0; i < imagesCount; ++i) {    
        pvm_recv(tid[1], i+2);   
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

        Image img(packed);
        int size = img.getSize();

        dt << i << " image unpacked:\n" << img;
        dt.flush();

        std::vector<std::future<std::vector<int>>> rows;
        rows.resize(size);
        std::vector<std::future<std::vector<int>>> cols;
        cols.resize(size);
        for(int i = 0; i < size; ++i) {
            rows.at(i) = (std::async(std::launch::async, calcRows, i, img));
            cols.at(i) = (std::async(std::launch::async, calcCols, i, img));
        }

        for(int i = 0; i < size; ++i) {
            std::vector<int> r = rows.at(i).get();
            std::vector<int> c = cols.at(i).get();
            for(int j = 0; j < size; ++j) {
                img.row(i, j) = r.at(j);
                img.col(i, j) = c.at(j);
            }
        }

        pvm_initsend(PvmDataDefault);

        PackedImage packedResult = img.pack();

        dt << i << " image packed:\n" << img;
        dt.flush(); 

        pvm_pkint(&packedResult.size, 1, 1);
        for(int i = 0; i < packedResult.size; ++i) {
            pvm_pkint(packedResult.data[i], packedResult.size * 3, 1);
        }
        for(int i = 0; i < packedResult.size; ++i) {
            pvm_pkint(packedResult.rows[i], packedResult.size, 1);
        }
        for(int i = 0; i < packedResult.size; ++i) {
            pvm_pkint(packedResult.cols[i], packedResult.size, 1);
        }
        pvm_send(ptid, i+2);
    }

    pvm_exit();
    return 0;
}

std::vector<int> calcRows(int i, Image img) {
    std::vector<int> out;
    int s = 1;
    for(int j = 0; j < img.getSize(); ++j) {
        if(j == img.getSize() - 1) {
            out.push_back(s);
        } else if(img(i, j) == img(i, j + 1)) {
            ++s;
        } else {
            out.push_back(s);
        }
    }
    for(int j = 0; j < img.getSize() - out.size(); ++j) out.push_back(0);
    return out;
}

std::vector<int> calcCols(int i, Image img) {
    std::vector<int> out;
    int s = 1;
    for(int j = 0; j < img.getSize(); ++j) {
        if(j == img.getSize() - 1) {
            out.push_back(s);
        } else if(img(j, i) == img(j + 1, i)) {
            ++s;
        } else {
            out.push_back(s);
        }
    }
    for(size_t j = 0; j < img.getSize() - out.size(); ++j) out.push_back(0);
    return out;
}