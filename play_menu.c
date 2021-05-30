
#include <stdlib.h>
#include <stdio.h>

#define buttons_pixel_interval 20
#define ALLEGRO_EVENT_MOUSE_LEFT_CLICKED 1
#define PLAY_MENU 4
#include <allegro5/color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <menu.h>

struct button
{
    char *name;
    float x_center;
    float y_center;
    float width;
    float height;
    struct data_storage (*what_to_do)(struct ALLEGRO_DISPLAY *,struct data_storage);

};
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

struct data_storage init_play_menu(struct ALLEGRO_DISPLAY *dp,struct data_storage data_storage)
{
    ALLEGRO_EVENT_QUEUE *event_q=NULL;
    int i=0;
    struct button *buttons=malloc(sizeof(struct button)*3);

    if(!al_is_mouse_installed())
     al_install_mouse();

    al_clear_to_color(al_map_rgb(255,204,204));
    al_init_font_addon();
    al_init_ttf_addon();
    make_button(300.0f,100.0f,450.0f,250.0f,al_map_rgb(255,4,0),"ARCADE",&buttons[0],NULL);
    make_button(300.0f,100.0f,450.0f,350.0f + buttons_pixel_interval,al_map_rgb(255,0,0),"NORMAL",&buttons[1],NULL);
    make_button(300.0f,100.0f,450.0f,450.0f + buttons_pixel_interval*2,al_map_rgb(255,0,0),"BACK",&buttons[2],NULL);

    event_q=al_create_event_queue();
    al_register_event_source(event_q,al_get_display_event_source(dp));
    al_register_event_source(event_q,al_get_mouse_event_source());
    al_flip_display();
    int dt=1;
    while(1)
    {
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_q,&ev);
            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                 al_destroy_display(dp);
                 al_uninstall_mouse();
                }
                else if(ev.mouse.button==ALLEGRO_EVENT_MOUSE_LEFT_CLICKED && ev.mouse.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                  for(i=0;i<3;i++)
                    if(clicked(&buttons[i],ev,dp))
                    {
                        al_destroy_event_queue(event_q);
                        destroy_score_table();
                        data_storage.from=PLAY_MENU;
                        data_storage.game.complete=0;
                        data_storage.game.level=0;
                        data_storage.game.score=0;
                        if(buttons[i].name[0]=='A')
                         data_storage.game.type_of_game=55;
                        else if(buttons[i].name[0]=='N')
                         data_storage.game.type_of_game=66;
                        else
                          data_storage.game.type_of_game=0;
                        return data_storage;
                    }
                }
    }
}
