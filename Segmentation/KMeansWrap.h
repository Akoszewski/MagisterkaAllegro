#ifndef KMEANS_WRAP_H
#define KMEANS_WRAP_H

#include "KMeans.h"

// Kmeans but every step is initializing new initial centroids and running steps of KMeans until it's finished
class KMeansWrap : public SegmentationStrategy
{
public:
    KMeansWrap(int K, CentroidType centroidType = CentroidType::Equalized, const std::vector<int>& initialCentroids = {});
    ~KMeansWrap();
    void RunStep(const Image& orygImage, const Mask& mask);
private:
    CentroidType centroidType;
    std::vector<int> initialCentroids;
    int K;
};

#endif
