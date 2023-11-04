#include "KMeans.h"

KMeans::KMeans(int K)
  : K(K)
{

}

KMeans::~KMeans(){}

void KMeans::Init(std::vector<ALLEGRO_COLOR> maskColors)
{
    this->maskColors = maskColors;
    clusterMeans.insert(clusterMeans.end(), K, 0);
}

int getMostSimilarCentroidIdx(const std::vector<int>& centroids, int pxColorGray)
{
    int minDiff = 255;
    int mostSimilarCentroidIdx = 0;
    for (int k = 0; k < centroids.size(); k++) {
        int diff = abs(pxColorGray - centroids[k]);
        if (diff < minDiff) {
            minDiff = diff;
            mostSimilarCentroidIdx = k;
        }
    }
    return mostSimilarCentroidIdx;
}

int getGray(ALLEGRO_COLOR color)
{
    byte r, g, b;
    al_unmap_rgb(color, &r, &g, &b);
    return 0.3 * r + 0.59 * g + 0.11 * b;
}

bool areColorsEqual(const ALLEGRO_COLOR &color1, const ALLEGRO_COLOR &color2)
{
    return color1.r == color2.r &&
           color1.g == color2.g &&
           color1.b == color2.b &&
           color1.a == color2.a;
}

int KMeans::getClusterFromColor(ALLEGRO_COLOR color)
{
    for (int i = 0; i < K; i++)
    {
        if (areColorsEqual(color, maskColors[i])) {
            return i;
        }
    }
    return -1;
}

void KMeans::NextStep(std::shared_ptr<const Image> orygImage, const Mask& mask)
{
    // Set centroids
    std::vector<int> centroids;
    for (int i = 0; i < K; i++)
    {
        if (step > 0) {
            centroids.push_back(clusterMeans[i]);
        } else {
            centroids.push_back(i * 255/K);
        }
    }

    for (int y = 0; y < al_get_bitmap_height(mask.bmp); y++)
    {
        for (int x = 0; x < al_get_bitmap_width(mask.bmp); x++)
        {
            int pxColorGray = getGray(al_get_pixel(orygImage->bmp, x, y));
            int mostSimilarCentroidIdx = getMostSimilarCentroidIdx(centroids, pxColorGray);
            al_put_pixel(x, y, maskColors[mostSimilarCentroidIdx]);
        }
    }

    std::vector<int> clusterSums(K, 0);
    std::vector<int> clusterCounts(K, 0);

    // Calculate mean of each cluster
    for (int y = 0; y < al_get_bitmap_height(mask.bmp); y++)
    {
        for (int x = 0; x < al_get_bitmap_width(mask.bmp); x++)
        {
            // Identify the cluster we are in
            ALLEGRO_COLOR readColor = al_get_pixel(mask.bmp, x, y);
            int cluster = getClusterFromColor(readColor);
            if (cluster == -1) {
                printf("Error decoding cluster from color\n");
                return;
            }

            int pxColorGray = getGray(al_get_pixel(orygImage->bmp, x, y));

            clusterSums[cluster] += pxColorGray;
            clusterCounts[cluster]++;
        }
    }

    for (int i = 0; i < K; i++)
    {
        if (clusterCounts[i] != 0) {
            clusterMeans[i] = clusterSums[i] / clusterCounts[i];
        }
    }

    al_set_target_backbuffer(al_get_current_display());

    printf("Cluster means: %d %d %d %d %d\n", clusterMeans[0], clusterMeans[1], clusterMeans[2], clusterMeans[3], clusterMeans[4]);

    step++;
}
