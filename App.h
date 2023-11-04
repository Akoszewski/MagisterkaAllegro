#ifndef APP_H
#define APP_H

#include "Mask.h"
#include "Constants.h"
#include "Segmentation/Segmentation.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

class App
{
public:
    App(int size_x, int size_y);
    ~App();
    void Run();
    int width;
    int height;
    Segmentation segmentation;
private:
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* disp;
    ALLEGRO_FONT* font;
    ALLEGRO_EVENT event;
    std::shared_ptr<Image> image;
    bool isClosed;
};

#endif
