#include <vector>
#include <future>
#include "Picture.hpp"
#include "lib/threadPool.hpp"

std::vector<std::vector<double>> GenerateGaussianKernel(int kernelRadius, double variance) {
    double sum = 0.0;

    std::vector<std::vector<double> > GaussianKernel(
            std::vector<std::vector<double>>(static_cast<unsigned long>(2 * kernelRadius + 1),
                                             std::vector<double>(2 * kernelRadius + 1, 0)));
    for (int i = 0; i <= kernelRadius * 2; ++i) {
        for (int j = 0; j <= kernelRadius * 2; ++j) {
            double r = (i - kernelRadius) * (i - kernelRadius) + (j - kernelRadius) * (j - kernelRadius);
            GaussianKernel[i][j] = (exp(-(r / (2 * variance * variance)))) / (M_PI * 2 * variance * variance);
            sum += GaussianKernel[i][j];
        }
    }
    for (auto &i : GaussianKernel)
        for (auto &j : i)
            j /= sum;

    return GaussianKernel;
}


void GaussianBlurThreads(Picture &img, int kernelRadius) {
    double sigma = 2.0;
    std::vector<std::vector<double> > GaussianKernel = GenerateGaussianKernel(kernelRadius, sigma);
    std::vector<std::vector<std::future<std::tuple<unsigned char, unsigned char, unsigned char> > > > newMatrixValues;
    ThreadPool threadPool(8);
    for (int i = 0; i < img.getHeight(); ++i) {
        newMatrixValues.emplace_back();
        for (int j = 0; j < img.getWidth(); ++j) {
            newMatrixValues[i].emplace_back(threadPool.enqueue([i, j, &kernelRadius, &GaussianKernel, &img] {
                double new_red = 0;
                double new_green = 0;
                double new_blue = 0;

                for (int x = -kernelRadius; x <= kernelRadius; ++x) {
                    for (int y = -kernelRadius; y <= kernelRadius; ++y) {

                        int aux_i = i + x;
                        int aux_j = j + y;
                        if (aux_i < 0 || aux_i >= img.getHeight() || aux_j < 0 || aux_j >= img.getWidth())
                            continue;
                        new_red += ((img(aux_i, aux_j).getRed())) * GaussianKernel[x + kernelRadius][y + kernelRadius];
                        new_green += ((img(aux_i, aux_j).getGreen())) * GaussianKernel[x + kernelRadius][y + kernelRadius];
                        new_blue += ((img(aux_i, aux_j).getBlue())) * GaussianKernel[x + kernelRadius][y + kernelRadius];
                    }
                }
                auto red_uc = static_cast<unsigned char>(new_red);
                auto green_uc = static_cast<unsigned char>(new_green);
                auto blue_uc = static_cast<unsigned char>(new_blue);
                return std::make_tuple(red_uc, green_uc, blue_uc);
            }));
        }
    }
    for (int i = 0; i < img.getHeight(); i++)
        for (int j = 0; j < img.getWidth(); j++) {
            auto result = newMatrixValues[i][j].get();
            img(i, j).setColors(std::get<0>(result), std::get<1>(result), std::get<2>(result));
        }
}