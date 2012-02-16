#include "pictures_control.h"
#include "slideshow.h"
#include <stdlib.h>
#include <stdio.h>


void PerformPicturesMovement(unsigned int microseconds_of_movement)
{
/* PICTURE CONTROL */
   if ( album[frame.active_image_place]->transparency>album[frame.active_image_place]->target_transparency)
      { album[frame.active_image_place]->transparency-=0.000001*microseconds_of_movement;
        if ( album[frame.active_image_place]->transparency<album[frame.active_image_place]->target_transparency)
         {
            album[frame.active_image_place]->transparency=album[frame.active_image_place]->target_transparency;
         }
       }
         else
   if ( album[frame.active_image_place]->transparency<album[frame.active_image_place]->target_transparency)
      { album[frame.active_image_place]->transparency+=0.000001*microseconds_of_movement;
        if ( album[frame.active_image_place]->transparency>album[frame.active_image_place]->target_transparency)
         {
            album[frame.active_image_place]->transparency=album[frame.active_image_place]->target_transparency;
         }
       }

  if ( album[frame.active_image_place]->transparency!=album[frame.active_image_place]->target_transparency )
    {
        fprintf(stderr,"Transparency %0.5f \n",album[frame.active_image_place]->transparency);
    }
}
