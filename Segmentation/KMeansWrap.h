#ifndef KMEANS_WRAP_H
#define KMEANS_WRAP_H

#include "SegmentationStrategy.h"

// Kmeans but every step is initializing new initial centroids and running steps of KMeans until it's finished
class KMeansWrap : public SegmentationStrategy
{
public:
    KMeansWrap(int K);
    ~KMeansWrap();
    void Init(const std::vector<ALLEGRO_COLOR>& maskColors);
    void RunStep(const Image& orygImage, const Mask& mask);
private:
    std::vector<ALLEGRO_COLOR> maskColors;
    int K;
};

#endif
