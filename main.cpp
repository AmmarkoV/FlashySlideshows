/* FancySlideShow..
An fancy , OpenGL slideshow application !
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
#include <unistd.h>
#include "slideshow.h"
#include "load_images.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>


struct Picture *album[100];
struct Picture *loading,*left_picture,*main_picture,*right_picture;

static int slices = 16;
static int stacks = 16;
unsigned char video_color[640*480*3]={0};
unsigned char video_depth[640*480*3]={0};
float vx=00.0,vy=00.0,vz=00.0;
float desired_x=00.0,desired_y=00.0,desired_z=00.0,desired_step=1.35;
float angle_x=0.0,angle_y=0.0,angle_z=180.0,step=0.05;
double last_load;
/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();           /*NEAR*/
    glFrustum(-ar, ar, -1.0, 1.0, 1.0, 800.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


void glColorRGB(unsigned char R,unsigned char G,unsigned char B)
{
  glColor3f((float) R/255,(float) G/255,(float) B/255);
}


void DisplayPicture(struct Picture * pic,float x,float y,float z,float heading,float pitch,float roll)
{
  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_NORMALIZE);

  glTranslated(x,y,z);
  if ( roll!=0 ) { glRotated(roll,0.0,0.0,1.0); }
  if ( heading!=0 ) { glRotated(heading,0.0,1.0,0.0); }
  if ( pitch!=0 ) { glRotated(pitch,1.0,0.0,0.0); }

 // glEnable(GL_BLEND);
 // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  // glColor4f(0.5,0.5,0.5,1.0);

  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work

  glEnable ( GL_TEXTURE_2D );
  glBindTexture(GL_TEXTURE_2D, pic->gl_rgb_texture );


   float size_x=12,size_y=9,ratio=0.0;
   if ( pic->height != 0 ) { ratio=pic->width/pic->height; } else
                           { fprintf(stderr,"Zero Height on this image %s !\n",pic->filename); return; }
   if ( ratio == 0 )  { fprintf(stderr,"Zero X/Y Ratio on this image %s !\n",pic->filename); return; }

   size_y=size_x/ratio;
   float xmin=(-1)*size_x/2,xmax=size_x/2,ymin=(-1)*size_y/2,ymax=size_y/2;
   ymin=-4.5;
   ymax=4.5;

   glBegin(GL_QUADS);
    glColor4f(1.0,1.0,1.0,1.0);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+xmin,y+ymin,z-5);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+xmax,y+ymin,z-5);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+xmax,y+ymax,z-5);	// Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+xmin,y+ymax,z-5);

   glEnd();

  glDisable ( GL_TEXTURE_2D );
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);


  glTranslated(-x,-y,-z);
//  glDisable(GL_NORMALIZE);
  glPopMatrix();
  return;
}


