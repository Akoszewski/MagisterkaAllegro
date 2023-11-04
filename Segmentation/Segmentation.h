#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "KMeans.h"
#include "Thresholding.h"
#include "../Mask.h"

#define MAX_COLORED_PIXELS 1000
#define MAX_MASK_COLORS 10

class Segmentation
{
public:
    Segmentation();
    ~Segmentation();
    void Init(std::shared_ptr<Image> img, std::unique_ptr<Mask> mask);
    void SetStrategy(std::unique_ptr<SegmentationStrategy> strategy);
    void Draw();
    void RunStep();
    std::unique_ptr<Mask> mask;

private:
    std::shared_ptr<Image> orygImage;
    std::unique_ptr<SegmentationStrategy> strategy;
    // Pixel pixels[MAX_COLORED_PIXELS];
};

#endif
