#ifndef MEAN_SHIFT_H
#define MEAN_SHIFT_H

#include "SegmentationStrategy.h"

class MeanShift : public SegmentationStrategy
{
public:
    MeanShift(int radius, DataPoint3D maxDataPoint, DimensionWeights dimentionWeights = {1.0, 1.0, 1.0});
    ~MeanShift() = default;
    void RunStep(const Image& orygImage, const Mask& mask);
private:
    DataPoint3D maxDataPoint;
    DimensionWeights dimensionWeights;
    int radius;
};

#endif
