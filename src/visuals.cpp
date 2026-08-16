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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "visuals.h"
#include "slideshow.h"
#include "hypervisor/load_images.h"
#include "filesystem/directory_listing.h"
#include "scene_objects.h"
#include "tools/sound.h"
#include <math.h>
#include "camera_control.h"


#include "layouts/layout_handler.h"

#include "visuals/hud.h"
#include "visuals/background.h"
#include "visuals/dynamic_background.h"
#include "visuals/picture_renderer.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

void glColorRGB(unsigned char R,unsigned char G,unsigned char B)
{
  glColor3f((float) R/255,(float) G/255,(float) B/255);
}


int NewLabel(float x,float y,char * text)
{
  Delete_All3DObjectsOfShape(/*LABEL ONLY*/ 0);
  unsigned int label_flying=Add_3DObject(frame.vx-3,frame.vy+3,frame.vz-4.1,6,1,/*LABEL ONLY*/0,4000000);
  objects[label_flying].velocity.x=0.02;
  strncpy(objects[label_flying].label,text,128);
  objects[label_flying].has_label=1;
  //objects[label_flying].velocity.y=0.001;
  return 1;
}


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                           OFF SCREEN PICTURE REJECTION
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

   MinPictureThatIsVisible()/MaxPictureThatIsVisible() pick a *band of the album* around
   the active picture ( images_per_line * LINES_AWAY_DRAWN of them on each side ) and
   everything in that band used to be handed to DisplayPicture unconditionally.

   That band is chosen so that nothing pops into existence while the camera travels , it
   was never a statement about what fits in the window. Zoomed out it is roughly right ,
   but zoomed in ( frame.vz down near GetLayoutMinimumZ() ) the window covers about
   11x6.7 world units while the pictures sit on a 14x12 grid : one to three of them are
   on screen and the remaining thirty-odd were still being rasterized at full size and
   then thrown away by the clipper. On a low end GPU that is pure wasted fill rate , and
   it is worst exactly where the camera is closest and each picture covers the most
   pixels. CameraSeesOnlyOnePicture() only rescued the special case of a landscape
   picture within distance 12.4 , so a portrait photo never got the benefit.

   The test below is a handful of float compares per picture and it throws the rest away
   before a single fragment is touched.
*/

/* The parts of the rejection test that are the same for every picture in a frame , so
   the trigonometry is paid for once instead of once per picture */
struct PictureCullingContext
{
  unsigned int enabled;
  float aspectRatio;
  float rollCos , rollSin; /* |cos| and |sin| of the world roll around z */
};

static void PrepareCullingContext(struct PictureCullingContext * cull)
{
  /* The z , c , t and g keys tilt the world around x and y. As soon as they do , the
     camera is no longer looking straight down -z , the picture plane is no longer
     parallel to the near plane and the flat test below stops describing what is on
     screen. Rather than get that wrong and blink pictures out of existence , the whole
     rejection is switched off while the world is tilted ( which is the debug case ,
     the slideshow itself never tilts ) */
  if ( (frame.angle_x!=0) || (frame.angle_y!=0) ) { cull->enabled=0; return; }

  cull->aspectRatio = frame.aspectRatio;
  if (cull->aspectRatio<=0.0) { cull->enabled=0; return; } /* no ResizeCallback yet */

  /* The world is rolled around z ( 180 degrees normally , the y and h keys move it ) so
     the rectangle the window covers on the picture plane comes out turned. The axis
     aligned box that contains a WxH rectangle turned by an angle is
     ( W|cos|+H|sin| ) by ( W|sin|+H|cos| ) , which is what the half extents get grown by
     further down. At the usual 180 degrees this is |cos|=1 , |sin|=0 and the box is the
     rectangle itself. */
  float rollRadians = (float) frame.angle_z * (float) (M_PI/180.0);
  cull->rollCos = (float) fabs(cos(rollRadians));
  cull->rollSin = (float) fabs(sin(rollRadians));

  cull->enabled=1;
}

