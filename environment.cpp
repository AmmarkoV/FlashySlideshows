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

#include "environment.h"
#include "load_images.h"
#include "load_textures.h"
#include "image_sensing.h"
#include "sound.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int PrintOpenGLDebugMsg()
{
  return 0;
}

int PrintPictureLoadingMsg()
{
  return 0;
}

int PrintDevMsg()
{
  return 0;
}

long timeval_diff ( struct timeval *difference, struct timeval *end_time, struct timeval *start_time )
{

   struct timeval temp_diff;

   if(difference==0) { difference=&temp_diff; }

  difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
  difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

  /* Using while instead of if below makes the code slightly more robust. */

  while(difference->tv_usec<0)
  {
    difference->tv_usec+=1000000;
    difference->tv_sec -=1;
  }

  return 1000000LL*difference->tv_sec+ difference->tv_usec;

}


int FileExists(const char *fname)
{
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}


int LoadStockTexturesAndSounds()
{

    char base_directory[128]={0};
    char filename[256]={0};

   if ( FileExists("app_clipart/star.png") )
      {
        //if we are running without an installation it is better to use this dir..!
        strcpy(base_directory,"app_clipart");
      } else
    if ( FileExists("/usr/share/flashyslideshows/app_clipart/star.png") )
      {
        strcpy(base_directory,"/usr/share/flashyslideshows/app_clipart");
      } else
      {
        fprintf(stderr,"Unable to locate /usr/share/flashyslideshows/app_clipart/ or app_clipart/\n ");
        return 0;
      }



    /* Loading Stock textures >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    sprintf(filename,"%s/star.png",base_directory);
    star=CreatePicture((char * )filename,1);
    make_texture(star,1);

    sprintf(filename,"%s/heart.png",base_directory);
    heart=CreatePicture((char * )filename,1);
    make_texture(heart,1);

    sprintf(filename,"%s/play.png",base_directory);
    play_img=CreatePicture((char * )filename,1);
    make_texture(play_img,1);

    sprintf(filename,"%s/pause.png",base_directory);
    pause_img=CreatePicture((char * )filename,1);
    make_texture(pause_img,1);

    sprintf(filename,"%s/loading_texture.jpg",base_directory);
    loading_texture=CreatePicture((char * )filename,1);
   // loading_texture->position.ok=1;
    make_texture(loading_texture,1);

    sprintf(filename,"%s/loading.jpg",base_directory);
    loading=CreatePicture((char * )filename,1);
   // loading->position.ok=1;
    make_texture(loading,1);

    sprintf(filename,"%s/failed.jpg",base_directory);
    failed=CreatePicture((char * )filename,1);
    //failed->position.ok=1;
    make_texture(failed,1);


    sprintf(filename,"%s/background%u.jpg",base_directory,frame.background_number);
    background=CreatePicture((char * )filename,1);
    make_texture(background,1); //frame.mipmaping <- no mipmapping on background ( is a performance hit :P ) but mipmaping screws the bacground details..!

    sprintf(filename,"%s/frame.jpg",base_directory);
    picture_frame=CreatePicture((char * )filename,1);
    make_texture(picture_frame,1);
    /*  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */



   if ( FileExists("sounds/pop.wav") )
      {
        //if we are running without an installation it is better to use this dir..!
        strcpy(base_directory,"sounds");
      } else
    if ( FileExists("/usr/share/flashyslideshows/sounds/pop.wav") )
      {
        strcpy(base_directory,"/usr/share/flashyslideshows/sounds");
      } else
      {
        fprintf(stderr,"Unable to locate /usr/share/flashyslideshows/sounds/ or sounds/\n ");
        return 0;
      }


    /* OpenAL Initialization >>>>>>>>>>>>>>>>> */
    StartSoundLibrary();

    sprintf(filename,"%s/pop.wav",base_directory);
    AddSoundBufferForLoad((char *)filename); //LOADED_PICTURE

    sprintf(filename,"%s/cling.wav",base_directory);
    AddSoundBufferForLoad((char *)filename); //LOADED_PICTURE

    sprintf(filename,"%s/slideshow_start.wav",base_directory);
    AddSoundBufferForLoad((char *)filename); //SLIDESHOW_START

    sprintf(filename,"%s/slideshow_stop.wav",base_directory);
    AddSoundBufferForLoad((char *)filename); //SLIDESHOW_STOP

    LoadSoundBuffers();
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */



    if ( FileExists("app_clipart/haarcascade_frontalface_alt.xml") )
      {
        //if we are running without an installation it is better to use this dir..!
        InitFaceRecognition((char*) "app_clipart/haarcascade_frontalface_alt.xml");
      } else
    if ( FileExists("/usr/share/flashyslideshows/app_clipart/haarcascade_frontalface_alt.xml") )
      {
        InitFaceRecognition((char*) "/usr/share/flashyslideshows/app_clipart/haarcascade_frontalface_alt.xml");
      } else
      {
        fprintf(stderr,"Unable to locate /usr/share/flashyslideshows/app_clipart/haarcascade_frontalface_alt.xml \n ");
        fprintf(stderr,"or  app_clipart/haarcascade_frontalface_alt.xml \n ");
        return 0;
      }


  return 1;
}


int UnLoadStockTexturesAndSounds()
{
    CloseFaceRecognition();
    return 1;
}


void DisableScreenSaver()
{
 int i=system("gconftool-2 --set /apps/gnome-screensaver/idle_activation_enabled --type bool FALSE");
     i=system("gconftool-2 --set /apps/gnome-powermanager/idle_activation_enabled --type bool FALSE");
}

void EnableScreenSaver()
{
  int i=system("gconftool-2 --set /apps/gnome-screensaver/idle_activation_enabled --type bool TRUE");
      i=system("gconftool-2 --set /apps/gnome-powermanager/idle_activation_enabled --type bool TRUE");
}
