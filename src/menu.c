#include "menu.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

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
    printf("Betöltés: %s\n", filename);
    FILE* f = fopen(filename, "rb");
    if (!f) {
        printf("Nem található a kép!\n");
        return;
    }
    unsigned char header[54];
    if (fread(header, 1, 54, f) != 54) {
        printf("Nem sikerült a BMP fejlécet beolvasni!\n");
        fclose(f);
        return;
    }
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Nem BMP fájl!\n");
        fclose(f);
        return;
    }
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    short bpp = *(short*)&header[28];
    if (bpp != 24) {
        printf("Csak 24 bites BMP támogatott! (bpp=%d)\n", bpp);
        fclose(f);
        return;
    }
    int size = 3 * width * height;
    unsigned char* data = (unsigned char*)malloc(size);
    if (fread(data, 1, size, f) != size) {
        printf("Nem sikerült a képadatokat beolvasni!\n");
        fclose(f);
        free(data);
        return;
    }
    fclose(f);

    bmp_flip_vertical(data, width, height);

    // majd utána a BGR->RGB átalakítás:
    for (int i = 0; i < size; i += 3) {
        unsigned char tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
    }

    glGenTextures(1, &menu_bg_tex);
    glBindTexture(GL_TEXTURE_2D, menu_bg_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    printf("Sikeres betöltés: %dx%d\n", width, height);
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