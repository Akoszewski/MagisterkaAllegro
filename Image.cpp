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

Image::Image(const Image& other)
  : x(other.x), y(other.y), width(other.width), height(other.height)
{
    if (other.bmp) {
        bmp = std::unique_ptr<ALLEGRO_BITMAP, BitmapDeleter>(al_clone_bitmap(other.bmp.get()));
    }
}

void Image::Center()
{
    x = (SCREEN_WIDTH - width)/2;
    y = (SCREEN_HEIGHT - height)/2;
}

void Image::Draw()
{
    if (!bmp) {
        printf("Trying to draw an empty image\n");
    }
    al_draw_bitmap(bmp.get(), x, y, 0);
}
