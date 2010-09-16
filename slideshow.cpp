#include "slideshow.h"
#include "camera_control.h"
#include <stdlib.h>
#include <stdio.h>

struct SlideShowData frame;

unsigned int ALBUM_SIZE=4096;
struct Picture *album[4096]={0};

struct Point3D up_left={3.6,-2.8,-3.4};
struct Point3D up_right={-3.6,-2.8,-3.4};
struct Point3D down_left={3.6,2.8,-3.4};
struct Point3D down_right={-3.6,2.8,-3.4};

unsigned int DISPLAY_ALL_PICTURES=0; /*DEBUG SWITCH*/

void InitSlideShow()
{
   frame.gpu.usedRAM=0;

   frame.distance_block_upper=20;
   frame.distance_block_lower=-4.0;

   frame.distance_barrier_after_considered_zoom=-2;
   frame.distance_barrier_after_considered_close=3;
   frame.distance_barrier_after_considered_far=5;

   frame.desired_step_zoom=0.2;
   frame.desired_step_close=1;
   frame.desired_step_far=5;

   frame.images_per_line=3;

   frame.automatic_slideshow_on=0;
   frame.time_ms_before_last_slide_change=0;
   frame.time_ms_between_two_transitions=5000;

   frame.active_image_x=2;  frame.active_image_y=2;
   frame.active_image_place=4;

   frame.desired_x=00.0; frame.desired_y=00.0; frame.desired_z=14.0;
   frame.desired_step=1.35;

   frame.angle_x=0.0;
   frame.angle_y=0.0;
   frame.angle_z=180.0;

   frame.step=0.05;

   frame.currently_loading=0;

   frame.vx=00.0;
   frame.vy=00.0;
   frame.vz=00.0;


   frame.effect_move_activated=0;

   frame.effect_start_x=00.0;
   frame.effect_start_y=00.0;
   frame.effect_start_z=00.0;

   frame.effect_end_x=00.0;
   frame.effect_end_y=00.0;
   frame.effect_end_z=00.0;

   frame.total_images_loaded=0;
   frame.total_images=0;
   frame.fullscreen=0;
   frame.force_mipmap_generation = 0;

   frame.active_image_x=1;
   frame.active_image_y=1;
   frame.active_image_place=4;

   /* GPU DATA */
   frame.gpu.usedRAM=0;
   frame.gpu.maxRAM=512 * 1024 * 1024;
   frame.gpu.maxTexture=1024; /* MAX TEXTURE DIMENSION , will be updated*/

   /* SYSTEM DATA */
   frame.system.usedRAM=0;
   frame.system.maxRAM=128 * 1024 * 1024;

   /* EFFECTS */
   frame.effects.fog_on=0;

   /*MOUSE SPECIFIC*/
   frame.dragging_screen=0;
   frame.mouse.mouse_x=0;
   frame.mouse.mouse_y=0;
   frame.mouse.mouse_z=0;
   frame.mouse.left_pressed=0;
   frame.mouse.right_pressed=0;


   frame.pictures_in_folder=0;
}

void TriggerEndOfSlideShow()
{
    if ( frame.automatic_slideshow_on==1 )
      {
          frame.automatic_slideshow_on=0;
      }
}

void TriggerNextPictureSlideShow()
{
    SetDestinationOverNextPicture();
    frame.time_ms_before_last_slide_change=frame.tick_count;
}

void ToggleAutomaticSlideshow()
{
     if ( frame.automatic_slideshow_on==0 )
      {
          SetDestinationOverPicture(frame.active_image_x,frame.active_image_y);
          frame.time_ms_before_last_slide_change=frame.tick_count;
          frame.automatic_slideshow_on=1;
      } else
      {
          frame.automatic_slideshow_on=0;
      }
}

void AutomaticSlideShowControl_if_needed()
{
  if ( frame.automatic_slideshow_on==0 ) { return; }


     if ( frame.tick_count-frame.time_ms_before_last_slide_change > frame.time_ms_between_two_transitions )
       {
         TriggerNextPictureSlideShow();
       }
}


int SlideshowFirstPictureToLoad()
{
  return 0;
}


int SlideshowLastPictureToLoad()
{
  return 0;
}



int MinPictureThatIsVisible()
{
  if ( DISPLAY_ALL_PICTURES == 1 ) { return 0; }/* OVERRIDE UNTIL EVERYTHING IS READY */

  unsigned int min_picture=0;
  if ( frame.active_image_place  < frame.images_per_line * 3 ) {} else
                                                               { min_picture=frame.active_image_place-frame.images_per_line * 3;
                                                                 if ( min_picture -1 > 0 ) { min_picture-=1; }
                                                               }
  fprintf(stderr," %u MinPictureThatIsVisible == \n",min_picture);
  return min_picture;
}

int MaxPictureThatIsVisible()
{
  if ( DISPLAY_ALL_PICTURES == 1 ) { return frame.total_images; } /* OVERRIDE UNTIL EVERYTHING IS READY */

  unsigned int max_picture=frame.active_image_place + frame.images_per_line * 3;
  if ( max_picture >= frame.total_images ) { max_picture=frame.total_images-1; }
  fprintf(stderr," %u MaxPictureThatIsVisible == \n",max_picture);
  return max_picture;
}

int PicturesNeededToBeLoaded()
{
  return 0;
}



