#include "environment.h"
#include "load_images.h"
#include "load_textures.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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


int LoadStockTextures()
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

    sprintf(filename,"%s/background.jpg",base_directory);
    background=CreatePicture((char * )filename,1);
    make_texture(background,1);

    sprintf(filename,"%s/frame.jpg",base_directory);
    picture_frame=CreatePicture((char * )filename,1);
    make_texture(picture_frame,1);
    /*  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

  return 1;
}
