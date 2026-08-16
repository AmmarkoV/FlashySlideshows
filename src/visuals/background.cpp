#include "background.h"
#include "dynamic_background.h"
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

/* How far behind the camera plane the background sits , and how much bigger than the
   strictly visible rectangle we make it so a tilted camera does not expose an edge */
#define BACKGROUND_DEPTH    15.0
#define BACKGROUND_OVERSCAN 1.20


/* The background used to be a hardcoded 160x120 world units quad scaled by 1.7 , which
   only covered the window when it happened to be 4:3 and the camera happened to be at
   its default distance. On a 16:9 window ( or after zooming out ) the quad ran out
   before the viewport did and the corners showed through as black.
   The projection is glFrustum(-ar,ar,-1,1,near,far) , so at a distance d in front of
   the eye the visible rectangle is exactly 2*d/near tall and ar times that wide. */
static void GetBackgroundExtents(float * halfWidth,float * halfHeight)
{
  float depth = frame.vz + BACKGROUND_DEPTH;
  if (depth<FRUSTUM_NEAR_PLANE) { depth=FRUSTUM_NEAR_PLANE; }

  float aspectRatio = frame.aspectRatio;
  if (aspectRatio<=0.0) { aspectRatio=4.0/3.0; } /* no ResizeCallback yet */

  *halfHeight = (depth * FRUSTUM_HALF_HEIGHT / FRUSTUM_NEAR_PLANE) * BACKGROUND_OVERSCAN;
  *halfWidth  = *halfHeight * aspectRatio;
}

/* The texture is stretched over whatever quad we ended up with , which squashes the
   background picture on a wide window. These give the texture coordinates of a
   "cover" fit instead : the picture keeps its own aspect ratio , fills the quad , and
   whatever does not fit is cropped away evenly on both sides. */
static void GetBackgroundTexCoords(float halfWidth,float halfHeight,float * uMargin,float * vMargin)
{
  *uMargin=0.0; *vMargin=0.0;
  if ( (background==0) || (background->width==0) || (background->height==0) ) { return; }

  float quadAspect    = halfWidth / halfHeight;
  float textureAspect = (float) background->width / (float) background->height;

  if (textureAspect>quadAspect)
   { /* picture is wider than the quad , crop its left and right */
     *uMargin = (1.0 - (quadAspect/textureAspect)) / 2;
   } else
   { /* picture is taller than the quad , crop its top and bottom */
     *vMargin = (1.0 - (textureAspect/quadAspect)) / 2;
   }
}


void DrawSimple2DBackground()
{
  ++times_drawn_background;
 // glClearColor(0,0,0,0);

  float halfWidth,halfHeight;
  GetBackgroundExtents(&halfWidth,&halfHeight);

  glBegin(GL_QUADS);
    glColor3f(0.0,0.0,0.0);

    float x=frame.vx,y=frame.vy,z=0;

    glVertex3f(x-halfWidth,y-halfHeight,z-BACKGROUND_DEPTH);	// Bottom Left Of The Texture and Quad
    glVertex3f(x+halfWidth,y-halfHeight,z-BACKGROUND_DEPTH);	// Bottom Right Of The Texture and Quad
    glVertex3f(x+halfWidth,y+halfHeight,z-BACKGROUND_DEPTH);	// Top Right Of The Texture and Quad
    glVertex3f(x-halfWidth,y+halfHeight,z-BACKGROUND_DEPTH);

   glEnd();

}

void DrawTextured2DBackground()
{
  ++times_drawn_background;

  float halfWidth,halfHeight,uMargin,vMargin;
  GetBackgroundExtents(&halfWidth,&halfHeight);
  GetBackgroundTexCoords(halfWidth,halfHeight,&uMargin,&vMargin);

  float uLow=uMargin , uHigh=1.0-uMargin;
  float vLow=vMargin , vHigh=1.0-vMargin;

  glEnable ( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D, background->gpu.gl_rgb_texture );

   glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);

    float x=frame.vx,y=frame.vy,z=0;

    /* u runs backwards because the whole world is rolled 180 degrees around z */
    glTexCoord2f(uHigh, vLow ); glVertex3f(x-halfWidth,y-halfHeight,z-BACKGROUND_DEPTH);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(uLow , vLow ); glVertex3f(x+halfWidth,y-halfHeight,z-BACKGROUND_DEPTH);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(uLow , vHigh); glVertex3f(x+halfWidth,y+halfHeight,z-BACKGROUND_DEPTH);	// Top Right Of The Texture and Quad
    glTexCoord2f(uHigh, vHigh); glVertex3f(x-halfWidth,y+halfHeight,z-BACKGROUND_DEPTH);

   glEnd();
   glDisable ( GL_TEXTURE_2D );
}

void DrawBackground()
{
  /* A shader background covers the whole viewport by itself , there is nothing left
     for the quads below to fill in */
  if ( DrawDynamicBackground() ) { return; }

  if ( (frame.try_for_best_render_quality)&&(!frame.plain_background_no_image) )
    {
      DrawTextured2DBackground();
    } else
    {
      DrawSimple2DBackground();
    }
}
