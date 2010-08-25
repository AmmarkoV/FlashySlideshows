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
#include <unistd.h>
#include <pthread.h>
#include "slideshow.h"
#include "visuals.h"
#include "directory_listing.h"
#include "camera_control.h"
#include "controls.h"
#include "load_images.h"
#include "load_textures.h"
#include "sound.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

#include <stdlib.h>

char APP_VERSION_STRING[50]="FlashySlideShow ver 0.1 / UNDER CONSTRUCTION!";
int STOP_APPLICATION=0;

void * ManageLoadingPicturesMemory_Thread(void * ptr);
pthread_t loadpicturesthread_id;


const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


struct Picture *album[100];

unsigned char video_color[640*480*3]={0};
unsigned char video_depth[640*480*3]={0};


unsigned int framecount,timenow,timebase,fps;

void ToggleFullscreen();

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


void * ManageLoadingPicturesMemory_Thread(void * ptr)
{

  while (!STOP_APPLICATION)
  {
    if ( album[0]==loading ) album[0]=CreatePicture((char * )"album/DSC01114.JPG"); else
    if ( album[1]==loading ) album[1]=CreatePicture((char * )"album/DSC01367.JPG"); else
    if ( album[2]==loading ) album[2]=CreatePicture((char * )"album/DSC01428.JPG"); else
    if ( album[3]==loading ) album[3]=CreatePicture((char * )"album/DSC00871.JPG"); else
    if ( album[4]==loading ) album[4]=CreatePicture((char * )"album/DSC05380.JPG"); else
    if ( album[5]==loading ) album[5]=CreatePicture((char * )"album/DSC01140.JPG"); else
    if ( album[6]==loading ) album[6]=CreatePicture((char * )"album/DSC01515.JPG"); else
    if ( album[7]==loading ) album[7]=CreatePicture((char * )"album/DSC01928.JPG"); else
    if ( album[8]==loading ) album[8]=CreatePicture((char * )"album/DSC02732.JPG");

    usleep(1000);
  }
  return 0;
}


int ManageCreatingTextures(int count_only)
{
  int count=0;

  if ( PictureLoadedOpenGLTexturePending(album[0]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[0],0); } else
  if ( PictureLoadedOpenGLTexturePending(album[1]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[1],0); } else
  if ( PictureLoadedOpenGLTexturePending(album[2]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[2],0); } else
  if ( PictureLoadedOpenGLTexturePending(album[3]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[3],0); } else
  if ( PictureLoadedOpenGLTexturePending(album[4]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[4],0); } else
  if ( PictureLoadedOpenGLTexturePending(album[5]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[5],0); } else
  if ( PictureLoadedOpenGLTexturePending(album[6]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[6],0); } else
  if ( PictureLoadedOpenGLTexturePending(album[7]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[7],0); } else
  if ( PictureLoadedOpenGLTexturePending(album[8]) ) { ++count; ++frame.total_images; if(!count_only) make_texture(album[8],0); }


  return count;
}




static void display(void)
{
	framecount+=1;
	timenow=glutGet(GLUT_ELAPSED_TIME);
	if (timenow - timebase>1000)
	{
	    fps = (unsigned int) framecount * 1000.0 / (timenow-timebase) ;
	    frame.fps=fps; // <- Store in slideshow
	 	timebase = timenow;
		framecount = 0;
	}


   if (ManageCreatingTextures(1)>0)  { frame.currently_loading=1; } else
                                     { frame.currently_loading=0; }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  glPushMatrix();
       glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

          glRotatef(frame.angle_x,-1.0,0,0); // Peristrofi gyrw apo ton x
          glRotatef(frame.angle_y,0,-1.0,0); // Peristrofi gyrw apo ton y
          glRotatef(frame.angle_z,0,0,-1.0);


          glTranslatef(-frame.vx, -frame.vy, -frame.vz);


              DisplayPicture(album[0],-7,-6,0,0,0,0);
              DisplayPicture(album[1],0,-6,0,0,0,0);
              DisplayPicture(album[2],7,-6,0,0,0,0);

              DisplayPicture(album[3],-7,0,0,0,0,0);
              DisplayPicture(album[4],0,0,0,0,0,0);
              DisplayPicture(album[5],7,0,0,0,0,0);
              DisplayPicture(loading,14,0,0,0,0,0);



              DisplayPicture(album[6],-7,6,0,0,0,0);
              DisplayPicture(album[7],0,6,0,0,0,0);
              DisplayPicture(album[8],7,6,0,0,0,0);



          glTranslatef(frame.vx,frame.vy,frame.vz);
       glPopMatrix();





   /* DRAW APPLICATION HUD */
       if ( frame.distance_barrier_after_considered_close<frame.desired_z )
        {
          /* Display HUD only if not zoomed */
          DisplayHUD();
        }
   /* -  -  -  -  -  -  -  */


    glutSwapBuffers();
   glFlush();


    /*  THIS COMMAND MOVES THE CAMERA ACCORDING TO THE USER INPUT*/
      PerformCameraStep();


   ManageCreatingTextures(0);
   usleep(10);

}


