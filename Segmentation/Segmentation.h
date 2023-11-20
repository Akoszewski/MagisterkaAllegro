#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "KMeans.h"
#include "Thresholding.h"
#include "../Mask.h"

#define MAX_COLORED_PIXELS 1000
#define MAX_MASK_COLORS 10

enum class FilterType
{
    Average,
    Median,
    Gaussian
};

class Segmentation
{
public:
    Segmentation();
    ~Segmentation();
    void Init(std::shared_ptr<Image> img);
    void Draw();
    void RunStep();
    std::unique_ptr<Image> filterImage(const Image& orygImage, int windowWidth, int windowHeight, FilterType filterType);
    std::vector<Mask> masks;
private:
    std::shared_ptr<Image> orygImage;
    std::shared_ptr<Image> layerVisualization;
    std::vector<std::unique_ptr<SegmentationStrategy>> strategies;
    int step;
    ALLEGRO_COLOR chooseLayerForMorphoology(const Mask& mask);
    bool areColorsEqual(ALLEGRO_COLOR color1, ALLEGRO_COLOR color2);

    // Pixel pixels[MAX_COLORED_PIXELS];
};

#endif
