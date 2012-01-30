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

int wiggle_count = 0;
float points[45][45][3];   // the array for the points on the grid of our "wave"


void glColorRGB(unsigned char R,unsigned char G,unsigned char B)
{
  glColor3f((float) R/255,(float) G/255,(float) B/255);
}




int DisplayPicture(struct Picture * pic,unsigned int place,float x,float y,float z,float heading,float pitch,float roll)
{
  if ( pic == 0 ) { fprintf(stderr,"\n\n\n\nDisplayPicture called for non existing picture outputed ( %f %f %f ) \n\n\n\n",x,y,z); return 0; }


  if ( pic -> failed_to_load == 1 ) { pic=failed; }
  if ( pic->position.ok == 0 ) { PositionPicture(pic,place); }


   float size_x=12,size_y=9,ratio=0.0;
   if ( pic->height != 0 ) { ratio=pic->width/pic->height; } else
                           { //fprintf(stderr,"Zero Height on this image %s !\n",pic->filename);
                             DisplayPicture(loading_texture,place,x,y,z,heading,pitch,roll);
                             return 0;
                           }
   if ( ratio == 0 )  { //fprintf(stderr,"Zero X/Y Ratio on this image %s !\n",pic->filename);
                        DisplayPicture(loading_texture,place,x,y,z,heading,pitch,roll);
                        return 0;
                      }

   size_y=size_x/ratio;
   float frame_size=0.9;


  x=0; y=0; z=0;
  if ( (pic!=loading_texture) && ( pic!=failed ) && ( pic!=loading ) ) { /* NORMAL PHOTO*/  } else
                                                                       { PositionPicture(pic,place); }


  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LINE_SMOOTH);

  if ( roll!=0 )    { glRotated(roll,0.0,0.0,1.0); }
  if ( heading!=0 ) { glRotated(heading,0.0,1.0,0.0); }
  if ( pitch!=0 )   { glRotated(pitch,1.0,0.0,0.0); }

  //glTranslated(x,y,z);


  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work


  /* if ( pic->transparency != 1.0 ) {  glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE); }*/

  glEnable ( GL_TEXTURE_2D );

  if( !ENABLE_WIGGLING )
  {
 /* DRAW FRAME >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
 glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glBindTexture(GL_TEXTURE_2D, picture_frame->gl_rgb_texture );
   glBegin(GL_QUADS);
    glColor4f(1.0,1.0,1.0,pic->transparency);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+pic->position.x-pic->position.size_x-frame_size,y+pic->position.y-pic->position.size_y-frame_size,z+pic->position.z-0.1);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+pic->position.x+pic->position.size_x+frame_size,y+pic->position.y-pic->position.size_y-frame_size,z+pic->position.z-0.1);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+pic->position.x+pic->position.size_x+frame_size,y+pic->position.y+pic->position.size_y+frame_size,z+pic->position.z-0.1);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+pic->position.x-pic->position.size_x-frame_size,y+pic->position.y+pic->position.size_y+frame_size,z+pic->position.z-0.1);
   glEnd();
  glDisable(GL_BLEND);

 /* DRAW PICTURE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

 glBindTexture(GL_TEXTURE_2D, pic->gl_rgb_texture );
   glBegin(GL_QUADS);
    glColor4f(1.0,1.0,1.0,pic->transparency);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+pic->position.x-pic->position.size_x,y+pic->position.y-pic->position.size_y,z+pic->position.z);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+pic->position.x+pic->position.size_x,y+pic->position.y-pic->position.size_y,z+pic->position.z);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+pic->position.x+pic->position.size_x,y+pic->position.y+pic->position.size_y,z+pic->position.z);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+pic->position.x-pic->position.size_x,y+pic->position.y+pic->position.size_y,z+pic->position.z);
   glEnd();
  }
   else
 if( ENABLE_WIGGLING )
  { //based on http://nehe.gamedev.net/tutorial/flag_effect_%28waving_texture%29/16002/
    glBindTexture(GL_TEXTURE_2D, pic->gl_rgb_texture );
    glPolygonMode(GL_BACK, GL_FILL);
    glPolygonMode(GL_FRONT, GL_LINE);

    glBegin(GL_QUADS);
    unsigned int ux,uy;
    float float_x,float_y,float_xb,float_yb;
    for (ux=0; ux<44; ux++)
    {
	  for (uy=0; uy<44; uy++)
       {
	    float_x  = (float) (ux)/44;
	    float_y  = (float) (uy)/44;
	    float_xb = (float) (ux+1)/44;
	    float_yb = (float) (uy+1)/44;

	    glTexCoord2f( float_x, float_y);
	                                 //-pic->position.size_x -*pic->position.size_y
	    glVertex3f(x+pic->position.x+ points[ux][uy][0]  ,
                   y+pic->position.y+ points[ux][uy][1]  ,
                   z+pic->position.z+points[ux][uy][2] );

	    glTexCoord2f( float_x, float_yb );
	    glVertex3f(x+pic->position.x+ points[ux][uy+1][0] ,
                   y+pic->position.y+ points[ux][uy+1][1],
                   z+pic->position.z+points[ux][uy+1][2] );

	    glTexCoord2f( float_xb, float_yb );
	    glVertex3f(x+pic->position.x+ points[ux+1][uy+1][0],
                   y+pic->position.y+ points[ux+1][uy+1][1],
                   z+pic->position.z+points[ux+1][uy+1][2] );

	    glTexCoord2f( float_xb, float_y );
	    glVertex3f(x+pic->position.x+ points[ux+1][uy][0],
                   y+pic->position.y+ points[ux+1][uy][1],
                   z+pic->position.z+points[ux+1][uy][2] );
	   }
    }
    glEnd();

    if (wiggle_count == 100)
      { // cycle the sine values
	    for (uy = 0; uy <45; uy++) { points[44][uy][2] = points[0][uy][2]; }

	    for( ux = 0; ux < 44; ux++ )
	     {
	      for( uy = 0; uy < 45; uy++)
	      {
		   points[ux][uy][2] = points[ux+1][uy][2];
	       }
	     }
	    wiggle_count = 0;
      }
    wiggle_count++;
  }

 /*  if ( pic->transparency != 1.0 ) {  glDisable(GL_BLEND);  }*/

  glDisable ( GL_TEXTURE_2D );
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);


 // glTranslated(-x,-y,-z);
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_NORMALIZE);
  glPopMatrix();
  return 1;
}