// Method to handle the mouse motion
void Motion(int x, int y)
{
 //  MouseLook(x,y);
         //   glutPostRedisplay();

}



// Method to handle the mouse buttons
void Mouse( int button,int state, int x, int y)
{

  if (state== GLUT_UP)   { Controls_Handle_MouseButtons(button,2,x,y); } else
  if (state== GLUT_DOWN) { Controls_Handle_MouseButtons(button,1,x,y); } else
                         { Controls_Handle_MouseButtons(button,0,x,y); }

  //  MouseLook(x,y);
   // glutPostRedisplay();
}


static void key(unsigned char key, int x, int y)
{
  if (key=='q') exit(0);
  //else if (key=='j') ToggleFullscreen();
  unsigned int nokey=0;


  nokey=Controls_Handle_Keyboard(key,x,y);
  if ( nokey == 0 )
  {
    fprintf(stderr,"X:%f Y:%f Z:%f \n",frame.vx,frame.vy,frame.vz);
     key=0;
     glutPostRedisplay();
  }
}




// Method to handle the keyboard's special buttons
void SpecialFunction (int key, int x, int y)
{
    int nokey=0;
	switch (key)
	{
		case GLUT_KEY_UP:    { Controls_Handle_Keyboard(1,x,y); break; }
		case GLUT_KEY_DOWN:  { Controls_Handle_Keyboard(2,x,y); break; }
		case GLUT_KEY_RIGHT: { Controls_Handle_Keyboard(4,x,y); break; }
		case GLUT_KEY_LEFT:  { Controls_Handle_Keyboard(3,x,y); break; }
        default: nokey=1; break;
	}

    if ( nokey == 0 )
    {
      key=0; glutPostRedisplay();
    }
}


static void idle(void)
{
    glutPostRedisplay();
}


void InitGlut()
{
    glutReshapeFunc(resize);
    glutMouseFunc ( Mouse );
    glutPassiveMotionFunc( Motion );
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc( SpecialFunction);
    glutIdleFunc(idle);

}

void ToggleFullscreen()
{
   if ( frame.fullscreen == 0 )
    {
      if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))  glutEnterGameMode(); else
                                                     fprintf(stderr,"Cannot enter fullscreen\n");

      glutGameModeString("1024x768:32");
      InitGlut();
      frame.fullscreen=1;

      /* TODO : ADD STATE RELOADING HERE */

      glutMainLoop();
    } else
   if ( frame.fullscreen == 1 )
    {
      glutLeaveGameMode();
      frame.fullscreen=0;
    }

}


/* Program entry point */

int main(int argc, char *argv[])
{


    glutInit(&argc, argv);
    glutInitWindowSize(1024,768);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow(APP_VERSION_STRING);

    InitGlut();

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


    StartSoundLibrary(); //Kanoume Initialize to OpenAL :)
    InitSlideShow();

    loading=CreatePicture((char * )"album/philosoraptor.jpg");

    int i=0;  for (i=0; i<9; i++) { album[i]=loading; }

    AddSoundBufferForLoad((char *)"sounds/pop.wav");
    LoadSoundBuffers();


    loadpicturesthread_id=0;
    pthread_create( &loadpicturesthread_id, NULL,ManageLoadingPicturesMemory_Thread,0);

    glutMainLoop();


    return EXIT_SUCCESS;
}
