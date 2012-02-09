/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammarkov.ath.cx
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

#include <stdlib.h>
#include <stdio.h>

#include "slideshow.h"
#include "camera_control.h"
#include "controls.h"
#include "visuals.h"
#include "directory_listing.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

void inline MouseLook(int x,int y)
{

 //  angle_y -= (float) (x-prev_x)*mouse_freedom;
 //  angle_x -= (float) (y-prev_y)*mouse_freedom;
 //  prev_x = x;
 //  prev_y = y;

}

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

int Controls_Handle_MouseButtons(int button,int state, int x, int y)
{
    frame.mouse.mouse_x=x;
    frame.mouse.mouse_y=y;
    fprintf(stderr,"Mouse %u,%u\n",x,y);
    if ( frame.mouse.block_mouse_calls_until > frame.tick_count ) { return 0; /* We are blocking mouse calls to improve user friendlieness :P */ }


    /*
       According to zoom factor we will use a desired_step matching it in order to have smooth movement over the image
       RefreshDesiredStep_AccordingToPosition calculates it and loads it in frame.desired_step variable
    */
    RefreshDesiredStep_AccordingToPosition();
    /*---------------------------------------------------------------------------------------------------------------*/

   int is_not_mouse_wheel_event=1;
   int is_a_doubleclick_event=0;

   /*  MOUSE WHEEL HANDLING
       >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
   if ( button==GLUT_WHEEL_UP )
   {
     if (state == GLUT_UP )
      {
        frame.desired_z-=frame.desired_step;
      }
     is_not_mouse_wheel_event=0;
   } else
   if ( button==GLUT_WHEEL_DOWN )
   {
     if (state == GLUT_UP )
      {
        frame.desired_z+=frame.desired_step;
      }
     is_not_mouse_wheel_event=0;
   }
   /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
   else


   /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
   if (button==GLUT_LEFT_BUTTON)
     {
       if (state == GLUT_UP )
       {
         frame.mouse.left_pressed=1;
         fprintf(stderr,"Left mouse button UP ( but down in reality ) \n");
       } else
       if (state == GLUT_DOWN )
       {
         frame.mouse.left_pressed=1;
         fprintf(stderr,"Left mouse button DOWN \n");
       } else
       {
         fprintf(stderr,"Left mouse button OTHER ( but up in reality :P )\n");
           if ( frame.mouse.left_pressed==1 ) {
                                                frame.mouse.left_pressed=0;
                                              } else /* RELEASING DRAG AND DROP */
           {
             /* UP Without already been used to drag */
             fprintf(stderr,"Strange state");
             frame.dragging_screen=0;
           }
       }
     }
   /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
     else
   /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
if (button==GLUT_RIGHT_BUTTON)
     {
       if (state == GLUT_DOWN )
       {
         fprintf(stderr,"Right mouse button DOWN \n");
       } else
       if (state == GLUT_UP )
       {
         fprintf(stderr,"Right mouse button UP \n");
            if ( frame.mouse.left_pressed==1 ) {  } else /* RELEASING DRAG AND DROP */
           {
             /* UP Without already been used to drag */
           }
       }
     }
   /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */






  if ( is_not_mouse_wheel_event == 1 )
   {

    if  ( frame.mouse.left_pressed == 1 ) {
                                         frame.dragging_screen=1;
                                         fprintf(stderr,"START DRAGGING \n");
                                       } else
                                       {
                                         frame.dragging_screen=0;
                                         fprintf(stderr,"START NO DRAGGING \n");
                                       }

    if ( frame.dragging_screen == 1 )
	      {
           frame.mouse.last_mouseclick_at_x=x;
	       frame.mouse.last_mouseclick_at_y=y;
	       frame.mouse.last_mouseclick_at_z=0;
//	       frame.mouse.button_pressed=button;
	       frame.dragging_screen=1;

           frame.mouse.last_click_time=frame.tick_count;
           return 1;
	      }


    if ( is_a_doubleclick_event == 1 )
    {
      /* DOUBLE CLICK*/
      //fprintf(stderr,"Double Click Pressed at %u %u , button %u time %u - %u = %u \n",x,y,button,frame.tick_count,frame.mouse.last_click_time,frame.tick_count - frame.mouse.last_click_time);
	  SetDestinationCenter();
	  return 1;
    }

  }

    return 0;

}

