#include <future>
#include <vector>
#include <cmath>

#include "model.hpp"
#include "pvm3.h"

Color average(int x, int y, float scale, const std::vector<std::vector<Color>>& img);
std::vector<std::vector<Color>> scaleImage(const std::vector<std::vector<Color>>& img, int scale, int level, int x, int y);

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

        Image result;

        if(scale != 100) {
            int newSize = img.getSize() / (100 / scale);

            std::vector<std::vector<Color>> imgdata;
            imgdata.resize(img.getSize());
            for(int i = 0; i < img.getSize(); ++i) {
                imgdata.at(i).resize(img.getSize());
                for(int j = 0; j < img.getSize(); ++j) {
                    imgdata.at(i).at(j) = img(i,j);
                }
            }

            std::future<std::vector<std::vector<Color>>> calcScaledImage = std::async(std::launch::async, scaleImage, std::ref(imgdata), scale, 0, 0, 0);
            std::vector<std::vector<Color>> newimgdata = calcScaledImage.get();

            result = Image(newSize);
            for(int i = 0; i < newSize; ++i) {
                for(int j = 0; j < newSize; ++j) {
                    result(i, j) = newimgdata.at(i).at(j);
                }
            }
        } else {
            result = img;
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
        pvm_send(tid[1], i+2);
    }
    pvm_exit();
    return 0;
}

Color average(int x, int y, float scale, const std::vector<std::vector<Color>>& img) {
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
    out.R = std::floor(r / (float)(scale * scale));
    out.G = std::floor(g / (float)(scale * scale));
    out.B = std::floor(b / (float)(scale * scale)); 
    return out;
}

std::vector<std::vector<Color>> scaleImage(const std::vector<std::vector<Color>>& img, int scale, int level, int x, int y) {
    int size = img.size();
    int p = 100/scale;
    int newSize = size/p;

    if(level == 0) {
        std::vector<std::vector<Color>> out;

        out.resize(newSize);
        for(size_t i = 0; i < out.size(); ++i) out.at(i).resize(newSize);

        if(newSize == 1) {
            out.at(0).at(0) = average(0, 0, p, img);
        } else {
            std::vector<std::vector<std::future<std::vector<std::vector<Color>>>>> quarters;
            quarters.resize(2);
            for(int i = 0; i < 2; ++i) {
                quarters.at(i).resize(2);
                for(int j = 0; j < 2; ++j) {
                    quarters.at(i).at(j) = ( 
                        std::async(std::launch::async, 
                            scaleImage, 
                            std::ref(img), 
                            scale, 
                            level + 1, 
                            i * (newSize / 2), 
                            j * (newSize / 2))
                        );
                }
            }

            for(size_t i = 0; i < 2; ++i) {
                for(size_t j = 0; j < 2; ++j) {
                    std::vector<std::vector<Color>> q = quarters.at(i).at(j).get();
                    for(size_t k = 0; k < q.size(); ++k) {
                        for(size_t l = 0; l < q.at(k).size(); ++l) {
                            out.at(i * (newSize / 2) + k).at(j * (newSize / 2) + l) = 
                                q.at(k).at(l);
                        }
                    }
                }
            }
        }

        return out;
    } else {
        std::vector<std::vector<Color>> out;

        out.resize(newSize / (2*level));
        for(size_t i = 0; i < out.size(); ++i) out.at(i).resize(newSize / (2*level));

        for(int i = 0; i < newSize / (2*level); ++i) {
            for(int j = 0; j < newSize / (2*level); ++j) {
                out.at(i).at(j) = average(i + x, j + y, p, img);
            }
        }
        return out;
    }
}