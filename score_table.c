
#include <allegro5/color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <menu.h>

#define ALLEGRO_EVENT_MOUSE_LEFT_CLICKED 1
#define SCORE_TABLE 2

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
    void (*what_to_do)(struct ALLEGRO_DISPLAY *);
};


void save_score(int score,int gdzie)
{

    FILE *fp=NULL;
    if(gdzie==1)
        fp=fopen("data/score_table_arcade.txt","r");
    else
        fp=fopen("data/score_table.txt","r");
    if(fp==NULL)
        {
            int q=0;
            DIR *dir=NULL;
            dir=opendir("data");
            if(dir==NULL)
                system("mkdir data");
            else
                closedir(dir);
            if(gdzie==1)
             fp=fopen("data/score_table_arcade.txt","w");
            else
             fp=fopen("data/score_table.txt","w");

            fclose(fp);
            if(gdzie==1)
             fp=fopen("data/score_table_arcade.txt","r");
            else
             fp=fopen("data/score_table.txt","r");

            if(fp==NULL)
                exit(-3);
        }
      int i=0;
      int tab[10]={0};
      for(i;i<10 && !feof(fp);i++)
        fscanf(fp,"%d",&tab[i]);
        fclose(fp);

        if(gdzie==1)
         fp=fopen("data/score_table_arcade.txt","w");
        else
         fp=fopen("data/score_table.txt","w");

      for(i=0;i<10;i++)
      {
          if(score>tab[i])
          {
              fprintf(fp,"%\n%d",score);
              for(i;i<9;i++)
                fprintf(fp,"\n%d",tab[i]);
          }
          else
            fprintf(fp,"\n%d",tab[i]);

      }
      fclose(fp);

}
int *get_score_table(int gdzie)
{
    int *tab=(int *)calloc(10,sizeof(int));
    FILE *fp=NULL;
    if(gdzie==1)
     fp=fopen("data/score_table_arcade.txt","r");
    else
     fp=fopen("data/score_table.txt","r");
    if(fp==NULL)
        return tab;
    int i=0;
    for(i;i<10 && !feof(fp);i++)
       fscanf(fp,"%d",&tab[i]);
    fclose(fp);
    return tab;
}

void print_score(int *tab,int gdzie)
{
    int i=1;
    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font=al_load_ttf_font("Bebas-Regular.otf",40,NULL);
    char *txt=(char *)malloc(sizeof(char)*7);
    if(gdzie==1)
     al_draw_text(font,al_map_rgb(0,0,0),250,20,ALLEGRO_ALIGN_CENTER,"SCORE TABLE ARCADE");
    else
     al_draw_text(font,al_map_rgb(0,0,0),650,20,ALLEGRO_ALIGN_CENTER,"SCORE TABLE NORMAL");

    for(i;i<=10;i++)
    {
        sprintf(txt,"%d",tab[i-1]);
      if(gdzie==1)
        al_draw_text(font,al_map_rgb(0,0,0),250,70*i,ALLEGRO_ALIGN_CENTER,txt);
      else
        al_draw_text(font,al_map_rgb(0,0,0),650,70*i,ALLEGRO_ALIGN_CENTER,txt);
    }
    free(tab);
    free(txt);
    al_destroy_font(font);
}


void destroy_score_table()
{
    if(al_is_mouse_installed())
        al_uninstall_mouse();
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
}


struct data_storage init_score_table(struct ALLEGRO_DISPLAY *dp,struct data_storage data_storage)
{
    ALLEGRO_EVENT_QUEUE *event_q=NULL;
    struct button *buttons=malloc(sizeof(struct button));

    if(!al_is_mouse_installed())
     al_install_mouse();

    al_clear_to_color(al_map_rgb(255,204,204));
    al_init_font_addon();
    al_init_ttf_addon();

    print_score(get_score_table(1),1);
    print_score(get_score_table(2),2);
    make_button(500.0f,100.0f,450.0f,820.0f,al_map_rgb(255,4,0),"EXIT TO MENU",buttons,&init_menu);

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
                    if(clicked(buttons,ev,dp))
                    {
                        al_destroy_event_queue(event_q);
                        destroy_score_table();
                        data_storage.from=SCORE_TABLE;
                        data_storage.game.complete=0;
                        data_storage.game.level=0;
                        data_storage.game.score=0;
                        data_storage.game.type_of_game=0;
                        return data_storage;
                    }
                }
    }
}

