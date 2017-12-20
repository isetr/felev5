#include <future>

#include "model.hpp"
#include "pvm3.h"

std::vector<int> calcRows(int i, const Image& img);
std::vector<int> calcCols(int i, const Image& img);

int main(int argc, char** argv) {
    int ptid = pvm_parent();
    int tid[3];
    int imagesCount;

    pvm_recv(ptid, 0);
    pvm_upkint(tid, 3, 1);
    pvm_upkint(&imagesCount, 1, 1);

    for(int i = 0; i < imagesCount; ++i) {        
        pvm_recv(tid[1], 0);
        PackedImage packed;
        pvm_upkint(&packed.size, 1, 1);
        pvm_upkint(packed.data, packed.size * packed.size * 3, 1);
        pvm_upkint(packed.rows, packed.size * packed.size, 1);
        pvm_upkint(packed.cols, packed.size * packed.size, 1);

        Image img(packed);
        int size = img.getSize();

        std::vector<std::future<std::vector<int>>> rows(size);
        std::vector<std::future<std::vector<int>>> cols(size);
        for(int i = 0; i < size; ++i) {
            rows.at(i).at(j) = std::async(std::launch::async, calcRows, i, img);
            cols.at(i).at(j) = std::async(std::launch::async, calcCols, i, img);
        }

        Image result(size);
        for(int i = 0; i < size; ++i) {
            std::vector<int> r = rows.at(i).get();
            std::vector<int> c = cols.at(i).get();
            for(int j = 0; j < size; ++j) {
                img.row(i, j) = r.at(j);
                img.col(i, j) = c.at(j);
            }
        }

        pvm_initsend(PvmDataDefault);

        PackedImage packedResult = result.pack();
        pvm_pkint(&packed.size, 1, 1);
        pvm_pkint(packed.data, packed.size * packed.size * 3, 1);
        pvm_pkint(packed.rows, packed.size * packed.size, 1);
        pvm_pkint(packed.cols, packed.size * packed.size, 1);
        pvm_send(ptid, 0);
    }

    pvm_exit();
    return 0;
}

std::vector<int> calcRows(int i, const Image& img) {
    std::vector<int> out;
    int s = 1;
    for(int j = 0; j < img.getSize(); ++j) {
        if(j == img.getSize() - 1) {
            out.push_back(s);
        } else if(img[i, j] == img[i, j + 1]) {
            ++s;
        } else {
            out.push_back(s);
        }
    }
    for(int j = 0; j < out.size() - img.getSize(); ++j) out.push_back(0);
    return out;
}

std::vector<int> calcCols(int i, const Image& img) {
    std::vector<int> out;
    int s = 1;
    for(int j = 0; j < img.getSize() - 1; ++j) {
        if(j == img.getSize() - 1) {
            out.push_back(s);
        } else if(img[j, i] == img[j + 1, i]) {
            ++s;
        } else {
            out.push_back(s);
        }
    }
    for(int j = 0; j < out.size() - img.getSize(); ++j) out.push_back(0);
    return out;
}