int Controls_Handle_MouseMotion(int button,int state, int x, int y)
{
  //fprintf(stderr,"Mouse %u,%u\n",x,y);
  frame.mouse.mouse_x=x;
  frame.mouse.mouse_y=y;

   if  ( frame.mouse.left_pressed == 1 ) {
                                         frame.dragging_screen=1;
                                         fprintf(stderr,"DRAGGING \n");
                                       } else
                                       {
                                         frame.dragging_screen=0;
                                       //  fprintf(stderr,"NO DRAGGING \n");
                                       }

  if ( frame.dragging_screen == 1 )
   {
         float difference_x = frame.mouse.mouse_x - frame.mouse.last_mouseclick_at_x ;
         float difference_y = frame.mouse.mouse_y - frame.mouse.last_mouseclick_at_y ;

         frame.mouse.last_mouseclick_at_x = frame.mouse.mouse_x;
         frame.mouse.last_mouseclick_at_y = frame.mouse.mouse_y;

         frame.desired_x+=difference_x/24; /* We want to drag the screen to the oposite direction / the number 24 is the total viewable area of the window*/
         frame.desired_y-=difference_y/16; /* We want to drag the screen to the oposite direction / the number 24 is the total viewable area of the window*/

       return 1;
   }

  return 0;
}



int Controls_Handle_Keyboard(unsigned char key, int x, int y)
{
    /*
       According to zoom factor we will use a desired_step matching it in order to have smooth movement over the image
       RefreshDesiredStep_AccordingToPosition calculates it and loads it in frame.desired_step variable
    */
   // RefreshDesiredStep_AccordingToPosition();
    /*---------------------------------------------------------------------------------------------------------------*/

    int nokey=0;
    fprintf(stderr,"Key %u \n",key);
    switch (key)
    {
        case 1 : /* UP */    MoveToPicture(D_UP); break;
        case 2 : /* DOWN */  MoveToPicture(D_DOWN); break;
        case 3 : /* LEFT */  MoveToPicture(D_LEFT); break;
        case 4 : /* RIGHT */ MoveToPicture(D_RIGHT); break;


        case 13 : /* ENTER */ ToggleAutomaticSlideshow(); break;

        case 48+0 : /*Number 0*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort0/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+1 : /*Number 1*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort1/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+2 : /*Number 2*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort2/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+3 : /*Number 3*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort3/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+4 : /*Number 4*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort4/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+5 : /*Number 5*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort5/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+6 : /*Number 6*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort6/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+7 : /*Number 7*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort7/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+8 : /*Number 8*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort8/")>0)  {MoveToPicture(D_RIGHT);}break;
        case 48+9 : /*Number 9*/ if (MoveFileToDir(GetPictureDirectoryListIndex(frame.active_image_place),(char *) "~/Pictures/FlashySlideshows/Sort9/")>0)  {MoveToPicture(D_RIGHT);}break;


        case 'o': SetDestinationCenter();
                  break;

        case 'r': MoveDestinationCenter(D_OUT); /*frame.desired_z+=frame.desired_step;*/ break;
        case 'f': MoveDestinationCenter(D_IN); /*frame.desired_z-=frame.desired_step;*/ break;
        case 'a': MoveDestinationCenter(D_LEFT); /*frame.desired_x+=frame.desired_step;*/ break;
        case 'd': MoveDestinationCenter(D_RIGHT); /*frame.desired_x-=frame.desired_step;*/ break;
        case 's': MoveDestinationCenter(D_UP); /*frame.desired_y+=frame.desired_step;*/ break;
        case 'w': MoveDestinationCenter(D_DOWN); /*frame.desired_y-=frame.desired_step;*/ break;
        case 'z': frame.angle_x-=0.5; break;
        case 'c': frame.angle_x+=0.5; break;
        case 't': frame.angle_y-=0.5; break;
        case 'g': frame.angle_y+=0.5; break;
        case 'y': frame.angle_z-=0.5; break;
        case 'h': frame.angle_z+=0.5; break;
        case 'n': if (ENABLE_WIGGLING) {ENABLE_WIGGLING=0;} else {ENABLE_WIGGLING=1;} break;
        case 'm': ToggleTransitionMode(); break;

        case 153: MoveToPicture(D_UP);   MoveToPicture(D_UP);   MoveToPicture(D_UP);  break; //PAGE UP
        case 161: MoveToPicture(D_DOWN); MoveToPicture(D_DOWN); MoveToPicture(D_DOWN); break; //PAGE DOWN

        case 'b': PickHoverEffect(frame.active_image_x,frame.active_image_y); break;

       default : nokey=1;
            break;
    }

  return nokey;
}





