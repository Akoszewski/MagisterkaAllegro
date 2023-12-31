#ifndef SEGMENTATION_STRATEGY
#define SEGMENTATION_STRATEGY

#include "../Mask.h"
#include "Pixel.h"
#include "Utils.h"

#include <vector>
#include <memory>

struct DimensionWeights
{
    double xWeight;
    double yWeight;
    double intensityWeight;
};

struct DataPoint3D
{
    double x;
    double y;
    double intensity;
    DataPoint3D(double x, double y, double intensity)
      : x(x), y(y), intensity(intensity)
    {}
    inline int operator==(const DataPoint3D& dp) const
    {
        return this->intensity == dp.intensity && this->x == dp.x && this->y == dp.y;
    }
};

constexpr int MAX_NORMALIZED_VALUE = 1000;

class SegmentationStrategy
{
public:
    SegmentationStrategy() = default;
    void Init(const std::vector<ALLEGRO_COLOR>& maskColors)
    {
        this->maskColors = maskColors;
    }
    virtual void RunStep(const Image& orygImage, const Mask& mask) = 0;
    virtual ~SegmentationStrategy() {}
    bool segmentationFinished = false;
    int lastClusterCount = 0;
protected:
    int step = 0;
    std::vector<ALLEGRO_COLOR> maskColors;
};

#endif
