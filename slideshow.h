#ifndef SLIDESHOW_H_INCLUDED
#define SLIDESHOW_H_INCLUDED

struct MouseState
{
  unsigned int block_mouse_calls_until;

  float last_click_time;
  float last_mouseclick_at_x,last_mouseclick_at_y,last_mouseclick_at_z;

  float mouse_x,mouse_y,mouse_z;
  unsigned int button_pressed;
  unsigned int is_currently_pressed;
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

  unsigned int total_files;

  unsigned int dragging_screen;
  struct MouseState mouse;

  struct FilePicture * pictures_in_folder;
};

extern struct SlideShowData frame;

void InitSlideShow();

void ToggleAutomaticSlideshow();

void AutomaticSlideShowControl_if_needed();

void TriggerEndOfSlideShow();

#endif // SLIDESHOW_H_INCLUDED
