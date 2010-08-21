#ifndef SLIDESHOW_H_INCLUDED
#define SLIDESHOW_H_INCLUDED




struct SlideShowData
{
   unsigned int lookahead;
   unsigned int max_memory_commited;

   unsigned int time_before_next_slide;
   unsigned int next_transition;

   unsigned int pictures_number;
   struct Pictures * pictures_data;

  float distance_block_upper,distance_block_lower;

  float distance_barrier_after_considered_zoom,desired_step_zoom;
  float distance_barrier_after_considered_close,desired_step_close;
  float distance_barrier_after_considered_far,desired_step_far;


};

extern struct SlideShowData main_slideshow;

void InitSlideShow();

#endif // SLIDESHOW_H_INCLUDED
