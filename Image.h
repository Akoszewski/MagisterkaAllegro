#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <memory>

struct BitmapDeleter {
    void operator()(ALLEGRO_BITMAP* bmp) const
    {
        if (bmp) {
            al_destroy_bitmap(bmp);
            bmp = nullptr;
        }
    }
};

class Image
{
public:
    Image();
    Image(const char* path, int x = 0, int y = 0);
    Image(const Image& other);

    void Center();
    void Draw();

    int x;
    int y;
    int width;
    int height;
    std::unique_ptr<ALLEGRO_BITMAP, BitmapDeleter> bmp;
};

#endif
