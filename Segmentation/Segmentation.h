#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "KMeans.h"
#include "Thresholding.h"
#include "../Mask.h"

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
    std::unique_ptr<Image> FilterImage(const Image& orygImage, int windowWidth, int windowHeight, FilterType filterType);
    void DrawSegmentLines();
    void DrawMaskVisualizations(const Mask& mask, int i);
    void PerformMorphOnMask(const Mask& mask, int chosenLayerColor);
    void ErodeMask(const Mask& mask, std::vector<std::vector<int>> structuringElement, int chosenLayerColorIdx);
    void DilateMask(const Mask& mask, std::vector<std::vector<int>> structuringElement, int chosenLayerColorIdx);
    std::vector<Mask> masks;
private:
    std::shared_ptr<Image> orygImage;
    std::unique_ptr<Image> filteredImage;
    std::vector<Image> layerVisualizations;
    std::vector<std::unique_ptr<SegmentationStrategy>> strategies;
    int step;
    int chooseLayerForMorphoology(const Mask& mask);
    bool areColorsEqual(ALLEGRO_COLOR color1, ALLEGRO_COLOR color2);
};

#endif
