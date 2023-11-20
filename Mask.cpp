#include "Mask.h"

void draw_black_outline(ALLEGRO_BITMAP* bitmap, int outline_thickness)
{
    // Get the dimensions of the bitmap
    int width = al_get_bitmap_width(bitmap);
    int height = al_get_bitmap_height(bitmap);

    // Set the target bitmap
    al_set_target_bitmap(bitmap);

    // Set the color for drawing (black)
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

    // Draw the rectangle
    // Note: We subtract 1 from the width and height to stay within the bitmap's boundary
    al_draw_rectangle(outline_thickness / 2.0, outline_thickness / 2.0,
                      width - outline_thickness / 2.0, height - outline_thickness / 2.0, 
                      black, outline_thickness);
}

Mask::Mask(const Image& orygImage, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent)
  : maskTransparency(0)
{    
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

void Mask::initMask(const Image& orygImage, byte xStartPercent, byte xEndPercent, byte yStartPercent, byte yEndPercent)
{
    x = orygImage.x + xStartPercent/100.0f * orygImage.width;
    y = orygImage.y + yStartPercent/100.0f * orygImage.height;

    byte widthPercent = abs(xEndPercent - xStartPercent);
    byte heightPercent = abs(yEndPercent - yStartPercent);

    int bmpWidth = widthPercent/100.0f * orygImage.width;
    int bmpHeight = heightPercent/100.0f * orygImage.height;

    bmp = std::unique_ptr<ALLEGRO_BITMAP, BitmapDeleter>(al_create_bitmap(bmpWidth, bmpHeight));
    if (!bmp) {
        puts("Failed to create new bitmap");
        return;
    }

    width = al_get_bitmap_width(bmp.get());
    height = al_get_bitmap_height(bmp.get());

    al_set_target_bitmap(bmp.get());
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    //draw_black_outline(bmp.get(), 2);
    al_set_target_backbuffer(al_get_current_display());
}
