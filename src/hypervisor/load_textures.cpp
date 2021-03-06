/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammar.gr
Written by Ammar Qammaz a.k.a. AmmarkoV 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "load_textures.h"
#include "load_images.h"
#include "../tools/environment.h"
#include "../filesystem/directory_listing.h"
#include "../slideshow.h"
#include "../tools/sound.h"
#include "memory_hypervisor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <GL/glext.h>

int complain_about_errors()
{
  int err=glGetError();
  if  ( err!=0 ) fprintf(stderr,"OpenGL Error %u ",(unsigned int )err);
  return err;
}

int wait_before_making_textures()
{
    fprintf(stderr,"Trying to wait patiently before making any textures\n");
    return 1;
}

int PictureLoadedOpenGLTexturePending(struct Picture * picturedata)
{ // PLEASE CHECK OTHER PARTS FOR picturedata->marked_for_texture_loading; THIS FUNCTION IS NOT USED IN THE TEXTURE LOADING THREAD FOR EXAMPLE
  // SINCE IT POPPED OUT AS A "BOTTLENECK" ON THE PROFILER :P
  if ( picturedata == 0 ) return 0;
  return picturedata->gpu.marked_for_texture_loading;
}

int PictureTextureLoaded(struct Picture * picturedata)
{
  if ( picturedata == 0 ) {return 0;}
  if ( picturedata->gpu.texture_loaded == 1 ) {return 1;}
  if ( picturedata->gpu.thumbnail_texture_loaded == 1 ) {return 1;}
  return 0;
}

void software_add_alpha_channel(char * inbuf,char * outbuf,unsigned int width,unsigned int height)
{
  //This code inflates the image to RGBA format :P
  int in=0,out=0,lim=width*height*3;
  while (in<lim)
   {
     outbuf[out++]=inbuf[in++];
     outbuf[out++]=inbuf[in++];
     outbuf[out++]=inbuf[in++];
     outbuf[out++]=50;
   }
}

int printoutOGLErr(unsigned int errnum)
{
  switch (errnum)
  {
     case GL_NO_ERROR          : fprintf(stderr,"No Error\n"); break;
     case GL_INVALID_ENUM      : fprintf(stderr,"An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n");  break;
     case GL_INVALID_VALUE     : fprintf(stderr,"A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag. \n");  break;
     case GL_INVALID_OPERATION : fprintf(stderr,"The specified operation is not allowed in the current state.The offending command is ignored and has no other side effect than to set the error flag.\n");  break;
     case GL_STACK_OVERFLOW    : fprintf(stderr,"This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag. \n");  break;
     case GL_STACK_UNDERFLOW   : fprintf(stderr,"This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag. \n");  break;
     case GL_OUT_OF_MEMORY     : fprintf(stderr,"There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded. \n");  break;
//     case GL_TABLE_TOO_LARGE   : fprintf(stderr,"The specified table exceeds the implementation's maximum supported table size.  The offending command is ignored and has no other side effect than to set the error flag. \n");  break;
  };
  return 1;
}


int refresh_max_texture_dimension()
{
  GLint texSize=0;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
  if (PrintOpenGLDebugMsg()) fprintf(stderr,"Maximum Texture Size is %u for ",(unsigned int) texSize);
  frame.gpu.maximum_frame_dimension_size=(unsigned int) texSize;

  return frame.gpu.maximum_frame_dimension_size;
}

int make_texture(struct Picture * picturedata,int enable_mipmaping)
{

	if ( picturedata == 0 ) { fprintf(stderr,"Error making texture from picture , accomodation structure is not allocated\n");
	                          return 0; }

    unsigned long this_texture = picturedata->width * picturedata->height * /*RGBA ->*/ 4 /* <- RGBA*/ ;
    if ( !GPU_Memory_can_accomodate(this_texture) ) {
                                                      fprintf(stderr,"Abort making texture , GPU cant accomodate it ( %u KB ) \n",(unsigned int) this_texture/1024);
                                                      SignalGPUFull=1;
                                                      return 0;
                                                    }



    glEnable(GL_TEXTURE_2D);

     PrintDirectoryListItem(picturedata->directory_list_index);

     GLuint new_tex_id=0;
     if (PrintOpenGLDebugMsg()) fprintf(stderr,"OpenGL Generating new Texture \n");
     glGenTextures(1,&new_tex_id);
     if ( complain_about_errors() ) { return 0; }

     if (PrintOpenGLDebugMsg()) fprintf(stderr,"OpenGL Binding new Texture \n");
     glBindTexture(GL_TEXTURE_2D,new_tex_id);
     if ( complain_about_errors() ) { return 0; }
     glFlush();

    picturedata->gpu.gl_rgb_texture=new_tex_id;


    unsigned int depth_flag=GL_RGB;
    char * rgba_data = picturedata->system.rgb_data;

/* RGBA Software conversion for debugging :p   HAS ANOTHER PART (line 150+ ) THAT DOES THE FREE CALL
    rgba_data = (char*) malloc(picturedata->width*picturedata->height*4*sizeof(unsigned char));
    if (rgba_data==0)
      {
       rgba_data = picturedata->rgb_data;
      } else
      {
       depth_flag=GL_RGBA;
       software_add_alpha_channel(picturedata->rgb_data,rgba_data,picturedata->width,picturedata->height);
       fprintf(stderr,"Using Alpha texture conversion\n");
      }
*/
  unsigned int error_num=0;

  if ( ( enable_mipmaping == 1 ) || ( frame.force_mipmap_generation ==1 ) )
   {
      // LOADING TEXTURE --WITH-- MIPMAPING
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);                      // GL_RGB
      glTexImage2D(GL_TEXTURE_2D, 0, depth_flag, picturedata->width , picturedata->height, 0, depth_flag, GL_UNSIGNED_BYTE, (const GLvoid *) rgba_data);
      error_num=glGetError();
      if  ( error_num!=0 ) { printoutOGLErr(error_num); fprintf(stderr,"Creating texture %ux%u:%u ( initial %ux%u )\n",picturedata->width,picturedata->height,depth_flag,picturedata->initial_width,picturedata->initial_height); return 0; }
   } else
   {
      /* LOADING TEXTURE --WITHOUT-- MIPMAPING - IT IS LOADED RAW*/
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);                       //GL_RGB
      glTexImage2D(GL_TEXTURE_2D, 0, depth_flag, picturedata->width , picturedata->height, 0, depth_flag, GL_UNSIGNED_BYTE,(const GLvoid *) rgba_data);
      error_num=glGetError();
      if  ( error_num!=0 ) { printoutOGLErr(error_num); fprintf(stderr,"Creating texture %ux%u:%u ( initial %ux%u )\n",picturedata->width,picturedata->height,depth_flag,picturedata->initial_width,picturedata->initial_height); return 0; }
   }

