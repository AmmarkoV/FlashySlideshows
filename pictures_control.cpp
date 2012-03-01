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


int RotatePictureRight()
{
  //fprintf(stderr,"RotatePictureRight is deactivated!\n");
  //return 0;
  album[frame.active_image_place]->rotate-=45;
  return 1;
}

int RotatePictureLeft()
{
  //fprintf(stderr,"RotatePictureLeft is deactivated!\n");
  //return 0;
  album[frame.active_image_place]->rotate+=45;
  return 1;
}



