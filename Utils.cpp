#include "Utils.h"

int getGray(ALLEGRO_COLOR color)
{
    byte r, g, b;
    al_unmap_rgb(color, &r, &g, &b);
    return 0.3 * r + 0.59 * g + 0.11 * b;
}
