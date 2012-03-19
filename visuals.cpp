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
#include "load_images.h"
#include "directory_listing.h"
#include "scene_objects.h"
#include "sound.h"
#include <math.h>
#include "camera_control.h"


#include "layouts/layout_handler.h"

#include "visuals/effects.h"
#include "visuals/hud.h"
#include "visuals/background.h"
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


int DrawEffects()
{
  DrawObject(25,0,0,(float) times_drawn_background/100,9,9,1,0);
  DrawObject(25,14,0,(float) -times_drawn_background/100,9,9,2,0);
  return 1;
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


void MainDisplayFunction()
{
  if ( frame.images_per_line == 0 ) { fprintf(stderr,"Wtf , 0 images per line ? :P \n "); return; }

  unsigned int line_at=(unsigned int ) MinPictureThatIsVisible()/frame.images_per_line;
  float y=-6;
  y+=6 * line_at;
  unsigned int album_traveler=0;
  unsigned int minpicture=MinPictureThatIsVisible(),maxpicture=MaxPictureThatIsVisible();

   if (frame.try_for_best_render_quality)
    {
     glEnable(GL_NORMALIZE);
     glEnable(GL_LINE_SMOOTH);
     glEnable (GL_POLYGON_SMOOTH);
     glEnable(GL_ALPHA_TEST);
     glAlphaFunc(GL_GREATER, 0.1);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }


  if ( CameraSeesOnlyOnePicture() )
   {
     minpicture=frame.active_image_place;
     maxpicture=frame.active_image_place+1;
   } else
   {
       DrawLayoutBackground();
   }


          for ( album_traveler=minpicture; album_traveler<=maxpicture; album_traveler++ )
           {

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




