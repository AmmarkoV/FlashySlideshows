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

#include "transitions/transition_handler.h"
#include "layouts/layout_handler.h"

#include "slideshow.h"
#include "camera_control.h"
#include "hypervisor/load_images.h"
#include "pictures_control.h"
#include "tools/environment.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tools/math_3d.h"
#include "tools/sound.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

float last_calculated_position_x=0.0;
float last_calculated_position_y=0.0;
float last_calculated_position_z=0.0;


int ChangeActiveImageMem(unsigned int place)
{
  //fprintf(stderr,"ChangeActiveImageMem(%u)\n",place);
   if ( place != frame.active_image_place )
     {
      frame.last_image_x=frame.active_image_x;
      frame.last_image_y=frame.active_image_y;
      frame.last_image_place=frame.active_image_place;

      //PictureIDtoXY(&frame.active_image_x,&frame.active_image_y,place);
      frame.active_image_x=PictureIDtoX(place);
      frame.active_image_y=PictureIDtoY(place);
      frame.active_image_place=place;
     }

    frame.time_ms_before_last_slide_change=frame.tick_count;
   return 1;
}



void RememberThatCalculationAccordingToPositionTookPlace()
{
   last_calculated_position_x=frame.vx;
   last_calculated_position_y=frame.vy;
   last_calculated_position_z=frame.vz;
}

inline int CloserThanDistance(float *x1,float *y1,float *z1,float *x2,float *y2,float *z2,float distance)
{
  if ( (*x1==*x2) && (*y1==*y2)&& (*z1==*z2) ) { return 1; }

  if ( distance*distance >= SquareddistanceBetween3DPoints(x1,y1,z1,x2,y2,z2) ) { return 1; }
  //fprintf(stderr,"Points %0.2f,%0.2f,%0.2f and %0.2f,%0.2f,%0.2f \n",*x1,*y1,*z1,*x2,*y2,*z2);
  //fprintf(stderr,"Failed with distance %0.2f < distance %0.2f \n",distanceBetween3DPoints(x1,y1,z1,x2,y2,z2),distance);
  return 0;
}

