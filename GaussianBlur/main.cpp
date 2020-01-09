#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <cstring>
#include "GaussianBlur.hpp"


void gaussianBlur(Picture &img, int kernelRadius);

int main(int argc, char** argv) {
    int kernelRadius = 5;
    std::string filename = "galeata.jpg";
    Picture p1(filename);
    if (argc == 1) {
        std::cout << "The linear algorithm\n";
        auto startLinear = std::chrono::high_resolution_clock::now();
        gaussianBlur(p1, kernelRadius);
        auto finishLinear = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timeLinear = finishLinear- startLinear;
        std::cout << timeLinear.count() << '\n';
        p1.write("linear.jpg");

        std::cout << "The parallel algorithm\n";

        auto startParallel = std::chrono::high_resolution_clock::now();
        Picture p2(filename);
        GaussianBlurThreads(p2, kernelRadius);
        auto finishParallel = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timeParallel = finishParallel- startParallel;
        std::cout << timeParallel.count() << '\n';
        p2.write("parallel.jpg");
    }

    if (argc > 1){
        if (strcmp(argv[1], "distributed") == 0 ) {
            std::cout << "The distributed algorithm\n";
            Picture p3(filename);
            auto startDistributed = std::chrono::high_resolution_clock::now();
            GaussianBlurDistributed(p3, kernelRadius);
            auto finishtDistributed = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> distributedTime = finishtDistributed- startDistributed;
            std::cout << distributedTime.count() << '\n';
        }
    }
    return 0;
}

void gaussianBlur(Picture &img, int kernelRadius) {
    double sigma = 2.0;
    std::vector<std::vector<double> > GaussianKernel = GenerateGaussianKernel(kernelRadius, sigma);

    std::vector<std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>> newMatrix(img.getHeight(),
                                                                                                std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>(
                                                                                                        img.getWidth(),
                                                                                                        {0, 0, 0}));
    for (int i = 0; i < img.getHeight(); ++i) {
        for (int j = 0; j < img.getWidth(); ++j) {
            double new_red = 0;
            double new_green = 0;
            double new_blue = 0;

            for (int x = -kernelRadius; x <= kernelRadius; ++x) {
                for (int y = -kernelRadius; y <= kernelRadius; ++y) {

                    int aux_i = i + x;
                    int aux_j = j + y;
                    if (aux_i < 0 || aux_i >= img.getHeight() || aux_j < 0 || aux_j >= img.getWidth())
                        continue;
                    // we know that aux_i and aux_j are inside the matrix
                    new_red += ((img(aux_i, aux_j).getRed())) * GaussianKernel[x + kernelRadius][y + kernelRadius];
                    new_green +=
                            ((img(aux_i, aux_j).getGreen())) * GaussianKernel[x + kernelRadius][y + kernelRadius];
                    new_blue +=
                            ((img(aux_i, aux_j).getBlue())) * GaussianKernel[x + kernelRadius][y + kernelRadius];
                }
            }
            auto red_uc = static_cast<unsigned char>(new_red);
            auto green_uc = static_cast<unsigned char>(new_green);
            auto blue_uc = static_cast<unsigned char>(new_blue);

            std::get<0>(newMatrix[i][j]) = red_uc;
            std::get<1>(newMatrix[i][j]) = green_uc;
            std::get<2>(newMatrix[i][j]) = blue_uc;
        }
    }
    for (int i = 0; i < img.getHeight(); ++i) {
        for (int j = 0; j < img.getWidth(); ++j)
            img(i, j).setColors(std::get<0>(newMatrix[i][j]), std::get<1>(newMatrix[i][j]),
                                std::get<2>(newMatrix[i][j]));
    }
}