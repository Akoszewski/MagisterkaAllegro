#ifndef KMEANS_H
#define KMEANS_H

#include "SegmentationStrategy.h"

class KMeans : public SegmentationStrategy
{
public:
    KMeans(int K);
    ~KMeans();
    void Init(std::vector<ALLEGRO_COLOR> maskColors);
    void NextStep(std::shared_ptr<const Image> orygImage, const Mask& mask);
private:
    int K;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<int> clusterMeans;

    int getClusterFromColor(ALLEGRO_COLOR color);
};

#endif
