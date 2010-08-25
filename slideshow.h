#ifndef SLIDESHOW_H_INCLUDED
#define SLIDESHOW_H_INCLUDED




struct SlideShowData
{
  unsigned int fps;
  unsigned int lookahead;
  unsigned int force_mipmap_generation;
  unsigned int max_memory_commited;

  unsigned int fullscreen;
  unsigned int currently_loading;

  unsigned int time_before_next_slide;
  unsigned int next_transition;

   unsigned int pictures_number;
   struct Pictures * pictures_data;

  float distance_block_upper,distance_block_lower;

  float distance_barrier_after_considered_zoom,desired_step_zoom;
  float distance_barrier_after_considered_close,desired_step_close;
  float distance_barrier_after_considered_far,desired_step_far;

  int total_images;
  int images_per_line;
  int active_image_x,active_image_y,active_image_place;

  float desired_x,desired_y,desired_z,desired_step;
  float angle_x,angle_y,angle_z,step;

  float vx,vy,vz;
};

extern struct SlideShowData frame;

void InitSlideShow();

void ToggleAutomaticSlideshow();

#endif // SLIDESHOW_H_INCLUDED
