#include "load_textures.h"
#include "load_images.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void complain_about_errors()
{
  int err=glGetError();
  if  ( glGetError()!=0 ) fprintf(stderr,"OpenGL Error %u ",(unsigned int )err);
  return;
}

int wait_before_making_textures()
{
    fprintf(stderr,"Trying to wait patiently before making any textures\n");
    return 1;
}

int PictureLoadedOpenGLTexturePending(struct Picture * picturedata)
{
  return picturedata->ready_for_texture;
}

int make_texture(struct Picture * picturedata)
{

   // wait_before_making_textures();
    fprintf(stderr,"Time to make some textures \n");

	if ( picturedata == 0 ) { fprintf(stderr,"Error making texture from picture , accomodation structure is not allocated\n");
	                          return 0; }

   // picturedata->gl_rgb_texture=0;
   // fprintf(stderr,"OpenGL Making Texture of size ( %u %u ) id is %u\n", picturedata->width , picturedata->height,(unsigned int) picturedata->gl_rgb_texture);
   // fprintf(stderr,"For some strange reason the next lines SEGFAULT\n");

    /*   !!!!!!!!!!!!!!!!!!!!!!!!!!!
              CODE NEEDS FIX SEGFAULTS
         !!!!!!!!!!!!!!!!!!!!!!!!!!!
    */

    glEnable(GL_TEXTURE_2D);

    fprintf(stderr,"For some strange reason the next lines SEGFAULT\n");

    GLint texSize;
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

    /*   !!!!!!!!!!!!!!!!!!!!!!!!!!!
              CODE NEEDS FIX SEGFAULTS
         !!!!!!!!!!!!!!!!!!!!!!!!!!!
    */


     fprintf(stderr,"CREATING ACTUAL TEXTURE \n");
     glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, picturedata->width , picturedata->height, 0, GL_RGB, GL_UNSIGNED_BYTE,(const GLvoid *) picturedata->rgb_data);
                                      //3
      picturedata->ready_for_texture=0;

  /*
    // mip mapping
	fprintf(stderr,"Setting Linear Filter\n");

	fprintf(stderr,"Building MipMaps\n");
    gluBuild2DMipmaps is depreceated as a call  gluBuild2DMipmaps(GL_TEXTURE_2D,3,picturedata->width,picturedata->height,GL_RGB,GL_UNSIGNED_BYTE,picturedata->rgb_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
 */


    complain_about_errors();

    /* PICTURE IS LOADED IN GPU SO WE CAN UNLOAD IT FROM MAIN RAM MEMORY */
      if ( picturedata->rgb_data != 0 ) free(picturedata->rgb_data);
      picturedata->rgb_data_size=0;

    fprintf(stderr,"OpenGL Texture of size ( %u %u ) id is %u\n", picturedata->width , picturedata->height,picturedata->gl_rgb_texture);

    glFlush();

    return 1;
}


