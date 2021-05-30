#include <stdlib.h>
#include <allegro5/allegro5.h>

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

void create_portal(struct data_storage data_storage,struct map_portal **map_portal)
{
     if(data_storage.game.level==2)
    {
         map_portal[0]=(struct map_portal *)malloc(sizeof(struct map_portal));
         map_portal[1]=(struct map_portal *)malloc(sizeof(struct map_portal));
        if(map_portal[0]==NULL || map_portal[1]==NULL)
            exit(-2);
        map_portal[0]->x=80;
        map_portal[0]->y=720;
        map_portal[1]->x=720;
        map_portal[1]->y=80;

        map_portal[0]->target=map_portal[1];
        map_portal[1]->target=map_portal[0];
    }else if(data_storage.game.level==3)
    {

        map_portal[0]=(struct map_portal *)malloc(sizeof(struct map_portal));
        map_portal[1]=(struct map_portal *)malloc(sizeof(struct map_portal));
        map_portal[2]=(struct map_portal *)malloc(sizeof(struct map_portal));
        map_portal[3]=(struct map_portal *)malloc(sizeof(struct map_portal));
        if(map_portal[0]==NULL || map_portal[1]==NULL || map_portal[2]==NULL || map_portal[3]==NULL)
            exit(-2);
        map_portal[0]->x=80;
        map_portal[0]->y=720;
        map_portal[1]->x=720;
        map_portal[1]->y=80;
        map_portal[2]->x=80;
        map_portal[2]->y=80;
        map_portal[3]->x=720;
        map_portal[3]->y=720;

        map_portal[0]->target=map_portal[1];
        map_portal[1]->target=map_portal[0];
        map_portal[2]->target=map_portal[3];
        map_portal[3]->target=map_portal[2];

    }


}

void check_portal(int *x,int *y,struct poter *xc,struct map_portal *map_portal[4])
{
    int i=0;
            for(i=0;i<4 && map_portal[i]!=NULL;i++)
                if(*x>=map_portal[i]->x && *x<map_portal[i]->x+80 && *y>=map_portal[i]->y && *y<map_portal[i]->y+80)
            {
              if(xc->direction==ALLEGRO_KEY_LEFT)
              {
                *x=map_portal[i]->target->x-40;
                *y=map_portal[i]->target->y;
              }
              else if(xc->direction==ALLEGRO_KEY_RIGHT)
              {
                *x=map_portal[i]->target->x+80;
                *y=map_portal[i]->target->y;
              }
              else if(xc->direction==ALLEGRO_KEY_UP)
              {
                  *x=map_portal[i]->target->x;
                  *y=map_portal[i]->target->y-40;
              }
              else if(xc->direction==ALLEGRO_KEY_DOWN)
              {
                  *x=map_portal[i]->target->x;
                  *y=map_portal[i]->target->y+80;
              }
            }
}
