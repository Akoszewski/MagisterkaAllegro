#include "App.h"

App::App(int size_x, int size_y)
  : width(size_x), height(size_y), isClosed(false)
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

    image = new Image();
    image->Center();
}

void App::Run()
{
    while (!isClosed)
    {
        al_get_next_event(queue, &event);
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            switch (event.keyboard.keycode)
            {  
                case ALLEGRO_KEY_ESCAPE:
                    isClosed = true;
                    break;
                case ALLEGRO_KEY_W:
                    image->y--;
                    break;
                case ALLEGRO_KEY_A:
                    image->x--;
                    break;
                case ALLEGRO_KEY_S:
                    image->y++;
                    break;
                case ALLEGRO_KEY_D:
                    image->x++;
                    break;
                case ALLEGRO_KEY_F:
                    al_toggle_display_flag(disp, ALLEGRO_FULLSCREEN_WINDOW, !(al_get_display_flags(disp) & ALLEGRO_FULLSCREEN_WINDOW));
                    break;
            }
        }

        al_clear_to_color(al_map_rgb(0, 50, 255));

        image->Draw();
        // al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
        
        al_flip_display();
    }
}

App::~App()
{
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
}
