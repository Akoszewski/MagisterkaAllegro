#include "Segmentation.h"
#include "Utils.h"
#include "Filtering.h"
#include "MorphOperations.h"

#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>

// From otsu method:
// Best threshold: 100
// Best threshold: 196
// Best threshold: 141
// Best threshold: 138
// Best threshold: 116

Segmentation::Segmentation(std::string&& imagePath, SegmentationParams&& segmentationParams, const std::vector<ALLEGRO_COLOR>& maskColors)
  : step(0), layerVisualizationsMargin(20), segmentationParams(std::move(segmentationParams)), maskColors(maskColors)
{
    Init(std::move(imagePath));
}

void Segmentation::Init(std::string&& imagePath)
{
    orygImage = std::make_unique<Image>(imagePath.c_str());
    filteredImage = std::make_unique<Image>(*orygImage);

    int percentageInterval = 100;
    for (int i = 0; i < 100; i += percentageInterval)
    {
        std::unique_ptr<SegmentationStrategy> strategy;
        DataPoint3D maxValues{orygImage->width, orygImage->height, 255};

        strategy = std::make_unique<KMeans3D>(7, maxValues, DimensionWeights{0.0, 0.2, 1.0}, CentroidType::Random);
        // strategy = std::make_unique<MeanShift>(240, 100, maxValues, DimensionWeights{0.0, 0.2, 1.0});
        // strategy = std::make_unique<Dbscan>(7, maxValues, DimensionWeights{0.0, 0.2, 1.0});
        // strategy = std::make_unique<KMeansWrap>(4, CentroidType::Random);

        segmentationRegions.emplace_back(Mask(*orygImage, 0, 100, i, i+percentageInterval), std::move(strategy), maskColors);
    }

    layerVisualizationY = orygImage->y + orygImage->height + layerVisualizationsMargin;
}

int Segmentation::chooseLayerForMorphoology(const Mask& mask)
{
    return 1;
}

