#ifndef KMEANS_H
#define KMEANS_H

#include "SegmentationStrategy.h"

class KMeans : public SegmentationStrategy
{
public:
    KMeans(int K);
    ~KMeans();
    void Init(const std::vector<ALLEGRO_COLOR>& maskColors);
    int RunStep(std::shared_ptr<const Image> orygImage, const Mask& mask, int param);
private:
    int K;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<int> clusterMeans;

    int getClusterFromColor(ALLEGRO_COLOR color);
};

#endif
