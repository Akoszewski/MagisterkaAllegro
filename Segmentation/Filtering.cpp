#include "Filtering.h"

#include <cmath>


std::vector<std::vector<float>> generateGaussianKernel(int kernelWidth, int kernelHeight, float sigmaX, float sigmaY)
{
    std::vector<std::vector<float>> kernel(kernelWidth, std::vector<float>(kernelHeight));
    float centerX = kernelWidth / 2.0f;
    float centerY = kernelHeight / 2.0f;
    float sum = 0.0f;

    for (int x = 0; x < kernelWidth; ++x)
    {
        for (int y = 0; y < kernelHeight; ++y)
        {
            kernel[x][y] = exp(-0.5 * (pow((x - centerX) / sigmaX, 2.0) + pow((y - centerY) / sigmaY, 2.0))) / (2 * M_PI * sigmaX * sigmaY);
            sum += kernel[x][y];
        }
    }

    // Normalize the kernel
    for (int x = 0; x < kernelWidth; ++x) {
        for (int y = 0; y < kernelHeight; ++y) {
            kernel[x][y] /= sum;
        }
    }

    return kernel;
}


