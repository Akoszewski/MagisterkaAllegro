#ifndef MASK_H
#define MASK_H

#include <memory>
#include "Image.h"

typedef unsigned char byte;

class Mask : public Image
{
public:
    Mask(std::shared_ptr<Image> orygImage, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent);
private:
    void initMask(std::shared_ptr<Image> orygImage, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent);
};

#endif // MASK