/* RGBA Software conversion for debugging :p HAS A PREVIOUS PART
   if ( (rgba_data != 0)&&(depth_flag=GL_RGBA) )  { free(rgba_data); }
*/


   if (enable_mipmaping)
    {
     if (PrintDevMsg()) fprintf(stderr,"Using mipmaps there is a lot more memory consumption , needs work..\n");
    }





    /* PICTURE IS LOADED IN GPU SO WE CAN UNLOAD IT FROM MAIN RAM MEMORY */
      if ( picturedata->system.rgb_data != 0 )
        {
          frame.system.usedRAM-=picturedata->system.rgb_data_size;
          free(picturedata->system.rgb_data);
          picturedata->system.rgb_data=0;
          picturedata->system.rgb_data_size=0;
        }

    frame.gpu.lastTexture=this_texture;
    frame.gpu.usedRAM+=frame.gpu.lastTexture;

    picturedata->gpu.texture_data_size=this_texture;
    picturedata->gpu.marked_for_texture_loading=0;
    picturedata->gpu.texture_loaded=1;

    if ( complain_about_errors() ) { return 0; }

    if (PrintOpenGLDebugMsg()) fprintf(stderr,"OpenGL Texture of size ( %u %u ) id is %u\n", picturedata->width , picturedata->height,picturedata->gpu.gl_rgb_texture);

    glFlush();

         SoundLibrary_PlaySound(LOADED_PICTURE);
    return 1;
}

unsigned int clear_texture(struct Picture * picturedata)
{
  	if ( picturedata == 0 ) { fprintf(stderr,"Error clearing texture from picture , accomodation structure is not allocated\n");
	                          return 0; }


    if ( ( picturedata->gpu.gl_rgb_texture != loading->gpu.gl_rgb_texture ) && ( picturedata->gpu.gl_rgb_texture != 0) )
       {
           if (PrintOpenGLDebugMsg()) fprintf(stderr,"Trying to delete texture for picture ");
           PrintDirectoryListItem(picturedata->directory_list_index);

           glDeleteTextures(1,&picturedata->gpu.gl_rgb_texture);
           if (PrintOpenGLDebugMsg()) fprintf(stderr,"... ok\n");

           picturedata->gpu.gl_rgb_texture=loading->gpu.gl_rgb_texture;

           frame.gpu.usedRAM-= picturedata->width * picturedata->height * /*RGBA ->*/ 4 /* <- RGBA*/ ;
       } else
       {
           fprintf(stderr,"clear_texture called for already clear texture ");
           PrintDirectoryListItem(picturedata->directory_list_index);
        }

/*
    if ( picturedata->gl_rgb_thumbnail_texture != loading->gl_rgb_texture )
       {
           glDeleteTextures(1,&picturedata->gl_rgb_thumbnail_texture);
           picturedata->gl_rgb_thumbnail_texture=loading->gl_rgb_texture;
           fprintf(stderr,"Todo NEED SIZE FOR THUMBNAIL :P\n");
           //frame.gpu.usedRAM-=picturedata->width*picturedata->height*4 ;
       }*/
     glFlush();



    picturedata->gpu.marked_for_texture_removal=0;
    picturedata->gpu.texture_loaded=0;
    picturedata->gpu.thumbnail_texture_loaded=0;
    picturedata->system.marked_for_rgbdata_loading=1;
    return 1;
}

unsigned int refresh_texture(struct Picture * picturedata)
{
  clear_texture(picturedata);
  make_texture(picturedata,frame.mipmaping);
  return 1;
}
