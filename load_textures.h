#ifndef LOAD_TEXTURES_H_INCLUDED
#define LOAD_TEXTURES_H_INCLUDED

#include <GL/glu.h>


int PictureLoadedOpenGLTexturePending(struct Picture * picturedata);

int make_texture(struct Picture * picturedata,int enable_mipmaping);
unsigned int clear_texture(struct Picture * picturedata);
unsigned int refresh_texture(struct Picture * picturedata);

#endif // LOAD_TEXTURES_H_INCLUDED
