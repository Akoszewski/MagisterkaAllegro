#ifndef SEGMENTATION_STRATEGY
#define SEGMENTATION_STRATEGY

#include "../Mask.h"
#include "Pixel.h"
#include "Utils.h"

#include <vector>
#include <memory>

struct DataPoint3D
{
    int x;
    int y;
    int intensity;
    DataPoint3D(int x, int y, int intensity)
      : x(x), y(y), intensity(intensity)
    {}
    inline int operator==(const DataPoint3D& dp) const
    {
        return this->intensity == dp.intensity && this->x == dp.x && this->y == dp.y;
    }
};

class SegmentationStrategy
{
public:
    SegmentationStrategy() = default;
    virtual void Init(const std::vector<ALLEGRO_COLOR>& maskColors) = 0;
    virtual void RunStep(const Image& orygImage, const Mask& mask) = 0;
    virtual ~SegmentationStrategy() {}
    bool segmentationFinished = false;
protected:
    int step = 0;
};

#endif
