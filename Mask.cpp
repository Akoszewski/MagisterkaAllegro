#include "Mask.h"

Mask::Mask(std::shared_ptr<Image> orygImage, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent)
  : maskTransparency(0)
{    
    printf("Przezroczystosc: %d\n", maskTransparency);
    maskColors =
    {
        al_map_rgba(100, 0, 0, maskTransparency),
        al_map_rgba(0, 100, 0, maskTransparency),
        al_map_rgba(0, 0, 100, maskTransparency),
        al_map_rgba(255, 0, 0, maskTransparency),
        al_map_rgba(0, 255, 0, maskTransparency),
        al_map_rgba(0, 0, 255, maskTransparency),
        al_map_rgba(100, 100, 0, maskTransparency),
        al_map_rgba(0, 100, 100, maskTransparency),
        al_map_rgba(255, 255, 0, maskTransparency),
        al_map_rgba(0, 255, 255, maskTransparency),
    };
    initMask(orygImage, xStartPercent, xEndPercent, yStartPercent, yEndPercent);
}

void Mask::initMask(std::shared_ptr<Image> orygImage, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent)
{
    x = orygImage->x + xStartPercent/100.0f * al_get_bitmap_width(orygImage->bmp);
    y = orygImage->y + yStartPercent/100.0f * al_get_bitmap_height(orygImage->bmp);

    byte widthPercent = abs(xEndPercent - xStartPercent);
    byte heightPercent = abs(yEndPercent - yStartPercent);

    int width = widthPercent/100.0f * al_get_bitmap_width(orygImage->bmp);
    int height = heightPercent/100.0f * al_get_bitmap_height(orygImage->bmp);

    bmp = al_create_bitmap(width, height);
    if (!bmp) {
        puts("Failed to create new bitmap");
        return;
    }
}
