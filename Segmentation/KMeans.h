#ifndef KMEANS_H
#define KMEANS_H

#include "SegmentationStrategy.h"

class KMeans : public SegmentationStrategy
{
public:
    KMeans(int K, const std::vector<int>& initialCentroids = {});
    ~KMeans();
    void Init(const std::vector<ALLEGRO_COLOR>& maskColors);
    void RunStep(const Image& orygImage, const Mask& mask);
private:
    int K;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<int> centroids;

    int getClusterFromColor(ALLEGRO_COLOR color);
};

#endif
