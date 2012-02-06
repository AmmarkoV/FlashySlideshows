#include "environment.h"
#include "load_images.h"
#include "load_textures.h"
#include "image_sensing.h"
#include "sound.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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

    srand( time(NULL) );
    unsigned int background_pic_to_load = rand()%8;
    if ( background_pic_to_load >= 7 ) { background_pic_to_load=0; }
    sprintf(filename,"%s/background%u.jpg",base_directory,background_pic_to_load);
    background=CreatePicture((char * )filename,1);
    make_texture(background,1);

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
  system("gconftool-2 --set /apps/gnome-screensaver/idle_activation_enabled --type bool FALSE");
  system("gconftool-2 --set /apps/gnome-powermanager/idle_activation_enabled --type bool FALSE");
}

void EnableScreenSaver()
{
  system("gconftool-2 --set /apps/gnome-screensaver/idle_activation_enabled --type bool TRUE");
  system("gconftool-2 --set /apps/gnome-powermanager/idle_activation_enabled --type bool TRUE");
}