static int PictureIsOffScreen(struct PictureCullingContext * cull,struct Picture * pic,unsigned int place)
{
  if (!cull->enabled) { return 0; }
  if (pic==0)         { return 0; }

  float pictureX,pictureY,pictureZ;
  float halfSizeX,halfSizeY;

  /* Where the picture is about to be drawn.
     A picture that has not been loaded yet is not a picture of its own , album[place]
     points at the one shared loading / loading_texture / failed placeholder , whose
     position member holds whatever place DisplayPicture last moved it to. Asking it
     where it is would give the answer for some other slot , so for those the position
     is taken from the layout instead , which is where DisplayPicture is going to put it
     anyway. position.ok==0 means nobody has run the layout for this picture yet , same
     story. */
  if ( (pic->position.ok!=0) && (pic!=loading) && (pic!=loading_texture) && (pic!=failed) )
   {
     pictureX=pic->position.x;  pictureY=pic->position.y;  pictureZ=pic->position.z;
     halfSizeX=pic->position.size_x; halfSizeY=pic->position.size_y;
   } else
   {
     GetLayoutCoordinatesForXY(PictureIDtoX(place),PictureIDtoY(place),&pictureX,&pictureY,&pictureZ);
     /* The biggest a picture is ever sized to , see FixOpenGLPictureSize */
     halfSizeX=6.0; halfSizeY=4.5;
   }

  /* How far in front of the camera the plane this picture lives on is. Anything at or
     behind the near plane is either behind us or clipped by OpenGL anyway , and the
     arithmetic below would be meaningless , so it is simply kept. */
  float distance = frame.vz - pictureZ;
  if (distance<FRUSTUM_NEAR_PLANE) { return 0; }

  /* The projection is glFrustum(-ar*H,ar*H,-H,H,near,far) , so at a distance d in front
     of the eye the window covers exactly 2*d*H/near vertically and ar times that
     horizontally. The eye sits at ( vx,vy,vz ) : the modelview is a rotation followed
     by a translation of -v , so v itself maps to the origin. */
  float halfHeight = distance * (float) FRUSTUM_HALF_HEIGHT / (float) FRUSTUM_NEAR_PLANE;
  float halfWidth  = halfHeight * cull->aspectRatio;

  /* Grow it by the roll ( see PrepareCullingContext ) and then by the picture's own
     radius , so a picture that is only half on screen still counts as on screen. The
     half diagonal is used rather than the half sizes because a picture can be turned by
     its exif orientation , by the expo layout or by the user rotating it , and the half
     diagonal is the one measure that survives any of that. The extra 0.2 is the white
     frame DisplayFrame draws around it. */
  float pictureRadius = (float) sqrt(halfSizeX*halfSizeX + halfSizeY*halfSizeY) + 0.2;

  float boundX = halfWidth*cull->rollCos + halfHeight*cull->rollSin + pictureRadius;
  float boundY = halfWidth*cull->rollSin + halfHeight*cull->rollCos + pictureRadius;

  if ( fabs(pictureX-frame.vx) > boundX ) { return 1; }
  if ( fabs(pictureY-frame.vy) > boundY ) { return 1; }

  return 0;
}


void MainDisplayFunction()
{
  unsigned int album_traveler=0;
  unsigned int minpicture=MinPictureThatIsVisible(),maxpicture=MaxPictureThatIsVisible();

  struct PictureCullingContext cull;
  PrepareCullingContext(&cull);

   if (frame.try_for_best_render_quality)
    {
     glEnable(GL_NORMALIZE);
     glEnable(GL_LINE_SMOOTH);
     //glEnable (GL_POLYGON_SMOOTH); this causes diagonal lines..
     glEnable(GL_ALPHA_TEST);
     glAlphaFunc(GL_GREATER, 0.1);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }

  //If we are over only one visible picture we dont have to draw background
  //and we dont have to draw anything else.. just this one picture!
  if ( CameraSeesOnlyOnePicture() )
   {
     minpicture=frame.active_image_place;
     maxpicture=frame.active_image_place+1;
     /* One picture does not necessarily cover the whole window , and letting an
        animated background blink out of existence every time the camera closes in
        on a photo looks like a bug , so it keeps being drawn */
     if (DynamicBackgroundIsActive()) { DrawLayoutBackground(); }
   } else
   {
       DrawLayoutBackground();
   }


          for ( album_traveler=minpicture; album_traveler<=maxpicture; album_traveler++ )
           {
               /* Everything in the band that the window does not reach is dropped here ,
                  before any of its fragments cost anything */
               if ( PictureIsOffScreen(&cull,album[album_traveler],album_traveler) ) { continue; }

               DisplayPicture( album[album_traveler],
                               album_traveler ,
                               album[album_traveler]->position.x,
                               album[album_traveler]->position.y,
                               album[album_traveler]->position.z,
                               album[album_traveler]->position.size_x,
                               album[album_traveler]->position.size_y,
                               album[album_traveler]->position.heading,
                               album[album_traveler]->position.pitch,
                               album[album_traveler]->position.roll + album[album_traveler]->rotate
                             );
           }

  if (frame.try_for_best_render_quality)
    {
     glDisable(GL_ALPHA_TEST);
     glDisable(GL_BLEND);
     glDisable(GL_LINE_SMOOTH);
     glDisable (GL_POLYGON_SMOOTH);
     glDisable(GL_NORMALIZE);
    }
}




