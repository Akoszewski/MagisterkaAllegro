#include "KMeans3D.h"

#include <random>
#include <stdexcept>

#define MAX_NORMALIZED_VALUE 1000

KMeans3D::KMeans3D(int K, DataPoint3D maxDataPoint, CentroidType centroidType, const std::vector<DataPoint3D>& initialCentroids)
  : K(K), maxDataPoint(maxDataPoint)
{
    switch (centroidType)
    {
        case CentroidType::Random:
            centroids = getRandomCentroids(maxDataPoint);
            break;
        case CentroidType::Equalized:
            centroids = getEqualizedCentroids(maxDataPoint);
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

KMeans3D::~KMeans3D(){}

void KMeans3D::Init(const std::vector<ALLEGRO_COLOR>& maskColors)
{
    this->maskColors = maskColors;
}

double KMeans3D::calculateDistance(DataPoint3D point, DataPoint3D centroid) const
{
    int normalizedPointXDistance = (point.x - centroid.x) * MAX_NORMALIZED_VALUE / maxDataPoint.x;
    int normalizedPointYDistance = (point.y - centroid.y) * MAX_NORMALIZED_VALUE / maxDataPoint.y;
    int normalizedPointIntensityDistance = (point.intensity - centroid.intensity) * MAX_NORMALIZED_VALUE / maxDataPoint.intensity;

    double xModifier = 0.1;
    double yModifier = 1.0;
    double intensityModifier = 1.0;

    return sqrt(
                xModifier * normalizedPointXDistance * normalizedPointXDistance +
                yModifier * normalizedPointYDistance * normalizedPointYDistance +
                intensityModifier * normalizedPointIntensityDistance * normalizedPointIntensityDistance
    );
}

int KMeans3D::getMostSimilarCentroidIdx(DataPoint3D point, const std::vector<DataPoint3D>& centroids) const
{
    int minDiff = sqrt(3 * MAX_NORMALIZED_VALUE * MAX_NORMALIZED_VALUE);
    int mostSimilarCentroidIdx = 0;
    for (int k = 0; k < centroids.size(); k++) {
        int diff = calculateDistance(point, centroids[k]);
        if (diff < minDiff) {
            minDiff = diff;
            mostSimilarCentroidIdx = k;
        }
    }
    return mostSimilarCentroidIdx;
}

int KMeans3D::getClusterFromColor(ALLEGRO_COLOR color)
{
    for (int i = 0; i < K; i++)
    {
        if (areColorsEqual(color, maskColors[i])) {
            return i;
        }
    }
    return -1;
}

void KMeans3D::printCentroids() const
{
    printf("Centroids: ");
    for (const auto& centroid : centroids)
    {
        printf("{%d %d %d} ", centroid.x, centroid.y, centroid.intensity);
    }
    printf("\n");
}

std::vector<DataPoint3D> KMeans3D::getRandomCentroids(DataPoint3D maxValues) const
{
    std::vector<DataPoint3D> initialCentroids;
    std::random_device randomGeneratorDevice;
    std::mt19937 generator(randomGeneratorDevice());

    std::uniform_int_distribution<int> distributionPxIntensity(0, maxValues.intensity);
    std::uniform_int_distribution<int> distributionX(0, maxValues.x);
    std::uniform_int_distribution<int> distributionY(0, maxValues.y);

    printf("Max values: %d %d %d\n", maxValues.intensity, maxValues.x, maxValues.y);
    for (int i = 0; i < K; i++)
    {
        initialCentroids.emplace_back(distributionX(generator), distributionY(generator), distributionPxIntensity(generator));
    }

    printCentroids();
    return initialCentroids;
}

std::vector<DataPoint3D> KMeans3D::getEqualizedCentroids(DataPoint3D maxValues) const
{
    std::vector<DataPoint3D> initialCentroids;
    for (int i = 0; i < K; i++)
    {
        initialCentroids.emplace_back(i * maxValues.x/K, maxValues.x/2, i * maxValues.intensity/K);
    }
    return initialCentroids;
}

void KMeans3D::RunStep(const Image& orygImage, const Mask& mask)
{
    double totalVariation = 0.0;

    for (int y = 0; y < mask.height; y++)
    {
        for (int x = 0; x < mask.width; x++)
        {
            int pxColorGray = getGray(al_get_pixel(orygImage.bmp.get(), x + mask.x - orygImage.x, y + mask.y - orygImage.y));
            int mostSimilarCentroidIdx = getMostSimilarCentroidIdx({x, y, pxColorGray}, centroids);
            al_put_pixel(x, y, maskColors[mostSimilarCentroidIdx]);
            double distance = calculateDistance({x, y, pxColorGray}, centroids[mostSimilarCentroidIdx]);
            totalVariation += distance * distance;
        }
    }

    std::vector<DataPoint3D> clusterSums(K, {0, 0, 0});
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

            clusterSums[cluster].x += x;
            clusterSums[cluster].y += y;
            clusterSums[cluster].intensity += pxColorGray;
            clusterCounts[cluster]++;
        }
    }

    for (int i = 0; i < K; i++)
    {
        if (clusterCounts[i] != 0) {
            centroids[i].x = clusterSums[i].x / clusterCounts[i];
            centroids[i].y = clusterSums[i].y / clusterCounts[i];
            centroids[i].intensity = clusterSums[i].intensity / clusterCounts[i];
        }
    }

    printf("Step: %d Variation: %f\n", step, totalVariation);
    printCentroids();
    printf("================================\n");

    if (centroids == previousCentroids) {
        printf("Segmentation finished\n");
        segmentationFinished = true;
    }

    previousCentroids = centroids;
    step++;
}