void CalculateActiveImage_AccordingToPosition(unsigned char force_check)
{
    if (frame.transitions.seek_move_activated) { return; }
    if (  (!force_check)&&
          (!LayoutMoving())&&
          (CloserThanDistance(&frame.vx,&frame.vy,&frame.vz,&last_calculated_position_x,&last_calculated_position_y,&last_calculated_position_z,2.0))
       ) { return; /*PERFORMANCE ++ */}

   ++frame.total_position_calculations;
   RememberThatCalculationAccordingToPositionTookPlace(); // calculations taking place so! NOTE IT!

   float camera_point[3]={frame.vx,frame.vy,frame.vz};
   float camera_direction[3]={0.0,0.0,-1.0};

   unsigned int x=0,y=0,album_traveler=0;
   float top_left[3]={0.0,0.0,-5} , top_right[3]={0.0,0.0,-5} , bot_left[3]={0.0,0.0,-5} , bot_right[3]={0.0,0.0,-5} ;




   unsigned int start_y=0;
   unsigned int total_y=(unsigned int) frame.total_images / frame.images_per_line;

   // ECONOMY :P
   start_y=(unsigned int) MinPictureThatIsVisible()  / frame.images_per_line;
   if (start_y>=2) { start_y-=2; }
   total_y=(unsigned int) MaxPictureThatIsVisible()  / frame.images_per_line;
   total_y+=3;



   album_traveler=total_y * frame.images_per_line;
   if (album_traveler > frame.total_images) { album_traveler=frame.total_images; }

   float inf_left[3]={0.0,0.0,-5} , inf_right[3]={0.0,0.0,-5};
   if ( !PictureOutOfBounds (total_y*frame.images_per_line) )
     {
       bot_left[0]=album[album_traveler]->position.x - 1000;
       bot_left[1]=album[album_traveler]->position.y + album[album_traveler]->position.size_y;

       bot_right[0]=album[album_traveler]->position.x + 1000;
       bot_right[1]=album[album_traveler]->position.y + album[album_traveler]->position.size_y;

       inf_left[0]=bot_left[0]-1000;  inf_right[0]=bot_right[0]+1000;
       inf_left[1]=bot_left[1]+1000;  inf_right[1]=bot_right[1]+1000;
       if ( rayIntersectsRectangle(camera_point,camera_direction,bot_left,bot_right,inf_left,inf_right) )
            {
                // CAMERA OUT OF LOADED IMAGES! DOWN
                //fprintf(stderr," CAMERA OUT OF LOADED IMAGES! DOWN  was %u/%u ",frame.active_image_x,frame.active_image_y);
                ChangeActiveImageMem(MaxPictureThatIsVisible());
                //fprintf(stderr," now %u/%u \n",frame.active_image_x,frame.active_image_y);
                return;
            }
      }


       start_y=MinPictureThatIsVisible()  / frame.images_per_line; // REDUCE COMPLEXITY 0  / frame.images_per_line
       total_y=MaxPictureThatIsVisible()  / frame.images_per_line; // REDUCE COMPLEXITY frame.total_images / frame.images_per_line
       album_traveler = start_y*frame.images_per_line;
       top_left[0]=album[album_traveler]->position.x - album[album_traveler]->position.size_x;
       top_left[1]=album[album_traveler]->position.y - album[album_traveler]->position.size_y;

       top_right[0]=album[album_traveler]->position.x + album[album_traveler]->position.size_x;
       top_right[1]=album[album_traveler]->position.y - album[album_traveler]->position.size_y;

       inf_left[0]=bot_left[0]-1000;  inf_right[0]=bot_right[0]+1000;
       inf_left[1]=bot_left[1]-1000;  inf_right[1]=bot_right[1]-1000;
       if ( rayIntersectsRectangle(camera_point,camera_direction,inf_left,inf_right,top_left,top_right) )
            {
                // CAMERA OUT OF LOADED IMAGES! UP
                //fprintf(stderr," CAMERA OUT OF LOADED IMAGES! UP  was %u/%u ",frame.active_image_x,frame.active_image_y);
                  ChangeActiveImageMem(MinPictureThatIsVisible());
                //fprintf(stderr," now %u/%u \n",frame.active_image_x,frame.active_image_y);
                return;
            }

   if (PrintDevMsg()) { fprintf(stderr,"The Following code should be improved i think it is buggy :P\n"); }
   album_traveler=start_y * frame.images_per_line;
   if (album_traveler > frame.total_images) { album_traveler=frame.total_images; }

   for (y=start_y; y<total_y; y++)
    {
     top_left[1]=album[album_traveler]->position.y - album[album_traveler]->position.size_y;
     bot_left[1]=album[album_traveler]->position.y + album[album_traveler]->position.size_y;
     top_right[1]=top_left[1]; bot_right[1]=bot_left[1];


     for (x=0; x<frame.images_per_line; x++)
     {
       top_left[0]= album[album_traveler]->position.x-album[album_traveler]->position.size_x;
       top_right[0]=album[album_traveler]->position.x+album[album_traveler]->position.size_x;
       bot_left[0]=top_left[0]; bot_right[0]=top_right[0];

            if ( rayIntersectsRectangle(camera_point,camera_direction,top_left,top_right,bot_right,bot_left) )
            {
                   if ((x!=frame.active_image_x)||(y!=frame.active_image_y))
                    {
                      //fprintf(stderr,"OVER (%f,%f,%f) PIC UP TRIANGLE %u/%u -> %u ",frame.vx,frame.vy,frame.vz,x,y,album_traveler);
                      //fprintf(stderr,"y seek from %u to %u\n",start_y,total_y);
                      ChangeActiveImageMem(album_traveler); //PictureXYtoID(x,y)
                      return;
                    }
                  return;
            }

       ++album_traveler;
       if (album_traveler > frame.total_images) { album_traveler=frame.total_images; return; }
     }
    }
   return ;
}


float RefreshDesiredStep_AccordingToPosition()
{
    /* Stored int frame.desired_step */
    if (frame.vz<frame.distance_barrier_after_considered_zoom)   frame.desired_step=frame.desired_step_zoom; else
    if (frame.vz<frame.distance_barrier_after_considered_close)  frame.desired_step=frame.desired_step_close; else
    if (frame.vz<frame.distance_barrier_after_considered_far)    frame.desired_step=frame.desired_step_far;

    return frame.desired_step;
}

void CameraReachedDestination()
{
  frame.transitions.effect_move_activated=0;
  frame.transitions.seek_move_activated=0;
}


void CameraBounced()
{
  SoundLibrary_PlaySound(UNABLE_TO_MOVE);
}

void MoveDestinationCenterRaw(float x,float y,float z)
{
    frame.transitions.seek_move_activated=0;
    frame.desired_x+=x;
    frame.desired_y+=y;
    frame.desired_z+=z;
}


