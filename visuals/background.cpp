#include "background.h"
#include "../slideshow.h"

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

unsigned int times_drawn_background=0;

float xmin=-80,xmax=80;
float ymin=-((xmax-xmin)*3/4)/2,ymax=ymin*(-1);

void DrawSimple2DBackground()
{
  ++times_drawn_background;
  glClearColor(0,0,0,0);

}

void DrawTextured2DBackground()
{
  ++times_drawn_background;

  glEnable ( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D, background->gpu.gl_rgb_texture );

   glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);

    float x=frame.vx,y=frame.vy,z=0;

    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+xmin,y+ymin,z-15);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+xmax,y+ymin,z-15);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+xmax,y+ymax,z-15);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+xmin,y+ymax,z-15);

   glEnd();
   glDisable ( GL_TEXTURE_2D );
}

void DrawBackground()
{
  if (frame.try_for_best_render_quality)
    {
      DrawTextured2DBackground();
    } else
    {
      DrawSimple2DBackground();
    }
}
