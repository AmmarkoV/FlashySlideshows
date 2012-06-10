#include "hud.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

#include "../slideshow.h"
#include "../directory_listing.h"



void setOrthographicProjection()
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, 1024, 0, 768);
	// invert the y axis, down is positive
	//glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0,0, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}



void resetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}


void ShowClock()
{
   setOrthographicProjection();
     glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
       glBegin(GL_QUADS);
        glColor4f(0.8,0.8,0.8,0.8);

        glVertex2f(300,50);	// Bottom Left Of The Texture and Quad
        glVertex2f(1024,50);	// Bottom Right Of The Texture and Quad
        glVertex2f(1024,0);	// Top Right Of The Texture and Quad
        glVertex2f(300,0);
       glEnd();
     glDisable(GL_BLEND);
     glColor3f(0.8,0.0,0.0);
     glRasterPos2f(335,15);

     char time_string[512]={0};


      /*This is a pretty badly written part of the hud :P */
     unsigned int tot_secs=frame.tick_count/1000 , tot_minutes=0 , tot_hours=0;

     unsigned int seconds_passed=tot_secs;
     unsigned int minutes_passed = seconds_passed/60;
     seconds_passed-=minutes_passed*60;
     unsigned int hours_passed = minutes_passed /60;
     minutes_passed-=hours_passed*60;

     unsigned int est_hours=0 , est_minutes=0 , est_secs=0;
     if (frame.active_image_place!=0)
     {
      est_secs=(unsigned int ) (tot_secs*GetTotalViewableFilesInDirectory())/frame.active_image_place;
      tot_secs+=est_secs;
      est_minutes = est_secs/60;
      est_secs-=est_minutes*60;
      est_hours = est_minutes /60;
      est_minutes-=est_hours*60;

      tot_minutes = tot_secs/60;
      tot_secs-=tot_minutes*60;
      tot_hours = tot_minutes /60;
      tot_minutes-=tot_hours*60;
     } else
     {
       tot_secs=0;
     }
      /*This is a pretty badly written part of the hud :P */
     sprintf(time_string,"Time %02u:%02u:%02u for %u/%u est remaining %02u:%02u:%02u est total %02u:%02u:%02u",hours_passed,minutes_passed,seconds_passed,
                                                                                               frame.active_image_place+1,GetTotalViewableFilesInDirectory(),
                                                                                               est_hours,est_minutes,est_secs,
                                                                                               tot_hours,tot_minutes,tot_secs
                                                                                               );
     glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) time_string);

     glColor4f(1.0,1.0,1.0,1.0);
     resetPerspectiveProjection();
     return;
}


void DisplayHUD(unsigned int view_instructions)
{
 if (view_instructions==2) { ShowClock(); return; }

 setOrthographicProjection();

 glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   glBegin(GL_QUADS);
    glColor4f(0.8,0.8,0.8,0.8);

    glVertex2f(0,50);	// Bottom Left Of The Texture and Quad
    glVertex2f(1024,50);	// Bottom Right Of The Texture and Quad
    glVertex2f(1024,0);	// Top Right Of The Texture and Quad
    glVertex2f(0,0);

    glVertex2f(0,768);	// Bottom Left Of The Texture and Quad
    glVertex2f(1024,768);	// Bottom Right Of The Texture and Quad
    glVertex2f(1024,718);	// Top Right Of The Texture and Quad
    glVertex2f(0,718);

    if (view_instructions==1)
    {
      glVertex2f(100,668);	// Bottom Left Of The Texture and Quad
      glVertex2f(924,668);	// Bottom Right Of The Texture and Quad
      glVertex2f(924,100);	// Top Right Of The Texture and Quad
      glVertex2f(100,100);
    }

   glEnd();
 glDisable(GL_BLEND);



    if (view_instructions==1)
    {
      glColor3f(0.0,0.0,0.0);
      glRasterPos2f(120,600); glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) "Instructions");
      glRasterPos2f(120,500); glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) "Arrow keys , move from picture to picture  - W = Up , S = Down , A = Left , D = Right camera");
      glRasterPos2f(120,450); glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) "M = Cycle transition modes , N = Cycle layouts , B = change hover mode  ");
      glRasterPos2f(120,400); glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) "F = Zoom out , R = Zoom in  ,Q = Rotate Counter Clockwise , E = Rotate Clockwise");
      glRasterPos2f(120,350); glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) "Enter/Space = Toggle Automatic Slideshow , J = Toggle Fullscreen ");
      glRasterPos2f(120,300); glutBitmapString(GLUT_BITMAP_HELVETICA_18,(const unsigned char*) "Escape = Quit ");
    }


      char fps_string[512]={0};
      unsigned int max_memory_used_by_gpu=(unsigned int) (frame.gpu.maxRAM/ (1024*1024));
      unsigned int total_memory_used_by_gpu=(unsigned int) (frame.gpu.usedRAM/ (1024*1024));

      unsigned int max_memory=(unsigned int) (frame.system.maxRAM/ (1024*1024));
      unsigned int total_memory=(unsigned int) (frame.system.usedRAM/ (1024*1024));
      sprintf(fps_string,"Rendering Speed : %u fps - %u Pic loads - %u Pos Calcs - GPU %u/%u MB - RAM %u/%u MB",frame.fps,frame.total_images_loaded,frame.total_position_calculations,total_memory_used_by_gpu,max_memory_used_by_gpu,total_memory,max_memory);
      glColor3f(1.0,0.0,0.0);
      glRasterPos2f(0,10);
      glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*)fps_string);

      glColor3f(0.0,0.0,0.0);
      sprintf(fps_string,"At %u,%u %u/%u over %s%s created %u/%u/%u %u:%02u:%02u",
                          frame.active_image_x,
                          frame.active_image_y,
                          frame.active_image_place,
                          GetTotalViewableFilesInDirectory(),

                          GetSubdir(frame.active_image_place),
                          GetFilename(frame.active_image_place),

                          GetItemDate(frame.active_image_place,0),GetItemDate(frame.active_image_place,1),GetItemDate(frame.active_image_place,2),
                          GetItemDate(frame.active_image_place,3),GetItemDate(frame.active_image_place,4),GetItemDate(frame.active_image_place,5)
              );
      glRasterPos2f(00,740);
      glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*)fps_string);

 glColor4f(1.0,1.0,1.0,1.0);
 resetPerspectiveProjection();
}