void MoveDestinationCenter(unsigned int movement_direction)
{
    unsigned int axis=0;
    unsigned int direction=0;

    switch(movement_direction)
    {
       case D_LEFT  : axis=0; direction=0; break;
       case D_RIGHT : axis=0; direction=1; break;
       case D_UP    : axis=1; direction=0; break;
       case D_DOWN  : axis=1; direction=1; break;
       case D_IN    : axis=2; direction=0; break;
       case D_OUT   : axis=2; direction=1; break;
    };

    /*
       According to zoom factor we will use a desired_step matching it in order to have smooth movement over the image
       RefreshDesiredStep_AccordingToPosition calculates it and loads it in frame.desired_step variable
    */
    RefreshDesiredStep_AccordingToPosition();
    /*---------------------------------------------------------------------------------------------------------------*/

    /* axis ( 0 x , 1 y , 2 z ) */
    /* direction ( 0 + , 1 - ) */
    //frame.transitions.effect_move_activated=0; /*Overriding hover*/
    frame.transitions.seek_move_activated=0; /*Setting Destination Over Point cancels seek move!*/

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

   //CalculateActiveImage_AccordingToPosition(1); // <- hardcode coords checking..
}




void SetDestinationCenter()
{
    //frame.transitions.seek_move_activated=0; /*Setting Destination Over Point cancels seek move!*/
    //frame.transitions.effect_move_activated=0;

    frame.desired_x=0; frame.desired_y=0; frame.desired_z=0;
    frame.angle_x=0;   frame.angle_y=0;   frame.angle_z=180;
}


void ResetCameraOrientation()
{
    frame.angle_x=0; frame.angle_y=0; frame.angle_z=180;
}

void SetDestinationOverPicture3dSeek(unsigned int pic_place)
{
  //frame.transitions.seek_move_activated=0; //Setting Destination Over Point cancels seek move!

  //ChangeActiveImageXY(x,y);
  ChangeActiveImageMem(pic_place);

  PositionPicture(album[pic_place],pic_place);

  frame.desired_x=album[pic_place]->position.x;
  frame.desired_y=album[pic_place]->position.y;
  frame.desired_z=album[pic_place]->position.z+4.5;//-0.5;
}

void SetDestinationOverPictureImmediate(unsigned int pic_place)
{
  //Call 3dSeek and basically set position at target :P
  SetDestinationOverPicture3dSeek(pic_place);
  frame.vx=frame.desired_x;
  frame.vy=frame.desired_y;
  frame.vz=frame.desired_z;
}




void SetDestinationOverPicture(unsigned int place)
{
   if (PictureOutOfBounds(place)) { fprintf(stderr,"SetDestinationOverPicture(%u) is an invalid destination\n",place); return; }

   //fprintf(stderr,"SetDestinationOverPicture(%u) called from %u \n",place,frame.active_image_place);


   switch ( frame.transitions.transition_mode)
   {
     case 0 : SetDestinationOverPicture3dSeek(place); ResetPictureRotation(); break;
     case 1 : SetDestinationOverPictureImmediate(place); ResetPictureRotation(); break;
     case 2 :
               fprintf(stderr,"We want to go to %u and we were at %u,%u (%u)\n",place,frame.active_image_x,frame.active_image_y,frame.active_image_place);
               SetDestinationOverPictureImmediate(place);
               CalculateActiveImage_AccordingToPosition(1);

               FadeInPicture();
               FreeRotatePicture(15);

               fprintf(stderr,"Transparency trick destination for pic %u ,  %0.2f -> %0.2f\n",frame.active_image_place,album[frame.active_image_place]->transparency,album[frame.active_image_place]->target_transparency);
              break;
     default :  SetDestinationOverPicture3dSeek(place); ResetPictureRotation(); break;
   };
}


int MoveToPicture(unsigned int direction)
{
  fprintf(stderr,"Move to picture direction = %u \n",direction);
  if (frame.active_image_place!=PictureXYtoID(frame.active_image_x,frame.active_image_y)) { fprintf(stderr,"ERROR Inconsistent x,y place\n"); }

    signed int new_mem_place = (signed int ) frame.active_image_place;

    if ( direction == D_UP   ) { new_mem_place-=frame.images_per_line; } else
    if ( direction == D_DOWN ) { new_mem_place+=frame.images_per_line; } else
    if ( direction == D_LEFT ) { --new_mem_place;                      } else
    if ( direction == D_RIGHT) { ++new_mem_place;                      }

    if (new_mem_place<0) { new_mem_place=0; } else
    if ((unsigned int ) new_mem_place>=frame.total_images) { new_mem_place=frame.total_images-1; }

    SetDestinationOverPicture(new_mem_place);
    frame.transitions.seek_move_activated=1; //THIS MOVEMENT IS A SEEK MOVEMENT SetDestinationOverPicture , sets this to 0 so it is important to set this right here!

   return 1;
}

