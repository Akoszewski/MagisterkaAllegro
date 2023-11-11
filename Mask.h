#ifndef MASK_H
#define MASK_H

#include <memory>
#include "Image.h"

#include <vector>

typedef unsigned char byte;

class Mask : public Image
{
public:
    Mask(const Image& orygImage, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent);
    std::vector<ALLEGRO_COLOR> maskColors;
private:
    void initMask(const Image& orygImage, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent);
    byte maskTransparency;
};

#endif // MASK
