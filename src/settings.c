#include "settings.h"
#include <GL/glut.h>
#include <stdio.h>

void settings_init(GameSettings* s) {
    s->brightness = 1.0f;
    s->key_forward = 'W';
    s->key_backward = 'S';
    s->key_left = 'A';
    s->key_right = 'D';
    s->key_action = 'E';
    s->volume = 1.0f;
    s->fog_enabled = 1;
    s->max_time_minutes = 5;
    s->max_time_seconds = 0;
}

void settings_save(const GameSettings* s, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    fwrite(s, sizeof(GameSettings), 1, f);
    fclose(f);
}

void settings_load(GameSettings* s, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        settings_init(s);
        settings_save(s, filename);
        return;
    }
    fread(s, sizeof(GameSettings), 1, f);
    fclose(f);
}

void settings_menu_init(SettingsMenuState* state) {
    state->selected = 0;
    state->editing_key = -1;
}

static const char* key_names[5] = { "Forward", "Back", "Left", "Right", "Action" };

void settings_menu_draw(const GameSettings* s, const SettingsMenuState* state) {
    float y = 0.7f;
    void* font = GLUT_BITMAP_HELVETICA_18;
    char buf[128];

    glColor3f(1,1,1);
    glRasterPos2f(-0.7f, y);
    const char* title = "Settings";
    for (const char* c = title; *c; ++c)
        glutBitmapCharacter(font, *c);

    y -= 0.15f;
    // 0: Brightness
    glColor3f(state->selected == 0 ? 1 : 1, state->selected == 0 ? 1 : 1, state->selected == 0 ? 0 : 1);
    sprintf(buf, "Brightness: %.2f  (bal/jobb nyíl)", s->brightness);
    glRasterPos2f(-0.7f, y); for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.1f;
    // 1: Volume
    glColor3f(state->selected == 1 ? 1 : 1, state->selected == 1 ? 1 : 1, state->selected == 1 ? 0 : 1);
    sprintf(buf, "Volume: %.2f  (bal/jobb nyíl)", s->volume);
    glRasterPos2f(-0.7f, y); for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.1f;
    // 2: Fog
    glColor3f(state->selected == 2 ? 1 : 1, state->selected == 2 ? 1 : 1, state->selected == 2 ? 0 : 1);
    sprintf(buf, "Fog: %s  (Enter/F1)", s->fog_enabled ? "On" : "Off");
    glRasterPos2f(-0.7f, y); for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.1f;
    // 3: Max time
    glColor3f(state->selected == 3 ? 1 : 1, state->selected == 3 ? 1 : 1, state->selected == 3 ? 0 : 1);
    sprintf(buf, "Max time: %02d:%02d  (bal/jobb nyíl)", s->max_time_minutes, s->max_time_seconds);
    glRasterPos2f(-0.7f, y); for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.1f;
    // 4: Keys
    if (state->editing_key == -1)
        glColor3f(state->selected == 4 ? 1 : 1, state->selected == 4 ? 1 : 1, state->selected == 4 ? 0 : 1);
    else
        glColor3f(1,0.5,0); // szerkesztés alatt

    sprintf(buf, "Forward: %c  Back: %c  Left: %c  Right: %c  Action: %c  (Enter: szerkeszt)",
        s->key_forward, s->key_backward, s->key_left, s->key_right, s->key_action);
    glRasterPos2f(-0.7f, y); for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    // Ha szerkesztésben vagyunk, írjuk ki melyik gombot várjuk
    if (state->editing_key != -1) {
        y -= 0.08f;
        glColor3f(1,0.5,0);
        sprintf(buf, "Nyomj le egy billentyűt a(z) %s irányhoz! (ESC: mégse)", key_names[state->editing_key]);
        glRasterPos2f(-0.7f, y); for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

        y -= 0.08f;
        sprintf(buf, "Tab: következő irány");
        glRasterPos2f(-0.7f, y); for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);
    }

    y -= 0.15f;
    // 5: Save
    glColor3f(state->selected == 5 ? 1 : 0.2f, state->selected == 5 ? 1 : 0.6f, state->selected == 5 ? 0 : 1.0f);
    glRasterPos2f(-0.7f, y);
    const char* save = "[ Save ]";
    for (const char* c = save; *c; ++c)
        glutBitmapCharacter(font, *c);

    y -= 0.1f;

   // ESC info
    glColor3f(0.8f, 0.8f, 0.8f);
    glRasterPos2f(-0.4f, -0.85f);
    const char* escmsg = "Press Esc to return to the main menu...";
    for (const char* c = escmsg; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
}

