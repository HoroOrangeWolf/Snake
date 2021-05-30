#define _WIN32_WINNT 0x0A000007
#include <windows.h>

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>


struct game
{
    int type_of_game;
    int level;
    int score;
    int complete;
};

struct data_storage
{
    int from;
    struct game game;
};



int main()
{
    struct ALLEGRO_DISPLAY *display=NULL;
    al_init_primitives_addon();
    if(!al_init())
        return -1;
    struct data_storage data_storage;
    ShowWindow(GetConsoleWindow(),HIDE_WINDOW);
    display=al_create_display(900,900);
    al_set_window_title(display,"SNAKE");
    init_menu(display,data_storage);
    return 0;
}
