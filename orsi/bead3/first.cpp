#include <future>
#include <vector>
#include <cmath>

#include "model.hpp"
#include "pvm3.h"

Color average(int x, int y, float scale, std::vector<std::vector<Color>> img);
std::vector<std::vector<Color>> scaleImage(std::vector<std::vector<Color>> img, int scale, int level, int x, int y);

int main(int argc, char** argv) {
    int scale;
    int imagesCount;
    int tid[3];
    int ptid = pvm_parent();

    pvm_recv(ptid, 0);

    pvm_upkint(tid, 3, 1);
    pvm_upkint(&imagesCount, 1, 1);

    pvm_recv(ptid, 1);
    pvm_upkint(&scale, 1, 1);

    for(int i = 0; i < imagesCount; ++i) {
        pvm_recv(ptid, i+2);
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

        df << i << "image unpacked:\n" << img; 
        df.flush();

        int newSize = img.getSize() / (100 / scale);

        std::vector<std::vector<Color>> imgdata;
        imgdata.resize(img.getSize());
        for(int i = 0; i < img.getSize(); ++i) {
            imgdata.at(i).resize(img.getSize());
            for(int j = 0; j < img.getSize(); ++j) {
                imgdata.at(i).at(j) = img(i,j);
            }
        }

        std::future<std::vector<std::vector<Color>>> calcScaledImage = std::async(std::launch::async, scaleImage, imgdata, scale, 0, 0, 0);
        std::vector<std::vector<Color>> newimgdata = calcScaledImage.get();


        Image result(newSize);
        for(int i = 0; i < newSize; ++i) {
            for(int j = 0; j < newSize; ++j) {
                result(i, j) = newimgdata.at(i).at(j);
            }
        }

        pvm_initsend(PvmDataDefault);

        PackedImage packedResult = result.pack();

        df << i << " image result packed: \n" << result;
        df.flush();

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
        pvm_send(tid[1], i+2);
    }
    pvm_exit();
    return 0;
}

Color average(int x, int y, float scale, std::vector<std::vector<Color>> img) {
    Color out;
    float r = 0;
    float g = 0;
    float b = 0;
    for(int i = x*scale; i < x*scale + scale; ++i) {
        for(int j = y*scale; j < y*scale + scale; ++j) {
            r += img.at(i).at(j).R;
            g += img.at(i).at(j).G;
            b += img.at(i).at(j).B;
        }
    }
    out.R = std::ceil(r / (float)(scale * scale));
    out.G = std::ceil(g / (float)(scale * scale));
    out.B = std::ceil(b / (float)(scale * scale)); 
    return out;
}

std::vector<std::vector<Color>> scaleImage(std::vector<std::vector<Color>> img, int scale, int level, int x, int y) {
    int size = img.size();
    int p = 100/scale;
    int newSize = size/p;
    std::vector<std::vector<Color>> out;
    out.resize(newSize);

    for(int i = 0; i < newSize; ++i) {
        out.at(i).resize(newSize);
        for(int j = 0; j < newSize; ++j) {
            out.at(i).at(j) = average(i, j, p, img);
        }
    }
    return out;
}