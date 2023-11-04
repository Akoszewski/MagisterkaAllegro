#include "Image.h"
#include "Constants.h"

Image::Image() {}

Image::Image(const char* path, int x, int y)
  : x(x), y(y)
{
    bmp = std::unique_ptr<ALLEGRO_BITMAP, BitmapDeleter>(al_load_bitmap(path));
    if (!bmp) {
        printf("Couldn't load image\n");
        return;
    }
    width = al_get_bitmap_width(bmp.get());
    height = al_get_bitmap_height(bmp.get());
}

void Image::Center()
{
    x = (SCREEN_WIDTH - width)/2;
    y = (SCREEN_HEIGHT - height)/2;
}

void Image::Draw()
{
    // printf("bmp: %p, %d %d\n", x, y);
    al_draw_bitmap(bmp.get(), x, y, 0);
}