static void display(void)
{
//    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  glPushMatrix();
       glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

          glRotatef(angle_x,-1.0,0,0); // Peristrofi gyrw apo ton x
          glRotatef(angle_y,0,-1.0,0); // Peristrofi gyrw apo ton y
          glRotatef(angle_z,0,0,-1.0);


          glTranslatef(-vx, -vy, -vz);

              DisplayPicture(album[0],-7,-6,0,0,0,0);
              DisplayPicture(album[1],0,-6,0,0,0,0);
              DisplayPicture(album[2],7,-6,0,0,0,0);

              DisplayPicture(left_picture,-7,0,0,0,0,0);
              DisplayPicture(main_picture,0,0,0,0,0,0);
              DisplayPicture(right_picture,7,0,0,0,0,0);
              DisplayPicture(loading,14,0,0,0,0,0);



              DisplayPicture(album[3],-7,6,0,0,0,0);
              DisplayPicture(album[4],0,6,0,0,0,0);
              DisplayPicture(album[5],7,6,0,0,0,0);


          glTranslatef(vx,vy,vz);
       glPopMatrix();

    glutSwapBuffers();
    usleep(100);


    /*
      -------------------------------------
      CAMERA SMOOTH ZOOM/PAN ETC
      -------------------------------------
    */
    float speed_multiplier=150;
    if ( desired_x != vx ) { if ( desired_x < vx )
                                 {
                                   /* CLOSING IN OPERATION */   vx = vx- ( vx - desired_x ) / speed_multiplier;
                                   if ( desired_x > vx ) { desired_x = vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( desired_x > vx )
                                 {
                                   /* CLOSING IN OPERATION */   vx = vx+ ( desired_x - vx ) / speed_multiplier;
                                   if ( desired_x < vx ) { desired_x = vx; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }

    if ( desired_y != vy ) { if ( desired_y < vy )
                                 {
                                   /* CLOSING IN OPERATION */   vy = vy- ( vy - desired_y ) / speed_multiplier;
                                   if ( desired_y > vy ) { desired_y = vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( desired_y > vy )
                                 {
                                   /* CLOSING IN OPERATION */   vy = vy+ ( desired_y - vy ) / speed_multiplier;
                                   if ( desired_y < vy ) { desired_y = vy; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }


    if ( desired_z != vz ) { if ( desired_z < vz )
                                 {
                                   /* CLOSING IN OPERATION */   vz = vz- ( vz - desired_z ) / speed_multiplier;
                                   if ( desired_z > vz ) { desired_z = vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 } else
                             if ( desired_z > vz )
                                 {
                                   /* CLOSING IN OPERATION */   vz = vz+ ( desired_z - vz ) / speed_multiplier;
                                   if ( desired_z < vz ) { desired_z = vz; } /* MAKE SURE NO OVERFLOW HAPPENED */
                                 }
                           }
  /* -------------------------------------
     CAMERA SAFE GUARD!
     -------------------------------------
  */
   if ( vz<=main_slideshow.distance_block_lower) { vz=main_slideshow.distance_block_lower; desired_z= vz; } /* DO NOT ALLOW ANY CLOSER */
   if ( vz>=main_slideshow.distance_block_upper) { vz=main_slideshow.distance_block_upper; desired_z= vz; } /* DO NOT ALLOW ANY CLOSER */
}



static void key(unsigned char key, int x, int y)
{

      if (vz<main_slideshow.distance_barrier_after_considered_zoom)  desired_step=main_slideshow.desired_step_zoom; else
      if (vz<main_slideshow.distance_barrier_after_considered_close)  desired_step=main_slideshow.desired_step_close; else
      if (vz<main_slideshow.distance_barrier_after_considered_far)  desired_step=main_slideshow.desired_step_far;

    unsigned int nokey=0;
    switch (key)
    {
        case 'r': desired_z+=desired_step; break;
        case 'f': desired_z-=desired_step; break;
        case 'a': desired_x+=desired_step; break;
        case 'd': desired_x-=desired_step; break;
        case 's': desired_y+=desired_step; break;
        case 'w': desired_y-=desired_step; break;
        case 'z': angle_x-=0.5; break;
        case 'c': angle_x+=0.5; break;
        case 't': angle_y-=0.5; break;
        case 'g': angle_y+=0.5; break;
        case 'y': angle_z-=0.5; break;
        case 'h': angle_z+=0.5; break;

        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;

       default :
             nokey=1;
            break;
    }

  if ( nokey == 0 )
  {
    fprintf(stderr,"X:%f Y:%f Z:%f \n",vx,vy,vz);

    key=0;
    glutPostRedisplay();
  }
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{


    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("FlashySlideShow");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    //glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
 //   glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);



    InitSlideShow();

    left_picture=CreatePicture((char * )"album/DSC01140.JPG");

    main_picture=CreatePicture((char * )"album/DSC00871.JPG");

    right_picture=CreatePicture((char * )"album/DSC05380.JPG");

    loading=CreatePicture((char * )"album/philosoraptor.jpg");
   // loading=CreatePicture((char * )"raw/dali-persistence-of-time.jpg");

    album[0]=CreatePicture((char * )"album/DSC01114.JPG");
    album[1]=CreatePicture((char * )"album/DSC01367.JPG");
    album[2]=CreatePicture((char * )"album/DSC01428.JPG");
    album[3]=CreatePicture((char * )"album/DSC01515.JPG");
    album[4]=CreatePicture((char * )"album/DSC01928.JPG");
    album[5]=CreatePicture((char * )"album/DSC02732.JPG");


      glutMainLoop();


    return EXIT_SUCCESS;
}
