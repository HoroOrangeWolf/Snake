#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED




void init_menu(struct ALLEGRO_DISPLAY *,struct data_storage);
void make_button(float,float,float,float,struct ALLEGRO_COLOR,char *,struct button *,void (*function)(struct ALLEGRO_DISPLAY *));
int clicked(struct button *,const ALLEGRO_EVENT,struct ALLEGRO_DISPLAY *);

#endif // MENU_H_INCLUDED
