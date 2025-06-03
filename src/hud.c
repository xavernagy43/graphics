#include "hud.h"
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

static int time_left = 0; // másodperc
static int last_tick = 0;
static int time_up = 0;

void hud_start_timer(const GameSettings* settings) {
    time_left = settings->max_time_minutes * 60 + settings->max_time_seconds;
    last_tick = (int)time(NULL);
    time_up = 0;
}

void hud_update_timer(void) {
    if (time_up || time_left <= 0) return;
    int now = (int)time(NULL);
    if (now > last_tick) {
        time_left -= (now - last_tick);
        last_tick = now;
        if (time_left <= 0) {
            time_left = 0;
            time_up = 1;
        }
    }
}

void hud_draw(int score) {
    char buf[64];

    // Jobb felső sarok: Score és idő
    glColor3f(1, 1, 1);
    glRasterPos2f(0.65f, 0.92f);
    sprintf(buf, "Score: %d", score);
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    int min = time_left / 60;
    int sec = time_left % 60;
    glRasterPos2f(0.65f, 0.85f);
    sprintf(buf, "Time: %02d:%02d", min, sec);
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Ha lejárt az idő, középre kiírjuk az eredményt
    if (time_up) {
        hud_draw_game_over(score);
    }
}

int hud_time_is_up(void) {
    return time_up;
}

void hud_reset(void) {
    time_left = 0;
    last_tick = 0;
    time_up = 0;
}

// --- Új: Game Over képernyő kirajzolása ---
void hud_draw_game_over(int score) {
    char buf[64];
    // Game Over felirat
    glColor3f(1, 0.2f, 0.2f);
    glRasterPos2f(-0.18f, 0.1f);
    const char* over = "Game Over";
    for (const char* c = over; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Total score
    glColor3f(1, 1, 0.2f);
    glRasterPos2f(-0.22f, 0.0f);
    sprintf(buf, "Total score: %d", score);
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Go back gomb
    glColor3f(0.8f, 0.8f, 1.0f);
    glRasterPos2f(-0.08f, -0.12f);
    const char* goback = "[ Go back ]";
    for (const char* c = goback; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

// --- Új: Egérkattintás kezelése Game Over képernyőn ---
// Visszaadja, hogy a Go back gombra kattintottak-e
int hud_game_over_mouse(int x, int y) {
    int win_width = glutGet(GLUT_WINDOW_WIDTH);
    int win_height = glutGet(GLUT_WINDOW_HEIGHT);

    // A "Go back" gomb kb. -0.08f, -0.12f pozíción van
    // Átalakítjuk a képernyő koordinátáira (feltételezve: gluOrtho2D(-1,1,-1,1))
    float fx = 2.0f * x / win_width - 1.0f;
    float fy = 1.0f - 2.0f * y / win_height;

    // A gomb kb. -0.08f < x < 0.32f, -0.18f < y < -0.06f tartományban van
    if (fx > -0.08f && fx < 0.32f && fy > -0.18f && fy < -0.06f) {
        return 1;
    }
    return 0;
}