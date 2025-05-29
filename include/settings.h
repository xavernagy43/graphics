#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct {
    float brightness;
    int key_forward, key_backward, key_left, key_right, key_action;
    float volume;
    int fog_enabled;
    int max_time_minutes, max_time_seconds;
} GameSettings;

typedef struct {
    int selected;      // 0: brightness, 1: volume, 2: fog, 3: max time, 4: keys, 5: save, 6: back
    int editing_key;   // -1 ha nem szerkeszt, 0-4: melyik irányítási gomb (0:forward, 1:back, 2:left, 3:right, 4:action)
} SettingsMenuState;

void settings_init(GameSettings* s);
void settings_save(const GameSettings* s, const char* filename);
void settings_load(GameSettings* s, const char* filename);

void settings_menu_init(SettingsMenuState* state);
void settings_menu_draw(const GameSettings* s, const SettingsMenuState* state);
void settings_menu_key(SettingsMenuState* state, GameSettings* s, int key);
void settings_menu_char(SettingsMenuState* state, GameSettings* s, unsigned char key);
int  settings_menu_mouse(const SettingsMenuState* state, int x, int y);

#endif