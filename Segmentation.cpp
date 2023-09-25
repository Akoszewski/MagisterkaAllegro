#include "Segmentation.h"

Segmentation::Segmentation()
  : maskTransparency(60), step(0)
{
    maskColors =
    {
        al_map_rgba(100, 0, 0, maskTransparency),
        al_map_rgba(0, 100, 0, maskTransparency),
        al_map_rgba(0, 0, 100, maskTransparency),
        al_map_rgba(255, 0, 0, maskTransparency),
        al_map_rgba(0, 255, 0, maskTransparency),
        al_map_rgba(0, 0, 255, maskTransparency),
        al_map_rgba(100, 100, 0, maskTransparency),
        al_map_rgba(0, 100, 100, maskTransparency),
        al_map_rgba(255, 255, 0, maskTransparency),
        al_map_rgba(0, 255, 255, maskTransparency),
    };
}

void Segmentation::Init(const Image* img)
{
    orygImage = img;
    mask.x = orygImage->x;
    mask.y = orygImage->y;

    int width = al_get_bitmap_width(orygImage->bmp);
    int height = al_get_bitmap_height(orygImage->bmp);

    mask.bmp = al_create_bitmap(width, height);
    if (!mask.bmp) {
        fprintf(stderr, "Failed to create new bitmap.\n");
        return;
    }

    al_set_target_bitmap(mask.bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_set_target_backbuffer(al_get_current_display());
}

void Segmentation::NextStep()
{
    al_set_target_bitmap(mask.bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    ALLEGRO_COLOR color = maskColors[0];

    int K = 5;
    std::vector<int> centroids;
    for (int i = 0; i < K; i++) {
        centroids.push_back(i * 255/K);
    }

    for (int y = 0; y < al_get_bitmap_height(mask.bmp); y++) {
        for (int x = 0; x < al_get_bitmap_width(mask.bmp); x++) {

            ALLEGRO_COLOR readColor = al_get_pixel(orygImage->bmp, x, y);
            unsigned char r, g, b;
            al_unmap_rgb(readColor, &r, &g, &b);
            int readColorGray = 0.3 * r + 0.59 * g + 0.11 * b;

            int minDiff = 255;
            int minDiffCentroidIndex = 0;
            for (int k = 0; k < K; k++) {
                int diff = abs(readColorGray - centroids[k]);
                if (diff < minDiff) {
                    minDiff = diff;
                    minDiffCentroidIndex = k;
                }
            }
            printf("%d ", minDiffCentroidIndex);

            al_put_pixel(x, y, maskColors[minDiffCentroidIndex]);
        }
    }

    al_set_target_backbuffer(al_get_current_display());

    printf("Next step %d\n", step);

    step++;
}

void Segmentation::Draw()
{
    mask.Draw();
}

Segmentation::~Segmentation() {}
