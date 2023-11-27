#ifndef UTILS_H
#define UTILS_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

typedef unsigned char byte;

int getGray(ALLEGRO_COLOR color);
bool areColorsEqual(const ALLEGRO_COLOR &color1, const ALLEGRO_COLOR &color2);

#endif
