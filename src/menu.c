#include "menu.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "texture.h"

static GLuint menu_bg_tex = 0;
static int menu_bg_width = 0, menu_bg_height = 0;

static const char* menu_items[MENU_ITEM_COUNT] = {
    "Start Game",
    "Settings",
    "Help",
    "Previous Runs",
    "Quit Game"
};

void menu_background_init(const char* filename) {
    if (menu_bg_tex) {
        glDeleteTextures(1, &menu_bg_tex);
        menu_bg_tex = 0;
    }
    menu_bg_tex = load_texture((char*)filename);
    if (!menu_bg_tex) {
        printf("Menu background not loaded: %s\n", filename);
    } else {
        printf("Menu background loaded: %s\n", filename);
    }
}

void menu_background_free(void) {
    if (menu_bg_tex) {
        glDeleteTextures(1, &menu_bg_tex);
        menu_bg_tex = 0;
    }
}

void menu_draw_background(void) {
    if (!menu_bg_tex) return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, menu_bg_tex);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-1, -1);
        glTexCoord2f(1, 1); glVertex2f( 1, -1);
        glTexCoord2f(1, 0); glVertex2f( 1,  1);
        glTexCoord2f(0, 0); glVertex2f(-1,  1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

const char* menu_get_item(int idx) {
    if (idx < 0 || idx >= MENU_ITEM_COUNT) return "";
    return menu_items[idx];
}

void menu_draw(int selected) {
    int i;
    float y = 0.4f; // Kicsit lejjebb, hogy középen legyen
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;

    for (i = 0; i < MENU_ITEM_COUNT; ++i) {
        const char* text = menu_get_item(i);

        // Szélesség kiszámítása
        int width = 0;
        for (const char* c = text; *c; ++c)
            width += glutBitmapWidth(font, *c);

        // OpenGL -1..1 tartományban, 1024 pixel szélességre skálázva
        float xpos = -((float)width / 1024.0f);

        if (i == selected) {
            glColor3f(1.0f, 1.0f, 0.0f); // Sárga a kijelölt
        } else {
            glColor3f(0.1f, 0.1f, 0.6f); // Sötétkék a többi
        }
        glRasterPos2f(xpos, y - i * 0.18f); // 0.18 a nagyobb betű miatt

        for (const char* c = text; *c; ++c) {
            glutBitmapCharacter(font, *c);
        }
    }
}

int menu_get_item_at(float y) {
    float start_y = 0.4f; // ugyanaz, mint menu_draw-ban
    float item_height = 0.18f; // ugyanaz, mint menu_draw-ban
    for (int i = 0; i < MENU_ITEM_COUNT; ++i) {
        float item_top = start_y - i * item_height + item_height / 2;
        float item_bottom = start_y - i * item_height - item_height / 2;
        if (y <= item_top && y >= item_bottom) {
            return i;
        }
    }
    return -1;
}

void bmp_flip_vertical(unsigned char* data, int width, int height) {
    int row_bytes = 3 * width;
    unsigned char* temp_row = (unsigned char*)malloc(row_bytes);
    if (!temp_row) return;
    for (int y = 0; y < height / 2; ++y) {
        unsigned char* row1 = data + y * row_bytes;
        unsigned char* row2 = data + (height - 1 - y) * row_bytes;
        memcpy(temp_row, row1, row_bytes);
        memcpy(row1, row2, row_bytes);
        memcpy(row2, temp_row, row_bytes);
    }
    free(temp_row);
}