#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/transformations.h>
#include <stdio.h>
#include <stdlib.h>
#include <menu.h>
#include <portal.h>


#define SIZE_OF_BLOCK 30
#define POISONED_FOOD -1
#define GOLD_FOOD 2
#define APPLE_OF_DEATH 0
#define NORMAL_FOOD 1
#define GAME 3
#define ARCADE_MODE 55
#define NORMAL_MODE 66

typedef int type;
typedef int bol;



struct food_counter
{
    int normal_food;
    int gold_food;
    int apple_of_death;
    int poisoned_food;
};

struct poter
{
    int x;
    int y;
    int direction;
    struct  poter *next;
};

struct map_portal
{
    int x;
    int y;
    struct map_portal *target;
};

struct item
{
    int x;
    int y;
    int multipler;
    type typ;
    bol exist;
};

struct food_list
{
    struct item food_item;
    struct food_list *next,*previous;
};

void loose_game(struct ALLEGRO_DISPLAY *ac,struct ALLEGRO_SAMPLE *die_sound_effect,struct ALLEGRO_SAMPLE *eat_sound_effect)
{
  al_clear_to_color(al_map_rgb(0,0,0));
  ALLEGRO_FONT *font=al_load_font("Bebas-Regular.otf",150,NULL);
  al_draw_text(font,al_map_rgb(255,51,204),450,450,ALLEGRO_ALIGN_CENTER,"GAME OVER");
  al_flip_display();
  al_play_sample(die_sound_effect,1.0f,0.0f,1.0f,ALLEGRO_PLAYMODE_ONCE,NULL);
  al_rest(5.0f);
  al_destroy_sample(die_sound_effect);
  al_destroy_sample(eat_sound_effect);
 if(al_is_keyboard_installed())
  al_uninstall_keyboard();
  al_destroy_font(font);
 if(al_is_audio_installed())
  al_uninstall_audio();
}

void show_score(int score,ALLEGRO_FONT *font)
{
    char *txt=malloc(sizeof(char)*12);
    sprintf(txt,"Score:%d",score);
    al_draw_text(font,al_map_rgb(0,0,0),770,0,ALLEGRO_ALIGN_CENTER,txt);
    free(txt);
}

void show_progress(int to_complete_level,ALLEGRO_FONT *font)
{
    char *txt=malloc(sizeof(char)*12);
    sprintf(txt,"PROG.%d",to_complete_level);
    al_draw_text(font,al_map_rgb(0,0,0),770,50,ALLEGRO_ALIGN_CENTER,txt);
    free(txt);
}
void show_speed(int to_complete_level,ALLEGRO_FONT *font)
{
    char *txt=malloc(sizeof(char)*12);
    sprintf(txt,"SPEED: %d",to_complete_level);
    al_draw_text(font,al_map_rgb(0,0,0),770,50,ALLEGRO_ALIGN_CENTER,txt);
    free(txt);
}
void push(struct poter **ls,int x,int y,int where)
{
    struct poter *qt=(struct poter *)malloc(sizeof(struct poter));
    if(qt==NULL)
        exit(-2);
    qt->x=x;
    qt->y=y;
    qt->next=*ls;
    qt->direction=where;
    *ls=qt;
}

void push_food(struct food_list **ls,struct item fo)
{
    struct food_list *food=(struct food_list *)malloc(sizeof(struct food_list));
    if(food==NULL)
        exit(-2);
    if(*ls!=NULL)
      (*ls)->previous=food;

    food->food_item=fo;
    food->next=*ls;
    food->previous=NULL;
    *ls=food;
}

bool find_food_place(struct poter *ls,struct food_list *food_xy,int *x1,int *y1)
{
    struct poter *ls_head=ls;
    struct food_list *food_xy_head=food_xy;
    int x=(rand()%21)*40,y=(rand()%21)*40;
    bool done=true;
    bool is_good=true;
    int i=0;
    while(done && i<=50)
    {
      while(ls!=NULL)
      {
          if(ls->x==x && ls->y==y)
            {
                is_good=false;
                ls=ls_head;
                break;
            }
         ls=ls->next;
      }
      while(food_xy!=NULL)
      {
          if(food_xy->food_item.x==x && food_xy->food_item.y==y)
          {
              is_good=false;
              food_xy=food_xy_head;
              break;
          }
          food_xy=food_xy->next;
      }
       if(is_good==true)
            {
                *x1=(x/40);
                *y1=(y/40);
                return true;
            }
       else
           {
               x=(rand()%21)*40;
               y=(rand()%21)*40;
               is_good=true;
           }
        i++;
    }
    return false;
}


