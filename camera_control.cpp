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

#include "transitions/basic_transition.h"

#include "slideshow.h"
#include "camera_control.h"
#include "load_images.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "math_3d.h"
#include "sound.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif


int ChangeActiveImage(unsigned int place)
{
   if ( place != frame.active_image_place )
     {
      frame.last_image_x=frame.active_image_x;
      frame.last_image_y=frame.active_image_y;
      frame.last_image_place=frame.active_image_place;
      frame.active_image_place=place;
      PictureIDtoXY(&frame.active_image_x,&frame.active_image_y,place);
     }

    return 1;
}


int ChangeActiveImage(unsigned int x,unsigned int y)
{

    if ( ( x != frame.active_image_x ) || ( y != frame.active_image_y ) )
    {
     fprintf(stderr,"Active Image is now %u,%u -> %u,%u -> %u,%u (now) \n",
             frame.last_image_x,frame.last_image_y ,
             frame.active_image_x, frame.active_image_y,
             x,y );
     frame.last_image_x=frame.active_image_x;
     frame.last_image_y=frame.active_image_y;
     frame.last_image_place=frame.active_image_place;

     frame.active_image_x=x;
     frame.active_image_y=y;
     frame.active_image_place=PictureXYtoID(x,y);

     PrintPictureData(album[frame.active_image_place]);

    }

  return 1;
}

void CalculateActiveImage_AccordingToPosition()
{
   float camera_point[3]={frame.vx,frame.vy,frame.vz};
   float camera_direction[3]={0.0,0.0,-1.0};

   unsigned int x,y,album_traveler;
   float top_left[3]={0.0,0.0,-5} , top_right[3]={0.0,0.0,-5} , bot_left[3]={0.0,0.0,-5} , bot_right[3]={0.0,0.0,-5} ;




   unsigned int start_y=0;
   unsigned int total_y=(unsigned int) frame.total_images / frame.images_per_line;

   // ECONOMY :P
   start_y=(unsigned int) MinPictureThatIsVisible()  / frame.images_per_line;
   total_y=(unsigned int) MaxPictureThatIsVisible()  / frame.images_per_line;
   //total_y+=3;



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
                ChangeActiveImage(MaxPictureThatIsVisible());
              //  ChangeActiveImage((unsigned int) MaxPictureThatIsVisible()%frame.images_per_line,
              //                    (unsigned int) MaxPictureThatIsVisible()/frame.images_per_line );
                //fprintf(stderr," now %u/%u \n",frame.active_image_x,frame.active_image_y);
                return;
            }
      }


       start_y=MinPictureThatIsVisible() /* REDUCE COMPLEXITY 0 */  / frame.images_per_line;
       total_y=MaxPictureThatIsVisible() /* REDUCE COMPLEXITY frame.total_images*/  / frame.images_per_line;
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
                             //  ChangeActiveImage((unsigned int) MinPictureThatIsVisible()%frame.images_per_line,
                             //                    (unsigned int) MinPictureThatIsVisible()/frame.images_per_line );
                  ChangeActiveImage(MinPictureThatIsVisible());
                //fprintf(stderr," now %u/%u \n",frame.active_image_x,frame.active_image_y);
                return;
            }


   album_traveler=start_y * frame.images_per_line;
   if (album_traveler > frame.total_images) { album_traveler=frame.total_images; }

   for (y=start_y; y<total_y; y++)
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

            if ( rayIntersectsRectangle(camera_point,camera_direction,top_left,top_right,bot_right,bot_left) )
            {
                //fprintf(stderr,">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                   //fprintf(stderr,"OVER (%f,%f,%f) PIC UP TRIANGLE %u/%u ",frame.vx,frame.vy,frame.vz,x,y);
                   //fprintf(stderr,"RECTANGLE (%f,%f,%f) (%f,%f,%f) (%f,%f,%f) (%f,%f,%f)\n",top_left[0],top_left[1],top_left[2]   ,top_right[0],top_right[1],top_right[2]   ,bot_right[0],bot_right[1],bot_right[2],bot_left[0],bot_left[1],bot_left[2]);

                   //frame.active_image_y=y;
                   //frame.active_image_x=x;
                   //frame.active_image_place = frame.active_image_x+frame.active_image_y*frame.images_per_line;

                   if ((x!=frame.active_image_x)||(y!=frame.active_image_y))
                    {
                      fprintf(stderr,"OVER (%f,%f,%f) PIC UP TRIANGLE %u/%u ",frame.vx,frame.vy,frame.vz,x,y);
                      fprintf(stderr,"y seek from %u to %u\n",start_y,total_y);
                      ChangeActiveImage(x,y);
                    }
                  return;
            }
                 else
                {
                  /*NOT OVER PICTURE*/
                }

       ++album_traveler;
       if (album_traveler > frame.total_images) { album_traveler=frame.total_images; return; }
     }
    }

}


