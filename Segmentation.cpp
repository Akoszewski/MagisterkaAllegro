#include "Segmentation.h"

Segmentation::Segmentation() {}

void Segmentation::Init(const Image* img)
{
    orygImage = img;
    image.x = orygImage->x;
    image.y = orygImage->y;
    image.bmp = al_clone_bitmap(orygImage->bmp);


}

void Segmentation::NextStep()
{
    printf("Next step\n");
}

void Segmentation::Draw()
{
    image.Draw();
}

Segmentation::~Segmentation() {}
