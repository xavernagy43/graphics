#ifndef HUD_H
#define HUD_H

#include "settings.h"

void hud_start_timer(const GameSettings* settings);
void hud_update_timer(void);
void hud_draw(int score);
int hud_time_is_up(void);
void hud_reset(void);

// Új funkciók:
void hud_draw_game_over(int score);
int hud_game_over_mouse(int x, int y); // visszaadja, hogy a "Go back"-re kattintottak-e

#endif