#ifndef MENU_H
#define MENU_H

#define MENU_ITEM_COUNT 5

typedef enum {
    MENU_START_GAME = 0,
    MENU_SETTINGS,
    MENU_HELP,
    MENU_PREVIOUS_RUNS,
    MENU_QUIT
} MenuItem;

void menu_draw(int selected);
const char* menu_get_item(int idx);
int menu_get_item_at(float y);
void menu_background_init(const char* filename);
void menu_background_free(void);
void menu_draw_background(void);

void bmp_flip_vertical(unsigned char* data, int width, int height);

#endif // MENU_H