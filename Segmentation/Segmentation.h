#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "KMeans.h"
#include "Thresholding.h"
#include "../Mask.h"
#include "DisplayOptions.h"


enum class FilterType
{
    Average,
    Median,
    Gaussian
};

enum class StepOperation
{
    Filter,
    Segmentate,
    Dilate,
};

class Segmentation
{
public:
    Segmentation();
    ~Segmentation();
    void Init(std::shared_ptr<Image> img);
    void Draw();
    void RunStep(StepOperation operation);
    std::unique_ptr<Image> FilterImage(const Image& orygImage, int windowWidth, int windowHeight, FilterType filterType);
    void DrawSegmentLines();
    void DrawMaskVisualizations(const Mask& mask, int i);
    void PerformMorphOnMask(Mask& mask, int chosenLayerColor);
    std::vector<Mask> masks;
    DisplayOptions displayOptions;
private:
    std::shared_ptr<Image> orygImage;
    std::unique_ptr<Image> filteredImage;
    std::vector<Image> layerVisualizations;
    std::vector<std::unique_ptr<SegmentationStrategy>> strategies;
    int step;
    int chooseLayerForMorphoology(const Mask& mask);
};

#endif
