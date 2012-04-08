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
#include "directory_listing.h"
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
    FILE *file= fopen(fname, "r");
    if (file!=0)
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

    sprintf(filename,"%s/label.jpg",base_directory);
    label=CreatePicture((char * )filename,1);
    make_texture(label,1);

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


unsigned int GetSystemFreeMemory()
{
  // TODO : cat /proc/meminfo , etc
  return 256*1024*1024;
}


unsigned int GetGraphicsCardMemory()
{
//Found on http://www.commandlinefu.com/commands/view/6894/get-information-on-your-graphics-card-on-linux-such-as-graphics-memory-size
//
fprintf(stderr,"Trying to get max texture memory\n");
char line[512];
char command[512];
sprintf(command,"lspci -v | sed -n '/VGA/,/^$/s,.* prefetchable.*size=\\(.\\+\\)].*,\\1,p'");
 FILE *fpipe;
unsigned int gpu_texture_caps=0;
unsigned int gpu_accepted_texture_caps=16*1024*1024; //64MB Default

 if ( !(fpipe = (FILE*)popen(command,"r")) )
   {
       fprintf(stderr,"Error extracting GetGraphicsCardMemory \n");
   } else
   {      while ( fgets( line, sizeof line, fpipe))
       {
         fprintf(stderr,"Received response for GPU Texture size is %s \n",line);
         unsigned int str_multiplier=0;
         if ( strlen(line)>1 )
          {
            if ( ( line[strlen(line)-1]==10 ) || ( line[strlen(line)-1]==13 ) ) { line[strlen(line)-1]=0; } // in case of 10 ( CR ) or 10 13 ( CR LF )
            if ( ( line[strlen(line)-1]==10 ) || ( line[strlen(line)-1]==13 ) ) { line[strlen(line)-1]=0; } // in case of 10 13 ( CR LF )
            if ( ( line[strlen(line)-1]==' ') ) { line[strlen(line)-1]=0; } // in case of a space..!
            if ( ( line[strlen(line)-1]=='b') || ( line[strlen(line)-1]=='B') ) { line[strlen(line)-1]=0; } // in case string is Mb or something :P

            if ( ( line[strlen(line)-1]=='M') || ( line[strlen(line)-1]=='m') ) { line[strlen(line)-1]=0; /*MB*/ str_multiplier=1024*1024; } else
            if ( ( line[strlen(line)-1]=='G') || ( line[strlen(line)-1]=='g') ) { line[strlen(line)-1]=0; /*GB*/ str_multiplier=1024*1024*1024; } else
                                                                                { fprintf(stderr,"Unrecognized quantity indicator , got %c=? , was expecting M=Megabyte \n",line[strlen(line)-1]); }

            gpu_texture_caps=atoi(line)*str_multiplier;
            gpu_texture_caps=gpu_texture_caps/(1024*1024); //OpenGL uses MB to count size
            fprintf(stderr,"GPU has space for %u MB of texture \n",gpu_texture_caps);

            if (gpu_texture_caps<16)   { fprintf(stderr,"GPU space seems terribly limited , or return string was wrong\n"); gpu_texture_caps=16 * 1024 * 1024; } else
            if (gpu_texture_caps<32)   { fprintf(stderr,"GPU space is limited :S \n"); gpu_texture_caps=32 * 1024 * 1024;  } else
            if (gpu_texture_caps>4096) { fprintf(stderr,"GPU space is enormous capping to 1GB :S \n"); gpu_texture_caps=1024 * 1024 * 1024;  } else
                                       {
                                          // gpu_texture_caps remains as is :)
                                          gpu_texture_caps=gpu_texture_caps * 1024 * 1024;
                                       }

            if (gpu_accepted_texture_caps<gpu_texture_caps) { gpu_accepted_texture_caps=gpu_texture_caps; }

          } else
          {
            fprintf(stderr,"Erroneous string returned while quering for graphics card texture size\n");
          }
       }
   }
 pclose(fpipe);

 return gpu_accepted_texture_caps;

}



unsigned int UploadPhotoToMyloader(char * photo)
{
if (photo==0) { fprintf(stderr,"Erroneous photograph in UploadPhotoToMyloader\n"); return 0; }
fprintf(stderr,"Trying to UploadPhotoToMyloader %s \n",photo);
char line[512];
char command[512];
sprintf(command,"scripts/myloader \"%s\"",photo);

FILE *fpipe;
 if ( !(fpipe = (FILE*)popen(command,"r")) )
   {
       fprintf(stderr,"Error UploadPhotoToMyloader\n");
       return 0;
   } else
   {
      while ( fgets( line, sizeof line, fpipe))
       {
         fprintf(stderr,"UploadPhotoToMyloader returned link %s \n",line);
       }
   }
 return 1;
}

unsigned int UploadCurrentPhotoToMyloader()
{
  fprintf(stderr,"Feature deactivated :P");
  return 0;

  char photo_filename[1024];
  GetFullFilenameforFile(frame.active_image_place,photo_filename);
  return  UploadPhotoToMyloader(photo_filename);
}


