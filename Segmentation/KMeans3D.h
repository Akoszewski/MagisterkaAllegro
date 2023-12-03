#ifndef KMeans3D_H
#define KMeans3D_H

#include "SegmentationStrategy.h"
#include "KMeans.h"

struct DimensionWeights
{
    double xWeight;
    double yWeight;
    double intensityWeight;
};

class KMeans3D : public SegmentationStrategy
{
public:
    KMeans3D(int K, DataPoint3D maxDataPoint, DimensionWeights dimentionWeights = {1.0, 1.0, 1.0}, CentroidType centroidType = CentroidType::Equalized, const std::vector<DataPoint3D>& initialCentroids = {});
    ~KMeans3D();
    void Init(const std::vector<ALLEGRO_COLOR>& maskColors);
    void RunStep(const Image& orygImage, const Mask& mask);
private:
    int K;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<DataPoint3D> centroids;
    std::vector<DataPoint3D> previousCentroids;
    DataPoint3D maxDataPoint;
    DimensionWeights dimensionWeights;

    int getClusterFromColor(ALLEGRO_COLOR color);
    std::vector<DataPoint3D> getRandomCentroids(DataPoint3D maxValues) const;
    std::vector<DataPoint3D> getEqualizedCentroids(DataPoint3D maxValues) const;
    int getMostSimilarCentroidIdx(DataPoint3D point, const std::vector<DataPoint3D>& centroids) const;
    double calculateDistance(DataPoint3D point, DataPoint3D centroid) const;
    void printCentroids() const;
};

#endif