void remove_food(struct food_list **la,struct food_list *to_del)
{
    struct food_list *buff=*la;
    if(*la==NULL)
        return;

    do{
        if((*la)==to_del)
           {
               struct food_list *buff2=(struct food_list *)malloc(sizeof(struct food_list));
               buff2=(*la)->next;
               if((*la)->previous==NULL)
               {
                   if(buff2!=NULL)
                     buff2->previous=NULL;
                   free(*la);
                   *la=buff2;
                   return;
               }
               else
               {
                   if(buff2!=NULL)
                      buff2->previous=(*la)->previous;
                  (*la)->previous->next=buff2;
               }
                free(*la);
               *la=buff;
               return;
           }

    }while((*la=(*la)->next)!=NULL);
    *la=buff;
}


int serch_for_food(struct food_list **ls,struct item *ils)
{
    if(*ls==NULL)
        return 0;
    struct item qts=(*ls)->food_item;
    *ils=qts;
    *ls=(*ls)->next;
    return 1;
};


struct item *create_item(int x,int y,int typ)
{
    struct item *create=(struct item *)malloc(sizeof(struct item));
    if(create==NULL)
        exit(-2);
    create->x=x*40;
    create->y=y*40;
    create->typ=typ;
    create->exist=true;
    if(typ==NORMAL_FOOD)
        create->multipler=1;
    else if(typ==GOLD_FOOD)
        create->multipler=3;
    else if(typ==POISONED_FOOD)
        create->multipler=-3;
    else if(typ==APPLE_OF_DEATH)
        create->multipler=-10;
    else
        create->multipler=0;
    return create;

};

void create_food_items(struct food_counter *food_counter,struct food_list **listfood,struct poter *xc,bool exist)
{
                        struct item *ite=NULL;
                        if(food_counter->normal_food<=2)
                        {
                          int food_x=NULL,food_y=NULL;
                          exist=find_food_place(xc,*listfood,&food_x,&food_y);
                          ite=create_item(food_x,food_y,NORMAL_FOOD);
                          food_counter->normal_food++;
                          push_food(listfood,*ite);
                        }
                        if(rand()%5==0 && food_counter->gold_food<=2)
                        {
                            int food_x=NULL,food_y=NULL;
                            exist=find_food_place(xc,*listfood,&food_x,&food_y);
                            ite=create_item(food_x,food_y,GOLD_FOOD);
                            food_counter->gold_food++;
                            push_food(listfood,*ite);
                            free(ite);
                        }else if(rand()%10==0 && food_counter->poisoned_food<=1)
                        {
                            int food_x=NULL,food_y=NULL;
                            exist=find_food_place(xc,*listfood,&food_x,&food_y);
                            ite=create_item(food_x,food_y,POISONED_FOOD);
                            food_counter->poisoned_food++;
                            push_food(listfood,*ite);
                            free(ite);
                        }else if(rand()%10==0 && food_counter->apple_of_death<1)
                        {
                            int food_x=NULL,food_y=NULL;
                            find_food_place(xc,*listfood,&food_x,&food_y);
                            ite=create_item(food_x,food_y,APPLE_OF_DEATH);
                            food_counter->apple_of_death++;
                            push_food(listfood,*ite);
                            free(ite);

                        }

}


struct data_storage init_game(struct ALLEGRO_DISPLAY *display,struct data_storage data_storage)
{
    int x=440,y=440,length=2,x_anim=80,y_anim=80;
    int speed_level=12;
    int anim_counter=0;
    struct poter *xc=(struct poter *)malloc(sizeof(struct poter));
    struct ALLEGRO_EVENT_QUEUE *event_queue=NULL;
    struct ALLEGRO_KEYBOARD_STATE *state=NULL;

    struct ALLEGRO_TIMER *times=al_create_timer((24-speed_level)/100.0f);
    struct ALLEGRO_TIMER *snake_anime=al_create_timer((24-speed_level)/1000.0f);
    struct ALLEGRO_TIMER *faster=NULL;

    struct ALLEGRO_SAMPLE *die_sound_effect=NULL;
    struct ALLEGRO_SAMPLE *eat_sound_effect=NULL;

    struct item *ite=NULL;
    struct food_list *listfood=NULL;
    struct food_counter food_counter={0,0,0,0};

    struct map_portal *map_portal[4]={NULL};
    create_portal(data_storage,&map_portal);;

    if(map_portal==NULL)
        exit(-2);

