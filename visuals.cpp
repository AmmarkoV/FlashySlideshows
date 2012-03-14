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

#include "effects.h"
#include "visuals.h"
#include "slideshow.h"
#include "load_images.h"
#include "directory_listing.h"
#include "scene_objects.h"
#include "sound.h"
#include <math.h>
#include "camera_control.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif


unsigned int ENABLE_WIGGLING=0;
unsigned int times_drawn_background=0;



void glColorRGB(unsigned char R,unsigned char G,unsigned char B)
{
  glColor3f((float) R/255,(float) G/255,(float) B/255);
}


int DisplayFrame(struct Picture * pic,unsigned int place,float x,float y,float z,float size_x,float size_y,float heading,float pitch,float roll)
{
   unsigned int SIMPLE_FAST_FRAME=1; // This is kind of a shitty flag :P

   float frame_size=0.1; // <---    FRAME SIZE SETTING

   if (SIMPLE_FAST_FRAME)
    {
          glDisable ( GL_TEXTURE_2D ); //No textures , transparencies , etc , just a white QUAD :P less is more .. :P
    } else
    {
          glBindTexture(GL_TEXTURE_2D, picture_frame->gl_rgb_texture );
    }
    glBegin(GL_QUADS);
//    if (pic->transparency>frame_enforced_transparency) { glColor4f(1.0,1.0,1.0,frame_enforced_transparency); } else //Frame always a little transparent..
//                                                       { glColor4f(1.0,1.0,1.0,pic->transparency); }
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x-size_x-frame_size,y-size_y-frame_size,z-0.05);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+size_x+frame_size,y-size_y-frame_size,z-0.05);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+size_x+frame_size,y+size_y+frame_size,z-0.05);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x-size_x-frame_size,y+size_y+frame_size,z-0.05);
   glEnd();

  if (SIMPLE_FAST_FRAME)
    {
          glEnable ( GL_TEXTURE_2D );
    }

//   glColor4f(1.0,1.0,1.0,1.0);
   return 1;
}


int DisplayPicture(struct Picture * pic,unsigned int place,float x,float y,float z,float size_x,float size_y,float heading,float pitch,float roll)
{
  if ( pic == 0 ) { fprintf(stderr,"\n\n\n\nDisplayPicture called for non existing picture outputed ( %f %f %f ) \n\n\n\n",x,y,z); return 0; }

 if (frame.transitions.transition_mode==2)
    { // AYTO EDW MALLON DEN PREPEI NA EINAI EDW <______
      glColor4f(1.0,1.0,1.0,pic->transparency);
    } else
    {
      glColor4f(1.0,1.0,1.0,1.0);
    }

  if ( pic->position.ok == 0 ) { PositionPicture(pic,place); }

  if (PictureCreationPending(pic)) { pic=loading; } else
  if (PictureLoadingPending(pic)) {    pic=loading_texture; } else
  if (PictureFailed(pic)) { pic=failed; } else
  if ( pic->height == 0 ) {
                             //fprintf(stderr,"Zero Height on this image %u !\n",pic->directory_list_index);
                             PrintDirectoryListItem(pic->directory_list_index);
                             pic=failed;
                           }


  if ( (pic!=loading_texture) && ( pic!=failed ) && ( pic!=loading ) ) { /* NORMAL PHOTO*/  } else
                                                                       { PositionPicture(pic,place); }





  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_CULL_FACE);


 glTranslated(x,y,z);


  float tx=0,ty=0,tz=0;