std::unique_ptr<Image> Segmentation::FilterImage(const Image& orygImage, int windowWidth, int windowHeight, FilterType filterType)
{
    std::unique_ptr<Image> image = std::make_unique<Image>(orygImage);

    al_lock_bitmap(orygImage.bmp.get(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
    al_lock_bitmap(image->bmp.get(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);

    al_set_target_bitmap(image->bmp.get());

    int edge_x = windowWidth / 2;
    int edge_y = windowHeight / 2;

    std::vector<int> window(windowWidth * windowHeight);
    for (int x = 0; x < image->width; x++)
    {
        for (int y = 0; y < image->height; y++)
        {
            int i = 0;
            for (int fx = 0; fx < windowWidth; fx++)
            {
                for (int fy = 0; fy < windowHeight; fy++)
                {
                    int clampedX = std::clamp(x + fx - edge_x, 0, image->width - 1);
                    int clampedY = std::clamp(y + fy - edge_y, 0, image->height - 1);
                    window[i++] = getGray(al_get_pixel(orygImage.bmp.get(), clampedX, clampedY));
                }
            }

            ALLEGRO_COLOR finalColor;

            if (filterType == FilterType::Average) {
                int average = std::accumulate(window.begin(), window.end(), 0) / window.size();
                finalColor = al_map_rgb(average, average, average);
            } else if (filterType == FilterType::Median) {
                std::nth_element(window.begin(), window.begin() + window.size()/2, window.end());
                int median = window[windowWidth * windowHeight / 2];
                finalColor = al_map_rgb(median, median, median);
            } else if (filterType == FilterType::Gaussian) {
                auto kernel = generateGaussianKernel(windowWidth, windowHeight, 0.4 * windowWidth, 0.4 * windowHeight);
                float filteredValue = std::inner_product(window.begin(), window.end(), kernel.begin(), 0.0f);
                finalColor = al_map_rgb(filteredValue, filteredValue, filteredValue);
            }
            al_put_pixel(x, y, finalColor);
        }
    }

    al_unlock_bitmap(orygImage.bmp.get());
    al_unlock_bitmap(image->bmp.get());
    al_set_target_backbuffer(al_get_current_display());
    return image;
}

void Segmentation::ToggleImageDisplayMode()
{
    switch (displayMode)
    {
    case DisplayMode::Filtered:
        displayMode = DisplayMode::Oryginal;
        break;
    case DisplayMode::Oryginal:
        displayMode = DisplayMode::MaskOnly;
        break;
    case DisplayMode::MaskOnly:
        displayMode = DisplayMode::Filtered;
        break;
    }
}

void Segmentation::PerformMorphOnMask(Mask& mask, int chosenLayerColorIdx)
{
    ALLEGRO_COLOR chosenLayerColor = mask.maskColors[chosenLayerColorIdx];
    std::vector<std::vector<int>> structuringElement = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    // std::vector<std::vector<int>> structuringElement = {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}};
    DilateMask(mask, structuringElement, chosenLayerColorIdx);
    // ErodeMask(mask, structuringElement, chosenLayerColorIdx, 2);
}

void Segmentation::ResetLayerVisualizations(SegmentationRegion& region, int clusters)
{
    region.layerVisualizations.clear();
    for (int i = 0; i < clusters; i++)
    {
        region.layerVisualizations.emplace_back(region.mask);
        region.layerVisualizations[i].y = layerVisualizationY + i*(region.layerVisualizations[i].height + layerVisualizationsMargin);
    }
}

void Segmentation::DrawLayerVisualizatons(const Mask& mask, int i, int clusters)
{
    for (auto& region : segmentationRegions)
    {
        ResetLayerVisualizations(region, clusters);
    }
    for (int j = 0; j < clusters; j++)
    {
        ALLEGRO_COLOR chosenLayerColor = mask.maskColors[j];
        al_set_target_bitmap(segmentationRegions[i].layerVisualizations[j].bmp.get());
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        // al_clear_to_color(al_map_rgb(255, 255, 0));

        for (int y = 0; y < mask.height; y++)
        {
            for (int x = 0; x < mask.width; x++)
            {
                ALLEGRO_COLOR readMaskColor = al_get_pixel(mask.bmp.get(), x, y);
                if (areColorsEqual(readMaskColor, chosenLayerColor)) {
                    ALLEGRO_COLOR readOrygImageColor = al_get_pixel(orygImage->bmp.get(),
                            x + mask.x - orygImage->x, y + mask.y - orygImage->y);
                    
                    al_put_pixel(x, y, readOrygImageColor);
                }
            }
        }
    }

    al_set_target_backbuffer(al_get_current_display());
}

// Trzy etapy: filtracja, segmentacja i operacje morfologiczne
void Segmentation::RunStep(StepOperation operation)
{
    if (operation == StepOperation::Filter) {
        filteredImage = FilterImage(*filteredImage.get(), 9, 5, FilterType::Median);
    } else if (operation == StepOperation::Segmentate) {
        for (int i = 0; i < segmentationRegions.size(); i++)
        {
            if (!segmentationRegions[i].strategy->segmentationFinished) {
                al_set_target_bitmap(segmentationRegions[i].mask.bmp.get());
                al_clear_to_color(al_map_rgba(0, 0, 0, 0));
                segmentationRegions[i].strategy->RunStep(*filteredImage.get(), segmentationRegions[i].mask);
                DrawLayerVisualizatons(segmentationRegions[i].mask, i, segmentationRegions[i].strategy->lastClusterCount);
                al_set_target_backbuffer(al_get_current_display());
            }
        }
    } else if (operation == StepOperation::Dilate) {
        for (int i = 0; i < segmentationRegions.size(); i++)
        {
            int chosenLayerColorIdx = chooseLayerForMorphoology(segmentationRegions[i].mask);
            al_set_target_bitmap(segmentationRegions[i].mask.bmp.get());
            PerformMorphOnMask(segmentationRegions[i].mask, chosenLayerColorIdx);
            DrawLayerVisualizatons(segmentationRegions[i].mask, i, segmentationRegions[i].strategy->lastClusterCount);
            al_set_target_backbuffer(al_get_current_display());
        }
    }
    step++;
}

 void Segmentation::DrawSegmentLines()
 {
    for (int i = 1; i < segmentationRegions.size(); i++)
    {
        al_draw_line(
            segmentationRegions[i].mask.x, segmentationRegions[i].mask.y,
            segmentationRegions[i].mask.x + segmentationRegions[i].mask.width, segmentationRegions[i].mask.y,
            al_map_rgb(0, 0, 0), 2.0f
        );
    }
 }

void Segmentation::Draw()
{
    if (displayMode == DisplayMode::Filtered && filteredImage) {
        filteredImage->Draw();
    } else if (displayMode == DisplayMode::Oryginal) {
        orygImage->Draw();
    } else if (displayMode == DisplayMode::Filtered && !filteredImage) {
        orygImage->Draw();
    }

    DrawSegmentLines();
    for (int i = 0; i < segmentationRegions.size(); i++)
    {
        segmentationRegions[i].mask.Draw();
    }

    for (auto& layerVisualization : segmentationRegions[currentVisualizedRegion].layerVisualizations)
    {
        layerVisualization.Draw();
    }
}

Segmentation::~Segmentation() {}