    int where=ALLEGRO_KEY_RIGHT;
    int score=0;
    if(data_storage.game.level!=1)
        score=data_storage.game.score;

    int animation_frequency=10;
    bool is_food_on_map=true;
    struct ALLEGRO_BITMAP *bitmap_background=NULL;

    if(data_storage.game.level==2)
        bitmap_background=al_load_bitmap("maps\\level_2_map_background.bmp");
    else if(data_storage.game.level==3)
        bitmap_background=al_load_bitmap("maps\\level_3_map_background.bmp");
    else
        bitmap_background=al_load_bitmap("maps\\level_1_map_background.bmp");

    struct ALLEGRO_BITMAP *bitmap_normal_apple=al_load_bitmap("map_items\\normal_apple.bmp");
    struct ALLEGRO_BITMAP *bitmap_gold_apple=al_load_bitmap("map_items\\gold_apple.bmp");
    struct ALLEGRO_BITMAP *bitmap_green_apple=al_load_bitmap("map_items\\green_apple.bmp");
    struct ALLEGRO_BITMAP *bitmap_black_apple=al_load_bitmap("map_items\\black_apple.bmp");

    struct ALLEGRO_BITMAP *head_element=al_load_bitmap("snake_pic\\head_element.bmp");
    struct ALLEGRO_BITMAP *end_tail_element=al_load_bitmap("snake_pic\\end_tail_element.bmp");