// float tx=x,ty=y,tz=z;

  if ( roll!=0 )    { glRotated(roll,0.0,0.0,1.0); }
  if ( heading!=0 ) { glRotated(heading,0.0,1.0,0.0); }
  if ( pitch!=0 )   { glRotated(pitch,1.0,0.0,0.0); }

  glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work



  if ( (!ENABLE_WIGGLING) || (!CameraOverPicture(place)))
  {
 /* DRAW FRAME >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    if (frame.transitions.transition_mode!=2) { DisplayFrame(pic,place,tx,ty,tz,size_x,size_y,heading,pitch,roll); }

 /* DRAW PICTURE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/



 glEnable ( GL_TEXTURE_2D );
 glBindTexture(GL_TEXTURE_2D, pic->gl_rgb_texture );

  //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // cheap scaling when image bigger than texture
  //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // cheap scaling when image smaller than

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // good quality when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // good quality when image smaller than





   glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f,1.0f);                              // back face points into the screen on z.
    glTexCoord2f(1.0f, 0.0f); glVertex3f(tx-size_x,ty-size_y,tz);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(tx+size_x,ty-size_y,tz);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(tx+size_x,ty+size_y,tz);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(tx-size_x,ty+size_y,tz);
   glEnd();



  }
   else
 if( ENABLE_WIGGLING )
  { //based on http://nehe.gamedev.net/tutorial/flag_effect_%28waving_texture%29/16002/
    RenderPictureWiggling(pic,place,x,y,z,heading,pitch,roll);
  }



  if( ENABLE_WIGGLING )
 { // This needs a nicer implementation :P
   PerformWiggling();
 }

  glDisable ( GL_TEXTURE_2D );
  glEnable(GL_COLOR_MATERIAL);

  glTranslated(-x,-y,-z);

  glPopMatrix();
  return 1;
}



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

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}

void DisplayHUD(unsigned int view_instructions)
{
 setOrthographicProjection();
 glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   glBegin(GL_QUADS);
    glColor4f(0.9,0.9,0.9,0.9);

    glVertex2f(0,50);	// Bottom Left Of The Texture and Quad
    glVertex2f(1024,50);	// Bottom Right Of The Texture and Quad
    glVertex2f(1024,0);	// Top Right Of The Texture and Quad
    glVertex2f(0,0);

    glVertex2f(0,768);	// Bottom Left Of The Texture and Quad
    glVertex2f(1024,768);	// Bottom Right Of The Texture and Quad
    glVertex2f(1024,718);	// Top Right Of The Texture and Quad
    glVertex2f(0,718);

    if (view_instructions)
    {
      glVertex2f(100,668);	// Bottom Left Of The Texture and Quad
      glVertex2f(924,668);	// Bottom Right Of The Texture and Quad
      glVertex2f(924,100);	// Top Right Of The Texture and Quad
      glVertex2f(100,100);
    }

   glEnd();
 glDisable(GL_BLEND);



    if (view_instructions)
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
      sprintf(fps_string,"Rendering Speed : %u fps - %u/%u pics loaded - GPU %u/%u MB - RAM %u/%u MB",frame.fps,frame.total_images_loaded,frame.total_images,total_memory_used_by_gpu,max_memory_used_by_gpu,total_memory,max_memory);
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


void MainDisplayFunction()
{
  if ( frame.images_per_line == 0 ) { fprintf(stderr,"Wtf , 0 images per line ? :P \n "); return; }

  unsigned int line_at=(unsigned int ) MinPictureThatIsVisible()/frame.images_per_line;
  float y=-6;
  y+=6 * line_at;
  unsigned int album_traveler=0;
  unsigned int minpicture=MinPictureThatIsVisible(),maxpicture=MaxPictureThatIsVisible();

    glEnable(GL_NORMALIZE);
    glEnable(GL_LINE_SMOOTH);
    glEnable (GL_POLYGON_SMOOTH);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); //


  if ( CameraSeesOnlyOnePicture() )
   {
     minpicture=frame.active_image_place;
     maxpicture=frame.active_image_place+1;
   } else
   {
       DrawBackground();
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
                               0,
                               0,
                               album[album_traveler]->rotate);
           }


     glDisable(GL_ALPHA_TEST);
     glDisable(GL_BLEND);
     glDisable(GL_LINE_SMOOTH);
     glDisable (GL_POLYGON_SMOOTH);
     glDisable(GL_NORMALIZE);



}

void DrawDecal(float x,float y,float z,float  rotation,float width,float height,unsigned int decal_type,char * text)
{
  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LINE_SMOOTH);


  glTranslated(x,y,z);
  if ( rotation!=0 )    { glRotated(rotation,0.0,0.0,1.0); }


if ( decal_type != 0 )
{ //Decal type zero may only have text :P

  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work
  glEnable ( GL_TEXTURE_2D );
 /* DRAW FRAME >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
 glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  if ( decal_type == 1 )  glBindTexture(GL_TEXTURE_2D, heart->gl_rgb_texture ); else
  if ( decal_type == 2 )  glBindTexture(GL_TEXTURE_2D, star->gl_rgb_texture ); else
  if ( decal_type == 3 )  glBindTexture(GL_TEXTURE_2D, play_img->gl_rgb_texture ); else
  if ( decal_type == 4 )  glBindTexture(GL_TEXTURE_2D, pause_img->gl_rgb_texture ); else
  if ( decal_type == 5 )  glBindTexture(GL_TEXTURE_2D, label->gl_rgb_texture ); else
                          glBindTexture(GL_TEXTURE_2D, heart->gl_rgb_texture ); /* DEFAULT */
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
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);
}


  if (text!=0)
    {
      glColor3f(1.0,1.0,1.0);
      glDisable( GL_LIGHTING );
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

      glEnable( GL_LIGHTING );
    }



  if ( rotation!=0 )    { glRotated(-rotation,0.0,0.0,1.0); }
  glTranslated(-x,-y,-z);
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_NORMALIZE);
  glPopMatrix();
}

void DrawBackground()
{
  ++times_drawn_background;

  glEnable ( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D, background->gl_rgb_texture );

   glBegin(GL_QUADS);
    glColor4f(1.0,1.0,1.0,1.0);

    float x=frame.vx,y=frame.vy,z=0;
    float xmin=-70,xmax=70;
    float ymin=-((xmax-xmin)*3/4)/2,ymax=ymin*(-1);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+xmin,y+ymin,z-15);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+xmax,y+ymin,z-15);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+xmax,y+ymax,z-15);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+xmin,y+ymax,z-15);

   glEnd();

    glDisable ( GL_TEXTURE_2D );
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);

}

int DrawEffects()
{

  DrawDecal(25,0,0,(float) times_drawn_background/100,9,9,1,0);
  DrawDecal(25,14,0,(float) -times_drawn_background/100,9,9,2,0);
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


int StaryFountain(unsigned int stock_image)
{
  float x,y,z,size,speed_x,speed_y;
  x=album[frame.active_image_place]->position.x;
  y=album[frame.active_image_place]->position.y;
  z=album[frame.active_image_place]->position.z+1;


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
        //objects[mem].velocity.z=0.1*(1+rand()%5);
    }


  return 1;
}