void SetDestinationOverNextPicture()
{
   unsigned int new_active_picture=frame.active_image_place+1;

    if ( new_active_picture >= frame.total_images ) { /* WE PASSED THE LAST ACTIVE PICTURE SO THERE ACTUALY ISN`t A NEXT PICTURE! */
                                                      TriggerEndOfSlideShow();
                                                      fprintf(stderr,"No more pictures to go to after set destination over next picture\n");
                                                    } else
                                                    { MoveToPicture(D_RIGHT); }
}

int CameraMoving()
{
    return ( (frame.desired_x != frame.vx)||(frame.desired_y != frame.vy)||(frame.desired_z != frame.vz) );
}

int CameraOverPicture(unsigned int x,unsigned int y)
{
    return ( (frame.active_image_x==x)&&(frame.active_image_y==y) );
}

int CameraOverPicture(unsigned int pic_place)
{
    return (frame.active_image_place==pic_place);
}

int CameraSeesOnlyOnePicture()
{
  if ( abs(frame.vx-frame.desired_x)>3 ) { return 0; }
  if ( abs(frame.vy-frame.desired_y)>3 ) { return 0; }


  if ( frame.vz<=GetLayoutMinimumZ()+1.9  )
   {
     float xcoord,ycoord,zcoord,distance;
     GetPictureCenterCoords(frame.active_image_place,&xcoord,&ycoord,&zcoord);
     distance=distanceBetween3DPoints(&xcoord,&ycoord,&zcoord,&frame.vx,&frame.vy,&frame.vz);

     if ((distance < 12.4) && (album[frame.active_image_place]->position.size_x>album[frame.active_image_place]->position.size_y) )
      {
        //TODO :P fprintf(stderr,"Only one picture ( distance = %0.2f ) \n",distance);
        fprintf(stderr,"Check Camera sees Only One Picture for %0.2f,%0.2f image and z=%0.2f\n",album[frame.active_image_place]->position.size_x,album[frame.active_image_place]->position.size_y,frame.vz);
        return 1;
      }
   }

  return 0;
}

int CameraSeesBackground()
{
   return (!CameraSeesOnlyOnePicture());
}

