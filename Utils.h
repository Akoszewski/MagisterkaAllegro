#ifndef UTILS_H
#define UTILS_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <vector>

typedef unsigned char byte;
constexpr int MAX_MASK_COLORS = 9;

int getGray(ALLEGRO_COLOR color);
bool areColorsEqual(const ALLEGRO_COLOR &color1, const ALLEGRO_COLOR &color2);
int getClusterFromColor(ALLEGRO_COLOR color, std::vector<ALLEGRO_COLOR> maskColors);

#endif
