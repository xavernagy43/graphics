#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "menu.h"
#include "settings.h"
#include "previous_runs.h"
#include "help.h"
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include "hud.h"

GameSettings settings;
SettingsMenuState settings_menu;
Mix_Music* main_theme = NULL;

int score = 0;
int in_game = 0;
int selected = 0;
int in_menu = 1;
int in_settings = 0;
int in_help = 0;
int help_from_menu = 0; // 1: menüből, 0: játékból
int in_previous_runs = 0;

void display() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (in_menu || in_settings || in_help || in_previous_runs) {
        menu_draw_background();
    }
    if (in_menu) {
        menu_draw(selected);
    } else if (in_settings) {
        settings_menu_draw(&settings, &settings_menu);
    } else if (in_help) {
        help_show(&settings);
    } else if( in_previous_runs) {
        previous_runs_show();
    }  else if(in_game) {
        hud_update_timer();
        hud_draw(score);
        if (hud_time_is_up()) {
            // Játék vége, csak a HUD-ot rajzoljuk, inputot kezeljük
        }
    }

    glutSwapBuffers();
}

void special(int key, int x, int y) {
    if (in_menu) {
        if (key == GLUT_KEY_UP) {
            selected = (selected - 1 + MENU_ITEM_COUNT) % MENU_ITEM_COUNT;
            glutPostRedisplay();
        } else if (key == GLUT_KEY_DOWN) {
            selected = (selected + 1) % MENU_ITEM_COUNT;
            glutPostRedisplay();
        }
    } else if (in_settings) {
        settings_menu_key(&settings_menu, &settings, key);
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (in_settings) {
        if (key == 27) { // ESC
            in_settings = 0;
            in_menu = 1;
            glutPostRedisplay();
        } else {
            settings_menu_char(&settings_menu, &settings, key);
            glutPostRedisplay();
        }
    } else if (in_help) {
        if (key == 27) { // ESC
            in_help = 0;
            if (help_from_menu) {
                in_menu = 1; // vissza főmenübe
            }
            // Ha játékból jöttünk, vissza a játékba:
            glutPostRedisplay();
        }
    } else if (in_previous_runs) {
        if (key == 27) { // ESC
            in_previous_runs = 0;
            in_menu = 1;
            glutPostRedisplay();
        }
    } else if (in_game && hud_time_is_up()) {
        if (key == 27) { // ESC
            in_game = 0;
            in_menu = 1;
            hud_reset();
            glutPostRedisplay();
        }
    } else if (in_game) {
        if (key == 'h' || key == 'H') {
            in_help = 1;
            help_from_menu = 0;
            glutPostRedisplay();
        }
    }
}

void mouse(int button, int state, int x, int y) {
    if (in_menu && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int win_height = glutGet(GLUT_WINDOW_HEIGHT);
        float norm_y = 1.0f - 2.0f * (float)y / win_height;
        int idx = menu_get_item_at(norm_y);
        if (idx != -1) {
            selected = idx;
            glutPostRedisplay();
            switch (selected) {
                case MENU_START_GAME:
                    in_menu = 0; // Játék indítása
                    in_game = 1;
                    score = 0;
                    hud_start_timer(&settings);
                    Mix_HaltMusic();
                    glutPostRedisplay();
                    break;
                case MENU_SETTINGS:
                    in_menu = 0;
                    in_settings = 1;
                    settings_menu_init(&settings_menu);
                    glutPostRedisplay();
                    break;
                case MENU_HELP:
                    in_menu = 0;
                    in_help = 1;
                    help_from_menu = 1;
                    glutPostRedisplay();
                    break;
                case MENU_PREVIOUS_RUNS:
                    in_menu = 0;
                    in_previous_runs = 1;
                    glutPostRedisplay();
                    break;
                case MENU_QUIT:
                    exit(0); // Kilépés a programból
                    break;
                default:
                    break;
            }
        }
    } else if (in_settings && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int action = settings_menu_mouse(&settings_menu, x, y);
        if (action == 2) { // Back gomb
            in_settings = 0;
            in_menu = 1;
            glutPostRedisplay();
        } else if (action == 1) { // Save gomb
            settings_save(&settings, "settings.dat");
            glutPostRedisplay();
        }
    }else if(in_game && hud_time_is_up() && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (hud_game_over_mouse(x, y)) {
            previous_runs_save(score);
            in_game = 0;
            in_menu = 1;
            hud_reset();
            glutPostRedisplay();
        }
    }
}

void idle_func(void) {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    printf("Actual directory: %s\n", cwd);

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("SDL audio init error: %s\n", SDL_GetError());
    exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer error: %s\n", Mix_GetError());
        exit(1);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Wolf Hunting");

    menu_background_init("assets/textures/main_menu_wolf.bmp");
    settings_load(&settings, "settings.dat");

    main_theme = Mix_LoadMUS("assets/sounds/main_theme.ogg");
    if (!main_theme) {
        printf("Problem with music: %s\n", Mix_GetError());
    } else {
        printf("Music loaded successfully.\n");
        Mix_VolumeMusic((int)(settings.volume * MIX_MAX_VOLUME));
        Mix_PlayMusic(main_theme, -1); // -1 = végtelen loop
    }

    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle_func);
    glutMainLoop();

    Mix_FreeMusic(main_theme);
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}