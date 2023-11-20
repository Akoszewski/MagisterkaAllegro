#include "Segmentation.h"
#include "Utils.h"

#include <algorithm>
#include <numeric>
#include <cmath>

Segmentation::Segmentation()
  : step(0)
{}

// From otsu method:
// Best threshold: 100
// Best threshold: 196
// Best threshold: 141
// Best threshold: 138
// Best threshold: 116

void Segmentation::Init(std::shared_ptr<Image> img)
{
    orygImage = img;

    int percentageInterval = 20;
    for (int i = 0; i < 100; i += percentageInterval)
    {
        masks.emplace_back(*orygImage.get(), 0, 100, i, i+percentageInterval);
        strategies.push_back(std::make_unique<KMeans>(3));
    }

    // masks.emplace_back(*orygImage.get(), 0, 100, 0, 20);
    // strategies.push_back(std::make_unique<Thresholding>(100));

    // masks.emplace_back(*orygImage.get(), 0, 100, 20, 40);
    // strategies.push_back(std::make_unique<Thresholding>(196));

    // masks.emplace_back(*orygImage.get(), 0, 100, 40, 60);
    // strategies.push_back(std::make_unique<Thresholding>(141));

    // masks.emplace_back(*orygImage.get(), 0, 100, 60, 80);
    // strategies.push_back(std::make_unique<Thresholding>(138));

    // masks.emplace_back(*orygImage.get(), 0, 100, 80, 100);
    // strategies.push_back(std::make_unique<Thresholding>(116));

    int margin = 20;
    int margin2 = 40;
    int layer_visualisation_y = orygImage->y + orygImage->height + margin;
    for (int i = 0; i < masks.size(); i++)
    {
        strategies[i]->Init(masks[i].maskColors);

        layerVisualizations.emplace_back(masks[0]);
        layerVisualizations[3*i].y = layer_visualisation_y;
        layer_visualisation_y += masks[0].height + margin;

        layerVisualizations.emplace_back(masks[0]);
        layerVisualizations[3*i + 1].y = layer_visualisation_y;
        layer_visualisation_y += masks[0].height + margin;


        layerVisualizations.emplace_back(masks[0]);
        layerVisualizations[3*i + 2].y = layer_visualisation_y;
        layer_visualisation_y += masks[0].height + margin;

        layer_visualisation_y += margin2;
    }
}

double gaussian(int x, int y, double sigma)
{
    return exp(-(x*x + y*y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
}

ALLEGRO_COLOR Segmentation::chooseLayerForMorphoology(const Mask& mask)
{
    return mask.maskColors[0];
}

bool Segmentation::areColorsEqual(ALLEGRO_COLOR color1, ALLEGRO_COLOR color2)
{
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a;
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
    for (int x = edge_x; x < image->width - edge_x; x++)
    {
        for (int y = edge_y; y < image->height - edge_y; y++)
        {
            int i = 0;
            for (int fx = 0; fx < windowWidth; fx++)
            {
                for (int fy = 0; fy < windowHeight; fy++)
                {
                    if (x + fx - edge_x < image->width && y + fy - edge_y < image->height) {
                        window[i++] = getGray(al_get_pixel(orygImage.bmp.get(), x + fx - edge_x, y + fy - edge_y));
                    }
                    else {
                        printf("Attempting to go out of bounds\n");
                    }
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
                // TODO: Add code here
            }
            al_put_pixel(x, y, finalColor);
        }
    }

    al_unlock_bitmap(orygImage.bmp.get());
    al_unlock_bitmap(image->bmp.get());
    al_set_target_backbuffer(al_get_current_display());
    return image;
}

void Segmentation::DrawMaskVisualizations(const Mask& mask, int i)
{
    for (int j = 0; j < 3; j++)
    {
        ALLEGRO_COLOR chosenLayerColor = mask.maskColors[j];
        al_set_target_bitmap(layerVisualizations[i*3 + j].bmp.get());
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

void Segmentation::PerformMorphOnMask(const Mask& mask, ALLEGRO_COLOR chosenLayerColor)
{
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

// Trzy etapy: filtracja, segmentacja i operacje morfologiczne
void Segmentation::RunStep()
{
    if (step == 0) {
    filteredImage = FilterImage(*orygImage.get(), 7, 5, FilterType::Median);
    // orygImage = FilterImage(*orygImage.get(), 7, 5, FilterType::Median);
    } else if (step < 12) {
        for (int i = 0; i < masks.size(); i++)
        {
            al_set_target_bitmap(masks[i].bmp.get());
            al_clear_to_color(al_map_rgba(0, 0, 0, 0));
            strategies[i]->RunStep(*filteredImage.get(), masks[i]);
            DrawMaskVisualizations(masks[i], i);
        }
    } else {
        // Morphological operations ?
        for (int i = 0; i < masks.size(); i++)
        {
            ALLEGRO_COLOR chosenLayerColor = chooseLayerForMorphoology(masks[i]);
            PerformMorphOnMask(masks[i], chosenLayerColor);
        }
    }
    step++;
}

 void Segmentation::DrawSegmentLines()
 {
    for (int i = 1; i < masks.size(); i++)
    {
        al_draw_line(masks[i].x, masks[i].y, masks[i].x + masks[i].width, masks[i].y, al_map_rgb(0, 0, 0), 2.0f);
    }
 }

void Segmentation::Draw()
{
    if (step == 1) {
        filteredImage->Draw();
    } else {
        orygImage->Draw();
    }

    DrawSegmentLines();
    for (auto &mask : masks)
    {
        mask.Draw();
    }
    for (int i = 0; i < layerVisualizations.size(); i++)
    {
        layerVisualizations[i].Draw();
    }
}

Segmentation::~Segmentation() {}
