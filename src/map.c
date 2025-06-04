#include "map.h"
#include "texture.h"
#include <GL/gl.h>
#include <stdio.h>

GLuint map_texture = 0;
#define MAP_SIZE 100.0f

void init_map(void) {
    map_texture = load_texture("assets/textures/main_map_texture.jpg");
    if (!map_texture) {
        printf("Nem sikerült betölteni a main_map_texture.jpg-t!\n");
    } else {
        printf("Map texture loaded successfully!\n");
    }
}

void draw_map(void) {

    // Égbolt (háttér)
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.4f, 0.7f, 1.0f); // világoskék ég
    glBegin(GL_QUADS);
        glVertex3f(-200, 100, -200);
        glVertex3f( 200, 100, -200);
        glVertex3f( 200, 100,  200);
        glVertex3f(-200, 100,  200);
    glEnd();

    // Talaj textúrázva main_map_texture.jpg-vel
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, map_texture);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);   glVertex3f(-MAP_SIZE, 0, -MAP_SIZE);
        glTexCoord2f(10, 0);  glVertex3f( MAP_SIZE, 0, -MAP_SIZE);
        glTexCoord2f(10, 10); glVertex3f( MAP_SIZE, 0,  MAP_SIZE);
        glTexCoord2f(0, 10);  glVertex3f(-MAP_SIZE, 0,  MAP_SIZE);
    glEnd();

    glDisable(GL_TEXTURE_2D);


    // Égbolt (háttér) - nagy kék quad a talaj felett
    /* glDisable(GL_TEXTURE_2D);
    glColor3f(0.4f, 0.7f, 1.0f); // világoskék ég
    glBegin(GL_QUADS);
        glVertex3f(-MAP_SIZE * 2, MAP_SIZE * 2, -MAP_SIZE * 2);
        glVertex3f( MAP_SIZE * 2, MAP_SIZE * 2, -MAP_SIZE * 2);
        glVertex3f( MAP_SIZE * 2, MAP_SIZE * 2,  MAP_SIZE * 2);
        glVertex3f(-MAP_SIZE * 2, MAP_SIZE * 2,  MAP_SIZE * 2);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, map_texture);
    glColor3f(1,1,1);

    printf("Drawing map with texture %u\n", map_texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-MAP_SIZE, 0, -MAP_SIZE);
        glTexCoord2f(10, 0); glVertex3f( MAP_SIZE, 0, -MAP_SIZE);
        glTexCoord2f(10, 10); glVertex3f( MAP_SIZE, 0,  MAP_SIZE);
        glTexCoord2f(0, 10); glVertex3f(-MAP_SIZE, 0,  MAP_SIZE);
    glEnd();

    glDisable(GL_TEXTURE_2D); */
}