#include "App.h"

App::App(int size_x, int size_y)
  : width(size_x), height(size_y)
{
    al_init();
    al_install_keyboard();

    queue = al_create_event_queue();
    disp = al_create_display(size_x, size_y);
    font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));

    if (!al_init_image_addon()) {
        printf("couldn't initialize image addon\n");
        return;
    }

    image = al_load_bitmap("pictures/roi.bmp");
    if (!image) {
        printf("Couldn't load image\n");
        return;
    }
}

void App::Run()
{
    while (true)
    {
        al_get_next_event(queue, &event);
        if((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) {
            break;
        }
        al_clear_to_color(al_map_rgb(0, 50, 255));
        // al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
        al_draw_bitmap(image, (width - al_get_bitmap_width(image))/2, (height - al_get_bitmap_height(image))/2, 0);
        al_flip_display();
    }
}

App::~App()
{
    al_destroy_bitmap(image);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
}
