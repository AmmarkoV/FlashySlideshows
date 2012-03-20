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

#include "scene_objects.h"
#include "visuals.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif


 struct SceneObject objects[MAX_SCENE_OBJECTS]; //={0}
 unsigned int existing_objects=0;


void DrawObject(float x,float y,float z,float  rotation,float width,float height,unsigned int decal_type,char * text)
{
  glPushMatrix();
  //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  if (frame.try_for_best_render_quality)
    {
     glEnable(GL_NORMALIZE);
     glEnable(GL_LINE_SMOOTH);
    }

  glTranslated(x,y,z);
  if ( rotation!=0 )    { glRotated(rotation,0.0,0.0,1.0); }


if ( decal_type != 0 )
{ //Decal type zero may only have text :P
  glEnable ( GL_TEXTURE_2D );
 /* DRAW FRAME >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
 glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  if ( decal_type == 1 )  glBindTexture(GL_TEXTURE_2D, heart->gpu.gl_rgb_texture ); else
  if ( decal_type == 2 )  glBindTexture(GL_TEXTURE_2D, star->gpu.gl_rgb_texture ); else
  if ( decal_type == 3 )  glBindTexture(GL_TEXTURE_2D, play_img->gpu.gl_rgb_texture ); else
  if ( decal_type == 4 )  glBindTexture(GL_TEXTURE_2D, pause_img->gpu.gl_rgb_texture ); else
  if ( decal_type == 5 )  glBindTexture(GL_TEXTURE_2D, label->gpu.gl_rgb_texture ); else
                          glBindTexture(GL_TEXTURE_2D, heart->gpu.gl_rgb_texture ); /* DEFAULT */
   glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
     float size_x=width,size_y=height;
     float xmin=(-1)*size_x/2,xmax=size_x/2,ymin=(-1)*size_y/2,ymax=size_y/2;
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0+xmin,0+ymin,0/*-4.1*/);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0+xmax,0+ymin,0/*-4.1*/);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0+xmax,0+ymax,0/*-4.1*/);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0+xmin,0+ymax,0/*-4.1*/);
   glEnd();
  glDisable ( GL_TEXTURE_2D );
  glDisable(GL_BLEND);
}


  if (text!=0)
    {
      if (frame.lighting_enabled) { glDisable( GL_LIGHTING ); }

      glColor3f(1.0,1.0,1.0);
      glRasterPos3f(width/2,0,0.1);
      //glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*)text);
        glPushMatrix();
         glRotated(180,0.0,0.0,1.0);
         glTranslated(-10,0,0);
         glLineWidth(3.0);
         static GLfloat f = 1.0/110;
         glScalef( f, f, f );
         glutStrokeString(GLUT_STROKE_ROMAN,(const unsigned char*)text); //GLUT_STROKE_ROMAN GLUT_STROKE_MONO_ROMAN
         glScalef( 1/f, 1/f, 1/f );
         glLineWidth(1.0);
         glTranslated(0,0,0);
         glRotated(-180,0.0,0.0,1.0);
        glPopMatrix();

      if (frame.lighting_enabled) { glEnable( GL_LIGHTING ); }
    }



  if ( rotation!=0 )    { glRotated(-rotation,0.0,0.0,1.0); }
  glTranslated(-x,-y,-z);
  if (frame.try_for_best_render_quality)
    {
     glDisable(GL_LINE_SMOOTH);
     glDisable(GL_NORMALIZE);
    }
  glPopMatrix();
}



unsigned int Render_3DObject(unsigned int objnum)
{
  char * iffed_label=0;
  if ( objects[objnum].has_label!= 0 ) { iffed_label = objects[objnum].label; }

  DrawObject(
             objects[objnum].position.x,
             objects[objnum].position.y,
             objects[objnum].position.z,
             objects[objnum].roll,
             objects[objnum].width,
             objects[objnum].height,
             objects[objnum].hardcoded_shape,
             iffed_label
           );
  return 1;
}

unsigned int Render_3DObjects()
{
  unsigned int object_traverser=0;
  while ( object_traverser < existing_objects )
   {
     Render_3DObject(object_traverser);
     ++object_traverser;
   }
  return 1;
}

unsigned int Full_of_3DObjects()
{
    if (MAX_SCENE_OBJECTS <=existing_objects) { return 1; }
    return 0;
}

unsigned int Object_Out_Of_Mem(unsigned int objnum)
{
    if (MAX_SCENE_OBJECTS <= objnum) { return 1; } else
    if (existing_objects<= objnum) { return 1; }
    return 0;
}


