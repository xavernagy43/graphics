#include "help.h"
#include "menu.h" // for menu_draw_background
#include <GL/glut.h>
#include <stdio.h>

void help_show(const GameSettings* settings) {
    float y = 0.7f;
    void* font = GLUT_BITMAP_HELVETICA_18;
    char buf[256];

    glColor3f(1, 1, 0.2f);
    glRasterPos2f(-0.25f, y);
    const char* title = "Help";
    for (const char* c = title; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.12f;
    font = GLUT_BITMAP_HELVETICA_12;

    glColor3f(1, 1, 1);
    glRasterPos2f(-0.25f, y);
    const char* controls = "Controls:";
    for (const char* c = controls; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.09f;
    glRasterPos2f(-0.25f, y);
    sprintf(buf, "  %c - Forward   %c - Left   %c - Backward   %c - Right",
        settings->key_forward, settings->key_left, settings->key_backward, settings->key_right);
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.07f;
    glRasterPos2f(-0.25f, y);
    sprintf(buf, "  %c - Action", settings->key_action);
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.07f;
    glRasterPos2f(-0.25f, y);
    sprintf(buf, "  Mouse - Move camera around the wolf");
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.07f;
    glRasterPos2f(-0.25f, y);
    sprintf(buf, "In game: h - show help");
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    // Empty line
    y -= 0.10f;

    // Goal
    glRasterPos2f(-0.25f, y);
    const char* goal = "Goal:";
    for (const char* c = goal; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.09f;
    glRasterPos2f(-0.25f, y);
    sprintf(buf, "  Hunt rabbits, deer and other animals in the forest!");
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.07f;
    glRasterPos2f(-0.25f, y);
    sprintf(buf, "  Collect points before the time runs out.");
    for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);

    glColor3f(0.8f, 0.8f, 0.8f);
    glRasterPos2f(-0.4f, -0.85f);
    const char* escmsg = "Press Esc to return...";
    for (const char* c = escmsg; *c; ++c) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
}