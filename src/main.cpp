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
#include <unistd.h>
#include <pthread.h>
#include "slideshow.h"
#include "visuals.h"
#include "directory_listing.h"
#include "camera_control.h"
#include "pictures_control.h"
#include "controls.h"
#include "load_images.h"
#include "load_textures.h"
#include "sound.h"
#include "memory_hypervisor.h"
#include "wxwidgets_stuff.h"
#include "joystick.h"
#include "environment.h"
#include "scene_objects.h"
#include "version.h"

#include "webinterface.h"
#include "layouts/layout_handler.h"
#include "visuals/hud.h"

#include <unistd.h>

#include <sys/time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif

#include <stdlib.h>



unsigned int STOP_APPLICATION=0;
unsigned int STOP_IDLE_CALLBACK=0;

struct timeval last_frame,this_frame,difference;
int originalWindow = 0, currentWindow;

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
    MasterMemoryStrategist();
    loaded_pictures_this_loop+=ExecuteMemoryStrategyPlanOnSystemMemory();
/*
    if ( loaded_pictures_this_loop == 0 ) { usleep(100000);  } else
                                          { usleep(10000);  }*/
    usleep(20000); // 20 ms
  }
  return 0;
}


int ManageCreatingTexturesMemory_OpenGLThread(int count_only)
{
  /* THIS FUNCITON BELONGS TO THE OPENGL THREAD AND LOADS/UNLOADS IMAGES
     FROM THE GPU AS TEXTURES!!!! THEY HAVE TO BE LOADED BY ManageLoadingPicturesMemory_Thread
  */
  return LoadTexturesIfNeeded(count_only);
}



/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            FRAME RATE LIMITER
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void timerCB(int millisec)
{
 glutPostWindowRedisplay(currentWindow);
 glutTimerFunc(millisec, timerCB, millisec);
}


int framerate_limiter()
{
 if ( frame.fps <= 50 ) { return 0; } else
 if ( frame.fps <= 100 ) {  usleep (500); return 0; } else
 if ( frame.fps <= 200 ) {  usleep (1000); return 0; } else
 if ( frame.fps <= 300 ) {  usleep (2000);return 0; } else
    {
      usleep (2000); //1ms flat sleep time for the card
    }
 return 1;

 /* THIS SLEEPING BEHAVIOUR CAUSES INCONSISTEN
  if ( frame.fps > 40 )
   {
     unsigned int frames_to_cut = frame.fps - 40;
     unsigned int time_to_cut = (unsigned int) 1000000/frames_to_cut;
     usleep (time_to_cut);
   }

  return 1;*/
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
    if (time_passed_microseconds>1000000)
     {
      fprintf(stderr,"FRAME DELAYED %u msecs\n",time_passed_microseconds/1000);
      fprintf(stderr,"to keep things from freaking out we will pretend We were paused and only 1 ms passed\n");
      time_passed_microseconds=1000;
     }
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
    /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
   /* Texture binding via OpenGL , it can only be done in this thread , before actually rendering  >>>>>>>>>>>>>>>>>>>>>>*/
   /* This code only counts textures to be created , they are actually loaded on the end of this function >>>>>>>>>>>>>>>*/

   /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */



    /* OPEN GL DRAWING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); //| GL_DEPTH_BUFFER_BIT
	  glPushMatrix();
       //glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

          glRotatef(frame.angle_x,-1.0,0,0); // Peristrofi gyrw apo ton x
          glRotatef(frame.angle_y,0,-1.0,0); // Peristrofi gyrw apo ton y
          glRotatef(frame.angle_z,0,0,-1.0);
          glTranslatef(-frame.vx, -frame.vy, -frame.vz);

             MainDisplayFunction();

             Render_3DObjects();

             DrawEffects();

          glTranslatef(frame.vx,frame.vy,frame.vz);
       glPopMatrix();
     /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */




   /* DRAW APPLICATION HUD */
        if (frame.show_time>0)
        {
           unsigned int diff_ms=time_passed_microseconds/1000;
           if (frame.show_time>diff_ms) { frame.show_time-=diff_ms; } else
                                        { frame.show_time=0; }
          DisplayHUD(2);
        }else
        if (frame.show_information>0)
        {
           unsigned int diff_ms=time_passed_microseconds/1000;
           if (frame.show_information>diff_ms) { frame.show_information-=diff_ms; } else
                                               { frame.show_information=0; }
          DisplayHUD(1);
        }else
        if ( frame.distance_barrier_after_considered_close<frame.desired_z )
        {
          /* Display HUD only if not zoomed */
          DisplayHUD(0);
        }
   /* -  -  -  -  -  -  -  */
   glFinish(); // Thats all the drawing


   /*STATE MANAGMENT ----------------------------------------------------*/
   AutomaticSlideShowControl_if_needed(); /* if automatic slide show is enabled it will relay commands */

   /*  THIS COMMAND MOVES THE CAMERA ACCORDING TO THE USER/COMPUTER INPUT*/
   PerformCameraMovement(time_passed_microseconds);
   PerformPicturesMovement(time_passed_microseconds);

   Run3DObjects(time_passed_microseconds);


   /* Texture binding via OpenGL , it can only be done in this thread , while not rendering
        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
   ManageCreatingTexturesMemory_OpenGLThread(0);
   /*   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */




  /* Changes on display behaviour  >>>>>>>>>>>>>>>>>>>>>>*/
  if ( !framerate_limiter() ) /* helps smoothing out framerate */
    {
      usleep(10); /*Some dead time */
    }


    glutSwapBuffers();
   // glFlush();
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
 // int res=0;
    /* The command is handled in controls.cpp / controls.h */
  if (state== GLUT_UP)   { /*res=*/Controls_Handle_MouseButtons(button,2,x,y); } else
  if (state== GLUT_DOWN) { /*res=*/Controls_Handle_MouseButtons(button,1,x,y); } else
                         { /*res=*/Controls_Handle_MouseButtons(button,0,x,y); }

  glutPostRedisplay();
}


