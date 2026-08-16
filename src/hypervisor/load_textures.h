#ifndef LOAD_TEXTURES_H_INCLUDED
#define LOAD_TEXTURES_H_INCLUDED

#include <GL/glu.h>


int PictureLoadedOpenGLTexturePending(struct Picture * picturedata);
int PictureTextureLoaded(struct Picture * picturedata);

/* The GL texture of album picture number "place" , ready to be handed to a shader , or
   0 when there is nothing safe to sample there yet.
   The memory hypervisor uploads and throws textures away behind the renderer's back ,
   so anything holding on to a texture id has to come back through here every frame
   instead of caching it. Also fills in the picture's pixel dimensions. */
unsigned int GetPictureTextureForSampling(int place,float * textureWidth,float * textureHeight);

int make_texture(struct Picture * picturedata,int enable_mipmaping);
unsigned int clear_texture(struct Picture * picturedata);
unsigned int refresh_texture(struct Picture * picturedata);

#endif // LOAD_TEXTURES_H_INCLUDED
