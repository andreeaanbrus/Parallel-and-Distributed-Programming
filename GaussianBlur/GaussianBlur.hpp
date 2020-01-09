#include <vector>
#include <future>
#include <mpi/mpi.h>
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
                        new_green +=
                                ((img(aux_i, aux_j).getGreen())) * GaussianKernel[x + kernelRadius][y + kernelRadius];
                        new_blue +=
                                ((img(aux_i, aux_j).getBlue())) * GaussianKernel[x + kernelRadius][y + kernelRadius];
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


void gaussianBlurSlave(int rank, Picture &img) {
    MPI_Status status;
    int begin, end, kernelRadius;
    MPI_Recv(&begin, 1, MPI_INT, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);
    MPI_Recv(&end, 1, MPI_INT, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);
    MPI_Recv(&kernelRadius, 1, MPI_INT, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);
    std::vector<std::vector<double> > GaussianKernel = GenerateGaussianKernel(kernelRadius, 2.0);
    std::vector<std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>> newMatrix(img.getHeight(),
                                                                                                std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>(
                                                                                                        img.getWidth(),
                                                                                                        {0, 0, 0}));

    // we have the kernel function
    for (int i = begin ; i < end; ++i) {
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
    for (int i = begin; i < end; ++i) {
        for (int j = 0; j < img.getWidth(); ++j) {
            int config[5] = {i, j, std::get<0>(newMatrix[i][j]), std::get<1>(newMatrix[i][j]), std::get<2>(newMatrix[i][j])};
            MPI_Send(config, 5, MPI_INT, 0, 123, MPI_COMM_WORLD);
//            img(i, j).setColors(std::get<0>(newMatrix[i][j]), std::get<1>(newMatrix[i][j]),
//                                std::get<2>(newMatrix[i][j]));
        }

    }
    img.write("out5654.jpg");

}

void GaussianBlurDistributed(Picture &img, int kernelRadius) {
    MPI_Init(0, 0);
    int rank;
    int nrProc;
    int begin = 0, end = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nrProc);
    int length = img.getHeight() / (nrProc - 1);
    if (rank == 0) {
//        master
        for (int i = 1; i < nrProc; i++) {
            begin = end;
            end = i == nrProc - 1 ? img.getHeight() : end + length;
            MPI_Send(&begin, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
            MPI_Send(&kernelRadius, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
        }
        int config[5];
//      config[0] is x pixel
//      config[1] is y pixel
//      config[2, 3,4] are the new color
        MPI_Status status;
        for (int i = 0; i < img.getHeight() * img.getWidth(); i++) {
            MPI_Recv(config, 5, MPI_INT, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);
            img(config[0], config[1]).setColors(config[2], config[3], config[4]);
        }
        img.write("distributed.jpg");

    } else {
        gaussianBlurSlave(rank, img);
    }
    MPI_Finalize();
}
