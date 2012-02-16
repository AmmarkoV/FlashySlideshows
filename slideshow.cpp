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
#include "slideshow.h"
#include "camera_control.h"
#include "sound.h"
#include "scene_objects.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "transitions/basic_transition.h"

struct SlideShowData frame;

unsigned int time_passed_microseconds=0;

unsigned int ALBUM_SIZE=10000;
struct Picture ** album=0;

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

unsigned int CreateSlideshowPictureStructure(unsigned int memory_size)
{
    ALBUM_SIZE=memory_size+150;
    if ( album != 0 ) { free(album); }

   album = ( struct Picture ** ) malloc((ALBUM_SIZE)*sizeof(struct Picture));
   if (album==0) { fprintf(stderr,"Could not allocate structure for picture info storage\n"); return 0; }


   unsigned int i=0;
   //struct Picture zeroPictureElement={0};
   for (i=0; i<ALBUM_SIZE; i++)
    {
       //album[i]=zeroPictureElement;
       album[i]=loading;
    }

   return 1;
}

void DestroySlideshowPictureStructure()
{

}

void InitSlideShow()
{
   srand( time(NULL) );
   frame.background_number = rand()%10;
   if (frame.background_number>10) {frame.background_number=0;}

   frame.transitions.transition_mode=0;

   frame.quality_setting=4; // Medium Quality
   frame.gpu.maximum_frame_total_size=GetWidthQuality(frame.quality_setting)*GetHeightQuality(frame.quality_setting)*4; /*RGBA*/
   frame.gpu.maximum_frame_total_size*=3; // <- Safety Factor
   frame.gpu.maximum_frame_dimension_size=1024;


   frame.enable_sound_effects=0;
   frame.allow_mv_operation_sorting=0;
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

   frame.transitions.automatic_slideshow_on=0;
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

   frame.transitions.currently_loading=0;

   frame.vx=00.0;
   frame.vy=00.0;
   frame.vz=00.0;

   frame.transitions.seek_move_activated=0;
   frame.transitions.effect_move_activated=0;

   frame.effect_start_x=00.0;
   frame.effect_start_y=00.0;
   frame.effect_start_z=00.0;

   frame.effect_end_x=00.0;
   frame.effect_end_y=00.0;
   frame.effect_end_z=00.0;

   frame.total_files=0;
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
   frame.gpu.lastTexture=0;

   /* SYSTEM DATA */
   frame.system.usedRAM=0;
   frame.system.maxRAM=128 * 1024 * 1024;
   frame.system.lastTexture=0;

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
    if ( frame.transitions.automatic_slideshow_on==1 )
      {
          frame.transitions.automatic_slideshow_on=0;
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


     if ( frame.transitions.automatic_slideshow_on==0 )
      {
          SoundLibrary_PlaySound(SLIDESHOW_START);
          SetDestinationOverPicture(frame.active_image_x,frame.active_image_y);
          frame.time_ms_before_last_slide_change=frame.tick_count;
          frame.transitions.automatic_slideshow_on=1;
          Add_3DObject(//0,0,0,
                       frame.vx,frame.vy,icon_z,
                       //frame.vx,frame.vy,frame.vz,
                       5,5,/*PLAY*/ 2 ,1000000);

      } else
      {
          SoundLibrary_PlaySound(SLIDESHOW_STOP);
          frame.transitions.automatic_slideshow_on=0;
          Add_3DObject(//0,0,0,
                       frame.vx,frame.vy,icon_z,
                       //frame.vx,frame.vy,frame.vz,
                       5,5,/*PAUSE*/ 3 ,1000000);
      }
}


void ToggleTransitionMode()
{
    /* 0 = 3d seek , 1 = immediate */
    ++frame.transitions.transition_mode;
    if ( frame.transitions.transition_mode == 2 ) { fprintf(stderr,"Transparency effect (UNDER CONSTRUCTION) enabled\n"); }


    if ( frame.transitions.transition_mode > 2 ) frame.transitions.transition_mode = 0;
}

void AutomaticSlideShowControl_if_needed()
{
  if ( frame.transitions.automatic_slideshow_on==0 ) { return; }


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
                                                                {
                                                                  min_picture=cur_place-frame.images_per_line * LINES_AWAY_DRAWN;
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


unsigned int PictureXYtoID(unsigned int x,unsigned int y)
{
  return x+(y*frame.images_per_line);
}


void PictureIDtoXY(unsigned int * x,unsigned int * y,unsigned int place)
{
  *x=place%frame.images_per_line;
  *y=place/frame.images_per_line;
}

unsigned int GetPictureDirectoryListIndex(unsigned int pic_place)
{
    if (PictureOutOfBounds(pic_place)) { return 0; }


    return album[pic_place]->directory_list_index;
}


int PicturesNeededToBeLoaded()
{
  return 0;
}


//SEEK MOVEMENT  =============================================
int Generic_HandleState_OneTime_Start_MovementToNextPicture()
{
  return HandleState_OneTime_Start_BasicMovementToNextPicture();
}

int Generic_HandleState_During_MovementToNextPicture()
{
  return HandleState_During_BasicMovementToNextPicture();
}

int Generic_HandleState_OneTime_End_MovementToNextPicture()
{
  return HandleState_OneTime_End_BasicMovementToNextPicture();
}


//KEN_BURNS(LIKE) EFFECT ======================================
int Generic_HandleState_OneTime_StartEffect()
{
  return HandleState_OneTime_BasicStartEffect();
}

int Generic_HandleState_During_Effect()
{
  return HandleState_During_BasicEffect();
}

int Generic_HandleState_OneTime_EndEffect()
{
  return HandleState_OneTime_BasicEndEffect();
}