    struct ALLEGRO_BITMAP *bitmap_portal=al_load_bitmap("map_items\\portal.bmp");

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);//usuwa bia³e t³o z bitmapy

    if(head_element==NULL || end_tail_element==NULL)
        exit(-5);
    if(bitmap_portal==NULL)
        exit(-5);
    if(bitmap_normal_apple==NULL)
        exit(-5);
    if(bitmap_background==NULL)
       exit(-5);
    if(bitmap_gold_apple==NULL)
       exit(-5);
    if(bitmap_black_apple==NULL)
       exit(-5);
    if(bitmap_green_apple==NULL)
       exit(-5);
    if(!al_is_keyboard_installed())
     al_install_keyboard();

    if(!al_is_audio_installed())
      al_install_audio();
    al_init_acodec_addon();
    if(!al_reserve_samples(2))
        exit(-4);
    die_sound_effect=al_load_sample("sound_effect\\lose_sound_effect.ogg");
    if(die_sound_effect==NULL)
        exit(-5);
    eat_sound_effect=al_load_sample("sound_effect\\eat_sound_effect.ogg");
    if(eat_sound_effect==NULL)
        exit(-5);
    event_queue = al_create_event_queue();

    srand(time(NULL));
    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT *font=al_load_font("Bebas-Regular.otf",50,NULL);

    al_register_event_source(event_queue,al_get_keyboard_event_source());

    al_register_event_source(event_queue,al_get_timer_event_source(times));
    al_register_event_source(event_queue,al_get_timer_event_source(snake_anime));

    al_clear_to_color(al_map_rgb(255,255,2));
    al_draw_bitmap(bitmap_background,0,0,ALLEGRO_FLIP_HORIZONTAL);
    al_draw_filled_rectangle(x,y,x+40,y+40,al_map_rgb(255,3,2));
    ite=create_item(rand()%11,rand()%11,NORMAL_FOOD);
    push_food(&listfood,*ite);
    food_counter.normal_food++;

    push(&xc,x,y,ALLEGRO_KEY_RIGHT);

    xc->x=x;
    xc->y=y;
    xc->next=NULL;

    push(&xc,x,y,ALLEGRO_KEY_RIGHT);
    al_flip_display();
    al_start_timer(snake_anime);
    al_start_timer(times);
    if(data_storage.game.type_of_game==ARCADE_MODE)
    {
        faster=al_create_timer(10.0f);
        al_register_event_source(event_queue,al_get_timer_event_source(faster));
        al_start_timer(faster);
    }
    while(1)
    {
        int i=0;
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        control_snake(&x,&y,&animation_frequency,&xc,&where,length,times,ev,display);

        if(ev.timer.source==times)
        {
            struct poter *dk=xc;
            check_portal(&x,&y,xc,map_portal);


        dk=xc;
        i=0;
        for(i=1;i<=length && dk != NULL;i++)
        {
             if(xc->x==dk->x && xc->y==dk->y && i!=1 || (length<=0))
             {
                  al_destroy_bitmap(bitmap_background);
                  al_destroy_bitmap(bitmap_normal_apple);
                  al_destroy_bitmap(bitmap_black_apple);
                  al_destroy_bitmap(bitmap_green_apple);
                  al_destroy_bitmap(bitmap_gold_apple);
                  al_destroy_font(font);
                  al_destroy_event_queue(event_queue);
                  al_stop_timer(times);
                  al_destroy_timer(times);
                  al_stop_timer(snake_anime);
                  al_destroy_timer(snake_anime);
                   if(faster!=NULL)
                  {
                      al_stop_timer(faster);
                      al_destroy_timer(faster);
                  }
                  loose_game(display,die_sound_effect,eat_sound_effect);
                  data_storage.from=GAME;
                  if((length>(data_storage.game.level*15)))
                     data_storage.game.complete=1;
                  else
                     data_storage.game.complete=0;
                  data_storage.game.score=score;
                  data_storage.game.type_of_game=NORMAL_MODE;
                  return data_storage;
             }
             dk=dk->next;
        }

        {
               struct food_list *food_list_check=listfood;
               struct item check_food;
               struct food_list *food_list_copy=listfood;


                while(serch_for_food(&food_list_check,&check_food))
                {
                    if((check_food.x >= (xc->x-SIZE_OF_BLOCK) && check_food.x<=(xc->x+SIZE_OF_BLOCK)) && (check_food.y>= (xc->y-SIZE_OF_BLOCK) && check_food.y<=(xc->y+SIZE_OF_BLOCK)))
                    {

                        if(check_food.typ==APPLE_OF_DEATH || length<=0)
                        {
                             al_destroy_bitmap(bitmap_background);
                             al_destroy_bitmap(bitmap_normal_apple);
                             al_destroy_bitmap(bitmap_black_apple);
                             al_destroy_bitmap(bitmap_green_apple);
                             al_destroy_bitmap(bitmap_gold_apple);
                             al_destroy_font(font);
                             al_destroy_event_queue(event_queue);
                             al_stop_timer(times);
                             al_destroy_timer(times);

                              if(faster!=NULL)
                              {
                               al_stop_timer(faster);
                               al_destroy_timer(faster);
                              }

                             al_stop_timer(snake_anime);
                             al_destroy_timer(snake_anime);
                             loose_game(display,die_sound_effect,eat_sound_effect);
                             data_storage.from=GAME;
                             if((length>(data_storage.game.level*15)))
                               data_storage.game.complete=1;
                             else
                               data_storage.game.complete=0;
                             data_storage.game.score=score;
                             data_storage.game.type_of_game=NORMAL_MODE;
                             return data_storage;
                        }
                        else
                        {
                            al_play_sample(eat_sound_effect,1.0f,0.0f,1.0f,ALLEGRO_PLAYMODE_ONCE,NULL);
                            score+=check_food.multipler*length*data_storage.game.level;
                            length+=check_food.typ;

                            if(check_food.typ==NORMAL_FOOD)
                                food_counter.normal_food--;
                            else if(check_food.typ==GOLD_FOOD)
                                food_counter.gold_food--;
                            else if(check_food.typ==POISONED_FOOD)
                                food_counter.poisoned_food--;

                        }
                        remove_food(&listfood,food_list_copy);
                        create_food_items(&food_counter,&listfood,xc,&is_food_on_map);
                        break;
                    }
                    if((food_list_copy=food_list_copy->next)==NULL)
                       break;

                }
                if(!is_food_on_map)
                  create_food_items(&food_counter,&listfood,xc,&is_food_on_map);

        }


        }
        if(ev.timer.source==snake_anime)
        {
         int i=0;
         struct poter *dk=xc;


          al_clear_to_color(al_map_rgb(255,255,2));
          al_draw_bitmap(bitmap_background,0,0,ALLEGRO_FLIP_HORIZONTAL);
         struct item *food_list_check=listfood;
         struct item check_food;
         struct poter *kod=NULL;
         for(i;i<length && dk!=NULL;i++,dk=dk->next)
         {
             anim_counter=animation_frequency*4;
            if(dk==xc)
            {
                if(dk->direction==ALLEGRO_KEY_RIGHT)
               al_draw_rotated_bitmap(head_element,0,40,dk->x-anim_counter,dk->y,ALLEGRO_PI/2,ALLEGRO_FLIP_VERTICAL);
             else if(dk->direction==ALLEGRO_KEY_LEFT)
               al_draw_rotated_bitmap(head_element,40,0,dk->x+anim_counter,dk->y,ALLEGRO_PI/2+ALLEGRO_PI,ALLEGRO_FLIP_VERTICAL);
             else if(dk->direction==ALLEGRO_KEY_UP)
               al_draw_rotated_bitmap(head_element,0,0,dk->x,dk->y+anim_counter,0,ALLEGRO_FLIP_VERTICAL);
             else if(dk->direction==ALLEGRO_KEY_DOWN)
                al_draw_rotated_bitmap(head_element,40,40,dk->x,dk->y-anim_counter,ALLEGRO_PI,ALLEGRO_FLIP_VERTICAL);
                continue;
            }
            else if(dk->next==NULL)
            {
                if(dk->direction==ALLEGRO_KEY_RIGHT)
               al_draw_rotated_bitmap(end_tail_element,0,40,dk->x-anim_counter,dk->y,ALLEGRO_PI/2,ALLEGRO_FLIP_VERTICAL);
             else if(dk->direction==ALLEGRO_KEY_LEFT)
               al_draw_rotated_bitmap(end_tail_element,40,0,dk->x+anim_counter,dk->y,ALLEGRO_PI/2+ALLEGRO_PI,ALLEGRO_FLIP_VERTICAL);
             else if(dk->direction==ALLEGRO_KEY_UP)
               al_draw_rotated_bitmap(end_tail_element,0,0,dk->x,dk->y+anim_counter,0,ALLEGRO_FLIP_VERTICAL);
             else if(dk->direction==ALLEGRO_KEY_DOWN)
                al_draw_rotated_bitmap(end_tail_element,40,40,dk->x,dk->y-anim_counter,ALLEGRO_PI,ALLEGRO_FLIP_VERTICAL);
                continue;
            }


          if(dk->direction==ALLEGRO_KEY_RIGHT)
           al_draw_filled_rectangle(dk->x-anim_counter,dk->y,dk->x+40-anim_counter,dk->y+40,al_map_rgb(0,255,130));
          else if(dk->direction==ALLEGRO_KEY_LEFT)
            al_draw_filled_rectangle(dk->x+anim_counter,dk->y,dk->x+40+anim_counter,dk->y+40,al_map_rgb(0,255,130));
          else if(dk->direction==ALLEGRO_KEY_UP)
            al_draw_filled_rectangle(dk->x,dk->y+anim_counter,dk->x+40,dk->y+40+anim_counter,al_map_rgb(0,255,130));
          else if(dk->direction==ALLEGRO_KEY_DOWN)
            al_draw_filled_rectangle(dk->x,dk->y-anim_counter,dk->x+40,dk->y+40-anim_counter,al_map_rgb(0,255,130));
          else
            al_draw_filled_rectangle(dk->x,dk->y,dk->x+40,dk->y+40,al_map_rgb(0,255,130));
         }
           animation_frequency--;

         if(food_list_check!=NULL)
           while(serch_for_food(&food_list_check,&check_food))
            if(check_food.typ==NORMAL_FOOD)
             al_draw_bitmap(bitmap_normal_apple,check_food.x,check_food.y,ALLEGRO_FLIP_HORIZONTAL);
            else if(check_food.typ==GOLD_FOOD)
             al_draw_bitmap(bitmap_gold_apple,check_food.x,check_food.y,ALLEGRO_FLIP_HORIZONTAL);
            else if(check_food.typ==POISONED_FOOD)
             al_draw_bitmap(bitmap_green_apple,check_food.x,check_food.y,ALLEGRO_FLIP_HORIZONTAL);
            else if(check_food.typ==APPLE_OF_DEATH)
             al_draw_bitmap(bitmap_black_apple,check_food.x,check_food.y,ALLEGRO_FLIP_HORIZONTAL);


               for(i=0;i<4 && map_portal[i]!=NULL;i++)
                al_draw_bitmap(bitmap_portal,map_portal[i]->x,map_portal[i]->y,ALLEGRO_FLIP_HORIZONTAL);
            show_score(score,font);
            if(data_storage.game.type_of_game==NORMAL_MODE)
            {
                if((i=data_storage.game.level*15-length)<=0)
                    i=0;
                show_progress(i,font);
            }else if(data_storage.game.type_of_game==ARCADE_MODE)
              show_speed(speed_level,font);

             al_flip_display();
        }

        if(ev.timer.source==faster && ev.timer.source!=NULL && speed_level<=17)
        {
            speed_level+=1;
            al_set_timer_speed(times,(24-speed_level)/100.0f);
            al_set_timer_speed(snake_anime,(24-speed_level)/1000.0f);
        }


      }
    }
