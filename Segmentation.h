#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "Image.h"

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
    Image image;
};

#endif
