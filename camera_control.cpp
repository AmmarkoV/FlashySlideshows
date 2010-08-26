#include "slideshow.h"
#include "camera_control.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif


float RefreshDesiredStep_AccordingToPosition()
{
    /* Stored int desired_step */
    if (frame.vz<frame.distance_barrier_after_considered_zoom)  frame.desired_step=frame.desired_step_zoom; else
    if (frame.vz<frame.distance_barrier_after_considered_close)  frame.desired_step=frame.desired_step_close; else
    if (frame.vz<frame.distance_barrier_after_considered_far)  frame.desired_step=frame.desired_step_far;

    return frame.desired_step;
}



void SetDestinationCenter()
{
    frame.desired_x=0;
    frame.desired_y=0;
    frame.desired_z=0;

    frame.angle_x=0;
    frame.angle_y=0;
    frame.angle_z=180;

}

void SetDestinationOverPicture(unsigned int x,unsigned int y)
{
  float vx=0.0,vy=0.0;
  if ( x==0 ) { vx= 14.0; } else
  if ( x==1 ) { vx= 0.0; } else
  if ( x==2 ) { vx=-14.0; }

  if ( y==0 ) { vy=-12.0; } else
  if ( y==1 ) { vy= 0.0; } else
  if ( y==2 ) { vy= 12.0; }


  frame.desired_x=vx;
  frame.desired_y=vy;
  frame.desired_z=-1.0;
}

void SetDestinationOverNextPicture()
{
   unsigned int new_active_x=frame.active_image_x;
   unsigned int new_active_y=frame.active_image_y;
   unsigned int new_active_picture=frame.active_image_place;

   if ( new_active_x>=frame.images_per_line-1 )
    {
      new_active_x=0;
      ++new_active_y;
    } else
    {
      ++new_active_x;
    }

    new_active_picture = new_active_x+new_active_y*frame.images_per_line;

    if ( new_active_picture >= frame.total_images ) { /* WE PASSED THE LAST ACTIVE PICTURE SO THERE ACTUALY ISN`t A NEXT PICTURE! */
                                                      TriggerEndOfSlideShow();
                                                    } else
    if ( new_active_picture == frame.active_image_place )
                                                    {
                                                      /* Weirdly no change was made to the image this is a bug ? Should stop slide show */
                                                      TriggerEndOfSlideShow();
                                                    } else
                                                    {
                                                       /*There is a next picture :) , we`re gonna change to it*/
                                                       fprintf(stderr,"New active picture is %u / %u \n ",new_active_picture+1, frame.total_images);

                                                       frame.active_image_x=new_active_x;
                                                       frame.active_image_y=new_active_y;
                                                       frame.active_image_place=new_active_picture;
                                                       SetDestinationOverPicture(new_active_x,new_active_y);
                                                    }
}

void PerformCameraStep()
{
   /*
       THE IDEA IS THE FOLLOWING
       We have to 3d states the desired coordinates ( desired_x , desired_y , desired_z )
       and the current render coordinates ( vx , vy , vz )

       We need to make a smooth transition to the desired coordinates from the current coordinates

    */


    /*
      -------------------------------------
      CAMERA SMOOTH ZOOM/PAN ETC
      -------------------------------------
    */
    float speed_multiplier=frame.fps/5; // 250;
    if ( speed_multiplier == 0 ) speed_multiplier=250;


    if ( frame.desired_x != frame.vx ) { if ( frame.desired_x < frame.vx )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vx = frame.vx- ( frame.vx - frame.desired_x ) / speed_multiplier;
                                   if ( frame.desired_x > frame.vx ) { frame.desired_x = frame.vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_x > frame.vx )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vx = frame.vx+ ( frame.desired_x - frame.vx ) / speed_multiplier;
                                   if ( frame.desired_x < frame.vx ) { frame.desired_x = frame.vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }

    if ( frame.desired_y != frame.vy ) { if ( frame.desired_y < frame.vy )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vy = frame.vy- ( frame.vy - frame.desired_y ) / speed_multiplier;
                                   if ( frame.desired_y > frame.vy ) { frame.desired_y = frame.vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_y > frame.vy )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vy = frame.vy+ ( frame.desired_y - frame.vy ) / speed_multiplier;
                                   if ( frame.desired_y < frame.vy ) { frame.desired_y = frame.vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }


    if ( frame.desired_z != frame.vz ) { if ( frame.desired_z < frame.vz )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vz = frame.vz- ( frame.vz - frame.desired_z ) / speed_multiplier;
                                   if ( frame.desired_z > frame.vz ) { frame.desired_z = frame.vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_z > frame.vz )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vz = frame.vz+ ( frame.desired_z - frame.vz ) / speed_multiplier;
                                   if ( frame.desired_z < frame.vz ) { frame.desired_z = frame.vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }
  /* -------------------------------------
     CAMERA SAFE GUARD!
     -------------------------------------
  */
   if ( frame.vz<=frame.distance_block_lower) { frame.vz=frame.distance_block_lower; frame.desired_z=frame.vz; } /* DO NOT ALLOW ANY CLOSER */
   if ( frame.vz>=frame.distance_block_upper) { frame.vz=frame.distance_block_upper; frame.desired_z=frame.vz; } /* DO NOT ALLOW ANY CLOSER */

}
