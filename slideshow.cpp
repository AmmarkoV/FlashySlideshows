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

#include "slideshow.h"
#include "camera_control.h"
#include "sound.h"
#include "scene_objects.h"
#include <stdlib.h>
#include <stdio.h>

struct SlideShowData frame;

unsigned int time_passed_microseconds=0;

unsigned int ALBUM_SIZE=10000;
struct Picture *album[10000]={0};

struct Point3D up_left={3.6,-2.8,-3.4};
struct Point3D up_right={-3.6,-2.8,-3.4};
struct Point3D down_left={3.6,2.8,-3.4};
struct Point3D down_right={-3.6,2.8,-3.4};

unsigned int DISPLAY_ALL_PICTURES=0; /*DEBUG SWITCH*/
unsigned int LINES_AWAY_DRAWN=2; /*THIS CONTROLS DRAWING TO CONSERVE GPU RESOURCES*/

void SetDisplayAllPictures(unsigned int newstate)
{
  DISPLAY_ALL_PICTURES=newstate;
}

void InitSlideShow()
{
   frame.quality_setting=4; // Medium Quality

   frame.enable_sound_effects=0;
   frame.gpu.usedRAM=0;

   frame.distance_block_upper=20;
   frame.distance_block_lower=-4.0;
   frame.distance_block_left=-20;
   frame.distance_block_right=20;
   frame.distance_block_up=20;
   frame.distance_block_down=-20;

   frame.distance_barrier_after_considered_zoom=-2;
   frame.distance_barrier_after_considered_close=3;
   frame.distance_barrier_after_considered_far=5;

   frame.desired_step_zoom=0.2;
   frame.desired_step_close=1;
   frame.desired_step_far=5;

   frame.images_per_line=3;

   frame.sort_ascending=1; //1=ascending by default
   frame.sort_type=0;

   frame.automatic_slideshow_on=0;
   frame.time_ms_before_last_slide_change=0;
   frame.time_ms_between_two_transitions=5000;

   frame.active_image_x=2;  frame.active_image_y=2;
   frame.active_image_place=4;

   frame.last_image_x=2;  frame.last_image_y=2;
   frame.last_image_place=4;


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

   frame.seek_move_activated=0;
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
   frame.mipmaping=0;

   frame.active_image_x=1;
   frame.active_image_y=1;
   frame.active_image_place=4;

   /* GPU DATA */
   frame.gpu.usedRAM=0;
   frame.gpu.maxRAM=256 * 1024 * 1024;
   frame.gpu.maxTexture=1024; /* MAX TEXTURE DIMENSION , will be updated*/

   /* SYSTEM DATA */
   frame.system.usedRAM=0;
   frame.system.maxRAM=64 * 1024 * 1024;

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

     float icon_z = frame.vz-6.5;
     if ( icon_z < -5 ) { icon_z = -4.5; }


     if ( frame.automatic_slideshow_on==0 )
      {
          SoundLibrary_PlaySound(SLIDESHOW_START);
          SetDestinationOverPicture(frame.active_image_x,frame.active_image_y);
          frame.time_ms_before_last_slide_change=frame.tick_count;
          frame.automatic_slideshow_on=1;
          Add_3DObject(//0,0,0,
                       frame.vx,frame.vy,icon_z,
                       //frame.vx,frame.vy,frame.vz,
                       5,5,/*PLAY*/ 2 ,1000000);

      } else
      {
          SoundLibrary_PlaySound(SLIDESHOW_STOP);
          frame.automatic_slideshow_on=0;
          Add_3DObject(//0,0,0,
                       frame.vx,frame.vy,icon_z,
                       //frame.vx,frame.vy,frame.vz,
                       5,5,/*PAUSE*/ 3 ,1000000);
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

  if ( CameraMoving() ) { }

  unsigned int min_picture=0;
  unsigned int cur_place=frame.active_image_place;
  if ( cur_place > frame.last_image_place ) { cur_place = frame.last_image_place; }


  if ( cur_place  <= frame.images_per_line * LINES_AWAY_DRAWN ) {} else
                                                               { min_picture=cur_place-frame.images_per_line * LINES_AWAY_DRAWN;
                                                                 if ( min_picture-1 >= 0 ) { min_picture-=1; }
                                                               }
  //fprintf(stderr," %u MinPictureThatIsVisible == \n",min_picture);
  return min_picture;
}

int PictureOutOfBounds(unsigned int pic_place)
{
   if ( pic_place > frame.total_images ) { return 1; }
   return 0;
}

int MaxPictureThatIsVisible()
{
  if ( DISPLAY_ALL_PICTURES == 1 ) { return frame.total_images; } /* OVERRIDE UNTIL EVERYTHING IS READY */
  unsigned int cur_place=frame.active_image_place;
  if ( cur_place < frame.last_image_place ) { cur_place = frame.last_image_place; }

  unsigned int max_picture=cur_place + (frame.images_per_line-1) + frame.images_per_line * LINES_AWAY_DRAWN;


  if ( max_picture >= frame.total_images ) { max_picture=frame.total_images-1; }
  //fprintf(stderr," %u MaxPictureThatIsVisible == \n",max_picture);
  return max_picture;
}


int GetPictureCenterCoords(unsigned int pic_place,float *x,float *y,float *z)
{
    if (PictureOutOfBounds(pic_place)) { return 0; }

    *x=album[pic_place]->position.x;
    *y=album[pic_place]->position.y;
    *z=album[pic_place]->position.z;
    return 1;
}



int PicturesNeededToBeLoaded()
{
  return 0;
}



