#include <allegro5/allegro5.h>

struct poter
{
    int x;
    int y;
    int direction;
    struct  poter *next;
};
void control_snake(int *x,int *y,int *animation_frequency,struct poter **xc,int *where,int length,struct ALLEGRO_TIMER *times,ALLEGRO_EVENT ev,struct ALLEGRO_DISPLAY *display)
{

       if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
            switch(ev.keyboard.keycode)
        {
        case ALLEGRO_KEY_DOWN:
            if((*xc)->next!=NULL)
                    if((*xc)->x==(*xc)->next->x && (*xc)->y<(*xc)->next->y || ((*xc)->y-(*xc)->next->y)>40)
                       break;
            *where=ALLEGRO_KEY_DOWN;
            break;
        case ALLEGRO_KEY_UP:
            if((*xc)->next!=NULL)
                    if((*xc)->x==(*xc)->next->x && (*xc)->y>(*xc)->next->y || ((*xc)->next->y-(*xc)->y)>40)
                       break;
            *where=ALLEGRO_KEY_UP;
            break;
        case ALLEGRO_KEY_LEFT:
            if((*xc)->next!=NULL)
                    if((*xc)->y==(*xc)->next->y && (*xc)->x>(*xc)->next->x || ((*xc)->next->x - (*xc)->x)>40)
                       break;
            *where=ALLEGRO_KEY_LEFT;
            break;
        case ALLEGRO_KEY_RIGHT:
            if((*xc)->next!=NULL)
                    if((*xc)->y==(*xc)->next->y && (*xc)->x<(*xc)->next->x || ((*xc)->x - (*xc)->next->x)>40)
                       break;
            *where=ALLEGRO_KEY_RIGHT;
            break;
        case ALLEGRO_KEY_ESCAPE:
            *where=ALLEGRO_KEY_ESCAPE;
            break;

        }

        if(ev.timer.source==times)
        {
            *animation_frequency=10;

              switch(*where)
            {
            case ALLEGRO_KEY_UP:
              if((*xc)->y<0)
              {
                *y=920;
                *where=ALLEGRO_KEY_UP;
              }
              else
              {
                *y-=40;
                *where=ALLEGRO_KEY_UP;
              }
                break;
            case ALLEGRO_KEY_DOWN:
                if((*xc)->y>900)
                {
                   *y=0;
                   *where=ALLEGRO_KEY_DOWN;
                }
                else
                {
                   *y+=40;
                   *where=ALLEGRO_KEY_DOWN;
                }
                break;
            case ALLEGRO_KEY_LEFT:
                if((*xc)->x<0)
                {
                    *x=920;
                    *where=ALLEGRO_KEY_LEFT;
                }
                 else
                 {
                    *x-=40;
                    *where=ALLEGRO_KEY_LEFT;
                 }
                break;
            case ALLEGRO_KEY_RIGHT:
                if((*xc)->x>900)
                {
                  *x=0;
                  *where=ALLEGRO_KEY_RIGHT;
                }
                else
                {
                  *x+=40;
                  *where=ALLEGRO_KEY_RIGHT;
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
                al_destroy_display(display);
                al_uninstall_keyboard();
                exit(-2);
            }
             push(xc,*x,*y,*where);
             struct poter *dk=*xc;

             int i=0;
             struct poter *del;
             for(i;i<length && (*xc)!=NULL ;i++)
                if(i==length-1 && (*xc)->next!=NULL)
                {
                    del=(*xc)->next;
                    (*xc)->next=NULL;
                    while(del!=NULL)
                    {
                        struct poter *ct=del->next;
                        free(del);
                        del=ct;
                    }

                }
                else
                    (*xc)=(*xc)->next;
                *xc=dk;



       }
}
