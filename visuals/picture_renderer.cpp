#include "picture_renderer.h"
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


inline int DisplayFrame(struct Picture * pic,unsigned int place,float *x,float *y,float *z,float *size_x,float *size_y,float *heading,float *pitch,float *roll)
{
   unsigned int SIMPLE_FAST_FRAME=1; // This is kind of a shitty flag :P

   float frame_size=0.1; // <---    FRAME SIZE SETTING

   if (SIMPLE_FAST_FRAME) { glDisable ( GL_TEXTURE_2D ); } else //No textures , transparencies , etc , just a white QUAD :P less is more .. :P
                          { glBindTexture(GL_TEXTURE_2D, picture_frame->gpu.gl_rgb_texture ); }
   glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(*x-*size_x-frame_size,*y-*size_y-frame_size,*z-0.05);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(*x+*size_x+frame_size,*y-*size_y-frame_size,*z-0.05);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(*x+*size_x+frame_size,*y+*size_y+frame_size,*z-0.05);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(*x-*size_x-frame_size,*y+*size_y+frame_size,*z-0.05);
   glEnd();

  if (SIMPLE_FAST_FRAME)  { glEnable ( GL_TEXTURE_2D ); }
   return 1;
}

int DisplayPicture(struct Picture * pic,unsigned int place,float x,float y,float z,float size_x,float size_y,float heading,float pitch,float roll)
{
  if ( pic == 0 ) { fprintf(stderr,"\n\n\n\nDisplayPicture called for non existing picture outputed ( %f %f %f ) \n\n\n\n",x,y,z); return 0; }

 // For transparency to work correctly we need to apply the color before the other OpenGL calls..!
 if (frame.transitions.transition_mode==2) { glColor4f(1.0,1.0,1.0,pic->transparency); } else
                                           { glColor4f(1.0,1.0,1.0,1.0); }

  if ( pic->position.ok == 0 ) { PositionPicture(pic,place); }

  if (PictureCreationPending(pic)) { pic=loading; } else
  if (PictureLoadingPending(pic))  { pic=loading_texture; } else
  if (PictureFailed(pic)) { pic=failed; } else
  if ( pic->height == 0 ) { pic=failed; }


  if ( (pic!=loading_texture) && ( pic!=failed ) && ( pic!=loading ) ) { /* NORMAL PHOTO*/  } else
                                                                       { PositionPicture(pic,place); }

  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_CULL_FACE);


  glTranslated(x,y,z);
  float tx=0,ty=0,tz=0;

  if ( roll!=0 )    { glRotated(roll,0.0,0.0,1.0); }
  if ( heading!=0 ) { glRotated(heading,0.0,1.0,0.0); }
  if ( pitch!=0 )   { glRotated(pitch,1.0,0.0,0.0); }

  glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work



  /* DRAW FRAME >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    DisplayFrame(pic,place,&tx,&ty,&tz,&size_x,&size_y,&heading,&pitch,&roll);
  /* DRAW PICTURE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/



 glEnable ( GL_TEXTURE_2D );
 glBindTexture(GL_TEXTURE_2D, pic->gpu.gl_rgb_texture );

   if (frame.try_for_best_render_quality)
    {
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // good quality when image bigger than texture
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // good quality when image smaller than
    } else
    {
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // cheap scaling when image bigger than texture
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // cheap scaling when image smaller than
    }


   glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f,1.0f);                              // back face points into the screen on z.
    glTexCoord2f(1.0f, 0.0f); glVertex3f(tx-size_x,ty-size_y,tz);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(tx+size_x,ty-size_y,tz);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(tx+size_x,ty+size_y,tz);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(tx-size_x,ty+size_y,tz);
   glEnd();

  glDisable ( GL_TEXTURE_2D );
  glEnable(GL_COLOR_MATERIAL);

  glTranslated(-x,-y,-z);

  glPopMatrix();
  return 1;
}
