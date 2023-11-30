#include "KMeans.h"

KMeans::KMeans(int K, const std::vector<int>& initialCentroids)
  : K(K), centroids(initialCentroids)
{
    if (initialCentroids.empty()) {
        for (int i = 0; i < K; i++)
        {
            centroids.push_back(i * 255/K);
        }
    }
}

KMeans::~KMeans(){}

void KMeans::Init(const std::vector<ALLEGRO_COLOR>& maskColors)
{
    this->maskColors = maskColors;
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

void KMeans::RunStep(const Image& orygImage, const Mask& mask)
{
    for (int y = 0; y < mask.height; y++)
    {
        for (int x = 0; x < mask.width; x++)
        {
            int pxColorGray = getGray(al_get_pixel(orygImage.bmp.get(), x + mask.x - orygImage.x, y + mask.y - orygImage.y));
            
            int mostSimilarCentroidIdx = getMostSimilarCentroidIdx(centroids, pxColorGray);
            al_put_pixel(x, y, maskColors[mostSimilarCentroidIdx]);
        }
    }

    std::vector<int> clusterSums(K, 0);
    std::vector<int> clusterCounts(K, 0);

    // Calculate mean of each cluster
    for (int y = 0; y < mask.height; y++)
    {
        for (int x = 0; x < mask.width; x++)
        {
            // Identify the cluster we are in
            ALLEGRO_COLOR readColor = al_get_pixel(mask.bmp.get(), x, y);
            int cluster = getClusterFromColor(readColor);
            if (cluster == -1) {
                printf("Error decoding cluster from color\n");
                return;
            }

            int pxColorGray = getGray(al_get_pixel(orygImage.bmp.get(), x + mask.x - orygImage.x, y + mask.y - orygImage.y));

            clusterSums[cluster] += pxColorGray;
            clusterCounts[cluster]++;
        }
    }

    for (int i = 0; i < K; i++)
    {
        if (clusterCounts[i] != 0) {
            centroids[i] = clusterSums[i] / clusterCounts[i];
        }
    }

    printf("Step: %d Centroids: %d %d %d %d %d\n", step, centroids[0], centroids[1], centroids[2], centroids[3], centroids[4]);
    step++;
}
