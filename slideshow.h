#ifndef SLIDESHOW_H_INCLUDED
#define SLIDESHOW_H_INCLUDED


#define FRAME_UP_LEFT 1
#define FRAME_UP_RIGHT 2
#define FRAME_DOWN_LEFT 3
#define FRAME_DOWN_RIGHT 4


struct Point3D
{
  float x,y,z;
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

struct SlideShowData
{
  unsigned int tick_count;
  unsigned int fps;
  unsigned int lookahead;
  unsigned int force_mipmap_generation;
  unsigned int max_memory_commited;

  unsigned int fullscreen;
  unsigned int automatic_slideshow_on;
  unsigned int currently_loading;

  unsigned int time_ms_before_last_slide_change;
  unsigned int time_ms_between_two_transitions;
  unsigned int next_transition;

   unsigned int pictures_number;
   struct Pictures * pictures_data;

  float distance_block_upper,distance_block_lower;

  float distance_barrier_after_considered_zoom,desired_step_zoom;
  float distance_barrier_after_considered_close,desired_step_close;
  float distance_barrier_after_considered_far,desired_step_far;

  unsigned int total_images;
  unsigned int images_per_line;
  unsigned int active_image_x,active_image_y,active_image_place;

  float desired_x,desired_y,desired_z,desired_step;
  float angle_x,angle_y,angle_z,step;

  float vx,vy,vz;

  unsigned int effect_move_activated;
  float effect_start_x,effect_start_y,effect_start_z;
  float effect_end_x,effect_end_y,effect_end_z;


  unsigned int total_files;

  unsigned int dragging_screen;
  struct MouseState mouse;

  /*
     SLIDESHOW EFFECTS
  */
  unsigned int fog_on;

  struct FilePicture * pictures_in_folder;
};

extern struct SlideShowData frame;

extern struct Point3D up_left;
extern struct Point3D up_right;
extern struct Point3D down_left;
extern struct Point3D down_right;

void InitSlideShow();

void ToggleAutomaticSlideshow();

void AutomaticSlideShowControl_if_needed();

void TriggerEndOfSlideShow();

#endif // SLIDESHOW_H_INCLUDED
