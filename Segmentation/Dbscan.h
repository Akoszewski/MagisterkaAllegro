#ifndef DBSCAN_H
#define DBSCAN_H

#include "SegmentationStrategy.h"

class Dbscan : public SegmentationStrategy
{
public:
    Dbscan(DataPoint3D maxDataPoint, DimensionWeights dimentionWeights = {1.0, 1.0, 1.0});
    ~Dbscan() = default;
    void RunStep(const Image& orygImage, const Mask& mask);
private:
    DataPoint3D maxDataPoint;
    DimensionWeights dimensionWeights;
};

#endif
