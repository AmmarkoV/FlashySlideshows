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
#include "memory_hypervisor.h"
#include "wxwidgets_stuff.h"
#include "joystick.h"
#include "environment.h"


#include <sys/time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

#include <stdlib.h>

char APP_VERSION_STRING[70]="FlashySlideShow ver 0.39 / UNDER CONSTRUCTION!";
int STOP_APPLICATION=0;

 struct timeval last_frame,this_frame,difference;

char pictures_filename_shared_stack[1024]={0};
unsigned int framecount=0,timenow=0,timebase=0,fps=0;

void ToggleFullscreen();

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


GLuint filter;						// Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
GLuint fogfilter= 0;					// Which Fog To Use
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};		// Fog Color







void * ManageLoadingPicturesMemory_Thread(void * ptr)
{
  /* THIS THREAD LOADS/UNLOADS IMAGES AROUND
     BUT !!!!!NOT!!!!! PASS THEM TO GPU AS TEXTURES
  */
  unsigned int loaded_pictures_this_loop=0;
  while (!STOP_APPLICATION)
  {
    loaded_pictures_this_loop= ManagePicturesMemory();// If no pictures loaded returns 0
    loaded_pictures_this_loop+=ManageTexturesMemory();// If no pictures adds 0

    if ( loaded_pictures_this_loop == 0 ) { usleep(1000000);  } else
                                          { usleep(10000);  }
  }
  return 0;
}


int ManageCreatingTextures(int count_only)
{
  /* THIS FUNCITON BELONGS TO THE OPENGL THREAD AND LOADS/UNLOADS IMAGES
     FROM THE GPU AS TEXTURES!!!! THEY HAVE TO BE LOADED BY ManageLoadingPicturesMemory_Thread
  */
  unsigned int count=0,i=0;

  UnLoadTexturesIfNeeded();
  for ( i=0; i<frame.total_images; i++)
   {
     if ( PictureLoadedOpenGLTexturePending(album[i]) ) { ++count;  if(!count_only) make_texture(album[i],0); }
   }

  return count;
}



/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            FRAME RATE LIMITER
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void timerCB(int millisec)
{
 glutPostRedisplay();
 glutTimerFunc(millisec, timerCB, millisec);
}


