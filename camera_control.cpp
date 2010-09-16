#include "slideshow.h"
#include "camera_control.h"
#include "load_images.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "math_3d.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

void CalculateActiveImage_AccordingToPosition()
{
   unsigned int total_y=frame.total_images/frame.images_per_line;

   /* WILL HAVE TO ADD Z CALCULATIONS ( 3d dimension ) to find the picture out where the camera is over*/

   float camera_point[3]={frame.vx,frame.vy,frame.vz};
   float camera_direction[3]={0.0,0.0,-1.0};


   unsigned int x,y,album_traveler=0;
   float top_left[3]={0.0,0.0,-5} , top_right[3]={0.0,0.0,-5} , bot_left[3]={0.0,0.0,-5} , bot_right[3]={0.0,0.0,-5} ;

   for (y=0; y<total_y; y++)
    {
       top_left[1]=album[album_traveler]->position.y - album[album_traveler]->position.size_y;
       bot_left[1]=album[album_traveler]->position.y + album[album_traveler]->position.size_y;
       top_right[1]=top_left[1];
       bot_right[1]=bot_left[1];


     for (x=0; x<frame.images_per_line; x++)
     {
       top_left[0]= album[album_traveler]->position.x-album[album_traveler]->position.size_x;
       top_right[0]=album[album_traveler]->position.x+album[album_traveler]->position.size_x;
       bot_left[0]=top_left[0];
       bot_right[0]=top_right[0];

            if (  rayIntersectsTriangle(camera_point,camera_direction,top_left,top_right,bot_right) )
                {
                   //fprintf(stderr,">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                   //fprintf(stderr,"OVER (%f,%f,%f) PIC UP TRIANGLE %u/%u ",frame.vx,frame.vy,frame.vz,x,y);
                   //fprintf(stderr,"TRIANGLE (%f,%f,%f) (%f,%f,%f) (%f,%f,%f) \n",top_left[0],top_left[1],top_left[2]   ,top_right[0],top_right[1],top_right[2]   ,bot_right[0],bot_right[1],bot_right[2]);
                   frame.active_image_y=y;
                   frame.active_image_x=x;
                   frame.active_image_place = frame.active_image_x+frame.active_image_y*frame.images_per_line;
                   return;
                } else
            if (  rayIntersectsTriangle(camera_point,camera_direction,bot_right,bot_left,top_left) )
                {
                   //fprintf(stderr,">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                   //fprintf(stderr,"OVER (%f,%f,%f) PIC DOWN TRIANGLE %u/%u ",frame.vx,frame.vy,frame.vz,x,y);
                   //fprintf(stderr,"TRIANGLE (%f,%f,%f) (%f,%f,%f) (%f,%f,%f) \n",bot_right[0],bot_right[1],bot_right[2]   ,bot_left[0],bot_left[1],bot_left[2]   ,top_left[0],top_left[1],top_left[2]);
                    frame.active_image_y=y;
                    frame.active_image_x=x;
                    frame.active_image_place = frame.active_image_x+frame.active_image_y*frame.images_per_line;
                   return;
                } else
                {
                  /*NOT OVER PICTURE*/
                }

       ++album_traveler;
     }
    }

}


float RefreshDesiredStep_AccordingToPosition()
{
    /* Stored int desired_step */
    if (frame.vz<frame.distance_barrier_after_considered_zoom)  frame.desired_step=frame.desired_step_zoom; else
    if (frame.vz<frame.distance_barrier_after_considered_close)  frame.desired_step=frame.desired_step_close; else
    if (frame.vz<frame.distance_barrier_after_considered_far)  frame.desired_step=frame.desired_step_far;

    return frame.desired_step;
}

void MoveDestinationCenter(unsigned int axis,unsigned int direction)
{
    /*
       According to zoom factor we will use a desired_step matching it in order to have smooth movement over the image
       RefreshDesiredStep_AccordingToPosition calculates it and loads it in frame.desired_step variable
    */
    RefreshDesiredStep_AccordingToPosition();
    /*---------------------------------------------------------------------------------------------------------------*/


    /* axis ( 0 x , 1 y , 2 z ) */
    /* direction ( 0 + , 1 - ) */

    frame.effect_move_activated=0; /*Overriding hover*/

    switch ( axis )
    {
       case 0 :
        if ( direction == 0 )  frame.desired_x+=frame.desired_step; else
        if ( direction == 1 )  frame.desired_x-=frame.desired_step;
       break;

       case 1 :
        if ( direction == 0 )  frame.desired_y+=frame.desired_step; else
        if ( direction == 1 )  frame.desired_y-=frame.desired_step;
       break;

       case 2 :
        if ( direction == 0 )  frame.desired_z+=frame.desired_step; else
        if ( direction == 1 )  frame.desired_z-=frame.desired_step;
      break;
    };

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
  float vx=0.0,vy=0.0,y_inc=12.0;
  if ( x==0 ) { vx= 14.0; } else
  if ( x==1 ) { vx= 0.0; } else
  if ( x==2 ) { vx=-14.0; }

  vy=-12.0 + y_inc * y;

  frame.desired_x=vx;
  frame.desired_y=vy;
  frame.desired_z=-1.0;
}

