/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammarkov.ath.cx
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

#include "memory_hypervisor.h"
#include "slideshow.h"
#include "load_images.h"
#include "load_textures.h"
#include "directory_listing.h"
#include <stdio.h>
#include <unistd.h>

/*



*/




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      Pictures are malloced structures that carry RGB data on them
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
int RAM_Memory_can_accomodate(unsigned int newfile)
{
  if ( frame.gpu.maxRAM < newfile + frame.gpu.usedRAM )
    {
       fprintf(stderr,"It looks like there is no free memory on GPU :P \n");
       return 0;
    }
   if (frame.system.maxRAM < frame.system.usedRAM + newfile)
    {
       fprintf(stderr,"System memory bounds reached"); return 0;
    }
 return 1;
}


int LoadPicturesIfNeeded()
{
  unsigned int MAX_album_traveler=MaxPictureThatIsVisible();
  unsigned int album_traveler=MinPictureThatIsVisible();
  unsigned int loaded_pictures_this_loop=0;


  char pictures_filename_shared_stack_mem_hyper[1024]={0};
  while (album_traveler<MAX_album_traveler)
   {

    if (RAM_Memory_can_accomodate(frame.system.lastTexture) ) //No point trying to load if it doesnt't fit
    { if (PictureCreationPending(album[album_traveler]))
      {
         // THIS SHOULD CREATE THE PICTURE
         if ( GetViewableFilenameforFile(album_traveler,(char *) frame.album_directory,pictures_filename_shared_stack_mem_hyper) == 1 )
            {
               album[album_traveler]=CreatePicture(pictures_filename_shared_stack_mem_hyper,0);
               if ( album[album_traveler] != 0 )
                 {
                     ++loaded_pictures_this_loop;
                 } else
                 {
                     fprintf(stderr,"Failed to Create Picture TODO remove it from album and RemoveListItem\n");
                     //RemoveListItem(album_traveler);
                 }

            } else { fprintf(stderr,"Could not retrieve filename for album item %u/%u\n",album_traveler, frame.total_images); }
      }
    }

    ++album_traveler;
   }

  return loaded_pictures_this_loop;
}

int UnLoadPicturesIfNeeded()
{
  return 0;
}

int ManagePicturesMemory()
{
   UnLoadPicturesIfNeeded();
   return LoadPicturesIfNeeded();
}







/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>




                         SYSTEM DATA /\




                          GPU DATA   \/







>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */












/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   Textures are GPU allocated structures that carry RGB data on them
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

int GPU_Memory_can_accomodate(unsigned int newfile)
{
  if ( frame.gpu.maxRAM < newfile + frame.gpu.usedRAM )
    {
       fprintf(stderr,"It looks like there is no free memory on GPU :P \n");
       return 0;
    }

 return 1;
}

int LoadTexturesIfNeeded()
{
  unsigned int MAX_album_traveler=MaxPictureThatIsVisible();
  unsigned int album_traveler=MinPictureThatIsVisible();
  unsigned int loaded_pictures_this_loop=0;


  char pictures_filename_shared_stack_mem_hyper[1024]={0};
  while (album_traveler<MAX_album_traveler)
   {
    if (GPU_Memory_can_accomodate(frame.gpu.lastTexture) ) //No point trying to load if it doesnt't fit
    { if ( PictureLoadingPending(album[album_traveler]) )
      {
          // THIS SHOULD LOAD THE PICTURE
          if ( GetViewableFilenameforFile(album_traveler,(char *) frame.album_directory,pictures_filename_shared_stack_mem_hyper) == 1 )
            {
              // fprintf(stderr,"directory_listing query for picture %u returned string `%s`\n",album_traveler,pictures_filename_shared_stack);
               LoadPicture(pictures_filename_shared_stack_mem_hyper,album[album_traveler]);
               PositionPicture(album[album_traveler],album_traveler);
               ++loaded_pictures_this_loop;
            } else { fprintf(stderr,"Could not retrieve filename for album item %u/%u\n",album_traveler, frame.total_images); }
      }
    }
    ++album_traveler;
   }

  return 0;
}

int UnLoadTexturesIfNeeded()
{
  if ( frame.total_images == 0 ) { return 0; }
  unsigned int MAX_album_traveler=MinPictureThatIsVisible();
  unsigned int album_traveler=0;
  unsigned int unloaded_textures_this_loop=0;

  /*SCAN THE PICTURES FROM 0 to MinVisible in order to free up some space*/
  while (album_traveler<MAX_album_traveler)
   {
    if (!GPU_Memory_can_accomodate(frame.gpu.lastTexture*6) ) //No point trying to load if it doesnt't fit
    {
       if ( PictureTextureLoaded(album[album_traveler]) )
       {
         clear_texture(album[album_traveler]);
         ++unloaded_textures_this_loop;
       }
    }
    ++album_traveler;
   }

  /*SCAN THE PICTURES FROM MaxVisible to END in order to free up some space*/

  album_traveler=frame.total_images-1;
  unsigned int MIN_album_traveler=MaxPictureThatIsVisible();
  while ((album_traveler>MIN_album_traveler) && (album_traveler>0) )
   {
    if (!GPU_Memory_can_accomodate(frame.gpu.lastTexture*6) ) //No point trying to load if it doesnt't fit
    {
       if ( PictureTextureLoaded(album[album_traveler]) )
       {
         clear_texture(album[album_traveler]);
         ++unloaded_textures_this_loop;
       }
    }
    if ( album_traveler != 0 ) { --album_traveler; } else
                               { break; }


   //  usleep(10);
   }


  return 0;
}

int ManageTexturesMemory()
{
  // UnLoadTexturesIfNeeded(); THIS ONLY SHOULD BE CALLED FROM THE MAIN THREAD
   return LoadTexturesIfNeeded();
}

