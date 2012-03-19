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



   if ( album[frame.active_image_place]->transparency==album[frame.active_image_place]->target_transparency)
      {
         // Do Nothing..! Just skip the following
      } else
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

   if ( album[frame.active_image_place]->rotate==album[frame.active_image_place]->target_rotate)
      {
         // Do Nothing..! Just skip the following
      } else
   if ( album[frame.active_image_place]->rotate>album[frame.active_image_place]->target_rotate)
      { album[frame.active_image_place]->rotate-=0.000001*microseconds_of_movement;
        if ( album[frame.active_image_place]->rotate<album[frame.active_image_place]->target_rotate)
         {
            album[frame.active_image_place]->rotate=album[frame.active_image_place]->target_rotate;
         }
       }
         else
   if ( album[frame.active_image_place]->rotate<album[frame.active_image_place]->target_rotate)
      { album[frame.active_image_place]->rotate+=0.000001*microseconds_of_movement;
        if ( album[frame.active_image_place]->rotate>album[frame.active_image_place]->target_rotate)
         {
            album[frame.active_image_place]->rotate=album[frame.active_image_place]->target_rotate;
         }
       }

}

int ResetPictureRotation()
{
  album[frame.active_image_place]->target_rotate=album[frame.active_image_place]->default_rotate;
  album[frame.active_image_place]->rotate=album[frame.active_image_place]->default_rotate;
  return 1;
}


int FadeInPicture()
{
  album[frame.active_image_place]->transparency=0.0;
  album[frame.active_image_place]->target_transparency=1.0;
  return 1;
}


int FadeOutPicture()
{
  album[frame.active_image_place]->transparency=1.0;
  album[frame.active_image_place]->target_transparency=0.0;
  return 1;
}

int RotatePictureRight()
{
  //fprintf(stderr,"RotatePictureRight is deactivated!\n");
  //return 0;
  album[frame.active_image_place]->target_rotate-=45;
  return 1;
}

int RotatePictureLeft()
{
  //fprintf(stderr,"RotatePictureLeft is deactivated!\n");
  //return 0;
  album[frame.active_image_place]->target_rotate+=45;
  return 1;
}

int FreeRotatePicture(unsigned int max_rotation_either_way)
{
  //fprintf(stderr,"RotatePictureRight is deactivated!\n");
  //return 0;

  signed int target_rotation = rand()%(2*max_rotation_either_way);
  target_rotation-=max_rotation_either_way;

  album[frame.active_image_place]->target_rotate=target_rotation;
  return 1;
}