unsigned int Clear_3DObject(unsigned int objnum)
{
    if (MAX_SCENE_OBJECTS <= objnum) { return 1; }

    struct SceneObject clear_obj = {0};
    objects[objnum]=clear_obj;

    return 0;
}


unsigned int Add_3DObject(struct SceneObject newobj)
{
  if (!Full_of_3DObjects())
   {
     objects[existing_objects]=newobj;
     ++existing_objects;
     fprintf(stderr,"Object %u Created \n",existing_objects);
     return existing_objects-1;
   }
  return 0; /*Return object number*/
}

unsigned int Add_3DObject(float x,float y,float z,float width,float height,unsigned int stock_number,unsigned int lifetime)
{
  if (!Full_of_3DObjects())
   {
     Clear_3DObject(existing_objects);
     objects[existing_objects].has_label=0;
     objects[existing_objects].position.x=x;
     objects[existing_objects].position.y=y;
     objects[existing_objects].position.z=z;
     objects[existing_objects].width=width;
     objects[existing_objects].height=height;
     objects[existing_objects].lifetime=lifetime;
     objects[existing_objects].hardcoded_shape=stock_number;

     fprintf(stderr,"Object %u Created @ %0.2f %0.2f %0.2f \n",existing_objects,x,y,z);
     ++existing_objects;
     return existing_objects-1;
   }
  return 0; /*Return object number*/
}


unsigned int Delete_3DObject(unsigned int objnum)
{
   if (existing_objects==0) { return 0; }
   if (Object_Out_Of_Mem(objnum)) { return 0; }
   if (objnum==existing_objects-1)
    {
       --existing_objects;
    } else
    {
       objects[objnum]=objects[existing_objects-1];
       --existing_objects;
    }

  return 1;
}


unsigned int Delete_All3DObjectsOfShape(unsigned int shape)
{
   if (existing_objects==0) { return 0; }

   unsigned int i=0;
   while (i<existing_objects)
    {
      if (objects[i].hardcoded_shape==shape)
       {
         Delete_3DObject(i);
       } else
       {
         ++i;
       }
    }

  return 1;
}


int CreateObjectFountain(unsigned int stock_image)
{
  float size,speed_x,speed_y;
  float x=album[frame.active_image_place]->position.x;
  float y=album[frame.active_image_place]->position.y;
  float z=album[frame.active_image_place]->position.z+1;


  unsigned int i=0;
  unsigned int mem=0;
   for (i=0; i<25; i++)
    {
        size=0.01;
        size+=(float) 0.15*(rand()%20);
        mem=Add_3DObject(x,y,z+0.25*i,size,size,stock_image,1000000);

        speed_x = (float) 60-rand()%120;
        speed_y = (float) 60-rand()%120;

        objects[mem].velocity.x=0.0001*speed_x;
        if (objects[mem].velocity.x<0) { objects[mem].velocity.x-=0.05;} else
                                       { objects[mem].velocity.x+=0.05;}
        objects[mem].velocity.y=0.001*speed_y;
        if (objects[mem].velocity.y<0) { objects[mem].velocity.y-=0.05;} else
                                       { objects[mem].velocity.y+=0.05;}


        objects[mem].rotation_velocity.z=1+rand()%2;
    }


  return 1;
}







void Run3DObjects(unsigned int microseconds)
{
  unsigned int object_traverser=0;
  while ( object_traverser < existing_objects )
   {

     objects[object_traverser].position.x+=objects[object_traverser].velocity.x;
     objects[object_traverser].position.y+=objects[object_traverser].velocity.y;
     objects[object_traverser].position.z+=objects[object_traverser].velocity.z;

     // NOT SURE IF AXIS ARE CORRECT :P
     objects[object_traverser].heading+=objects[object_traverser].rotation_velocity.y;
     objects[object_traverser].roll+=objects[object_traverser].rotation_velocity.z;
     objects[object_traverser].pitch+=objects[object_traverser].rotation_velocity.x;


     if (objects[object_traverser].lifetime!=0)
      {
          if (microseconds>=objects[object_traverser].lifetime)
            { //Object Lifetime Expired..!
              objects[object_traverser].lifetime=0;
              Delete_3DObject(object_traverser);
              fprintf(stderr,"Object %u Timed out\n",object_traverser);
            } else
            {
              objects[object_traverser].lifetime-=microseconds;
            }
      }




     ++object_traverser;
   }
}
