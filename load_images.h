#ifndef LOAD_IMAGES_H_INCLUDED
#define LOAD_IMAGES_H_INCLUDED

#include <GL/glu.h>

#define OVERFLOW_GUARD_BYTES 10101001

struct Position3D
{
  float x,y,z;
  float heading,yaw,pitch;
};

struct Picture
{
  unsigned int height;
  unsigned int width;
  unsigned int depth;

  GLuint gl_rgb_texture;

  unsigned int time_viewed;
  unsigned int times_viewed;

  struct Position3D position;

  char filename[512];
  char ppm_filename[512];

  unsigned long rgb_data_size;
  char * rgb_data;
  unsigned long overflow_guard;

};

extern int OpenGL_is_rendering;
extern int OpenGL_is_making_textures;

struct Picture * CreatePicture(char * filename);
int LoadPicture(char * filename,struct Picture * pic);
int UnLoadPicture(struct Picture * pic);

#endif // LOAD_IMAGES_H_INCLUDED
