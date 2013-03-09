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

#include "memory_hypervisor.h"
#include "../slideshow.h"
#include "load_images.h"
#include "load_textures.h"
#include "../directory_listing.h"
#include "../tools/environment.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

int SignalGPUFull=0;


inline int PictureIsLoadedInSystem(struct Picture * pic)
{
  return   ( pic->system.rgb_data != 0 );
}

inline int PictureIsLoadedInGPU(struct Picture * pic)
{
  return   pic->gpu.texture_loaded;
}

int CheckIfSignalGPUFullAppliesAnyMore()
{
  if (SignalGPUFull)
   {
     if (GPU_Memory_can_accomodate(frame.gpu.maximum_frame_total_size)) { SignalGPUFull=0; }
   }

  return 1;
}

int GPU_Memory_can_accomodate(unsigned int newfile)
{
  if ( frame.gpu.maxRAM < newfile + frame.gpu.usedRAM )
    {
       //fprintf(stderr,"GPU_Memory_can_accomodate :  no free memory\n");
       return 0;
    }

 return 1;
}

int MasterMemoryStrategist()
{
  return 0;
  /*
      Due to the complexity of the various usage scenarios instead of the current
      approach where unloadpictures for examples both contains the logic and does the picture unloading
      to be able to have a more consistent policy for loading and unloading all the operations will be decided here
      and unload pictures will only unload pictures which have been set with the appropriate flag..!

      OUTER                                                                          OUTER
      REGION                                                                         REGION
   --------------                                                                --------------
      Previous                                                                        Next
      Pictures                                                                      Pictures

      |-----|      |-----|      |-----|      |-----|      |-----|      |-----|      |-----|
      |     |      | MIN |      |     |      | CUR |      |     |      | MAX |      |     |
      |-----|      |-----|      |-----|      |-----|      |-----|      |-----|      |-----|

                     /\                        /\                         /\
                     ||                        ||                         ||
                 Min Visible                 Current                  Max Visible
                   Picture                   Picture                   Picture

      Current Picture can move in both ways and even skip an arbitrary number of pictures so
      it is important to always have memory provisions for a stream of new pictures on a suddent
      movement!

      The pictures that belong to the sets Previour Pictures or Next Pictures ( and according to their distance from the current picture )
      can be marked for unloading both from gpu as well as system memory at the hypervisors discretion to ensure enough memory is saved for the visible pictures

      The visible pictures on the other hand must be set to be loaded asap and according to the current picture direction of change with the best possible order
      so that current picture will always be over a loaded picture

  */

   // Some better mnemonic variable names for code clarity
   unsigned int FirstPicture = 0;
   unsigned int LastPicture  = frame.total_images-1;
   unsigned int FirstPictureVisible = MinPictureThatIsVisible();
   unsigned int LastPictureVisible = MaxPictureThatIsVisible();
   unsigned int album_traveler=0;


   // This will yield the results
   frame.hypervisor.pictures_to_be_created=0;
   frame.hypervisor.pictures_to_be_loaded=0;
   frame.hypervisor.thumbnail_textures_to_be_loaded=0;
   frame.hypervisor.textures_to_be_loaded=0;


   frame.hypervisor.pictures_to_be_destroyed=0;
   frame.hypervisor.pictures_to_be_unloaded=0;
   frame.hypervisor.thumbnail_textures_to_be_unloaded=0;
   frame.hypervisor.textures_to_be_unloaded=0;

   unsigned int current_system_usedRAM=frame.system.usedRAM;
   unsigned int after_changes_system_usedRAM=frame.system.usedRAM;



  // FIRST , LETS SCAN THE OUTER REGION PREVIOUS
  album_traveler=FirstPicture;
  while (album_traveler<FirstPictureVisible)
  {
    if ( (PictureIsLoadedInSystem(album[album_traveler])) && (!album[album_traveler]->system.marked_for_rgbdata_removal) )
     {  //If the picture is loaded and the hypervisor hasn't already marked it for rgb_data removal do it now !
          album[album_traveler]->system.marked_for_rgbdata_removal=1;
          after_changes_system_usedRAM-=album[album_traveler]->system.rgb_data_size;
     }
    if ( (PictureIsLoadedInGPU(album[album_traveler])) && (!album[album_traveler]->gpu.marked_for_texture_removal) )
     {  //If the picture is loaded and the hypervisor hasn't already marked it for rgb_data removal do it now !
          album[album_traveler]->gpu.marked_for_texture_removal=1;
          after_changes_system_usedRAM-=album[album_traveler]->gpu.texture_data_size;
     }
    ++album_traveler;
  }

  // SECOND , LETS SCAN THE OUTER REGION FUTURE
  album_traveler=LastPicture;
  while (album_traveler>LastPictureVisible)
  {
    if ( (PictureIsLoadedInSystem(album[album_traveler])) && (!album[album_traveler]->system.marked_for_rgbdata_removal) )
     {  //If the picture is loaded and the hypervisor hasn't already marked it for rgb_data removal do it now !
          album[album_traveler]->system.marked_for_rgbdata_removal=1;
          after_changes_system_usedRAM-=album[album_traveler]->system.rgb_data_size;
     }
    if ( (PictureIsLoadedInGPU(album[album_traveler])) && (!album[album_traveler]->gpu.marked_for_texture_removal) )
     {  //If the picture is loaded and the hypervisor hasn't already marked it for rgb_data removal do it now !
          album[album_traveler]->gpu.marked_for_texture_removal=1;
          after_changes_system_usedRAM-=album[album_traveler]->gpu.texture_data_size;
     }
    --album_traveler;
  }




  // THIRD , LETS MARK THE VISIBLE PICTURES AS READY TO LOAD TEXTURES!
  album_traveler=FirstPictureVisible;
  while (album_traveler<=LastPictureVisible)
  {

    if (PictureCreationPending(album[album_traveler]))
      {
      }

    if ( (!PictureIsLoadedInSystem(album[album_traveler])) && (!album[album_traveler]->system.marked_for_rgbdata_loading) )
     {  //If the picture is loaded and the hypervisor hasn't already marked it for rgb_data removal do it now !
          album[album_traveler]->system.marked_for_rgbdata_loading=1;
          after_changes_system_usedRAM+=album[album_traveler]->system.rgb_data_size;
     } else
     if (PictureIsLoadedInSystem(album[album_traveler]))
     {

     }
    --album_traveler;
  }






  /* IMPLEMENTATION IS A STUB FOR NOW !*/

  return 1;
}



