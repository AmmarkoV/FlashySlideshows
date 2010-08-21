#include "slideshow.h"

struct SlideShowData main_slideshow;


void InitSlideShow()
{
   main_slideshow.distance_block_upper=20;
   main_slideshow.distance_block_lower=-4.0;

   main_slideshow.distance_barrier_after_considered_zoom=-2;
   main_slideshow.distance_barrier_after_considered_close=3;
   main_slideshow.distance_barrier_after_considered_far=5;

   main_slideshow.desired_step_zoom=0.2;
   main_slideshow.desired_step_close=1;
   main_slideshow.desired_step_far=5;
}
