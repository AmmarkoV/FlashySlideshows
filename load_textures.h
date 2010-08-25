#ifndef LOAD_TEXTURES_H_INCLUDED
#define LOAD_TEXTURES_H_INCLUDED

#include <GL/glu.h>


int PictureLoadedOpenGLTexturePending(struct Picture * picturedata);
int make_texture(struct Picture * picturedata,int enable_mipmaping);

#endif // LOAD_TEXTURES_H_INCLUDED