/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      Pictures are malloced structures that carry RGB data on them
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
int RAM_System_Memory_can_accomodate(unsigned int newfile)
{
  /*
  if ( frame.gpu.maxRAM < newfile + frame.gpu.usedRAM )
    {
       fprintf(stderr,"It looks like there is no free memory on GPU :P \n");
       return 0;
    }*/

   if (frame.system.maxRAM <= frame.system.usedRAM + newfile)
    {
       fprintf(stderr,"System memory bounds reached ( while on range %u - %u [ over %u , last %u ] ) \n",MinPictureThatIsVisible(),MaxPictureThatIsVisible(),frame.active_image_place,frame.last_image_place);
       UnLoadPicturesIfNeeded(0,1); // Clear system RAM , because it has obviously run out :P !
       return 0;
    }
 return 1;
}


int CreatePicturesIfNeeded()
{
  unsigned int MAX_album_traveler=MaxPictureThatIsVisible();
  unsigned int album_traveler=MinPictureThatIsVisible();
  unsigned int created_pictures_this_loop=0;


  char pictures_filename_shared_stack_mem_hyper[1024]={0};
  while (album_traveler<=MAX_album_traveler)
   {
     /*In case the other thread has moved focus , adapt on the fly --*/
       if (album_traveler<MinPictureThatIsVisible()) { album_traveler=MinPictureThatIsVisible(); }
       if (album_traveler>MaxPictureThatIsVisible()) { return created_pictures_this_loop; }
     /*-----------------------------------------------------------*/
                                                        //lastTexture
    if (!RAM_System_Memory_can_accomodate(frame.system.maximum_frame_total_size) )  { /*No point trying to load if it doesnt't fit*/ } else
     {
      if (PictureCreationPending(album[album_traveler])) // We need to create a picture structure , so lets do it!
      { if ( GetViewableFilenameforFile(album_traveler,(char *) frame.album_directory,pictures_filename_shared_stack_mem_hyper) == 1 )
            {
               album[album_traveler]=CreatePicture(pictures_filename_shared_stack_mem_hyper,0);
               if ( album[album_traveler] != 0 )
                 {
                    album[album_traveler]->is_jpeg=list[album_traveler].is_jpeg;
                    album[album_traveler]->directory_list_index=album_traveler; // Create a link between the directory list and the picture struct
                    ++created_pictures_this_loop;
                 } else
                 {
                     fprintf(stderr,"Failed to Create Picture TODO remove it from album and RemoveListItem\n");
                     RemoveListItem(album_traveler);
                 }
            } else { fprintf(stderr,"Could not retrieve filename for album item %u/%u\n",album_traveler, frame.total_images); }
       }
     }
    ++album_traveler;
   }

  return created_pictures_this_loop;
}


