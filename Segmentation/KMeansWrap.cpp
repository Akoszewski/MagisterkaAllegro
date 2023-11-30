#include "KMeansWrap.h"

#include <random>

KMeansWrap::KMeansWrap(int K, CentroidType centroidType, const std::vector<int>& initialCentroids)
  : K(K), centroidType(centroidType)
{

}

KMeansWrap::~KMeansWrap(){}

void KMeansWrap::Init(const std::vector<ALLEGRO_COLOR>& maskColors)
{
    this->maskColors = maskColors;
}


void KMeansWrap::RunStep(const Image& orygImage, const Mask& mask)
{
    KMeans kmeans(K, centroidType, initialCentroids);
    kmeans.Init(maskColors);
    for (int i = 0; i < 10; i++)
    {
        kmeans.RunStep(orygImage, mask);
    }

    step++;
}
