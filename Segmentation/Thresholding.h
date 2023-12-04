#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include "SegmentationStrategy.h"

class Thresholding : public SegmentationStrategy
{
public:
    Thresholding(int threshold);
    ~Thresholding();
    void RunStep(const Image& orygImage, const Mask& mask);
private:
    int threshold;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<int> clusterMeans;

    int getClusterFromColor(ALLEGRO_COLOR color);
};

#endif
