#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "Image.h"
#include "Pixel.h"

#include <allegro5/allegro_primitives.h>
#include <vector>

#define MAX_COLORED_PIXELS 1000
#define MAX_MASK_COLORS 10

typedef unsigned char byte;

class Segmentation
{
public:
    Segmentation();
    ~Segmentation();
    void Init(const Image* img);
    void Draw();
    void NextStep();
private:
    const Image* orygImage;
    Image mask;
    Pixel pixels[MAX_COLORED_PIXELS];
    byte maskTransparency;
    std::vector<ALLEGRO_COLOR> maskColors;
    std::vector<int> clusterMeans;
    int step;

    void initMask(const Image* img, byte xStartPercent, byte yStartPercent, byte xEndPercent, byte yEndPercent);
    int getClusterFromColor(ALLEGRO_COLOR color);
};

#endif
