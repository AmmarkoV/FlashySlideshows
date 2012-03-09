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

#ifndef SLIDESHOW_H_INCLUDED
#define SLIDESHOW_H_INCLUDED

#define MAX_PATH 1024

#define FRAME_UP_LEFT 1
#define FRAME_UP_RIGHT 2
#define FRAME_DOWN_LEFT 3
#define FRAME_DOWN_RIGHT 4

#include "load_images.h"

 struct Color
 {
    float r,g,b;
 };

struct Point3D
{
  float x,y,z;
};

struct GPU_Data
{
  unsigned long maxRAM;
  unsigned long maxTexture;
  unsigned long lastTexture;
  unsigned long usedRAM;
  unsigned long maximum_frame_total_size;
  unsigned int  maximum_frame_dimension_size;
};

struct System_Data
{
  unsigned long maxRAM;
  unsigned long lastTexture;
  unsigned long usedRAM;
};

struct Effects_Data
{
  unsigned int fog_on;
};


struct MouseState
{
  unsigned int block_mouse_calls_until;


  float last_click_time;
  float last_mouseclick_at_x,last_mouseclick_at_y,last_mouseclick_at_z;

  float mouse_x,mouse_y,mouse_z;
  unsigned int left_pressed;
  unsigned int right_pressed;
};

struct FilePicture
{
  unsigned int filename_size;
  char * filename;
};

struct TransitionState
{


  unsigned int transition_mode;

  unsigned int automatic_slideshow_on;
  unsigned int currently_loading;

  unsigned int seek_move_activated;
  unsigned int effect_move_activated;

};


struct SlideShowData
{
  unsigned int background_number;
  unsigned int enable_sound_effects;


  unsigned int show_information;

  unsigned int tick_count;
  unsigned int fps;
  unsigned int lookahead;
  unsigned int force_mipmap_generation;
  unsigned int mipmaping;

  struct TransitionState transitions;

  unsigned int quality_setting; //Smaller is better
  unsigned int fullscreen;

  unsigned int allow_operation_move;
  unsigned int allow_operation_copy;
  unsigned int allow_operation_resize;
  char rescale_resolution_string[100];


  unsigned int time_ms_before_last_slide_change;
  unsigned int time_ms_between_two_transitions;
  unsigned int next_transition;

   unsigned int pictures_number;
   struct Pictures * pictures_data;

  float distance_block_upper,distance_block_lower;
  float distance_block_left,distance_block_right,distance_block_up,distance_block_down;


  float distance_barrier_after_considered_zoom,desired_step_zoom;
  float distance_barrier_after_considered_close,desired_step_close;
  float distance_barrier_after_considered_far,desired_step_far;


  unsigned int sort_ascending; //0=descending 1=ascending
  unsigned int sort_type; //0 = name , 1=date , 2=size

  unsigned int total_images_loaded;
  unsigned int recursive;
  unsigned int total_images;
  unsigned int images_per_line;
  unsigned int active_image_x,active_image_y,active_image_place;
  unsigned int last_image_x,last_image_y,last_image_place;

  float desired_x,desired_y,desired_z,desired_step;
  float angle_x,angle_y,angle_z,step;

  float vx,vy,vz;


  float effect_start_x,effect_start_y,effect_start_z;
  float effect_end_x,effect_end_y,effect_end_z;


  unsigned int total_files;

  unsigned int dragging_screen;



  /*
     MOUSE DATA
  */
  struct MouseState mouse;

  /*
     System / GPU DATA
  */
  struct GPU_Data gpu;
  struct System_Data system;

  /*
     SLIDESHOW EFFECTS
  */
  struct Effects_Data effects;

  unsigned char program_data_directory[MAX_PATH];
  unsigned char album_directory[MAX_PATH];
  unsigned char move_directory[MAX_PATH];
  unsigned char copy_directory[MAX_PATH];
  unsigned char resize_directory[MAX_PATH];


  struct FilePicture * pictures_in_folder;
};

extern unsigned int time_passed_microseconds;




extern struct SlideShowData frame;
extern unsigned int ALBUM_SIZE;
extern struct Picture **album;

extern struct Point3D up_left;
extern struct Point3D up_right;
extern struct Point3D down_left;
extern struct Point3D down_right;

void InitSlideShow();

unsigned int CreateSlideshowPictureStructure(unsigned int memory_size);
void DestroySlideshowPictureStructure();

int PictureOutOfBounds(unsigned int pic_place);

void SetDisplayAllPictures(unsigned int newstate);

void ToggleAutomaticSlideshow();

void ToggleTransitionMode();

void AutomaticSlideShowControl_if_needed();

int SetTransitionTime(unsigned int trans_time);

void TriggerEndOfSlideShow();
void TriggerNextPictureSlideShow();

int GetPictureCenterCoords(unsigned int pic_place,float *x,float *y,float *z);
unsigned int GetPictureDirectoryListIndex(unsigned int pic_place);

int MaxPictureThatIsVisible();
int MinPictureThatIsVisible();

unsigned int PictureXYtoID(unsigned int x,unsigned int y);
void PictureIDtoXY(unsigned int * x,unsigned int * y,unsigned int place);


int Generic_HandleState_OneTime_Start_MovementToNextPicture();
int Generic_HandleState_During_MovementToNextPicture();
int Generic_HandleState_OneTime_End_MovementToNextPicture();
int Generic_HandleState_OneTime_StartEffect();
int Generic_HandleState_During_Effect();
int Generic_HandleState_OneTime_EndEffect();

#endif // SLIDESHOW_H_INCLUDED
