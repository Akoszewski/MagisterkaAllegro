#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <allegro5/allegro5.h>

class Image
{
public:
    Image(int x = 0, int y = 0);
    void Center();
    void Draw();
    ~Image();

    int x;
    int y;
    ALLEGRO_BITMAP *bmp;
};

#endif