void PerformCameraMovement(unsigned int microseconds_of_movement)
{
   /*
       THE IDEA IS THE FOLLOWING
       We have the 3d states  of the desired coordinates ( desired_x , desired_y , desired_z )
       and the current render coordinates ( vx , vy , vz )

       We need to make a smooth transition to the desired coordinates from the current coordinates
    */

    if ( ( frame.desired_x == frame.vx ) &&
         ( frame.desired_y == frame.vy ) &&
         ( frame.desired_z == frame.vz )
       )
       {
           // This is the case when camera is supposed to be still , so we really should avoid re calculating the same things
           // just to find out that the camera goes nowhere :P ( PERFORMANCE ++ )
           CameraReachedDestination();
           return ;
       }

    /* -------------------------------------
        CAMERA SMOOTH ZOOM/PAN ETC
       ------------------------------------- */
    unsigned int reached_target=0;


    float speed=2; // 2 is a good value , 1 is an also good value ( faster :P )
    float speed_factor = 3/2; /* 1/5 */

    if ( frame.transitions.effect_move_activated >= 2 ) { /* Frame is beeing hovered for an effect move , so we prefer slow speed */
                                              speed_factor = (float) 3/2;
                                            } else
                                            {
                                              /* Frame is beeing plainly hovered , so we prefer fast speed */
                                              speed_factor = (float) 1/5;
                                            }


    float speed_multiplier = 1.4;
    if ( microseconds_of_movement != 0 ) { speed_multiplier=(float) 1000000/microseconds_of_movement; }


    float speed_rate = 1/(speed * speed_factor * speed_multiplier);

    //fprintf(stderr,"Microsecs : %u , SpeedRate %0.4f SpeedMultiplier %0.2f\n",microseconds_of_movement,speed_rate,speed_multiplier);

    if ( frame.desired_x != frame.vx )
                           {
                             if ( frame.desired_x < frame.vx )
                                 { /* CLOSING IN OPERATION */
                                   frame.vx -= ( frame.vx - frame.desired_x ) * speed_rate;
                                   if ( frame.desired_x > frame.vx ) { frame.desired_x = frame.vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_x > frame.vx )
                                 { /* CLOSING IN OPERATION */
                                   frame.vx += ( frame.desired_x - frame.vx ) * speed_rate;
                                   if ( frame.desired_x < frame.vx ) { frame.desired_x = frame.vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           } else { reached_target+=1; } /* + One coordinate has reached target */

    if ( frame.desired_y != frame.vy )
                          {
                             if ( frame.desired_y < frame.vy )
                                 { /* CLOSING IN OPERATION */
                                   frame.vy -= ( frame.vy - frame.desired_y ) * speed_rate;
                                   if ( frame.desired_y > frame.vy ) { frame.desired_y = frame.vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_y > frame.vy )
                                 { /* CLOSING IN OPERATION */
                                   frame.vy += ( frame.desired_y - frame.vy ) * speed_rate;
                                   if ( frame.desired_y < frame.vy ) { frame.desired_y = frame.vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           } else { reached_target+=3; } /* + One coordinate has reached target */


    if ( frame.desired_z != frame.vz )
                          {
                              if ( frame.desired_z < frame.vz )
                                 { /* CLOSING IN OPERATION */
                                   frame.vz -= ( frame.vz - frame.desired_z ) * speed_rate;
                                   if ( frame.desired_z > frame.vz ) { frame.desired_z = frame.vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_z > frame.vz )
                                 { /* CLOSING IN OPERATION */
                                   frame.vz += ( frame.desired_z - frame.vz ) * speed_rate;
                                   if ( frame.desired_z < frame.vz ) { frame.desired_z = frame.vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           } else { reached_target+=5; } /* + One coordinate has reached target */



  /* -------------------------------------
     CAMERA ROUNDING ERROR CORRECTION
     ------------------------------------- */
  if ( ( frame.desired_x > frame.vx ) && ( frame.desired_x < frame.vx+0.005 ) ) { frame.vx = frame.desired_x; } else
  if ( ( frame.desired_x < frame.vx ) && ( frame.desired_x > frame.vx-0.005 ) ) { frame.vx = frame.desired_x; }

  if ( ( frame.desired_y > frame.vy ) && ( frame.desired_y < frame.vy+0.005 ) ) { frame.vy = frame.desired_y; } else
  if ( ( frame.desired_y < frame.vy ) && ( frame.desired_y > frame.vy-0.005 ) ) { frame.vy = frame.desired_y; }

  if ( ( frame.desired_z > frame.vz ) && ( frame.desired_z < frame.vz+0.005 ) ) { frame.vz = frame.desired_z; } else
  if ( ( frame.desired_z < frame.vz ) && ( frame.desired_z > frame.vz-0.005 ) ) { frame.vz = frame.desired_z; }


  if  ( (frame.desired_x==frame.vx)&&(frame.desired_y==frame.vy)&&(frame.desired_z==frame.vz) ) { CameraReachedDestination(); }

  CalculateActiveImage_AccordingToPosition(0);



  /* -------------------------------------
     CAMERA SAFE GUARD!
     ------------------------------------- */
   if ( frame.vx<GetLayoutMinimumX())  { frame.vx=GetLayoutMinimumX(); frame.desired_x=frame.vx; CameraBounced(); } else/* DO NOT ALLOW ANY LEFTER */
   if ( frame.vx>GetLayoutMaximumX())  { frame.vx=GetLayoutMaximumX(); frame.desired_x=frame.vx; CameraBounced(); }     /* DO NOT ALLOW ANY RIGHTER */

   //BUGGY if ( frame.vy<GetLayoutMinimumY())  { frame.vy=GetLayoutMinimumY(); frame.desired_y=frame.vy; CameraBounced(); } else/* DO NOT ALLOW ANY UPPER */
   //if ( frame.vy>GetLayoutMaximumY())  { frame.vy=GetLayoutMaximumY(); frame.desired_y=frame.vy; CameraBounced(); }     /* DO NOT ALLOW ANY DOWNER */

   if ( frame.vz<GetLayoutMinimumZ())  { frame.vz=GetLayoutMinimumZ(); frame.desired_z=frame.vz; CameraBounced(); } else/* DO NOT ALLOW ANY CLOSER */
   if ( frame.vz>GetLayoutMaximumZ())  { frame.vz=GetLayoutMaximumZ(); frame.desired_z=frame.vz; CameraBounced(); }     /* DO NOT ALLOW ANY FURTHER */

}
