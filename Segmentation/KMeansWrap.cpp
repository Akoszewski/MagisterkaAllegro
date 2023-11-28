#include "KMeansWrap.h"
#include "KMeans.h"

#include <random>

KMeansWrap::KMeansWrap(int K)
  : K(K)
{

}

KMeansWrap::~KMeansWrap(){}

void KMeansWrap::Init(const std::vector<ALLEGRO_COLOR>& maskColors)
{
    this->maskColors = maskColors;
}


void KMeansWrap::RunStep(const Image& orygImage, const Mask& mask)
{
    // Set initial centroids
    std::vector<int> initialCentroids;
    std::random_device randomGeneratorDevice;
    std::mt19937 generator(randomGeneratorDevice());
    std::uniform_int_distribution<int> distribution(0, 255);

    for (int i = 0; i < K; i++)
    {
        initialCentroids.push_back(distribution(generator));
    }

    KMeans kmeans(K, initialCentroids);
    kmeans.Init(maskColors);
    for (int i = 0; i < 10; i++)
    {
        kmeans.RunStep(orygImage, mask);
    }

    step++;
}