float RefreshDesiredStep_AccordingToPosition()
{
    /* Stored int frame.desired_step */
    if (frame.vz<frame.distance_barrier_after_considered_zoom)  frame.desired_step=frame.desired_step_zoom; else
    if (frame.vz<frame.distance_barrier_after_considered_close)  frame.desired_step=frame.desired_step_close; else
    if (frame.vz<frame.distance_barrier_after_considered_far)  frame.desired_step=frame.desired_step_far;

    return frame.desired_step;
}

void CameraReachedDestination()
{
  frame.transitions.effect_move_activated=0;
  frame.transitions.seek_move_activated=0;
}


void CheckForTargetInBounds()
{
   unsigned int camera_safe_guard_activated=0;
   if ( frame.desired_z<frame.distance_block_lower) { frame.desired_z=frame.distance_block_lower; camera_safe_guard_activated=1; } /* DO NOT ALLOW ANY CLOSER */
   if ( frame.desired_z>frame.distance_block_upper) { frame.desired_z=frame.distance_block_upper; camera_safe_guard_activated=1; } /* DO NOT ALLOW ANY CLOSER */

   if ( camera_safe_guard_activated ) { SoundLibrary_PlaySound(UNABLE_TO_MOVE); }
}


void MoveDestinationCenterRaw(float x,float y,float z)
{
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

    frame.transitions.effect_move_activated=0; /*Overriding hover*/
    frame.transitions.seek_move_activated=1; /*Setting Destination Over Point cancels seek move!*/

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

   // If target is out of bounds fix it
   CheckForTargetInBounds();
   CalculateActiveImage_AccordingToPosition(); // <- hardcode coords checking..
}




void SetDestinationCenter()
{
    frame.transitions.seek_move_activated=0; /*Setting Destination Over Point cancels seek move!*/
    frame.transitions.effect_move_activated=0;

    frame.desired_x=0;
    frame.desired_y=0;
    frame.desired_z=0;

    frame.angle_x=0;
    frame.angle_y=0;
    frame.angle_z=180;
}


void SetDestinationOverPicture3dSeek(unsigned int x,unsigned int y)
{
  frame.transitions.seek_move_activated=0; //Setting Destination Over Point cancels seek move!

  ChangeActiveImage(x,y);

  float vx=0.0,vy=0.0,y_inc=12.0;
  if ( x==0 ) { vx= 14.0; } else
  if ( x==1 ) { vx= 0.0; } else
  if ( x==2 ) { vx=-14.0; } else
              {
                vx=(x-1)*-14.0;
              }

  vy=-12.0 + y_inc * y;

  frame.desired_x=vx;
  frame.desired_y=vy;
  frame.desired_z=-0.5;
}

void SetDestinationOverPictureImmediate(unsigned int x,unsigned int y)
{
  //Call 3dSeek and basically set position at target :P
  SetDestinationOverPicture3dSeek(x,y);
  frame.vx=frame.desired_x;
  frame.vy=frame.desired_y;
  frame.vz=frame.desired_z;
}



void SetDestinationOverPicture(unsigned int x,unsigned int y)
{
   unsigned int place = PictureXYtoID(x,y);

   switch ( frame.transitions.transition_mode)
   {
     case 0 : SetDestinationOverPicture3dSeek(x,y); break;
     case 1 : SetDestinationOverPictureImmediate(x,y); break;
     case 2 :
               fprintf(stderr,"We want to go to %u,%u (%u) and we were at %u,%u (%u)\n",x,y,place,frame.active_image_x,frame.active_image_y,frame.active_image_place);
               SetDestinationOverPictureImmediate(x,y);

               album[frame.active_image_place]->transparency=0.0;
               album[frame.active_image_place]->target_transparency=1.0;
               fprintf(stderr,"Transparency trick destination for pic %u ,  %0.2f -> %0.2f\n",frame.active_image_place,album[frame.active_image_place]->transparency,album[frame.active_image_place]->target_transparency);
              break;
     default :  SetDestinationOverPicture3dSeek(x,y);  break;
   };
}


