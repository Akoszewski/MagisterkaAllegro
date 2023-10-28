#ifndef SEGMENTATION_STRATEGY
#define SEGMENTATION_STRATEGY

#include "Image.h"
#include "Pixel.h"

#include <vector>
#include <memory>

typedef unsigned char byte;

class SegmentationStrategy
{
public:
    SegmentationStrategy() = default;
    virtual void Init(std::vector<ALLEGRO_COLOR> maskColors) = 0;
    virtual void NextStep(std::shared_ptr<const Image> orygImage, const Image& mask) = 0;
    virtual ~SegmentationStrategy() {}
protected:
    int step = 0;
};

#endif
