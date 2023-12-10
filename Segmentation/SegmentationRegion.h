#ifndef SEGMENTATION_REGION
#define SEGMENTATION_REGION

#include "SegmentationStrategy.h"
#include "Mask.h"

#include <memory>

class SegmentationRegion
{
public:
    SegmentationRegion(Mask&& mask, std::unique_ptr<SegmentationStrategy> strategy, const std::vector<ALLEGRO_COLOR>& maskColors);
    Mask mask;
    std::unique_ptr<SegmentationStrategy> strategy;
    std::vector<Image> layerVisualizations;
    std::vector<ALLEGRO_COLOR> maskColors;
};

#endif
