#include <stdlib.h>
#include <stdio.h>

#include "slideshow.h"
#include "camera_control.h"
#include "controls.h"

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

void Controls_Handle_MouseButtons(int button,int state, int x, int y)
{
    if ( frame.mouse.block_mouse_calls_until > frame.tick_count ) { return; /* We are blocking mouse calls to improve user friendlieness :P */ }


    /*
       According to zoom factor we will use a desired_step matching it in order to have smooth movement over the image
       RefreshDesiredStep_AccordingToPosition calculates it and loads it in frame.desired_step variable
    */
    RefreshDesiredStep_AccordingToPosition();
    /*---------------------------------------------------------------------------------------------------------------*/


   int is_not_mouse_wheel_event=0;
   int is_a_doubleclick_event=0;


   if (state == GLUT_UP )
	{
	       if ( button == GLUT_WHEEL_UP )   { frame.desired_z-=frame.desired_step; }
      else if ( button == GLUT_WHEEL_DOWN ) { frame.desired_z+=frame.desired_step; }
      else is_not_mouse_wheel_event=1;
	} else is_not_mouse_wheel_event=1;

    if ( is_not_mouse_wheel_event == 1 )
    {
       /* To event den exei na kanei me to mouse wheel*/

       if (state == GLUT_DOWN )
	   {
           fprintf(stderr,"Click Released at %u %u , button %u \n",x,y,button);
	       frame.mouse.button_pressed=0;
	       frame.mouse.is_currently_pressed=0;
	   } else
	    if (state == GLUT_UP )
	   {
	     if ( frame.tick_count - frame.mouse.last_click_time < 1200 )
	     {
	         /* DOUBLE CLICK*/
	         //fprintf(stderr,"Double Click Pressed at %u %u , button %u time %u - %u = %u \n",x,y,button,frame.tick_count,frame.mouse.last_click_time,frame.tick_count - frame.mouse.last_click_time);
	         SetDestinationCenter();
	         frame.dragging_screen=0;
	         frame.mouse.block_mouse_calls_until=frame.tick_count+1000;
	         return;
         }

	     if ( frame.dragging_screen ==0 )
	      {
           fprintf(stderr,"Click Pressed at %u %u , button %u \n",x,y,button);
	       frame.mouse.last_mouseclick_at_x=x;
	       frame.mouse.last_mouseclick_at_y=y;
	       frame.mouse.last_mouseclick_at_z=0;
	       frame.mouse.button_pressed=button;
	       frame.mouse.is_currently_pressed=1;
	       frame.dragging_screen=1;

           frame.mouse.last_click_time=frame.tick_count;

	      } else
	      {
	        frame.mouse.is_currently_pressed=0;
            frame.dragging_screen=0;
	      }





	   }
    }

    if ( is_a_doubleclick_event == 1 )
    {
        /* WILL HAVE TO USE THIS SPACE TO CLEAR UP MOUSE FUNCTIONS !*/
    }

}

void Controls_Handle_MouseMotion(int button,int state, int x, int y)
{
  //fprintf(stderr,"Mouse moved at %u %u\n",x,y);
  frame.mouse.mouse_x=x;
  frame.mouse.mouse_y=y;

  if ( frame.mouse.is_currently_pressed == 1 )
   {
      if ( ( frame.mouse.button_pressed == 0 ) && ( frame.dragging_screen == 1 ) )
      {
         float difference_x = frame.mouse.mouse_x - frame.mouse.last_mouseclick_at_x ;
         float difference_y = frame.mouse.mouse_y - frame.mouse.last_mouseclick_at_y ;

         frame.mouse.last_mouseclick_at_x = frame.mouse.mouse_x;
         frame.mouse.last_mouseclick_at_y = frame.mouse.mouse_y;

         frame.desired_x+=difference_x/24; /* We want to drag the screen to the oposite direction / the number 24 is the total viewable area of the window*/
         frame.desired_y-=difference_y/16; /* We want to drag the screen to the oposite direction / the number 24 is the total viewable area of the window*/
      }
   }
}


int MoveToPicture(unsigned int direction)
{
  fprintf(stderr,"Move to picture direction = %u \n",direction);
  unsigned int last_active_picture=frame.active_image_place;
  unsigned int last_line=0;
  if ( frame.images_per_line>0) last_line=frame.total_images/frame.images_per_line;

  fprintf(stderr,"Picture X/Y was %u / %u \n",frame.active_image_x,frame.active_image_y);

    if ( direction == 1 ) {  /* UP */    if ( frame.active_image_y > 0 ) {  frame.active_image_y-=1; } } else
    if ( direction == 2 ) {  /* DOWN */  if ( frame.active_image_y < last_line-1 ) {  frame.active_image_y+=1; } } else
    if ( direction == 3 ) {  /* LEFT */  if ( frame.active_image_x > 0 ) {  frame.active_image_x-=1; } } else
    if ( direction == 4 ) {  /* RIGHT */ if ( frame.active_image_x < frame.images_per_line-1 ) {  frame.active_image_x+=1; } }

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
    /*
       According to zoom factor we will use a desired_step matching it in order to have smooth movement over the image
       RefreshDesiredStep_AccordingToPosition calculates it and loads it in frame.desired_step variable
    */
    RefreshDesiredStep_AccordingToPosition();
    /*---------------------------------------------------------------------------------------------------------------*/

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
