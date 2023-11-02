#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "KMeans.h"
#include "Thresholding.h"

#define MAX_COLORED_PIXELS 1000
#define MAX_MASK_COLORS 10

class Segmentation
{
public:
    Segmentation();
    ~Segmentation();
    void Init(std::shared_ptr<Image> img);
    void SetStrategy(std::unique_ptr<SegmentationStrategy> strategy);
    void Draw();
    void NextStep();
private:
    std::shared_ptr<Image> orygImage;
    Image mask;
    Pixel pixels[MAX_COLORED_PIXELS];
    byte maskTransparency;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::unique_ptr<SegmentationStrategy> strategy{};

    void initMask(std::shared_ptr<Image> img, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent);
};

#endif
