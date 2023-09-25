#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "Image.h"
#include "Pixel.h"

#include <allegro5/allegro_primitives.h>
#include <vector>

#define MAX_COLORED_PIXELS 1000
#define MAX_MASK_COLORS 10

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
    unsigned char maskTransparency;
    std::vector<ALLEGRO_COLOR> maskColors;
    int step;
};

#endif
