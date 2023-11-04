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
    void Init(std::shared_ptr<Image> img);
    void Draw();
    void RunStep();
    std::vector<Mask> masks;

private:
    std::shared_ptr<Image> orygImage;
    std::vector<std::unique_ptr<SegmentationStrategy>> strategies;
    
    // Pixel pixels[MAX_COLORED_PIXELS];
};

#endif
