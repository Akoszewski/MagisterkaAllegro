#include "Segmentation.h"
#include "Utils.h"

#include <algorithm>
#include <numeric>

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


    for (int i = 0; i < masks.size(); i++)
    {
        strategies[i]->Init(masks[i].maskColors);
    }
}

std::unique_ptr<Image> Segmentation::filterImage(const Image& orygImage, int windowWidth, int windowHeight, FilterType filterType)
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
            }
            al_put_pixel(x, y, finalColor);
        }
    }

    al_unlock_bitmap(orygImage.bmp.get());
    al_unlock_bitmap(image->bmp.get());
    al_set_target_backbuffer(al_get_current_display());
    return image;
}


void Segmentation::RunStep()
{
    if (step == 0) {
    // std::unique_ptr<Image>(filteredImage) = filterImage(*orygImage.get(), 5, 5, FilterType::Median);
    orygImage = filterImage(*orygImage.get(), 5, 5, FilterType::Median);
    } else {
        for (int i = 0; i < masks.size(); i++)
        {
            al_set_target_bitmap(masks[i].bmp.get());
            al_clear_to_color(al_map_rgba(0, 0, 0, 0));
            strategies[i]->RunStep(*orygImage.get(), masks[i]);
        }
    }
    step++;
}

void Segmentation::Draw()
{
    orygImage->Draw();
    for (auto &mask : masks)
    {
        mask.Draw();
    }
}

Segmentation::~Segmentation() {}
