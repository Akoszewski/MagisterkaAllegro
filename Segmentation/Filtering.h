#ifndef FILTERING_H
#define FILTERING_H

#include <vector>

std::vector<float> generateGaussianKernel(int kernelWidth, int kernelHeight, float sigmaX, float sigmaY);

#endif
