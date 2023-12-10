#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "KMeans.h"
#include "Thresholding.h"
#include "KMeansWrap.h"
#include "KMeans3D.h"
#include "MeanShift.h"
#include "Dbscan.h"

#include "../Mask.h"
#include "DisplayOptions.h"
#include "SegmentationStrategy.h"
#include "SegmentationParams.h"
#include "SegmentationRegion.h"


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
    Segmentation(std::string&& imagePath, SegmentationParams&& segmentationParams, const std::vector<ALLEGRO_COLOR>& maskColors);
    ~Segmentation();
    void Init(std::string&& imagePath);
    void Draw();
    void RunStep(StepOperation operation);
    std::unique_ptr<Image> FilterImage(const Image& orygImage, int windowWidth, int windowHeight, FilterType filterType);
    void DrawSegmentLines();
    void ResetLayerVisualizations(SegmentationRegion& region, int clusters);
    void DrawLayerVisualizatons(const Mask& mask, int i, int clusters);
    void PerformMorphOnMask(Mask& mask, int chosenLayerColor);
    DisplayMode displayMode = DisplayMode::Filtered;
    void ToggleImageDisplayMode();
    std::unique_ptr<Image> orygImage;
    std::unique_ptr<Image> filteredImage;
    
    std::vector<SegmentationRegion> segmentationRegions;
    int currentVisualizedRegion{};
private:
    int step;
    SegmentationParams segmentationParams;
    int chooseLayerForMorphoology(const Mask& mask);
    int layerVisualizationY{};
    int layerVisualizationsMargin{};
    std::vector<ALLEGRO_COLOR> maskColors{};
};

#endif
