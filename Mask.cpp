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
    x = orygImage->x + xStartPercent/100.0f * orygImage->width;
    y = orygImage->y + yStartPercent/100.0f * orygImage->height;

    byte widthPercent = abs(xEndPercent - xStartPercent);
    byte heightPercent = abs(yEndPercent - yStartPercent);

    int bmpWidth = widthPercent/100.0f * orygImage->width;
    int bmpHeight = heightPercent/100.0f * orygImage->height;

    bmp = std::unique_ptr<ALLEGRO_BITMAP, BitmapDeleter>(al_create_bitmap(bmpWidth, bmpHeight));
    if (!bmp) {
        puts("Failed to create new bitmap");
        return;
    }

    width = al_get_bitmap_width(bmp.get());
    height = al_get_bitmap_height(bmp.get());

    al_set_target_bitmap(bmp.get());
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    al_set_target_backbuffer(al_get_current_display());
}
