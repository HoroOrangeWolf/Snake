#include <game_engine.h>
#include <score_table.h>
#include <play_menu.h>

#define count_buttons 3
#define buttons_pixel_interval 20

#define ALLEGRO_EVENT_MOUSE_LEFT_CLICKED 1


#include <allegro5/color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

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
struct button
{
    char *name;
    float x_center;
    float y_center;
    float width;
    float height;
    struct data_storage (*what_to_do)(struct ALLEGRO_DISPLAY *,struct data_storage);

};

void destroy_menu()
{
    al_uninstall_mouse();
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
}

void make_button(float width,float height,float x_center,float y_center,struct ALLEGRO_COLOR utc,char *text,struct button *tc,struct data_storage (*function)(struct ALLEGRO_DISPLAY *))
{
    al_draw_filled_rectangle(x_center-(width/2),y_center-(height/2),x_center+(width/2),y_center+(height/2),utc);
    ALLEGRO_FONT *font=al_load_ttf_font("Bebas-Regular.otf",height,NULL);
    al_draw_text(font,al_map_rgb(0,0,0),x_center,y_center-height*0.65,ALLEGRO_ALIGN_CENTER,text);
    tc->x_center=x_center;
    tc->y_center=y_center;
    tc->width=width;
    tc->height=height;
    tc->what_to_do=function;
    tc->name=text;
    al_destroy_font(font);
}

void exit_menu(struct ALLEGRO_DISPLAY *dp)
{
    al_set_target_bitmap(NULL);
    if(al_is_audio_installed())
        al_uninstall_audio();
    exit(-2);
}

int clicked(struct button *ls,const ALLEGRO_EVENT ev,struct ALLEGRO_DISPLAY *dp)
{
    int mouse_x=ev.mouse.x;
    int mouse_y=ev.mouse.y;
    if((ls->x_center-((ls->width)/2)<=mouse_x) && (ls->x_center+((ls->width)/2)>=mouse_x) && (ls->y_center-((ls->height)/2)<=mouse_y) && (ls->y_center+((ls->height)/2)>=mouse_y))
     return 1;


    return 0;

}
void back_to_menu(struct ALLEGRO_DISPLAY *display,struct button **buttons,struct ALLEGRO_SAMPLE **music,ALLEGRO_EVENT **event_q)
{
     if(!al_is_mouse_installed())
       al_install_mouse();
    al_clear_to_color(al_map_rgb(255,204,204));
    if(!al_is_audio_installed())
    {
        al_install_audio();
        if(*music!=NULL)
          free(*music);
        if(*buttons!=NULL)
          free(*buttons);

          al_reserve_samples(1);
          al_init_acodec_addon();

          *music=al_load_sample("music\\Buttercup.ogg");
        if(*music==NULL)
         exit(-5);
          al_play_sample(*music,1.0f,0.0f,1.0f,ALLEGRO_PLAYMODE_LOOP,NULL);
    }

        if(*buttons!=NULL)
            free(*buttons);
    *buttons=(struct button *)malloc(sizeof(struct button)*count_buttons);
    make_button(300.0f,100.0f,450.0f,250.0f,al_map_rgb(255,4,0),"PLAY",&(*buttons)[0],&init_play_menu);
    make_button(300.0f,100.0f,450.0f,350.0f + buttons_pixel_interval,al_map_rgb(255,0,0),"SCORES",&(*buttons)[1],&init_score_table);
    make_button(300.0f,100.0f,450.0f,450.0f + buttons_pixel_interval*2,al_map_rgb(255,0,0),"EXIT",&(*buttons)[2],&exit_menu);
    if(*event_q!=NULL)
        free(*event_q);

    *event_q=al_create_event_queue();
    al_register_event_source(*event_q,al_get_display_event_source(display));
    al_register_event_source(*event_q,al_get_mouse_event_source());
    al_flip_display();
}
void init_menu(struct ALLEGRO_DISPLAY *display,struct data_storage data_storage)
{
    ALLEGRO_EVENT_QUEUE *event_q=NULL;
    struct button *buttons=(struct button *)malloc(sizeof(struct button)*count_buttons);
    struct ALLEGRO_SAMPLE *music=NULL;

    data_storage.game.score=0;

    if(!al_is_mouse_installed())
     al_install_mouse();

    al_clear_to_color(al_map_rgb(255,204,204));
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();

    if(!al_is_audio_installed())
        al_install_audio();
    al_reserve_samples(1);
    al_init_acodec_addon();

    music=al_load_sample("music\\Buttercup.ogg");
    if(music==NULL)
        exit(-5);


    make_button(300.0f,100.0f,450.0f,250.0f,al_map_rgb(255,4,0),"PLAY",&buttons[0],&init_play_menu);
    make_button(300.0f,100.0f,450.0f,350.0f + buttons_pixel_interval,al_map_rgb(255,0,0),"SCORES",&buttons[1],&init_score_table);
    make_button(300.0f,100.0f,450.0f,450.0f + buttons_pixel_interval*2,al_map_rgb(255,0,0),"EXIT",&buttons[2],&exit_menu);

    event_q=al_create_event_queue();
    al_register_event_source(event_q,al_get_display_event_source(display));
    al_register_event_source(event_q,al_get_mouse_event_source());
    al_flip_display();
    al_play_sample(music,1.0f,0.0f,1.0f,ALLEGRO_PLAYMODE_LOOP,NULL);
    int dt=1;
    while(1)
    {
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_q,&ev);
            if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                 al_destroy_display(display);
                 al_uninstall_mouse();
                 return -1;
                }
                else if(ev.mouse.button==ALLEGRO_EVENT_MOUSE_LEFT_CLICKED && ev.mouse.type==ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                   int i=0;
                   for(i;i<count_buttons;i++)
                    if(clicked(&buttons[i],ev,display))
                    {
                        al_destroy_event_queue(event_q);
                        destroy_menu();
                        if(!(buttons[i].name[0]=='S'|| buttons[i].name[0]=='P'))
                        {
                          al_destroy_sample(music);
                          if(al_is_audio_installed())
                            al_uninstall_audio();
                        }
                        data_storage=buttons[i].what_to_do(display,data_storage);
                     if(data_storage.game.type_of_game==66)
                     {
                         al_destroy_sample(music);
                          if(al_is_audio_installed())
                            al_uninstall_audio();
                        data_storage.game.level=1;
                        data_storage=init_game(display,data_storage);
                        if(data_storage.game.complete==1)
                        {

                            data_storage.game.level=2;
                            data_storage=init_game(display,data_storage);

                            if(data_storage.game.complete==1)
                            {
                                data_storage.game.level=3;
                                data_storage=init_game(display,data_storage);
                                save_score(data_storage.game.score,2);
                            }
                            else
                              save_score(data_storage.game.score,2);
                        }
                        else
                            save_score(data_storage.game.score,2);
                     }
                     else if(data_storage.game.type_of_game==55)
                     {
                         al_destroy_sample(music);
                          if(al_is_audio_installed())
                            al_uninstall_audio();

                         data_storage.game.level=3;
                         data_storage=init_game(display,data_storage);
                         save_score(data_storage.game.score,1);
                     }

                      if(data_storage.from==2 || data_storage.from==3 || data_storage.from==4)
                        {
                            back_to_menu(display,&buttons,&music,&event_q);
                        }

                    }
                }
    }
}





