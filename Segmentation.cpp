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
    clusterMeans.insert(clusterMeans.end(), MAX_MASK_COLORS, 0);
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

bool areColorsEqual(const ALLEGRO_COLOR &color1, const ALLEGRO_COLOR &color2) {
    return color1.r == color2.r &&
           color1.g == color2.g &&
           color1.b == color2.b &&
           color1.a == color2.a;
}

int Segmentation::getClusterFromColor(ALLEGRO_COLOR color)
{
    for (int i = 0; i < MAX_MASK_COLORS; i++) {
        if (areColorsEqual(color, maskColors[i])) {
            return i;
        }
    }
    return -1;
}

void Segmentation::NextStep()
{
    al_set_target_bitmap(mask.bmp);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));

    ALLEGRO_COLOR color = maskColors[0];

    int K = 4;

    // Set centroids
    std::vector<int> centroids;
    for (int i = 0; i < K; i++) {
        if (step > 0) {
            centroids.push_back(clusterMeans[i]);
        } else {
            centroids.push_back(i * 255/K);
        }
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
            // printf("%d ", minDiff);

            al_put_pixel(x, y, maskColors[minDiffCentroidIndex]);
        }
    }

    std::vector<int> clusterSums(MAX_MASK_COLORS, 0);
    std::vector<int> clusterCounts(MAX_MASK_COLORS, 0);

    // Calculate mean of each cluster
    for (int y = 0; y < al_get_bitmap_height(mask.bmp); y++) {
        for (int x = 0; x < al_get_bitmap_width(mask.bmp); x++) {
            // Identify the cluster we are in
            ALLEGRO_COLOR readColor = al_get_pixel(mask.bmp, x, y);
            int cluster = getClusterFromColor(readColor);
            if (cluster == -1) {
                printf("Error decoding cluster from color\n");
                return;
            }

            // Read actual pixel value here
            ALLEGRO_COLOR readOrygImgColor = al_get_pixel(orygImage->bmp, x, y);
            unsigned char r, g, b;
            al_unmap_rgb(readOrygImgColor, &r, &g, &b);
            int readColorGray = 0.3 * r + 0.59 * g + 0.11 * b;

            clusterSums[cluster] += readColorGray;
            clusterCounts[cluster]++;
        }
    }
    
    for (int i = 0; i < K; i++) {
        clusterMeans[i] = clusterSums[i] / clusterCounts[i];
    }

    al_set_target_backbuffer(al_get_current_display());

    printf("Cluster means: %d %d %d %d %d\n", clusterMeans[0], clusterMeans[1], clusterMeans[2], clusterMeans[3], clusterMeans[4]);

    step++;
}

void Segmentation::Draw()
{
    mask.Draw();
}

Segmentation::~Segmentation() {}
