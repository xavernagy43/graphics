#include "texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <stdio.h>

GLuint load_texture(char* filename)
{
    SDL_Surface* surface;
    GLuint texture_name;

    surface = IMG_Load(filename);
    if (!surface) {
        printf("Nem sikerült betölteni: %s\n", filename);
        return 0;
    }

    glGenTextures(1, &texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);

    GLenum format = GL_RGB;
    if (surface->format->BytesPerPixel == 4) {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);

    return texture_name;
}