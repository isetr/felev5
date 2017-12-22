#include <future>
#include <vector>

#include "model.hpp"
#include "pvm3.h"

Color average(int x, int y, int newSize, Image img);

int main(int argc, char** argv) {
    int scale;
    int imagesCount;
    int tid[3];
    int ptid = pvm_parent();

    debug << "first: waiting for parent\n";
    debug.flush();

    pvm_recv(ptid, 0);

    pvm_upkint(tid, 3, 1);
    pvm_upkint(&imagesCount, 1, 1);

    debug << "first: got tid, count waiting\n";
    debug.flush();
    pvm_recv(ptid, 0);
    pvm_upkint(&scale, 1, 1);
    debug << "first: got scale\n";

    for(int i = 0; i < imagesCount; ++i) {
        debug << "first: waiting for image\n";
        debug.flush();
        pvm_recv(ptid, 0);
        debug << "first: got iamge\n";
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
        
        Image img(packed);
        int newSize = img.getSize() / (100 / scale);

        std::vector<std::vector<std::future<Color>>> newColorsCalc(newSize);
        for(int i = 0; i < newSize; ++i) {
            newColorsCalc.at(i).resize(newSize);
            for(int j = 0; j < newSize; ++j) {
                newColorsCalc.at(i).at(j) = std::async(std::launch::async, average, i, j, (100/scale), img);
            }
        }

        Image result(newSize);
        for(int i = 0; i < newSize; ++i) {
            for(int j = 0; j < newSize; ++j) {
                result(i, j) = newColorsCalc.at(i).at(j).get();
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
        pvm_send(tid[1], 0);
    }
    pvm_exit();
    return 0;
}

Color average(int x, int y, int scale, Image img) {
    Color out;
    for(int i = x; i < x + scale; ++i) {
        for(int j = y; j < y + scale; ++j) {
            out.R += img(i, j).R;
            out.G += img(i, j).G;
            out.B += img(i, j).B;
        }
    }
    out.R /= scale;
    out.G /= scale;
    out.B /= scale;
    return out;
}