// Fel/le/bal/jobb/Enter/F1 kezelés
void settings_menu_key(SettingsMenuState* state, GameSettings* s, int key) {
    if (state->editing_key != -1) return; // szerkesztés közben csak karakteres input
    if (key == GLUT_KEY_UP) {
        state->selected = (state->selected - 1 + 7) % 7;
    } else if (key == GLUT_KEY_DOWN) {
        state->selected = (state->selected + 1) % 7;
    } else if (key == GLUT_KEY_LEFT) {
        if (state->selected == 0 && s->brightness > 0.0f) s->brightness -= 0.05f;
        if (state->selected == 1 && s->volume > 0.0f) s->volume -= 0.05f;
        if (state->selected == 3 && (s->max_time_minutes > 0 || s->max_time_seconds > 0)) {
            if (s->max_time_seconds > 0) s->max_time_seconds--;
            else if (s->max_time_minutes > 0) { s->max_time_minutes--; s->max_time_seconds = 59; }
        }
    } else if (key == GLUT_KEY_RIGHT) {
        if (state->selected == 0 && s->brightness < 1.0f) s->brightness += 0.05f;
        if (state->selected == 1 && s->volume < 1.0f) s->volume += 0.05f;
        if (state->selected == 3 && (s->max_time_minutes < 99 || s->max_time_seconds < 59)) {
            if (s->max_time_seconds < 59) s->max_time_seconds++;
            else if (s->max_time_minutes < 99) { s->max_time_minutes++; s->max_time_seconds = 0; }
        }
    } else if (key == GLUT_KEY_F1) {
        if (state->selected == 2) s->fog_enabled = !s->fog_enabled;
    } else if (key == 13 || key == '\r' || key == '\n') {
        // Enter: szerkesztés indítása vagy Save/Back
        if (state->selected == 2) s->fog_enabled = !s->fog_enabled;
        if (state->selected == 4) state->editing_key = 0; // Forward szerkesztése
        if (state->selected == 5) settings_save(s, "settings.dat");
        // A Back kezelése a main-ben történik (in_settings = 0, in_menu = 1)
    }
}

// Karakteres input: irányítási gomb szerkesztése
void settings_menu_char(SettingsMenuState* state, GameSettings* s, unsigned char key) {
    if (state->editing_key != -1) {
        if (key == 27) { state->editing_key = -1; return; } // ESC megszakítja
        if (key == 9) { // TAB: következő irány
            state->editing_key = (state->editing_key + 1) % 5;
            return;
        }
        switch (state->editing_key) {
            case 0: s->key_forward = key; break;
            case 1: s->key_backward = key; break;
            case 2: s->key_left = key; break;
            case 3: s->key_right = key; break;
            case 4: s->key_action = key; break;
        }
        state->editing_key = (state->editing_key + 1) % 5; // automatikusan következő irány
        return;
    }
    // Egyéb karakteres input nincs
}

// Egérkattintás: Save vagy Back gomb
int settings_menu_mouse(const SettingsMenuState* state, int x, int y) {
    int win_height = glutGet(GLUT_WINDOW_HEIGHT);
    // Egyszerűsített: ha az alsó 100 pixelre kattintanak, vagy a Save/Back van kijelölve
    if (state->selected == 6) return 2; // Back
    if (state->selected == 5) return 1; // Save
    if (y > win_height - 100) return 2; // Back
    return 0;
}