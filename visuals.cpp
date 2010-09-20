#include <stdlib.h>
#include <stdio.h>
#include "visuals.h"
#include "slideshow.h"
#include "load_images.h"
#include "scene_objects.h"
#include "sound.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

unsigned int times_drawn_background=0;

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
   glEnd();
 glDisable(GL_BLEND);

      glColor3f(1,0.0,0.0);
      glRasterPos2f(0,0);

      glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) "UNDER CONSTRUCTION!");

       if (frame.currently_loading==1)
       {
        // SoundLibrary_PlaySound(0);
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


