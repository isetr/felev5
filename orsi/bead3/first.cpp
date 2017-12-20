#include <future>
#include <vector>

#include "model.hpp"
#include "pvm3.h"

Color average(int i, int j, int newSize, Image img);

int main(int argc, char** argv) {
    int scale;
    int imagesCount;
    int tid[3];
    int ptid = pvm_parent();

    pvm_recv(ptid, 0);

    pvm_upkint(tid, 3, 1);
    pvm_upkint(&imagesCount, 1, 1);

    pvm_recv(ptid, 0);
    pvm_upkint(&scale, 1, 1);

    for(int i = 0; i < imagesCount; ++i) {
        pvm_recv(ptid, 0);
        PackedImage packed;
        pvm_upkint(&packed.size, 1, 1);
        pvm_upkint(packed.data, packed.size * packed.size * 3, 1);
        pvm_upkint(packed.rows, packed.size * packed.size, 1);
        pvm_upkint(packed.cols, packed.size * packed.size, 1);
        
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
        pvm_pkint(&packed.size, 1, 1);
        pvm_pkint(packed.data, packed.size * packed.size * 3, 1);
        pvm_pkint(packed.rows, packed.size * packed.size, 1);
        pvm_pkint(packed.cols, packed.size * packed.size, 1);
        pvm_send(tid[1], 0);
    }
    pvm_exit();
    return 0;
}

Color average(int i, int j, int scale, Image img) {
    Color out;
    for(int i = 0; i < scale; ++i) {
        for(int j = 0; j < scale; ++j) {
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