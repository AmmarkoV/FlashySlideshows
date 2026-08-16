/** @file shader_transitions.cpp
 *  @brief see shader_transitions.h
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

#include "shader_transitions.h"
#include "../visuals/shadertoy.h"
#include "../slideshow.h"
#include "../hypervisor/load_textures.h"

#include <stdio.h>
#include <string.h>

#define SHADER_TRANSITION_PREFIX "transition_"

/* How long one lasts. Kept well under the slide duration below , a transition that is
   still running when the next slide is due looks like a stutter rather than an effect */
#define SHADER_TRANSITION_MS 900

/* The transition mode that turns these on , see ToggleTransitionMode in slideshow.cpp */
#define TRANSITION_MODE_SHADER 3

static struct shadertoyEffectList transitions;
static unsigned int selectedTransition=0;

static int    transitionFromPlace=-1;
static int    transitionToPlace=-1;
static unsigned int transitionStartedAtTick=0;
static unsigned int transitionDurationMs=SHADER_TRANSITION_MS;
static unsigned int transitionPlaying=0;

static unsigned int lastDrawTickCount=0;
static unsigned int transitionFrameNumber=0;


unsigned int InitShaderTransitions()
{
  selectedTransition=0;
  transitionPlaying=0;

  if (!shadertoy_loadEffectDirectory(SHADER_TRANSITION_PREFIX,&transitions)) { return 0; }

  fprintf(stderr,"Shader transitions availiable :");
  shadertoy_printEffectNames(&transitions);
  fprintf(stderr,"\n");

  return transitions.count;
}

unsigned int GetNumberOfShaderTransitions()
{
  return transitions.count;
}

int SelectShaderTransitionByName(const char * name)
{
  int found = shadertoy_findEffectByName(&transitions,name);
  if (found<0)
   {
     fprintf(stderr,"There is no %s transition , availiable ones are :",(name!=0)?name:"");
     shadertoy_printEffectNames(&transitions);
     fprintf(stderr,"\n");
     return 0;
   }

  selectedTransition=(unsigned int) found;
  fprintf(stderr,"Using the %s shader transition\n",transitions.entry[selectedTransition].name);
  return 1;
}

void CycleShaderTransition()
{
  if (transitions.count==0) { return; }
  selectedTransition = (selectedTransition+1) % transitions.count;
  fprintf(stderr,"Using the %s shader transition\n",transitions.entry[selectedTransition].name);
  frame.forceDrawOneMoreTime=1;
}

const char * GetActiveShaderTransitionName()
{
  if (transitions.count==0) { return ""; }
  return transitions.entry[selectedTransition].name;
}


void StartShaderTransition(int fromPlace,int toPlace)
{
  if (transitions.count==0) { return; }
  if (frame.transitions.transition_mode!=TRANSITION_MODE_SHADER) { return; }
  if (fromPlace==toPlace) { return; }
  if (fromPlace<0)        { return; }

  /* A transition that outlives its slide would still be running when the next one is
     due , so it never takes more than a third of the time a slide is on screen */
  transitionDurationMs = SHADER_TRANSITION_MS;
  if (frame.transitions.automatic_slideshow_on)
   {
     unsigned int budget = frame.time_ms_between_two_transitions / 3;
     if (transitionDurationMs>budget) { transitionDurationMs=budget; }
   }
  if (transitionDurationMs<50) { return; } /* not worth drawing */

  transitionFromPlace     = fromPlace;
  transitionToPlace       = toPlace;
  transitionStartedAtTick = frame.tick_count;
  transitionPlaying       = 1;
  frame.forceDrawOneMoreTime = 1;
}

unsigned int ShaderTransitionIsPlaying()
{
  return transitionPlaying;
}


int DrawShaderTransition()
{
  if (transitions.count==0) { return 0; }
  if (frame.transitions.transition_mode!=TRANSITION_MODE_SHADER) { transitionPlaying=0; return 0; }

  struct shadertoyEffect * fx = transitions.entry[selectedTransition].fx;
  if (fx==0) { transitionPlaying=0; return 0; }

  /* Transition mode 3 draws this pass on every frame , not only while an effect is
     playing , so the two glutGet calls that used to be here were an X server round
     trip per frame. ResizeCallback maintains frame.viewportWidth/Height instead. */
  float width  = (float) frame.viewportWidth;
  float height = (float) frame.viewportHeight;
  if ( (width<1) || (height<1) ) { return 0; }

  float progress = 1.0;
  if (transitionPlaying)
   {
     progress = (float) (frame.tick_count - transitionStartedAtTick) / (float) transitionDurationMs;
     /* An unsigned wrap of the tick counter comes out enormous rather than negative ,
        either way there is nothing sensible left to interpolate */
     if ( (progress<0.0) || (progress>=1.0) ) { progress=1.0; transitionPlaying=0; }
   }

  struct shadertoyInputs inputs;
  memset(&inputs,0,sizeof(inputs));

  /* Between transitions the two channels are the same picture and progress sits at 1 ,
     so whichever effect is selected simply shows the photo we are on. Mode 3 is a
     presentation : the photo is on screen full size the whole time , and the effects
     are what carries it from one to the next , instead of the effect appearing out of
     nowhere over the album view and vanishing again when it finishes. */
  int fromPlace = (transitionPlaying) ? transitionFromPlace : (int) frame.active_image_place;
  int toPlace   = (transitionPlaying) ? transitionToPlace   : (int) frame.active_image_place;

  inputs.channel[0] = GetPictureTextureForSampling(fromPlace,&inputs.channelWidth[0],&inputs.channelHeight[0]);
  inputs.channel[1] = GetPictureTextureForSampling(toPlace  ,&inputs.channelWidth[1],&inputs.channelHeight[1]);

  /* The photo we are arriving at is the one that has to be there. Without it there is
     nothing to present and the album view underneath is the better thing to show. */
  if (inputs.channel[1]==0) { transitionPlaying=0; return 0; }
  /* The one we are leaving may have been unloaded mid transition , in which case the
     effect finishes early rather than fading out of a black rectangle */
  if (inputs.channel[0]==0) { inputs.channel[0]=inputs.channel[1];
                              inputs.channelWidth[0]=inputs.channelWidth[1];
                              inputs.channelHeight[0]=inputs.channelHeight[1];
                              progress=1.0; transitionPlaying=0; }

  inputs.time      = (float) frame.tick_count / 1000;
  inputs.timeDelta = (float) (frame.tick_count - lastDrawTickCount) / 1000;
  lastDrawTickCount = frame.tick_count;
  if ( (inputs.timeDelta<0.0) || (inputs.timeDelta>1.0) ) { inputs.timeDelta=1.0/60; }

  inputs.frameNumber = (int) transitionFrameNumber++;
  inputs.width    = width;
  inputs.height   = height;
  inputs.mouseX   = frame.mouse.mouse_x;
  inputs.mouseY   = frame.mouse.mouse_y;
  inputs.progress = progress;
  /* Everything outside the photo is left transparent , so the animated background of
     visuals/dynamic_background.cpp keeps showing in the letterbox bars */
  inputs.useAlpha = 1;

  shadertoy_draw(fx,&inputs);

  /* Keep the redraws coming until it has played out , the economic mode would
     otherwise leave the effect frozen halfway through */
  if (transitionPlaying) { frame.forceDrawOneMoreTime = 1; }
  return 1;
}


void CloseShaderTransitions()
{
  shadertoy_unloadEffectList(&transitions);
  selectedTransition=0;
  transitionPlaying=0;
}