static void KeyCallback(unsigned char key, int x, int y)
{ //'q'
  if (key==27) {  glutLeaveMainLoop(); /*EnableScreenSaver(); exit(0);*/ } else/* Closes Application on Escape Key*/
  if (key=='j') ToggleFullscreen();  /* Toggles Fullscreen "window" */

  /* The rest commands are handled in controls.cpp / controls.h */
  unsigned int nokey = Controls_Handle_Keyboard(key,x,y);
  if ( nokey == 0 )
  {
     key=0;
     if (PrintDevMsg()) fprintf(stderr,"X:%f Y:%f Z:%f \n",frame.vx,frame.vy,frame.vz);
     usleep(100);
     glutPostRedisplay();
  }
}




// Method to handle the keyboard's special buttons
void SpecialFunctionCallback (int key, int x, int y)
{
    int nokey=0;
	switch (key)
	{
		case GLUT_KEY_UP:           Controls_Handle_Keyboard(1,x,y); break;
		case GLUT_KEY_DOWN:         Controls_Handle_Keyboard(2,x,y); break;
		case GLUT_KEY_RIGHT:        Controls_Handle_Keyboard(4,x,y); break;
		case GLUT_KEY_LEFT:         Controls_Handle_Keyboard(3,x,y); break;
		case GLUT_KEY_PAGE_UP:      Controls_Handle_Keyboard(153,x,y); break;
		case GLUT_KEY_PAGE_DOWN:    Controls_Handle_Keyboard(161,x,y); break;
		case GLUT_KEY_INSERT:       UploadCurrentPhotoToMyloader(); break;
		case GLUT_KEY_F1:    break;
		case GLUT_KEY_F2:    break;
		case GLUT_KEY_F3:    break;
		case GLUT_KEY_F4:   break;
		case GLUT_KEY_F5:    break;
		case GLUT_KEY_F6:    break;
		case GLUT_KEY_F7:    break;
		case GLUT_KEY_F8:   break;
		case GLUT_KEY_F9:    break;
		case GLUT_KEY_F10:    break;
		case GLUT_KEY_F11:    break;
		case GLUT_KEY_F12:    break;
		case GLUT_KEY_HOME: { SetDestinationOverPicture(0); break; }
		case GLUT_KEY_END:  { SetDestinationOverPicture(frame.total_images-1); break; }
        default: nokey=1; break;
	}

    if ( nokey == 0 )
    {
      key=0; glutPostRedisplay();
    }
}


