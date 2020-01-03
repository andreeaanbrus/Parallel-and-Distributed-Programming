#include <iostream>
#include <cmath>
#include <vector>
#include <tuple>
#include <chrono>
#include "GaussianBlur.hpp"


void gaussianBlur(Picture &img, int kernelRadius);

int main() {
//    Picture p1("galeata.jpg");
//    std::cout << "The linear algorithm\n";
//    auto startLinear = std::chrono::high_resolution_clock::now();
//    gaussianBlur(p1, 10);
//    auto finishLinear = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> timeLinear = finishLinear- startLinear;
//    std::cout << timeLinear.count() << '\n';
//    p1.write("out2.jpg");

    std::cout << "The parallel algorithm\n";

    auto startParallel = std::chrono::high_resolution_clock::now();
    Picture p2("galeata.jpg");
    GaussianBlurThreads(p2, 100);

    auto finishParallel = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> timeParallel = finishParallel- startParallel;
    std::cout << timeParallel.count() << '\n';
//    std::cout << timeLinear.count() << '\n';
    p2.write("out3.jpg");
}

void gaussianBlur(Picture &img, int kernelRadius) {
    double sigma = 2.0;
    std::vector<std::vector<double> > GaussianKernel = GenerateGaussianKernel(kernelRadius, sigma);

    std::vector<std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>> newMatrix(img.getHeight(),
                                                                                                std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>(
                                                                                                        img.getWidth(),
                                                                                                        {0, 0, 0}));

    // we have the kernel function
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