int DestroyPicturesIfNeeded()
{
  return 0;
}


int LoadPicturesIfNeeded()
{
  unsigned int MAX_album_traveler=MaxPictureThatIsVisible();
  unsigned int album_traveler=MinPictureThatIsVisible();
  unsigned int loaded_pictures_this_loop=0;


  char pictures_filename_shared_stack_mem_hyper[1024]={0};
  while (album_traveler<=MAX_album_traveler)
   {
     /*In case the other thread has moved focus , adapt on the fly --*/
       if (album_traveler<MinPictureThatIsVisible()) { album_traveler=MinPictureThatIsVisible(); }
       if (album_traveler>MaxPictureThatIsVisible()) { return loaded_pictures_this_loop; }
     /*-----------------------------------------------------------*/

    if (RAM_System_Memory_can_accomodate(frame.gpu.lastTexture) ) //No point trying to load if it doesnt't fit
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

  return loaded_pictures_this_loop;
}

int UnLoadPicturesIfNeeded(unsigned int clear_gpu_ram,unsigned int clear_system_ram)
{

  if ( frame.total_images == 0 ) { return 0; }

  if ( (!clear_system_ram)&&(clear_gpu_ram) )
  { //If we only want to clean GPU RAM and GPU RAM seems to be enough dont do anything :P
    if ((GPU_Memory_can_accomodate(frame.gpu.maximum_frame_total_size))&&(!SignalGPUFull))  { /*No need to unload anything */ return 1;}
  }


  unsigned int MAX_album_traveler=MinPictureThatIsVisible();
  unsigned int album_traveler=0;
  unsigned int unloaded_textures_this_loop=0;
  unsigned int unloaded_pictures_this_loop=0;

  /*SCAN THE PICTURES FROM 0 to MinVisible in order to free up some space*/
  while (album_traveler<MAX_album_traveler)
   {
     /*In case the other thread has moved focus , adapt on the fly --*/
       if (MAX_album_traveler<MinPictureThatIsVisible()) { MAX_album_traveler=MinPictureThatIsVisible(); }
     /*-----------------------------------------------------------*/


    if ((!GPU_Memory_can_accomodate(frame.gpu.maximum_frame_total_size) ) || ( SignalGPUFull ) )
    {
       if ( (PictureTextureLoaded(album[album_traveler])) && (clear_gpu_ram) )
       {
         clear_texture(album[album_traveler]);
         CheckIfSignalGPUFullAppliesAnyMore();
         ++unloaded_textures_this_loop;
       }
       if (clear_system_ram)
       {
         UnLoadPicture(album[album_traveler]);
         ++unloaded_pictures_this_loop;
       }
    }
    ++album_traveler;
   }

  /*SCAN THE PICTURES FROM MaxVisible to END in order to free up some space*/

  album_traveler=frame.total_images-1;
  unsigned int MIN_album_traveler=MaxPictureThatIsVisible();
  while ((album_traveler>MIN_album_traveler) && (album_traveler>0) )
   {
     /*In case the other thread has moved focus , adapt on the fly --*/
       if (album_traveler<=MaxPictureThatIsVisible()) { return unloaded_textures_this_loop; }
     /*-----------------------------------------------------------*/

    if ( (!GPU_Memory_can_accomodate(frame.gpu.maximum_frame_total_size) )|| ( SignalGPUFull ) )
    {
       if ( (PictureTextureLoaded(album[album_traveler])) && (clear_gpu_ram) )
       {
         clear_texture(album[album_traveler]);
         CheckIfSignalGPUFullAppliesAnyMore();
         ++unloaded_textures_this_loop;
       }
       if (clear_system_ram)
       {
         UnLoadPicture(album[album_traveler]);
         ++unloaded_pictures_this_loop;
       }
    }
    if ( album_traveler != 0 ) { --album_traveler; } else
                               { break; }
   }

  if ( (unloaded_pictures_this_loop>0)||(unloaded_textures_this_loop>0) )
   {
      fprintf(stderr,"Unloaded a total of %u textures and %u pictures \n",unloaded_textures_this_loop,unloaded_pictures_this_loop);
   }

  return unloaded_textures_this_loop;
}





int LoadTexturesIfNeeded(int count_only)
{
  /* THIS FUNCITON BELONGS TO THE OPENGL THREAD AND LOADS/UNLOADS IMAGES
     FROM THE GPU AS TEXTURES!!!! THEY HAVE TO BE LOADED BY ManageLoadingPicturesMemory_Thread
  */
  unsigned int fail_count=0,count=0;

   struct timeval start_creating_textures,now,difference;

  gettimeofday(&start_creating_textures,0x0);

  UnLoadPicturesIfNeeded(1,1);

  unsigned int MAX_album_traveler=MaxPictureThatIsVisible();
  unsigned int album_traveler=MinPictureThatIsVisible();

  while (album_traveler<=MAX_album_traveler)
   {
     /*In case the other thread has moved focus , adapt on the fly --*/
       if (album_traveler<MinPictureThatIsVisible()) { album_traveler=MinPictureThatIsVisible(); }
       if (album_traveler>MaxPictureThatIsVisible()) { return count; }
     /*-----------------------------------------------------------*/

     if ( album[album_traveler]->gpu.marked_for_texture_loading ) //PictureLoadedOpenGLTexturePending(album[album_traveler]) not using this for perfromance reasons..!
       {
         ++count;
         if(!count_only)
          {
            if ( !make_texture(album[album_traveler],frame.mipmaping) )
              {
                //Failed making the texture , ( and picture was loaded correctly .. ! )
                ++fail_count;
              } else
              {
                // Only go in the trouble of making the "expensive" timeval call if a texture was loaded ( and thus the system used precious time anyways )
                gettimeofday(&now,0x0);
                unsigned int elapsed_time = timeval_diff(&difference,&now,&start_creating_textures);
                if (elapsed_time>1000)
                {
                 if (PrintDevMsg()) fprintf(stderr,"Stopping texture operation , it takes too long ( %u ) \n",elapsed_time);
                 return count;
                }
              }
          }
        }

      ++album_traveler;
   }

  return count;
}


/*
    -------------------------------------------
                  GENERIC CALLS
     -------------------------------------------
*/

int ManagePicturesCreationMemory()
{
   DestroyPicturesIfNeeded();
   return CreatePicturesIfNeeded();
}


int ManagePicturesLoadingMemory()
{
  // UnLoadPicturesIfNeeded(); THIS ONLY SHOULD BE CALLED FROM THE MAIN THREAD
   return LoadPicturesIfNeeded();
}


int ExecuteMemoryStrategyPlanOnSystemMemory()
{
   ManagePicturesCreationMemory();
   return ManagePicturesLoadingMemory();
}
