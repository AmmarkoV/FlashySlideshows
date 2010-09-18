#include "load_textures.h"
#include "load_images.h"
#include "slideshow.h"
#include "memory_hypervisor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void complain_about_errors()
{
  int err=glGetError();
  if  ( err!=0 ) fprintf(stderr,"OpenGL Error %u ",(unsigned int )err);
  return;
}

int wait_before_making_textures()
{
    fprintf(stderr,"Trying to wait patiently before making any textures\n");
    return 1;
}

int PictureLoadedOpenGLTexturePending(struct Picture * picturedata)
{
  if ( picturedata == 0 ) return 0;
  return picturedata->marked_for_texture_loading;
}

int make_texture(struct Picture * picturedata,int enable_mipmaping)
{
	if ( picturedata == 0 ) { fprintf(stderr,"Error making texture from picture , accomodation structure is not allocated\n");
	                          return 0; }

    frame.gpu.lastTexture=picturedata->width*picturedata->height* /*RGBA ->*/ 4 /* <- RGBA*/ ;
    if ( GPU_Memory_can_accomodate(frame.gpu.lastTexture)==0 ) { return 0; }

    glEnable(GL_TEXTURE_2D);
    GLint texSize=0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
    fprintf(stderr,"Maximum Texture Size is %u\n",(unsigned int) texSize);

    GLuint new_tex_id=0;
    fprintf(stderr,"OpenGL Generating new Texture \n");
    glGenTextures(1,&new_tex_id);

    complain_about_errors();

    fprintf(stderr,"OpenGL Binding new Texture \n");
    glBindTexture(GL_TEXTURE_2D,new_tex_id);
    glFlush();

    picturedata->gl_rgb_texture=new_tex_id;
    complain_about_errors();


  if ( ( enable_mipmaping == 1 ) || ( frame.force_mipmap_generation ==1 ) )
   {
      /* LOADING TEXTURE --WITH-- MIPMAPING */
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, picturedata->width , picturedata->height, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *) picturedata->rgb_data);
      if  ( glGetError()!=0 ) { fprintf(stderr,"No GPU memory availiable! \n"); return 0; }
   } else
   {
      /* LOADING TEXTURE --WITHOUT-- MIPMAPING - IT IS LOADED RAW*/
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, picturedata->width , picturedata->height, 0, GL_RGB, GL_UNSIGNED_BYTE,(const GLvoid *) picturedata->rgb_data);
      if  ( glGetError()!=0 ) { fprintf(stderr,"No GPU memory availiable! \n"); return 0; }
   }

    fprintf(stderr,"PLease note that when using mipmaps there is a lot more memory consumption :P \n");


    frame.gpu.usedRAM+=frame.gpu.lastTexture;

    picturedata->marked_for_texture_loading=0;

    complain_about_errors();

    /* PICTURE IS LOADED IN GPU SO WE CAN UNLOAD IT FROM MAIN RAM MEMORY */
      if ( picturedata->rgb_data != 0 )
        {
          frame.system.usedRAM-=picturedata->rgb_data_size;
          free(picturedata->rgb_data);
          picturedata->rgb_data=0;
          picturedata->rgb_data_size=0;
        }


    fprintf(stderr,"OpenGL Texture of size ( %u %u ) id is %u\n", picturedata->width , picturedata->height,picturedata->gl_rgb_texture);

    glFlush();

    return 1;
}