static void IdleCallback(void)
{
  //  if ( STOP_IDLE_CALLBACK>0 ) { STOP_IDLE_CALLBACK=2; return; }
    glutPostWindowRedisplay(currentWindow);
}

static void IdleCallbackFS(void)
{
  //  if ( STOP_IDLE_CALLBACK>0 ) { STOP_IDLE_CALLBACK=2; return; }
    glutPostRedisplay();
}


static void VisibilityCallback(int vis)
{
  //if (vis == GLUT_VISIBLE) { glutIdleFunc(IdleCallback); }
  //                  else { glutIdleFunc(0);}
}





void InitGlut()
{
    //glutTimerFunc(20, timerCB, 20); // draw every 50 ms
    glutReshapeFunc(ResizeCallback);
    glutMouseFunc (MouseCallback);
    glutMotionFunc(MotionCallback); // This is needed for drag&drop functionality xD
    glutPassiveMotionFunc(MotionCallback);
    glutVisibilityFunc(VisibilityCallback);
    glutDisplayFunc(DisplayCallback);
    glutKeyboardFunc(KeyCallback);
    glutSpecialFunc(SpecialFunctionCallback);
    glutIdleFunc(IdleCallback);

    currentWindow = glutGetWindow();

}

unsigned int WaitForVariableToTakeValue(unsigned int *var,unsigned int value , unsigned int maxloops)
{
  unsigned int loops=0;
  while ( ( *var!=value ) && ( loops<maxloops) )
       {
           usleep(100);
           ++loops;
       }
  if (*var!=value) { return 0; }
  return 1;
}


void ToggleFullscreen()
{
   //fprintf(stderr,"Fullscreen toggling not implemented , skipping command for safety \n");
   //return;

   if ( frame.fullscreen == 0 )
    {
      STOP_IDLE_CALLBACK=1;
      int width_x=glutGet(GLUT_SCREEN_WIDTH);
      int width_y=glutGet(GLUT_SCREEN_HEIGHT);
      char mode_string[128]={0};
      sprintf(mode_string,"%ux%u:32",width_x,width_y);
      fprintf(stderr,"Attempting fullscreen %s\n",mode_string);
      glutGameModeString(mode_string);
      if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))  {
                                                        glutEnterGameMode();
                                                     } else
                                                     {
                                                       fprintf(stderr,"Cannot enter fullscreen\n");
                                                       STOP_IDLE_CALLBACK=0;
                                                       return;
                                                     }

      gettimeofday(&last_frame,0x0); // Avoid the unaccounted time passed make movement glitchy
      InitGlut();
      glutIdleFunc(IdleCallbackFS);
      frame.fullscreen=1;
      STOP_IDLE_CALLBACK=0;
      glutMainLoop();
    } else
   if ( frame.fullscreen == 1 )
    {
      if (originalWindow != 0 && currentWindow != originalWindow)
      {
       glutIdleFunc(0);
       STOP_IDLE_CALLBACK=1;
       WaitForVariableToTakeValue(&STOP_IDLE_CALLBACK,2,1000);
       glutLeaveGameMode();
       frame.fullscreen=0;
       STOP_IDLE_CALLBACK=0;
       gettimeofday(&last_frame,0x0); // Avoid the unaccounted time passed make movement glitchy
       glutIdleFunc(IdleCallback);
       currentWindow = originalWindow;
      }

    }


   	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) == 0) fprintf(stderr,"Current Mode: Window"); else
		                                             fprintf(stderr,"Current Mode: Game Mode %dx%d at %d hertz, %d bpp",glutGameModeGet(GLUT_GAME_MODE_WIDTH),
			                                                                                                            glutGameModeGet(GLUT_GAME_MODE_HEIGHT),
			                                                                                                            glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE),
			                                                                                                            glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH)   );



}




