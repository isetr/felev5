#include <future>
#include <vector>

#include "model.hpp"
#include "pvm3.h"

std::vector<Color> matchColor(int row, Image img);

int main(int argc, char** argv) {
    int ptid = pvm_parent();
    int tid[3];
    int imagesCount;

    pvm_recv(ptid, 0);
    pvm_upkint(tid, 3, 1);
    pvm_upkint(&imagesCount, 1, 1);

    for(int i = 0; i < imagesCount; ++i) {
        pvm_recv(tid[0], 0);
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

        std::vector<std::future<std::vector<Color>>> newColorsCalc;
        newColorsCalc.resize(size);
        for(int i = 0; i < size; ++i) {
            newColorsCalc.at(i) = std::async(std::launch::async, matchColor, i, img);
        }

        Image result(size);
        for(int i = 0; i < size; ++i) {
            std::vector<Color> tmp = newColorsCalc.at(i).get();
            for(int j = 0; j < size; ++j) {
                result(i, j) = tmp.at(j);
            }
        }

        pvm_initsend(PvmDataDefault);

        PackedImage packedResult = result.pack();
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
        pvm_send(tid[2], 0);
    }

    pvm_exit();
    return 0;
}

std::vector<Color> matchColor(int row, Image img) {
    std::vector<Color> out;
    for(int i = 0; i < img.getSize(); ++i) {
        Color color = img(row, i);
        Color tmp;
        tmp.R = (color.R < 128)?0:255;
        tmp.G = (color.G < 128)?0:255;
        tmp.B = (color.B < 128)?0:255;
        out.push_back(tmp);
    }
    return out;
}