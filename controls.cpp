#include <stdlib.h>
#include <stdio.h>

#include "slideshow.h"
#include "camera_control.h"
#include "controls.h"


void inline MouseLook(int x,int y)
{

 //  angle_y -= (float) (x-prev_x)*mouse_freedom;
 //  angle_x -= (float) (y-prev_y)*mouse_freedom;
 //  prev_x = x;
 //  prev_y = y;

}

void Controls_Handle_MouseButtons(int button,int state, int x, int y)
{

}

void Controls_Handle_MouseMotion(int button,int state, int x, int y)
{

}


int MoveToPicture(int direction)
{
  fprintf(stderr,"Move to picture direction = %u \n",direction);
  unsigned int last_active_picture=frame.active_image_place;

  fprintf(stderr,"Picture X/Y was %u / %u \n",frame.active_image_x,frame.active_image_y);
  if ( direction == 1 )
   {  /* UP */
     if ( frame.active_image_y > 0 ) {  frame.active_image_y-=1; }
   } else
    if ( direction == 2 )
   {  /* DOWN */
     if ( frame.active_image_y < 10 ) {  frame.active_image_y+=1; }
   } else
    if ( direction == 3 )
   {  /* LEFT */
     if ( frame.active_image_x > 0 ) {  frame.active_image_x-=1; }
   } else
    if ( direction == 4 )
   {  /* RIGHT */
     if ( frame.active_image_x < frame.images_per_line-1 ) {  frame.active_image_x+=1; }
   }

   fprintf(stderr,"Picture X/Y now is %u / %u \n",frame.active_image_x,frame.active_image_y);
   unsigned int current_active_picture=frame.active_image_x+frame.active_image_y*frame.images_per_line;

   if ( current_active_picture!=last_active_picture )
    {
      fprintf(stderr,"Changing destination\n");
      frame.active_image_place=current_active_picture;
      SetDestinationOverPicture(frame.active_image_x,frame.active_image_y);
      return 1;
    }
   return 0;
}


int Controls_Handle_Keyboard(unsigned char key, int x, int y)
{
    if (frame.vz<frame.distance_barrier_after_considered_zoom)  frame.desired_step=frame.desired_step_zoom; else
    if (frame.vz<frame.distance_barrier_after_considered_close)  frame.desired_step=frame.desired_step_close; else
    if (frame.vz<frame.distance_barrier_after_considered_far)  frame.desired_step=frame.desired_step_far;

    int nokey=0;
    switch (key)
    {
        case 1 : /* UP */ MoveToPicture(1); break;
        case 2 : /* DOWN */ MoveToPicture(2); break;
        case 3 : /* LEFT */ MoveToPicture(3); break;
        case 4 : /* RIGHT */ MoveToPicture(4); break;


        case 13 : /* ENTER */ ToggleAutomaticSlideshow(); break;

        case 'o': SetDestinationCenter();
                  break;

        case 'r': frame.desired_z+=frame.desired_step; break;
        case 'f': frame.desired_z-=frame.desired_step; break;
        case 'a': frame.desired_x+=frame.desired_step; break;
        case 'd': frame.desired_x-=frame.desired_step; break;
        case 's': frame.desired_y+=frame.desired_step; break;
        case 'w': frame.desired_y-=frame.desired_step; break;
        case 'z': frame.angle_x-=0.5; break;
        case 'c': frame.angle_x+=0.5; break;
        case 't': frame.angle_y-=0.5; break;
        case 'g': frame.angle_y+=0.5; break;
        case 'y': frame.angle_z-=0.5; break;
        case 'h': frame.angle_z+=0.5; break;

       default : nokey=1;
            break;
    }

  return nokey;
}
