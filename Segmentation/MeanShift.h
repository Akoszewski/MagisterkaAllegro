#ifndef MEAN_SHIFT_H
#define MEAN_SHIFT_H

#include "SegmentationStrategy.h"

class MeanShift : public SegmentationStrategy
{
public:
    MeanShift(int radius, int epsilon, DataPoint3D maxDataPoint, DimensionWeights dimentionWeights = {1.0, 1.0, 1.0});
    ~MeanShift() = default;
    void RunStep(const Image& orygImage, const Mask& mask);
private:
    DataPoint3D maxDataPoint;
    DimensionWeights dimensionWeights;
    int radius;
    int epsilon;
    std::vector<DataPoint3D> centroids;
    double calculateSquaredDistance(const DataPoint3D& point, const DataPoint3D& centroid) const;
    bool addCentroidIfUnique(const DataPoint3D& newPoint, double epsilon);
    int getMostSimilarCentroidIdx(DataPoint3D point, const std::vector<DataPoint3D>& centroids) const;
};

#endif