void setOrthographicProjection() {

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

void DisplayHUD()
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

   glEnd();
 glDisable(GL_BLEND);

      glColor3f(1,0.0,0.0);
      glRasterPos2f(0,0);

      glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) "UNDER CONSTRUCTION!");

       if (frame.currently_loading==1)
       {
         SoundLibrary_PlaySound(LOADED_PICTURE);
        //   SoundLibrary_PlaySoundPos(0,0,0,0);
         glRasterPos2f(0,20);
         glColor3f(1.0,0.0,0.0);
         glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) "LOADING PICTURE");
        }

      char fps_string[512]={0};
      unsigned int max_ram_used_by_gpu=(unsigned int) (frame.gpu.maxRAM/ (1024*1024));
      unsigned int total_ram_used_by_gpu=(unsigned int) (frame.gpu.usedRAM/ (1024*1024));
      sprintf(fps_string,"Rendering Speed : %u fps - %u/%u pics loaded - %u/%u MB",frame.fps,frame.total_images_loaded,frame.total_images,total_ram_used_by_gpu,max_ram_used_by_gpu);
      glColor3f(1.0,1.0,0.0);
      glRasterPos2f(400,10);
      glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*)fps_string);

      glColor3f(0.0,0.0,0.0);
      sprintf(fps_string,"%u/%u over %s created %u/%u/%u %u:%02u:%02u",
                          frame.active_image_place,
                          GetTotalViewableFilesInDirectory(),

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


  if ( CameraSeesOnlyOnePicture() )
   {
     minpicture=frame.active_image_place;
     maxpicture=frame.active_image_place+1;
   }
          for ( album_traveler=minpicture; album_traveler<maxpicture; album_traveler++ )
           {
               if ( album_traveler%3==0 ) { if ( DisplayPicture(album[album_traveler],album_traveler, 7,y,-5,0,0,0)!= 1 ) { /*fprintf(stderr,"Error 1 Drawing pic %u \n",album_traveler);*/ } } else
               if ( album_traveler%3==1 ) { if ( DisplayPicture(album[album_traveler],album_traveler,  0,y,-5,0,0,0)!= 1 ) { /*fprintf(stderr,"Error 2 Drawing pic %u \n",album_traveler);*/ } } else
               if ( album_traveler%3==2 ) { if ( DisplayPicture(album[album_traveler],album_traveler, -7,y,-5,0,0,0)!= 1 ) { /*fprintf(stderr,"Error 3 Drawing pic %u \n",album_traveler);*/ }
                                                                                                              y+=6;  } else
                                          { fprintf(stderr,"Wtf"); }
           }





}

void DrawDecal(float x,float y,float z,unsigned int rotation,unsigned int decal_type)
{
  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LINE_SMOOTH);

  glTranslated(x,y,z);
  if ( rotation!=0 )    { glRotated(rotation,0.0,0.0,1.0); }

  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work
  glEnable ( GL_TEXTURE_2D );
 /* DRAW FRAME >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
 glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  if ( decal_type == 0 )  glBindTexture(GL_TEXTURE_2D, heart->gl_rgb_texture ); else
  if ( decal_type == 1 )  glBindTexture(GL_TEXTURE_2D, star->gl_rgb_texture ); else
                          glBindTexture(GL_TEXTURE_2D, heart->gl_rgb_texture ); /* DEFAULT */
   glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
     float size_x=9,size_y=9;
     float xmin=(-1)*size_x/2,xmax=size_x/2,ymin=(-1)*size_y/2,ymax=size_y/2;
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+xmin,y+ymin,z-4.1);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+xmax,y+ymin,z-4.1);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+xmax,y+ymax,z-4.1);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+xmin,y+ymax,z-4.1);
   glEnd();
  glDisable ( GL_TEXTURE_2D );
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);

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
    float xmin=(-1)*100,xmax=100,ymin=(-1)*100,ymax=100;

    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+xmin,y+ymin,z-15);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+xmax,y+ymin,z-15);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+xmax,y+ymax,z-15);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+xmin,y+ymax,z-15);

   glEnd();

    glDisable ( GL_TEXTURE_2D );
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);

}

int DrawEffects()
{
  DrawDecal(14,0,0,times_drawn_background/1000,0);
  DrawDecal(14,14,0,times_drawn_background/1000,1);
  return 1;
}




void InitEffects()
{
  float float_x , float_y;
  for(float_x = 0.0f; float_x < 9.0f; float_x +=  0.2f )
  {
	for(float_y = 0.0f; float_y < 9.0f; float_y += 0.2f)
	{
	    points[ (int) (float_x*5) ][ (int) (float_y*5) ][0] = float_x - 4.4f;
	    points[ (int) (float_x*5) ][ (int) (float_y*5) ][1] = float_y - 4.4f;
	    points[ (int) (float_x*5) ][ (int) (float_y*5) ][2] = (float) (sin( ( (float_x*5*8)/360 ) * 3.14159 * 2 ));
	}
  }
}

