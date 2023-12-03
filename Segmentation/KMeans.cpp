#include "KMeans.h"

#include <random>
#include <stdexcept>

KMeans::KMeans(int K, CentroidType centroidType, const std::vector<int>& initialCentroids)
  : K(K)
{
    switch (centroidType)
    {
        case CentroidType::Random:
            centroids = getRandomCentroids();
            break;
        case CentroidType::Equalized:
            centroids = getEqualizedCentroids();
            break;
        case CentroidType::Custom:
            if (initialCentroids.size() != K) {
                throw std::invalid_argument("Error: Initial centroids size is not equal to K!");
            } else {
                centroids = initialCentroids;
            }
            break;
    }
}

KMeans::~KMeans(){}

void KMeans::Init(const std::vector<ALLEGRO_COLOR>& maskColors)
{
    this->maskColors = maskColors;
}

int getMostSimilarCentroidIdx(int pxColorGray, const std::vector<int>& centroids)
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

std::vector<int> KMeans::getRandomCentroids() const
{
    std::vector<int> initialCentroids;
    std::random_device randomGeneratorDevice;
    std::mt19937 generator(randomGeneratorDevice());
    std::uniform_int_distribution<int> distribution(0, 255);

    for (int i = 0; i < K; i++)
    {
        initialCentroids.push_back(distribution(generator));
    }

    return initialCentroids;
}

std::vector<int> KMeans::getEqualizedCentroids() const
{
    std::vector<int> initialCentroids;
    for (int i = 0; i < K; i++)
    {
        initialCentroids.push_back(i * 255/K);
    }
    return initialCentroids;
}

void KMeans::RunStep(const Image& orygImage, const Mask& mask)
{
    double totalVariation = 0.0;

    for (int y = 0; y < mask.height; y++)
    {
        for (int x = 0; x < mask.width; x++)
        {
            int pxColorGray = getGray(al_get_pixel(orygImage.bmp.get(), x + mask.x - orygImage.x, y + mask.y - orygImage.y));
            int mostSimilarCentroidIdx = getMostSimilarCentroidIdx(pxColorGray, centroids);
            al_put_pixel(x, y, maskColors[mostSimilarCentroidIdx]);
            double distance = pxColorGray - centroids[mostSimilarCentroidIdx];
            totalVariation += distance * distance;
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

    printf("Step: %d Variation: %f Centroids: %d %d %d %d %d\n", step, totalVariation, centroids[0], centroids[1], centroids[2], centroids[3], centroids[4]);

    if (centroids == previousCentroids) {
        printf("Segmentation finished\n");
        segmentationFinished = true;
    }

    lastClusterCount = centroids.size();
    previousCentroids = centroids;
    step++;
}
