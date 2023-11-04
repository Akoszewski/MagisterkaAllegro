#include "Thresholding.h"

Thresholding::Thresholding(int threshold)
  : threshold(threshold)
{
    
}

Thresholding::~Thresholding(){}

void Thresholding::Init(const std::vector<ALLEGRO_COLOR>& maskColors)
{
    this->maskColors = maskColors;
}

int getMatchingPixelGroup(const std::vector<int>& thresholds, int pxColorGray)
{
    int minDiff = 255;
    int matchingPixelGroupIdx = 0;
    if (pxColorGray > thresholds[0]) {
        matchingPixelGroupIdx = 1;
    }
    // for (int i = 0; i < thresholds.size(); i++) {

    // }
    return matchingPixelGroupIdx;
}

static int getGray(ALLEGRO_COLOR color)
{
    byte r, g, b;
    al_unmap_rgb(color, &r, &g, &b);
    return 0.3 * r + 0.59 * g + 0.11 * b;
}

int calculateOtsu()
{
    return 0;
}

static bool areColorsEqual(const ALLEGRO_COLOR &color1, const ALLEGRO_COLOR &color2)
{
    return color1.r == color2.r &&
           color1.g == color2.g &&
           color1.b == color2.b &&
           color1.a == color2.a;
}

int Thresholding::getClusterFromColor(ALLEGRO_COLOR color)
{
//     for (int i = 0; i < K; i++)
//     {
//         if (areColorsEqual(color, maskColors[i])) {
//             return i;
//         }
//     }
    return -1;
}



int Thresholding::RunStep(std::shared_ptr<const Image> orygImage, const Mask& mask, int param)
{
    int histogram[256] = {0};
    int roiWidth = mask.width;
    int roiHeight = mask.height;
    int totalPixels = roiWidth * roiHeight;
    int sumTotalIntensity = 0;
    int w1 = 0;
    int w2 = 0;
    int pixelGroupSums[2] = {0, 0};
    double pixelGroupMeans[2] = {0, 0};
    int pixelGroupWeights[2] = {0, 0};
    // int pixelGroupVariances[2] = {0};
    // int varianceBetween = 0;


    for (int y = 0; y < roiHeight; y++)
    {
        for (int x = 0; x < roiWidth; x++)
        {
            int pxColorGray = getGray(al_get_pixel(orygImage->bmp.get(), x + mask.x - orygImage->x, y + mask.y - orygImage->y));
            int matchingPixelGroupIdx = getMatchingPixelGroup({param}, pxColorGray);
            // int matchingPixelGroupIdx = getMatchingPixelGroup({step * 10}, pxColorGray);
            al_put_pixel(x, y, mask.maskColors[matchingPixelGroupIdx]);

            histogram[pxColorGray]++;
            sumTotalIntensity += pxColorGray;
            pixelGroupSums[matchingPixelGroupIdx] += pxColorGray;
            pixelGroupWeights[matchingPixelGroupIdx]++;
        }
    }

    pixelGroupMeans[0] = (pixelGroupWeights[0] > 0) ? pixelGroupSums[0] / (double)pixelGroupWeights[0] : 0;
    pixelGroupMeans[1] = (pixelGroupWeights[1] > 0) ? pixelGroupSums[1] / (double)pixelGroupWeights[1] : 0;
    // val = wB * wF * ((sumB / wB) - mF) * ((sumB / wB) - mF)
    long long varianceBetween = 0;
    if (pixelGroupWeights[0] != 0 && pixelGroupWeights[1]!= 0) {
        varianceBetween = (long)pixelGroupWeights[0] * (long)pixelGroupWeights[1] * (pixelGroupMeans[1] - pixelGroupMeans[0]) * (pixelGroupMeans[1] - pixelGroupMeans[0]);
    }
    // printf("varianceBetween: %lld\n", varianceBetween);
    // printf("step: %d\n\n", step);

    al_set_target_backbuffer(al_get_current_display());
    step++;

    return varianceBetween;
}