int framerate_limiter()
{
 //return 0; /*Disabled */

  if ( frame.fps > 105 )
   {
     unsigned int frames_to_cut = frame.fps - 100;
     unsigned int time_to_cut = (unsigned int) 1000000/frames_to_cut;
     usleep (time_to_cut);
   }

  return 1;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/



static void ResizeCallback(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();           /*NEAR*/
    glFrustum(-ar, ar, -1.0, 1.0, 1.0, 800.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void DisplayCallback(void)
{
    /*KEEP TRACK OF TIME -----------------*/
    gettimeofday(&this_frame,0x0);
    time_passed_microseconds = timeval_diff(&difference,&this_frame,&last_frame);
    last_frame = this_frame;
    /*---------------------------------------*/



    /* FRAME RATE COUNTING
       >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
	framecount+=1;
	timenow=glutGet(GLUT_ELAPSED_TIME);
	frame.tick_count=timenow; // <- Slideshow triggering
	if (timenow - timebase>1000)
	{
	    float real_fps = (framecount * 1000.0) / (timenow-timebase);
	    fps = (unsigned int) real_fps ;
	    frame.fps=fps; // <- Store in slideshow
	 	timebase = timenow;
		framecount = 0;
	}
	//if ( fps < 25 ) { fprintf(stderr,"Low Framerate %u fps \n",fps); }
    /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/



    if (  frame.effects.fog_on==1 ) { glEnable(GL_FOG);	} else
                                    { glDisable(GL_FOG);	}

    /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


   /* Texture binding via OpenGL , it can only be done in this thread , before actually rendering  >>>>>>>>>>>>>>>>>>>>>>*/
   /* This code only counts textures to be created , they are actually loaded on the end of this function >>>>>>>>>>>>>>>*/
   if (ManageCreatingTextures(1)>0)  { frame.currently_loading=1; } else
                                     { frame.currently_loading=0; }
   /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */



    /* OPEN GL DRAWING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glPushMatrix();
       glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

          glRotatef(frame.angle_x,-1.0,0,0); // Peristrofi gyrw apo ton x
          glRotatef(frame.angle_y,0,-1.0,0); // Peristrofi gyrw apo ton y
          glRotatef(frame.angle_z,0,0,-1.0);
          glTranslatef(-frame.vx, -frame.vy, -frame.vz);

             DrawBackground();

             MainDisplayFunction();

             DrawEffects();

          glTranslatef(frame.vx,frame.vy,frame.vz);
       glPopMatrix();
     /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */




   /* DRAW APPLICATION HUD */
       if ( frame.distance_barrier_after_considered_close<frame.desired_z )
        {
          /* Display HUD only if not zoomed */
          DisplayHUD();
        }
   /* -  -  -  -  -  -  -  */




    glutSwapBuffers();
   glFlush();


   AutomaticSlideShowControl_if_needed(); /* if automatic slide show is enabled it will relay commands */

   /*  THIS COMMAND MOVES THE CAMERA ACCORDING TO THE USER/COMPUTER INPUT*/
   PerformCameraMovement(time_passed_microseconds);


   /* Texture binding via OpenGL , it can only be done in this thread , while not rendering  >>>>>>>>>>>>>>>>>>>>>>*/
   ManageCreatingTextures(0);
   /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */


  /* Changes on display behaviour  >>>>>>>>>>>>>>>>>>>>>>*/
  if ( !framerate_limiter() ) /* helps smoothing out framerate */
    {
      usleep(10); /*Some dead time */
    }

}


// Method to handle the mouse motion
void MotionCallback(int x, int y)
{
    /* The command is handled in controls.cpp / controls.h */
   if ( Controls_Handle_MouseMotion(666,666,x,y) == 1 )
    {
      glutPostRedisplay();
    }
}



// Method to handle the mouse buttons
void MouseCallback( int button,int state, int x, int y)
{
  int res=0;
    /* The command is handled in controls.cpp / controls.h */
  if (state== GLUT_UP)   { res=Controls_Handle_MouseButtons(button,2,x,y); } else
  if (state== GLUT_DOWN) { res=Controls_Handle_MouseButtons(button,1,x,y); } else
                         { res=Controls_Handle_MouseButtons(button,0,x,y); }

  glutPostRedisplay();
}


static void KeyCallback(unsigned char key, int x, int y)
{ //'q'
  if (key==27) exit(0); /* Closes Application on Escape Key*/
        else
  if (key=='j') ToggleFullscreen();  /* Toggles Fullscreen "window" */

  /* The rest commands are handled in controls.cpp / controls.h */
  unsigned int nokey = Controls_Handle_Keyboard(key,x,y);
  if ( nokey == 0 )
  {
     key=0;
     fprintf(stderr,"X:%f Y:%f Z:%f \n",frame.vx,frame.vy,frame.vz);
     glutPostRedisplay();
  }
}




// Method to handle the keyboard's special buttons
void SpecialFunctionCallback (int key, int x, int y)
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


static void IdleCallback(void)
{
    glutPostRedisplay();
}


void InitGlut()
{

    //glutTimerFunc(20, timerCB, 20); // draw every 50 ms
    glutReshapeFunc(ResizeCallback);
    glutMouseFunc (MouseCallback);
    glutPassiveMotionFunc(MotionCallback);
    glutDisplayFunc(DisplayCallback);
    glutKeyboardFunc(KeyCallback);
    glutSpecialFunc(SpecialFunctionCallback);
    glutIdleFunc(IdleCallback);

}

void ToggleFullscreen()
{
   fprintf(stderr,"Fullscreen toggling not implemented , skipping command for safety \n");
   return;
   if ( frame.fullscreen == 0 )
    {
      if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))  glutEnterGameMode(); else
                                                     {
                                                      fprintf(stderr,"Cannot enter fullscreen\n");
                                                      return;
                                                     }

      glutGameModeString("1920x1080:32");
      //glutGameModeString("1024x768:32");
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
    strcpy((char * ) frame.album_directory,"album");
    if ( argc <1 )
     {
       fprintf(stderr,"Something weird is happening , argument zero should be executable path :S \n");
       return 1;
     } else
    if ( argc == 1 )
     {
       fprintf(stderr,"Usage flashyslideshow \"path\\to\\directory\\of\\album\"\n\n");
       strncpy((char * ) frame.program_data_directory,argv[0],MAX_PATH);
     } else
     {
        int i=0;
        for (i=0; i<argc; i++)
         {
             fprintf(stderr,"Arg %u - %s\n",i,argv[i]);
         }
       strncpy((char * ) frame.program_data_directory,argv[0],MAX_PATH);
       strncpy((char * ) frame.album_directory,argv[1],MAX_PATH);

       if ( strncmp((char*)frame.album_directory,"pick",4)==0 )
         {
           /* PICK ALBUM DIR VIA GUI */
           if ( LaunchGUI_PickDir() == 0 )  { fprintf(stderr,"No dir selected! \n"); return 0; }
         }
     }
    strcat((char * ) frame.album_directory,"/");
    fprintf(stderr,"Active directory is %s \n", frame.album_directory);




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
   glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
     glEnable(GL_COLOR_MATERIAL);

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

  //  glClearColor(0.5f,0.5f,0.5f,1.0f);			// We'll Clear To The Color Of The Fog ( Modified )

    glFogi(GL_FOG_MODE, fogMode[fogfilter]);		// Fog Mode
    glFogfv(GL_FOG_COLOR, fogColor);			// Set Fog Color
    glFogf(GL_FOG_DENSITY, 0.35f);				// How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_DONT_CARE);			// Fog Hint Value
    glFogf(GL_FOG_START, 1.0f);				// Fog Start Depth
    glFogf(GL_FOG_END, 5.0f);				// Fog End Depth
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */



    /* Initialize Slideshow variables */
    InitSlideShow();


    /* Initialize WxWidgets */
    WxWidgetsContext wxlibstuff;
    wxlibstuff.OnInit();


    LoadStockTexturesAndSounds();




    CountPicturesInDirectory((char*)frame.album_directory);
    fprintf(stderr,"Album directory has %u pictures inside \n",GetTotalViewableFilesInDirectory());
    GetDirectoryList((char*)frame.album_directory,GetTotalViewableFilesInDirectory()); /* Load Directory List */

    frame.total_images=GetTotalViewableFilesInDirectory();
    unsigned int i=0;  for (i=0; i<frame.total_images; i++) { album[i]=loading; }



    /* Initialize Joystick Thread (if a joystick is connected and detected )*/
    StartJoystickControl();

    /* Initialize Picture Loading Thread */
    loadpicturesthread_id=0;
    pthread_create( &loadpicturesthread_id, NULL,ManageLoadingPicturesMemory_Thread,0);


    gettimeofday(&last_frame,0x0);
    /* Start Rendering */
    glutMainLoop();

    StopJoystickControl();
    wxlibstuff.OnClose();
    return EXIT_SUCCESS;
}
