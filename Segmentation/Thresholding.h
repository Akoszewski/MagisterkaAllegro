#ifndef THRESHOLDING_H
#define THRESHOLDING_H

#include "SegmentationStrategy.h"

class Thresholding : public SegmentationStrategy
{
public:
    Thresholding(int threshold);
    ~Thresholding();
    void Init(std::vector<ALLEGRO_COLOR> maskColors);
    void NextStep(std::shared_ptr<const Image> orygImage, const Image& mask);
private:
    int threshold;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<int> clusterMeans;

    int getClusterFromColor(ALLEGRO_COLOR color);
};

#endif
