#ifndef APP_H
#define APP_H

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
private:
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* disp;
    ALLEGRO_FONT* font;
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP* image;
};

#endif
