#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include "SegmentationStrategy.h"

class Thresholding : public SegmentationStrategy
{
public:
    Thresholding(int threshold);
    ~Thresholding();
    void Init(const std::vector<ALLEGRO_COLOR>& maskColors);
    void RunStep(std::shared_ptr<const Image> orygImage, const Mask& mask);
private:
    int threshold;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<int> clusterMeans;

    int getClusterFromColor(ALLEGRO_COLOR color);
};

#endif