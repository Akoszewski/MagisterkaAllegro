#ifndef APP_H
#define APP_H

#include "Mask.h"
#include "Constants.h"
#include "Segmentation/Segmentation.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

class App
{
public:
    App(int size_x, int size_y);
    ~App();
    void Run();
    int width;
    int height;
    std::unique_ptr<Segmentation> segmentation;
private:
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* disp;
    ALLEGRO_FONT* font;
    ALLEGRO_EVENT event;
    std::shared_ptr<Image> image;
    bool isClosed;
    int currentBackgroundColorIdx = 0;
    void toggleBackground();
    std::vector<ALLEGRO_COLOR> backgroundColors;
};

#endif
