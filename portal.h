#ifndef PORTAL_H_INCLUDED
#define PORTAL_H_INCLUDED

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


void check_portal(int *,int *,struct poter *,struct map_portal *);
void create_portal(struct data_storage,struct map_portal *(*)[4]);
#endif // PORTAL_H_INCLUDED