int MoveToPicture(unsigned int direction)
{
  fprintf(stderr,"Move to picture direction = %u \n",direction);
  if (frame.active_image_place!=PictureXYtoID(frame.active_image_x,frame.active_image_y)) { fprintf(stderr,"ERROR Inconsistent x,y place\n"); }
  unsigned int last_active_picture=frame.active_image_place;
  unsigned int last_line=0;
  if ( frame.images_per_line>0) last_line=frame.total_images/frame.images_per_line;

  fprintf(stderr,"Picture X/Y was %u / %u \n",frame.active_image_x,frame.active_image_y);
  unsigned int image_x=frame.active_image_x,image_y=frame.active_image_y;
    if ( direction == D_UP )
                               {  /* UP */
                                    fprintf(stderr,"For UP at %u,%u we got ",image_x,image_y);
                                    if ( image_y > 0 ) {  image_y-=1; }
                                    fprintf(stderr,"%u,%u\n",image_x,image_y);
                               } else
    if ( direction == D_DOWN )
                               {  /* DOWN */
                                    fprintf(stderr,"For DOWN at %u,%u we got ",image_x,image_y);
                                    if ( image_y < last_line-1 ) {  image_y+=1; }
                                    fprintf(stderr,"%u,%u\n",image_x,image_y);
                               } else
    if ( direction == D_LEFT ) {  /* LEFT */
                                    fprintf(stderr,"For LEFT at %u,%u we got ",image_x,image_y);
                                    if ( image_x > 0 ) {  image_x-=1; } else
                                    { //Go to the previous row functionality :P
                                       if ( image_y >0 )
                                       {
                                           image_x=frame.images_per_line-1;
                                         --image_y;
                                       }
                                    }
                                    fprintf(stderr,"%u,%u\n",image_x,image_y);
                               } else
    if ( direction == D_RIGHT ) {  /* RIGHT */
                                    fprintf(stderr,"For RIGHT at %u,%u we got ",image_x,image_y);
                                    if ( image_x < frame.images_per_line-1 ) {  image_x+=1; } else
                                    if ( image_y < last_line-1 )
                                       {//Go to the next row functionality :P
                                           image_x=0;
                                         ++image_y;
                                       }
                                    fprintf(stderr,"%u,%u\n",image_x,image_y);
                                }

   unsigned int current_active_picture=PictureXYtoID(image_x,image_y);

   if ( current_active_picture!=last_active_picture )
    {
      frame.active_image_place=current_active_picture;
      SetDestinationOverPicture(image_x,image_y);
      fprintf(stderr,"Picture X/Y from %u/%u -> %u/%u \n",frame.last_image_x,frame.last_image_y,frame.active_image_x,frame.active_image_y);
      frame.transitions.seek_move_activated=1; /*THIS MOVEMENT IS A SEEK MOVEMENT SetDestinationOverPicture , sets this to 0
                                     so it is important to set this right here!*/
      return 1;
    } else
    {
      fprintf(stderr,"Staying over the same picture\n");
    }
   return 0;
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

        frame.transitions.effect_move_activated=1; /* ACTIVATE START Coords */

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
   unsigned int new_active_picture=frame.active_image_place+1;
   unsigned int new_active_x;
   unsigned int new_active_y;
   PictureIDtoXY(&new_active_x,&new_active_y,new_active_picture);


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
                                                       fprintf(stderr,"New active picture is %u / %u \n ",new_active_picture, frame.total_images);
                                                       SetDestinationOverPicture(new_active_x,new_active_y);
                                                       fprintf(stderr,"Picture new destination %u/%u -> %u/%u \n ",frame.last_image_x,frame.last_image_y,frame.active_image_x,frame.active_image_y);
                                                    }
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

  if ( frame.vz<=frame.distance_block_lower+2.5  )
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
    //distanceBetween3DPoints(float *x1,float*y1,float *z1,float *x2,float*y2,float *z2)
   return (!CameraSeesOnlyOnePicture());
}

