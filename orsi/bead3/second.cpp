#include <future>
#include <vector>

#include "model.hpp"
#include "pvm3.h"

Color matchColor(const Color& color);

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
        pvm_upkint(packed.data, packed.size * packed.size * 3, 1);
        pvm_upkint(packed.rows, packed.size * packed.size, 1);
        pvm_upkint(packed.cols, packed.size * packed.size, 1);

        Image img(packed);
        int size = img.getSize();

        std::vector<std::vector<std::future<Color>>> newColorsCalc(size);
        for(int i = 0; i < size; ++i) {
            newColorsCalc.at(i).resize(size);
            for(int j = 0; j < size; ++j) {
                newColorsCalc.at(i).at(j) = std::async(std::launch::async, matchColor, img(i, j);
            }
        }

        Image result(size);
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; ++j) {
                result(i, j) = newColorsCalc.at(i).at(j).get();
            }
        }

        pvm_initsend(PvmDataDefault);

        PackedImage packedResult = result.pack();
        pvm_pkint(&packed.size, 1, 1);
        pvm_pkint(packed.data, packed.size * packed.size * 3, 1);
        pvm_pkint(packed.rows, packed.size * packed.size, 1);
        pvm_pkint(packed.cols, packed.size * packed.size, 1);
        pvm_send(tid[2], 0);
    }

    pvm_exit();
    return 0;
}

Color matchColor(const Color& color) {
    Color out;
    out.R = (color.R < 128)?0:255;
    out.G = (color.G < 128)?0:255;
    out.B = (color.B < 128)?0:255;
    return out;
}