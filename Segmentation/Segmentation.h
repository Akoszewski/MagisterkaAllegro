#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "KMeans.h"
#include "Thresholding.h"
#include "KMeansWrap.h"
#include "KMeans3D.h"
#include "../Mask.h"
#include "DisplayOptions.h"
#include "SegmentationStrategy.h"
#include "SegmentationParams.h"


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
    Segmentation(std::string&& imagePath, SegmentationParams&& segmentationParams);
    ~Segmentation();
    void Init(std::string&& imagePath);
    void Draw();
    void RunStep(StepOperation operation);
    std::unique_ptr<Image> FilterImage(const Image& orygImage, int windowWidth, int windowHeight, FilterType filterType);
    void DrawSegmentLines();
    void InitLayerVisualizations(int clusters);
    void DrawLayerVisualizations(const Mask& mask, int i, int clusters);
    void PerformMorphOnMask(Mask& mask, int chosenLayerColor);
    std::vector<Mask> masks;
    DisplayMode displayMode = DisplayMode::Filtered;
    void ToggleImageDisplayMode();
    std::unique_ptr<Image> orygImage;
    std::unique_ptr<Image> filteredImage;
    std::vector<Image> layerVisualizations;
    std::vector<std::unique_ptr<SegmentationStrategy>> strategies;
private:
    int step;
    SegmentationParams segmentationParams;
    int chooseLayerForMorphoology(const Mask& mask);
};

#endif