/* Program entry point */

int main(int argc, char *argv[])
{
    /* Initialize Slideshow variables before the arguments are parsed..*/
    InitSlideShow();
    DisableScreenSaver();


    strcpy((char * ) frame.album_directory,"album"); // album or test_album for debugging..
    strcpy((char * ) frame.copy_directory,"Copied");
    strcpy((char * ) frame.move_directory,"Moved");
    strcpy((char * ) frame.resize_directory,"Resized");

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
             if (strcmp(argv[i],"-web")==0)
                   { //Recursive Directory command
                       fprintf(stderr,"Web Interface Enabled %u - %s\n",i,argv[i]);
                       if (!WebInterfaceCompiledIn())
                          {
                            //Web interface has been enabled by user BUT its not compiled in! :S
                            EmmitDialogWarning( (char *) "Please note that this build of FlashySlideshow hasn't got a compiled in WebInterface..!\n To download it run ./update_from_git.sh from the project's root dir..\n\n\nCheck here for more info :\nhttps://github.com/AmmarkoV/FlashySlideshows\n");
                          }
                       frame.enable_web_interface=1;
                   } else
             if (strcmp(argv[i],"-r")==0)
                   { //Recursive Directory command
                       fprintf(stderr,"Recursive Directory Enabled %u - %s\n",i,argv[i]);
                       frame.recursive=1;
                   } else
             if (strcmp(argv[i],"-play")==0)
                   { //Recursive Directory command
                       fprintf(stderr,"AutoStarting Playback %u - %s\n",i,argv[i]);
                       ToggleAutomaticSlideshow();
                   } else
             if (strcmp(argv[i],"-sfx")==0)
                   { //Sound Effects command
                       fprintf(stderr,"%u Sound Effects Enabled %s\n",i,argv[i]);
                       frame.enable_sound_effects=1;
                   } else
             if (strcmp(argv[i],"-file_move")==0)
                   { //Move Files command
                       fprintf(stderr,"%u Move Sorting Enabled with keys 0 to 9 %s\n",i,argv[i]);
                       frame.allow_operation_move=1;
                   } else
             if (strcmp(argv[i],"-file_resize")==0)
                   { //Resize Files command
                        if (i+1<=argc)
                        {
                         fprintf(stderr,"%u Move Resized Enabled at Resolution %s with keys 0 to 9 %s\n",i,argv[i+1],argv[i]);
                         frame.allow_operation_resize=1;
                         strcpy(frame.rescale_resolution_string,argv[i+1]);
                        }
                   } else
             if (strcmp(argv[i],"-no_hud")==0)
                   { //File Copy command
                       fprintf(stderr,"Hud Disabled..");
                       frame.disable_hud=1;
                   } else
             if (strcmp(argv[i],"-no_background")==0)
                   { //File Copy command
                       fprintf(stderr,"Background Disabled..");
                       frame.plain_background_no_image=1;
                   } else
             if (strcmp(argv[i],"-file_copy")==0)
                   { //File Copy command
                       fprintf(stderr,"%u Move Sorting Enabled with keys 0 to 9 %s\n",i,argv[i]);
                       frame.allow_operation_copy=1;
                   } else
             if (strcmp(argv[i],"-file_link")==0)
                   { //File Copy command
                       fprintf(stderr,"%u Link Sorting Enabled with keys 0 to 9 %s\n",i,argv[i]);
                       frame.allow_operation_link=1;
                   } else
             if (strcmp(argv[i],"-fd")==0)
                   { //Face Detection command
                       fprintf(stderr,"%u Face Detection Enabled %s\n",i,argv[i]);
                   } else
             if (strcmp(argv[i],"-v")==0)
                   { //Visuals Enabled command
                       fprintf(stderr,"%u Visuals Enabled %s\n",i,argv[i]);
                   } else
             if (strcmp(argv[i],"-m")==0)
                   { //Mipmaping Enabled command
                       fprintf(stderr,"%u Mipmaping Enabled %s\n",i,argv[i]);
                       frame.mipmaping=1;
                   } else
             if (strcmp(argv[i],"-q")==0)
                   { //Quality command
                      if (i+1<=argc)
                      {
                       frame.quality_setting=atoi(argv[i+1]); // Quality
                       fprintf(stderr,"%u Image Quality %s = %s ( %u )\n",i,argv[i],argv[i+1],frame.quality_setting);
                       if (frame.quality_setting>=3) { frame.try_for_best_render_quality=1; }
                       fprintf(stderr,"   Rendering Quality Switch %u\n",frame.try_for_best_render_quality);
                       //THIS HAS A SECOND PAYLOAD THAT HAPPENS AFTERWARDS----> to initialize frame.gpu.maximum_frame_total_size
                      }
                   } else
             if (strcmp(argv[i],"-images_per_line")==0)
                   { //Images per line command
                      if (i+1<=argc)
                      {
                        frame.images_per_line=atoi(argv[i+1]); // Background
                        fprintf(stderr,"%u Images per line Manually Chosen %s = %s ( %u )\n",i,argv[i],argv[i+1],frame.images_per_line);
                        frame.active_image_x=((unsigned int) frame.images_per_line/2);
                        frame.active_image_y=1;
                        frame.active_image_place=PictureXYtoID(frame.active_image_x,frame.active_image_y);

                        frame.last_image_x=frame.active_image_x;
                        frame.last_image_y=frame.active_image_y;
                        frame.last_image_place=frame.active_image_place;
                        GetLayoutCoordinatesForXY(frame.active_image_x,frame.active_image_y,&frame.vx,&frame.vy,&frame.vz);
                        frame.vz=8.0;
                        frame.desired_x=frame.vx; frame.desired_y=frame.vy; frame.desired_z=GetLayoutMaximumZ()-0.5;

                        fprintf(stderr,"Setting Center to %u,%u (%u) --> coords %0.2f,%0.2f,%0.2f\n",frame.active_image_x,frame.active_image_y,frame.active_image_place,frame.vx,frame.vy,frame.vz);

                      }
                   } else
             if (strcmp(argv[i],"-b")==0)
                   { //Quality command
                      if (i+1<=argc)
                      {
                        frame.background_number=atoi(argv[i+1]); // Background
                        fprintf(stderr,"%u Frame Background Manually Chosen %s = %s ( %u )\n",i,argv[i],argv[i+1],frame.background_number);
                      }
                   } else
             if (strcmp(argv[i],"-d")==0)
                   { //Transition delay command
                      if (i+1<=argc)
                      {
                       frame.time_ms_between_two_transitions=atoi(argv[i+1]);
                       fprintf(stderr,"%u Transition Delay %s = %s ( %u ) \n",i,argv[i],argv[i+1],frame.time_ms_between_two_transitions);
                      }
                   } else
             if (strcmp(argv[i],"-t")==0)
                   { //Transition Type command
                      if (i+1<=argc)
                      {
                       fprintf(stderr,"%u Transition Type %s=%s\n",i,argv[i],argv[i+1]);
                       if  ( *argv[i+1]=='0' ) { fprintf(stderr,"3D Seek\n");      frame.transitions.transition_mode=0; } else
                       if  ( *argv[i+1]=='1' ) { fprintf(stderr,"Immediate\n");    frame.transitions.transition_mode=1; } else
                       if  ( *argv[i+1]=='2' ) { fprintf(stderr,"Transparency\n"); frame.transitions.transition_mode=2; }  else
                                               { fprintf(stderr,"Incorrect Transition Type..\n"); }
                      }
                   } else
             if (strcmp(argv[i],"-l")==0)
                   { //Layout Type command
                      if (i+1<=argc)
                      {
                       fprintf(stderr,"%u Layout Type %s=%s\n",i,argv[i],argv[i+1]);
                       if  ( *argv[i+1]=='0' ) { fprintf(stderr,"Basic Layout\n");      frame.layout.layout_mode=0; } else
                       if  ( *argv[i+1]=='1' ) { fprintf(stderr,"Expo\n");    frame.layout.layout_mode=1; } else
                                               { fprintf(stderr,"Incorrect Layout Type..\n"); }
                      }
                   } else
             if (strcmp(argv[i],"-s")==0)
                   { //Sorting Type command
                      if (i+1<=argc)
                      {
                       fprintf(stderr,"%u Sorting Type %s=%s\n",i,argv[i],argv[i+1]);
                       if  ( *argv[i+1]=='0' ) { fprintf(stderr,"Ascending Name\n"); frame.sort_type=0; frame.sort_ascending=1; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='1' ) { fprintf(stderr,"Ascending Date\n"); frame.sort_type=1; frame.sort_ascending=1; frame.sort_randomization=0;  } else
                       if  ( *argv[i+1]=='2' ) { fprintf(stderr,"Ascending Size\n"); frame.sort_type=2; frame.sort_ascending=1; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='3' ) { fprintf(stderr,"Descending Name\n"); frame.sort_type=0; frame.sort_ascending=0; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='4' ) { fprintf(stderr,"Descending Date\n"); frame.sort_type=1; frame.sort_ascending=0; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='5' ) { fprintf(stderr,"Descending Size\n"); frame.sort_type=2; frame.sort_ascending=0; frame.sort_randomization=0; } else
                       if  ( *argv[i+1]=='r' ) { fprintf(stderr,"Randomized\n"); frame.sort_randomization=1; }  else
                                               { fprintf(stderr,"Incorrect Sorting Type..\n"); }
                      }
                   }

         }
       strncpy((char * ) frame.program_data_directory,argv[0],MAX_PATH);
       strncpy((char * ) frame.album_directory,argv[argc-1],MAX_PATH);

       if ( strncmp((char*)frame.album_directory,"pick",4)==0 )
         {
           /* PICK ALBUM DIR VIA GUI */
           if ( LaunchGUI_PickDir() == 0 )  { fprintf(stderr,"No dir selected! \n"); return 0; }
         }
     }
    strcat((char * ) frame.album_directory,"/");
    fprintf(stderr,"Slideshow directory is = %s (%u chars) \n", frame.album_directory ,(unsigned int) strlen((char*) frame.album_directory));

    /* GLUT Initialization >>>>>>>>>>>>>>>>>> */
    glutInit(&argc, argv);
    glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE ,GLUT_ACTION_CONTINUE_EXECUTION);

    int width_x=glutGet(GLUT_SCREEN_WIDTH);
    int width_y=glutGet(GLUT_SCREEN_HEIGHT)-50; // 50 for the lxpanel bar :P
    fprintf(stderr,"System Resolution is %ux%u\n",width_x,width_y);
    if (width_x>1024) { width_x=1024; }
    if (width_y>600)  { width_y=600; }
    fprintf(stderr,"Window Resolution is %ux%u\n",width_x,width_y);
    glutInitWindowSize(width_x,width_y);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |  GLUT_ALPHA | GLUT_DEPTH ); // depth buffer and multisampling disabled for older systems..!  |GLUT_MULTISAMPLE | GLUT_DEPTH

    char title[512]={0};
    sprintf(title,"Flashy Slideshows v%s %s - build %u - %s/%s/%s ",AutoVersion::FULLVERSION_STRING,AutoVersion::STATUS,(unsigned int) AutoVersion::BUILDS_COUNT,AutoVersion::DATE,AutoVersion::MONTH,AutoVersion::YEAR);
    originalWindow = glutCreateWindow(title);

    InitGlut();
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */


    /* OpenGL Initialization >>>>>>>>>>>>>>>>> */
    glClearColor(0,0,0,0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer



   if (frame.try_for_best_render_quality)
    {
     glEnable(GL_NORMALIZE);
     glShadeModel(GL_SMOOTH);
     glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
     glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
     glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
     glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    } else
    {
     glShadeModel(GL_FLAT);
    }

   /* LIGHTING , ENABLED ON GOOD RENDERING QUALITY ( TO ENABLE TRANSPARENCY ETC ) */
   if (frame.try_for_best_render_quality)
    {
    frame.lighting_enabled=1;
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    } else
    {
      frame.lighting_enabled=0;
    }
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

   //Now that we have an OpenGL context we can query the maximum texture dimension..
   GLint texSize=0;
   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
   frame.gpu.maximum_frame_dimension_size=(unsigned int) texSize;
   frame.gpu.maximum_frame_total_size=GetWidthQuality(frame.quality_setting)*GetHeightQuality(frame.quality_setting)*4; /*RGBA*/
   frame.gpu.lastTexture=frame.gpu.maximum_frame_total_size;
   frame.system.maximum_frame_total_size=GetWidthQuality(frame.quality_setting)*GetHeightQuality(frame.quality_setting)*3; /*RGB*/
   frame.system.lastTexture=frame.gpu.maximum_frame_total_size;
   //frame.gpu.maximum_frame_total_size*=3; // <- Safety Factor


   fprintf(stderr,"Graphics card vendor : %s\n",(const char *)glGetString(GL_VENDOR));
   fprintf(stderr,"Renderer : %s\n",(const char *)glGetString(GL_RENDERER));
   fprintf(stderr,"Version : %s\n",(const char *)glGetString(GL_VERSION));
   //fprintf(stderr,"Extensions : %s\n",(const char *)glGetString(GL_EXTENSIONS));
   fprintf(stderr,"Maximum Texture Dimension Size is %u\n",(unsigned int) texSize);
   frame.gpu.maximum_frame_dimension_size=(unsigned int) texSize;
   fprintf(stderr,"Texture Sizes set to quality %u = %ux%u\n",frame.quality_setting,GetWidthQuality(frame.quality_setting),GetHeightQuality(frame.quality_setting));




   frame.gpu.maxRAM=GetGraphicsCardMemory();
   fprintf(stderr,"Software GPU texture limit set to %u MB\n",(unsigned int)frame.gpu.maxRAM/(1024*1024));
   frame.system.maxRAM=GetSystemFreeMemory();
   fprintf(stderr,"Software RAM limit set to %u MB\n",(unsigned int)frame.system.maxRAM/(1024*1024));

    /* Initialize WxWidgets */
    WxWidgetsContext wxlibstuff;
    wxlibstuff.OnInit();
    fprintf(stderr,"Done with wxWidgets initialization\n");


    LoadStockTexturesAndSounds();
    InitEffects();

    if (!LoadPicturesOfDirectory((char*)frame.album_directory,frame.sort_type,frame.sort_ascending,frame.sort_randomization,frame.recursive))
      {
          EmmitDialogWarning( (char *) "Could not find any pictures in the directory of your selection");
          return 0;
      }
    frame.total_images=GetTotalViewableFilesInDirectory();
    if (!CreateSlideshowPictureStructure(GetTotalViewableFilesInDirectory()))
      {
          fprintf(stderr,"Unrecoverable error , could not allocate enough memory for %u Picture structure pointers..\nYou may try to re-run when more RAM will be availiable\n",GetTotalViewableFilesInDirectory());
          return 1;
      }

    /*At this point we have initialized the picture structures and the slideshow structures
      so lets make sure that the camera is over an existing picture..! */
    PutCameraOverExistingPictureIfItIsOff();

    /* Initialize Joystick Thread (if a joystick is connected and detected )*/
    StartJoystickControl();

    /* Initialize Picture Loading Thread */
    loadpicturesthread_id=0;
    pthread_create( &loadpicturesthread_id, NULL,ManageLoadingPicturesMemory_Thread,0);

    //This has to happen after LoadStockTexturesAndSounds in order for  frame.app_clipart_dir to get populated
    if (frame.enable_web_interface) { StartWebInterface((char*) "0.0.0.0", 8080 , (char*) frame.album_directory , ( char*) frame.app_clipart_dir); }

    gettimeofday(&last_frame,0x0);
    /* Start Rendering */
    glutMainLoop();
    fprintf(stderr,"Gracefully closing program\n");


    if (frame.enable_web_interface) { StopWebInterface(); }

    EnableScreenSaver();
    StopJoystickControl();
    UnLoadStockTexturesAndSounds();
    DestroySlideshowPictureStructure();
    wxlibstuff.OnClose();

    return EXIT_SUCCESS;
}
