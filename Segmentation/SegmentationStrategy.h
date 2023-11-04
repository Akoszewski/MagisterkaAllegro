#ifndef SEGMENTATION_STRATEGY
#define SEGMENTATION_STRATEGY

#include "../Mask.h"
#include "Pixel.h"

#include <vector>
#include <memory>

class SegmentationStrategy
{
public:
    SegmentationStrategy() = default;
    virtual void Init(const std::vector<ALLEGRO_COLOR>& maskColors) = 0;
    virtual int RunStep(std::shared_ptr<const Image> orygImage, const Mask& mask, int param) = 0;
    virtual ~SegmentationStrategy() {}
protected:
    int step = 0;
};

#endif
