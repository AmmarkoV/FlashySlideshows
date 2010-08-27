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

char APP_VERSION_STRING[50]="FlashySlideShow ver 0.2 / UNDER CONSTRUCTION!";
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

char pictures_filename_shared_stack[1024]={0};


unsigned int framecount=0,timenow=0,timebase=0,fps=0;

void ToggleFullscreen();




void * ManageLoadingPicturesMemory_Thread(void * ptr)
{
  unsigned int album_traveler=0;
  while (!STOP_APPLICATION)
  {
    if ( album_traveler >10 ) { fprintf(stderr,"SOMEONE!"); }
    if ( album[album_traveler]==loading )
      {
          if ( GetViewableFilenameforFile(album_traveler,(char *) "album/",pictures_filename_shared_stack) == 1 )
            {
               fprintf(stderr,"directory_listing query for picture %u returned string `%s`",album_traveler,pictures_filename_shared_stack);
               album[album_traveler]=CreatePicture(pictures_filename_shared_stack);
            }
      }


    ++album_traveler;
    if ( album_traveler > GetTotalViewableFilesInDirectory() )  { album_traveler = 0; }
    usleep(1000);
  }
  return 0;
}


int ManageCreatingTextures(int count_only)
{
  unsigned int count=0,i=0;

  for ( i=0; i<GetTotalViewableFilesInDirectory(); i++)
   {
     if ( PictureLoadedOpenGLTexturePending(album[i]) ) { ++count;  if(!count_only) make_texture(album[i],0); }
   }


  return count;
}





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


static void display(void)
{
	framecount+=1;
	timenow=glutGet(GLUT_ELAPSED_TIME);
	frame.tick_count=timenow; // <- Slideshow triggering
	if (timenow - timebase>1000)
	{
	    fps = (unsigned int) framecount * 1000.0 / (timenow-timebase) ;
	    frame.fps=fps; // <- Store in slideshow
	 	timebase = timenow;
		framecount = 0;
	}

   AutomaticSlideShowControl_if_needed();

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
   Controls_Handle_MouseMotion(0,0,x,y);
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
  else if (key=='j') ToggleFullscreen();


  unsigned int nokey = Controls_Handle_Keyboard(key,x,y);
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
   fprintf(stderr,"Fullscreen toggling not fully implemented , skipping command for safety \n");
   return;
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
    /* GLUT Initialization >>>>>>>>>>>>>>>>>> */
    glutInit(&argc, argv);
    glutInitWindowSize(1024,768);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow(APP_VERSION_STRING);

    InitGlut();
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */




    /* OpenGL Initialization >>>>>>>>>>>>>>>>> */
    glClearColor(1,1,1,1);
    /*
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
     glEnable(GL_COLOR_MATERIAL);*/

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
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
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */



    /* OpenAL Initialization >>>>>>>>>>>>>>>>> */
    StartSoundLibrary();
    AddSoundBufferForLoad((char *)"sounds/pop.wav");
    LoadSoundBuffers();
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */




    /* Initialize Slideshow variables */
    InitSlideShow();



    /* DEMO , DEVELOPMENT SETTINGS < WILL BE REMOVED > */
    loading=CreatePicture((char * )"album/philosoraptor.jpg");
    int i=0;  for (i=0; i<9; i++) { album[i]=loading; }


    GetDirectoryList((char * )"album/",0); /* First Call using zero as a second parameter to only count directory size */
    fprintf(stderr,"Album directory has %u pictures inside \n",GetTotalViewableFilesInDirectory());
    GetDirectoryList((char * )"album/",GetTotalViewableFilesInDirectory()); /* Load Directory List */

    frame.total_images=GetTotalViewableFilesInDirectory();




    /* Initialize Picture Loading Thread , start rendering  */

    loadpicturesthread_id=0;
    pthread_create( &loadpicturesthread_id, NULL,ManageLoadingPicturesMemory_Thread,0);

  //  sleep(1);
  //  return 0;
    glutMainLoop();

    return EXIT_SUCCESS;
}