void PerformCameraMovement(unsigned int microseconds_of_movement)
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


    float speed=2; // 2 is a good value , 1 is an also good value ( faster :P )
    float speed_factor = 3/2; /* 1/5 */

    if ( frame.transitions.effect_move_activated >= 2 ) { /* Frame is beeing hovered for an effect move , so we prefer slow speed */
                                              speed_factor = (float) 3/2;
                                            } else
                                            {
                                              /* Frame is beeing plainly hovered , so we prefer fast speed */
                                              speed_factor = (float) 1/5;
                                            }


    float speed_multiplier = 1.0;
    if ( microseconds_of_movement != 0 ) { speed_multiplier=(1000*1000/microseconds_of_movement); }


    float speed_rate = 1/(speed * speed_factor * speed_multiplier);

    //fprintf(stderr,"Microsecs : %u , SpeedRate %0.4f SpeedMultiplier %0.2f\n",microseconds_of_movement,speed_rate,speed_multiplier);

    if ( frame.desired_x != frame.vx )
                           {
                             if ( frame.desired_x < frame.vx )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vx -= ( frame.vx - frame.desired_x ) * speed_rate;
                                   if ( frame.desired_x > frame.vx ) { frame.desired_x = frame.vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_x > frame.vx )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vx += ( frame.desired_x - frame.vx ) * speed_rate;
                                   if ( frame.desired_x < frame.vx ) { frame.desired_x = frame.vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           } else { reached_target+=1; } /* + One coordinate has reached target */

    if ( frame.desired_y != frame.vy )
                          {
                             if ( frame.desired_y < frame.vy )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vy -= ( frame.vy - frame.desired_y ) * speed_rate;
                                   if ( frame.desired_y > frame.vy ) { frame.desired_y = frame.vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_y > frame.vy )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vy += ( frame.desired_y - frame.vy ) * speed_rate;
                                   if ( frame.desired_y < frame.vy ) { frame.desired_y = frame.vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           } else { reached_target+=3; } /* + One coordinate has reached target */


    if ( frame.desired_z != frame.vz )
                          {
                              if ( frame.desired_z < frame.vz )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vz -= ( frame.vz - frame.desired_z ) * speed_rate;
                                   if ( frame.desired_z > frame.vz ) { frame.desired_z = frame.vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( frame.desired_z > frame.vz )
                                 {
                                   /* CLOSING IN OPERATION */   frame.vz += ( frame.desired_z - frame.vz ) * speed_rate;
                                   if ( frame.desired_z < frame.vz ) { frame.desired_z = frame.vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           } else { reached_target+=5; } /* + One coordinate has reached target */


 switch ( frame.transitions.effect_move_activated )
  {
     case 0 :
      /* DEAD STATE :) */
     break;

     /* Start movement to start */
     case 1 :
      frame.desired_x=frame.effect_start_x;
      frame.desired_y=frame.effect_start_y;
      frame.desired_z=frame.effect_start_z;
      frame.transitions.effect_move_activated=2;
     break;

     /* Reach start place*/
     case 2 :
      if ( reached_target >= 9 ) { frame.transitions.effect_move_activated=3; }
     break;

     /* Start movement to end */
     case 3 :
      frame.desired_x=frame.effect_end_x;
      frame.desired_y=frame.effect_end_y;
      frame.desired_z=frame.effect_end_z;
      frame.transitions.effect_move_activated=4;
     break;

     /* Reach end place*/
     case 4 :
      if ( reached_target >= 9 ) { frame.transitions.effect_move_activated = 0; }
     break;

     default :
      fprintf(stderr,"Erroneous state :P \n");
      frame.transitions.effect_move_activated = 0;
     break;
  };


//test floating effectframe.vz+=0.0001*microseconds_of_movement;


  /* -------------------------------------
     CAMERA SAFE GUARD!
     -------------------------------------
  */
   if ( frame.vz<=frame.distance_block_lower) { frame.vz=frame.distance_block_lower; frame.desired_z=frame.vz; } /* DO NOT ALLOW ANY CLOSER */
   if ( frame.vz>=frame.distance_block_upper) { frame.vz=frame.distance_block_upper; frame.desired_z=frame.vz; } /* DO NOT ALLOW ANY CLOSER */

   if ( frame.vx<=frame.distance_block_left)  { frame.vx=frame.distance_block_left;  frame.desired_x=frame.vx; } /* DO NOT ALLOW ANY LEFTER */
   if ( frame.vx>=frame.distance_block_right) { frame.vx=frame.distance_block_right; frame.desired_x=frame.vx; } /* DO NOT ALLOW ANY RIGHTER */

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

  if  ( (frame.desired_x==frame.vx)&&(frame.desired_y==frame.vy)&&(frame.desired_z==frame.vz) ) { CameraReachedDestination(); }

  if ( !frame.transitions.seek_move_activated ) { CalculateActiveImage_AccordingToPosition(); }
  /* If we are performing a seek move ( i.e. keyboard arrows ) we dont want to calculate active_image again , we
     know where we are headed*/

}














