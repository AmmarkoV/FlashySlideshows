#include <stdlib.h>
#include <stdio.h>
#include "visuals.h"
#include "slideshow.h"
#include "load_images.h"
#include "sound.h"

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

int DisplayPicture(struct Picture * pic,float x,float y,float z,float heading,float pitch,float roll)
{
  if ( pic == 0 ) { fprintf(stderr,"DisplayPicture called for non existing picture outputed ( %f %f %f ) \n",x,y,z); return 0; }


  if ( pic -> failed_to_load == 1 ) { pic=failed; } else


  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_NORMALIZE);

  glTranslated(x,y,z);
  if ( roll!=0 ) { glRotated(roll,0.0,0.0,1.0); }
  if ( heading!=0 ) { glRotated(heading,0.0,1.0,0.0); }
  if ( pitch!=0 ) { glRotated(pitch,1.0,0.0,0.0); }


  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work

  glEnable ( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D, pic->gl_rgb_texture );



   float size_x=12,size_y=9,ratio=0.0;
   if ( pic->height != 0 ) { ratio=pic->width/pic->height; } else
                           { fprintf(stderr,"Zero Height on this image %s !\n",pic->filename); return 0; }
   if ( ratio == 0 )  { fprintf(stderr,"Zero X/Y Ratio on this image %s !\n",pic->filename); return 0; }

   size_y=size_x/ratio;
   float xmin=(-1)*size_x/2,xmax=size_x/2,ymin=(-1)*size_y/2,ymax=size_y/2;
   ymin=-4.5;
   ymax=4.5;

   if ( pic->transparency != 1.0 )
    {  glEnable(GL_BLEND);
       glBlendFunc(GL_SRC_ALPHA, GL_ONE); }


   glBegin(GL_QUADS);
    glColor4f(1.0,1.0,1.0,pic->transparency);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+xmin,y+ymin,z-5);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+xmax,y+ymin,z-5);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+xmax,y+ymax,z-5);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+xmin,y+ymax,z-5);

   glEnd();

   if ( pic->transparency != 1.0 )
    {  glDisable(GL_BLEND);  }

  glDisable ( GL_TEXTURE_2D );
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);


  glTranslated(-x,-y,-z);
//  glDisable(GL_NORMALIZE);
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
         SoundLibrary_PlaySound(0);
        //   SoundLibrary_PlaySoundPos(0,0,0,0);

         glRasterPos2f(0,20);
         glColor3f(1.0,0.0,0.0);
         glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*) "LOADING PICTURE MIPMAPS");
        }

      char fps_string[40]={0};
      sprintf(fps_string,"Rendering Speed : %u fps",frame.fps);
      glColor3f(1.0,1.0,0.0);
      glRasterPos2f(700,10);
      glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,(const unsigned char*)fps_string);


 glColor4f(1.0,1.0,1.0,1.0);
 resetPerspectiveProjection();
}


