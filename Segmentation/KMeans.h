#ifndef KMEANS_H
#define KMEANS_H

#include "SegmentationStrategy.h"
#include "KMeans.h"

enum class CentroidType
{
    Random,
    Equalized,
    Custom
};

class KMeans : public SegmentationStrategy
{
public:
    KMeans(int K, CentroidType centroidType = CentroidType::Equalized, const std::vector<int>& initialCentroids = {});
    ~KMeans();
    void RunStep(const Image& orygImage, const Mask& mask);
protected:
    int K;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<int> centroids;
    std::vector<int> previousCentroids;

    int getClusterFromColor(ALLEGRO_COLOR color);
    std::vector<int> getRandomCentroids() const;
    std::vector<int> getEqualizedCentroids() const;
};

#endif