void SetDestinationOverPicture_HoverEffect(unsigned int x,unsigned int y,unsigned int position_start,unsigned int position_end)
{
   SetDestinationOverPicture(x,y);

   switch (position_start)
   {
        case FRAME_UP_LEFT :
        frame.effect_start_x=up_left.x;
        frame.effect_start_y=up_left.y;
        frame.effect_start_z=up_left.z;
        break;

        case FRAME_UP_RIGHT :
        frame.effect_start_x=up_right.x;
        frame.effect_start_y=up_right.y;
        frame.effect_start_z=up_right.z;
        break;

        case FRAME_DOWN_LEFT :
        frame.effect_start_x=down_left.x;
        frame.effect_start_y=down_left.y;
        frame.effect_start_z=down_left.z;
        break;

        case FRAME_DOWN_RIGHT :
        frame.effect_start_x=down_right.x;
        frame.effect_start_y=down_right.y;
        frame.effect_start_z=down_right.z;
        break;

        default :
        fprintf(stderr,"Incorrect position :S \n");
        frame.effect_start_x=frame.vx;
        frame.effect_start_y=frame.vy;
        frame.effect_start_z=frame.vz;
        break;
   };


      switch (position_end)
   {
        case FRAME_UP_LEFT :
        frame.effect_end_x=up_left.x;
        frame.effect_end_y=up_left.y;
        frame.effect_end_z=up_left.z;
        break;

        case FRAME_UP_RIGHT :
        frame.effect_end_x=up_right.x;
        frame.effect_end_y=up_right.y;
        frame.effect_end_z=up_right.z;
        break;

        case FRAME_DOWN_LEFT :
        frame.effect_end_x=down_left.x;
        frame.effect_end_y=down_left.y;
        frame.effect_end_z=down_left.z;
        break;

        case FRAME_DOWN_RIGHT :
        frame.effect_end_x=down_right.x;
        frame.effect_end_y=down_right.y;
        frame.effect_end_z=down_right.z;
        break;

        default :
        fprintf(stderr,"Incorrect position :S \n");
        frame.effect_end_x=frame.vx;
        frame.effect_end_y=frame.vy;
        frame.effect_end_z=frame.vz;
        break;
   };

        frame.effect_move_activated=1; /* ACTIVATE START Coords */

        frame.effect_start_x=frame.desired_x+frame.effect_start_x;
        frame.effect_start_y=frame.desired_y+frame.effect_start_y;
        frame.effect_start_z=frame.desired_z+frame.effect_start_z;

        frame.effect_end_x=frame.desired_x+frame.effect_end_x;
        frame.effect_end_y=frame.desired_y+frame.effect_end_y;
        frame.effect_end_z=frame.desired_z+frame.effect_end_z;
}

void PickHoverEffect(unsigned int x,unsigned int y)
{
  unsigned int start,end;
  start=rand()%4;
  end=rand()%4;
  SetDestinationOverPicture_HoverEffect(x,y,start,end);
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

void PerformCameraMovement()
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
    unsigned int reached_target=0;
    float speed_factor = 3/2; /* 1/5 */

    if ( frame.effect_move_activated >= 2 ) { /* Frame is beeing hovered for an effect move , so we prefer slow speed */
                                              speed_factor = 3/2;
                                            } else
                                            {
                                              /* Frame is beeing plainly hovered , so we prefer fast speed */
                                              speed_factor = 1/5;
                                            }

    float speed_multiplier=frame.fps * speed_factor;
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
                           } else { reached_target+=1; } /* + One coordinate has reached target */

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
                           } else { reached_target+=3; } /* + One coordinate has reached target */


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
                           } else { reached_target+=5; } /* + One coordinate has reached target */


 switch ( frame.effect_move_activated )
  {
     case 0 :
      /* DEAD STATE :) */
     break;

     /* Start movement to start */
     case 1 :
      frame.desired_x=frame.effect_start_x;
      frame.desired_y=frame.effect_start_y;
      frame.desired_z=frame.effect_start_z;
      frame.effect_move_activated=2;
     break;

     /* Reach start place*/
     case 2 :
      if ( reached_target >= 9 ) { frame.effect_move_activated=3; }
     break;

     /* Start movement to end */
     case 3 :
      frame.desired_x=frame.effect_end_x;
      frame.desired_y=frame.effect_end_y;
      frame.desired_z=frame.effect_end_z;
      frame.effect_move_activated=4;
     break;

     /* Reach end place*/
     case 4 :
      if ( reached_target >= 9 ) { frame.effect_move_activated = 0; }
     break;

     default :
      fprintf(stderr,"Erroneous state :P \n");
      frame.effect_move_activated = 0;
     break;
  };




  /* -------------------------------------
     CAMERA SAFE GUARD!
     -------------------------------------
  */
   if ( frame.vz<=frame.distance_block_lower) { frame.vz=frame.distance_block_lower; frame.desired_z=frame.vz; } /* DO NOT ALLOW ANY CLOSER */
   if ( frame.vz>=frame.distance_block_upper) { frame.vz=frame.distance_block_upper; frame.desired_z=frame.vz; } /* DO NOT ALLOW ANY CLOSER */

  /* -------------------------------------
     CAMERA ROUNDING ERROR CORRECTION
     -------------------------------------
  */
  if ( ( frame.desired_x > frame.vx ) && ( frame.desired_x < frame.vx+0.005 ) ) { frame.vx = frame.desired_x; } else
  if ( ( frame.desired_x < frame.vx ) && ( frame.desired_x > frame.vx-0.005 ) ) { frame.vx = frame.desired_x; }

  if ( ( frame.desired_y > frame.vy ) && ( frame.desired_y < frame.vy+0.005 ) ) { frame.vy = frame.desired_y; } else
  if ( ( frame.desired_y < frame.vy ) && ( frame.desired_y > frame.vy-0.005 ) ) { frame.vy = frame.desired_y; }

  if ( ( frame.desired_z > frame.vz ) && ( frame.desired_z < frame.vz+0.005 ) ) { frame.vz = frame.desired_z; } else
  if ( ( frame.desired_z < frame.vz ) && ( frame.desired_z > frame.vz-0.005 ) ) { frame.vz = frame.desired_z; }

  /*
     UPDATE ACTIVE IMAGE
  */

  CalculateActiveImage_AccordingToPosition();

}














