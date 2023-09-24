#include "Image.h"
#include "Constants.h"

Image::Image(int x, int y)
  : x(x), y(y)
{
    bmp = al_load_bitmap("pictures/roi.bmp");
    if (!bmp) {
        printf("Couldn't load image\n");
        return;
    }
}

Image::~Image()
{
    al_destroy_bitmap(bmp);
}

void Image::Center()
{
    x = (SCREEN_WIDTH - al_get_bitmap_width(bmp))/2;
    y = (SCREEN_HEIGHT - al_get_bitmap_height(bmp))/2;
}

void Image::Draw()
{
    al_draw_bitmap(bmp, x, y, 0);
}
