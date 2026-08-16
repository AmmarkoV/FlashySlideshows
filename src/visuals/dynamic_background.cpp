/** @file dynamic_background.cpp
 *  @brief see dynamic_background.h
 *  @author Ammar Qammaz (AmmarkoV)
 */

/* GLEW has to come before any other GL header , it refuses to load after gl.h */
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

#include "dynamic_background.h"
#include "shadertoy.h"
#include "background.h"
#include "../slideshow.h"
#include "../tools/environment.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

#define MAX_DYNAMIC_BACKGROUNDS 32
#define DYNAMIC_BACKGROUND_PREFIX "background_"

struct dynamicBackground
{
  struct shadertoyEffect * fx;
  char name[64];
};

static struct dynamicBackground backgrounds[MAX_DYNAMIC_BACKGROUNDS];
static unsigned int numberOfBackgrounds=0;

/* 0 = the old static picture background , 1..numberOfBackgrounds = a shader */
static unsigned int selectedBackground=0;

static unsigned int lastDrawTickCount=0;

/* An animated background needs a continuous redraw , but -alwaysRedraw is also a
   switch the user can ask for on its own , so remember what they wanted and go back
   to it instead of switching it off on their behalf */
static unsigned int alwaysRedrawWasRequested=0;


/* "some/path/background_aurora.frag" -> "aurora" */
static void NameFromShaderPath(const char * path,char * name,unsigned int nameSize)
{
  const char * base = strrchr(path,'/');
  base = (base!=0) ? base+1 : path;
  snprintf(name,nameSize,"%s",base+strlen(DYNAMIC_BACKGROUND_PREFIX));
  char * dot = strrchr(name,'.');
  if (dot!=0) { *dot=0; }
}

/* The shaders live next to app_clipart , which is either in the directory the binary
   was started from ( a build tree ) or under /usr/share ( an installation ). The
   binary itself lands in src/ so the development case has to look one level up too. */
static int FindShadersDirectory(char * directory,unsigned int directorySize)
{
  const char * candidates[] = { "shaders",
                                "../shaders",
                                "/usr/share/flashyslideshows/shaders",
                                0 };
  unsigned int i=0;
  for (i=0; candidates[i]!=0; i++)
   {
     char probe[MAX_PATH+64]={0};
     snprintf(probe,MAX_PATH+64,"%s/background_aurora.frag",candidates[i]);
     if (FileExists(probe))
      {
        snprintf(directory,directorySize,"%s",candidates[i]);
        return 1;
      }
   }
  return 0;
}


unsigned int InitDynamicBackgrounds()
{
  numberOfBackgrounds=0;
  selectedBackground=0;
  alwaysRedrawWasRequested=frame.alwaysRedraw;

  if (!shadertoyAvailiable) { return 0; }

  char directory[MAX_PATH]={0};
  if (!FindShadersDirectory(directory,MAX_PATH))
   {
     fprintf(stderr,"Unable to locate a shaders/ directory , dynamic backgrounds are off\n");
     return 0;
   }

  char pattern[MAX_PATH+64]={0};
  snprintf(pattern,MAX_PATH+64,"%s/%s*.frag",directory,DYNAMIC_BACKGROUND_PREFIX);

  glob_t files;
  memset(&files,0,sizeof(files));
  if (glob(pattern,0,0,&files)!=0) { return 0; }

  unsigned int i=0;
  for (i=0; (i<files.gl_pathc) && (numberOfBackgrounds<MAX_DYNAMIC_BACKGROUNDS); i++)
   {
     struct shadertoyEffect * fx = shadertoy_load(files.gl_pathv[i]);
     if (fx!=0)
      {
        backgrounds[numberOfBackgrounds].fx = fx;
        NameFromShaderPath(files.gl_pathv[i],backgrounds[numberOfBackgrounds].name,64);
        ++numberOfBackgrounds;
      }
   }
  globfree(&files);

  fprintf(stderr,"Dynamic backgrounds availiable :");
  for (i=0; i<numberOfBackgrounds; i++) { fprintf(stderr," %s",backgrounds[i].name); }
  fprintf(stderr,"\n");

  return numberOfBackgrounds;
}


unsigned int GetNumberOfDynamicBackgrounds()
{
  return numberOfBackgrounds;
}

const char * GetDynamicBackgroundName(unsigned int index)
{
  if (index>=numberOfBackgrounds) { return ""; }
  return backgrounds[index].name;
}


int SelectDynamicBackgroundByName(const char * name)
{
  if (name==0) { return 0; }

  unsigned int i=0;
  for (i=0; i<numberOfBackgrounds; i++)
   {
     if (strcmp(backgrounds[i].name,name)==0)
      {
        selectedBackground=i+1;
        /* A shader background is a moving picture , the event driven redraw would
           freeze it on whatever frame it stopped at */
        frame.alwaysRedraw=1;
        fprintf(stderr,"Using the %s dynamic background\n",name);
        return 1;
      }
   }

  fprintf(stderr,"There is no %s background , availiable ones are :",name);
  for (i=0; i<numberOfBackgrounds; i++) { fprintf(stderr," %s",backgrounds[i].name); }
  fprintf(stderr,"\n");
  return 0;
}


void CycleDynamicBackground()
{
  if (numberOfBackgrounds==0) { return; }

  selectedBackground = (selectedBackground+1) % (numberOfBackgrounds+1);

  if (selectedBackground==0)
   {
     frame.alwaysRedraw=alwaysRedrawWasRequested;
     fprintf(stderr,"Back to the static picture background\n");
   } else
   {
     frame.alwaysRedraw=1;
     fprintf(stderr,"Using the %s dynamic background\n",backgrounds[selectedBackground-1].name);
   }
  frame.forceDrawOneMoreTime=1;
}


unsigned int DynamicBackgroundIsActive()
{
  return ( (numberOfBackgrounds>0) && (selectedBackground>0) );
}

const char * GetActiveDynamicBackgroundName()
{
  if (!DynamicBackgroundIsActive()) { return ""; }
  return backgrounds[selectedBackground-1].name;
}


int DrawDynamicBackground()
{
  if (!DynamicBackgroundIsActive()) { return 0; }

  struct shadertoyEffect * fx = backgrounds[selectedBackground-1].fx;
  if (fx==0) { return 0; }

  float width  = (float) glutGet(GLUT_WINDOW_WIDTH);
  float height = (float) glutGet(GLUT_WINDOW_HEIGHT);
  if ( (width<1) || (height<1) ) { return 0; }

  float timeNow   = (float) frame.tick_count / 1000;
  float timeDelta = (float) (frame.tick_count - lastDrawTickCount) / 1000;
  lastDrawTickCount = frame.tick_count;
  if ( (timeDelta<0.0) || (timeDelta>1.0) ) { timeDelta=1.0/60; } /* first frame , or a long stall */

  shadertoy_draw(fx,
                 timeNow,timeDelta,(int) times_drawn_background,
                 width,height,
                 frame.mouse.mouse_x,frame.mouse.mouse_y,
                 0,0,
                 0.0);

  ++times_drawn_background;
  return 1;
}


void CloseDynamicBackgrounds()
{
  unsigned int i=0;
  for (i=0; i<numberOfBackgrounds; i++)
   {
     shadertoy_unload(backgrounds[i].fx);
     backgrounds[i].fx=0;
   }
  numberOfBackgrounds=0;
  selectedBackground=0;
}
