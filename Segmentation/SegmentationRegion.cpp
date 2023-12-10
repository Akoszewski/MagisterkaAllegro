#include "SegmentationRegion.h"

SegmentationRegion::SegmentationRegion(Mask&& mask, std::unique_ptr<SegmentationStrategy> strategy, const std::vector<ALLEGRO_COLOR>& maskColors)
  : mask(std::move(mask))
  , strategy(std::move(strategy))
  , maskColors(maskColors)
{
    this->strategy->Init(maskColors);
    this->mask.maskColors = maskColors;
}
