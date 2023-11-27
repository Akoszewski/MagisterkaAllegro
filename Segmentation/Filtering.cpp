#include "Filtering.h"

#include <cmath>
#include <algorithm>


std::vector<float> generateGaussianKernel(int kernelWidth, int kernelHeight, float sigmaX, float sigmaY)
{
    std::vector<float> kernel(kernelWidth * kernelHeight);
    float centerX = kernelWidth / 2.0f;
    float centerY = kernelHeight / 2.0f;
    float sum = 0.0f;

    int i = 0;
    for (int x = 0; x < kernelWidth; x++)
    {
        for (int y = 0; y < kernelHeight; y++)
        {
            kernel[i] = exp(-0.5 * (pow((x - centerX) / sigmaX, 2.0) + pow((y - centerY) / sigmaY, 2.0))) / (2 * M_PI * sigmaX * sigmaY);
            sum += kernel[i++];
        }
    }

    // Normalize the kernel
    std::for_each(kernel.begin(), kernel.end(), [sum](float& val) { val /= sum; });
    return kernel;
}
