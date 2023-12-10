#include "Utils.h"

int getGray(ALLEGRO_COLOR color)
{
    byte r, g, b;
    al_unmap_rgb(color, &r, &g, &b);
    return 0.3 * r + 0.59 * g + 0.11 * b;
}

bool areColorsEqual(const ALLEGRO_COLOR &color1, const ALLEGRO_COLOR &color2)
{
    return color1.r == color2.r &&
           color1.g == color2.g &&
           color1.b == color2.b &&
           color1.a == color2.a;
}

int getClusterFromColor(ALLEGRO_COLOR color, std::vector<ALLEGRO_COLOR> maskColors)
{
    for (int i = 0; i < MAX_MASK_COLORS; i++)
    {
        if (areColorsEqual(color, maskColors[i])) {
            return i;
        }
    }
    return -1;
}
