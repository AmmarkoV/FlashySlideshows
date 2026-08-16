/** @file shader_transitions.h
 *  @brief Fragment shader transitions between the photo we are leaving and the photo
 *         we are arriving at , drawn over the whole viewport while a slide change is
 *         in flight.
 *
 *         Selected with transition mode 3 ( -t 3 , or M until it comes round ) , which
 *         also snaps the camera onto the new photo so the 3D seek does not compete
 *         with the effect for attention.
 *
 *         Every shaders/transition_*.frag found at startup becomes a selectable
 *         effect. They may be written either in the ShaderToy dialect , or in the
 *         GL Transitions one ( gl-transitions.com : a vec4 transition(vec2 uv) using
 *         progress / ratio / getFromColor / getToColor ) , which visuals/shadertoy.cpp
 *         detects and wires up , so shaders from that collection work unmodified.
 *  @author Ammar Qammaz (AmmarkoV)
 */

#ifndef SHADER_TRANSITIONS_H_INCLUDED
#define SHADER_TRANSITIONS_H_INCLUDED

/* Compile every shaders/transition_*.frag , call once after shadertoy_init().
   retval = how many compiled ( 0 = transition mode 3 will behave like immediate ) */
unsigned int InitShaderTransitions();

unsigned int GetNumberOfShaderTransitions();

/* Pick one by name , retval 0 = no such transition , the selection is unchanged */
int SelectShaderTransitionByName(const char * name);

/* Move to the next transition shader , wrapping around */
void CycleShaderTransition();

/* Name of the selected one , "" when none are loaded */
const char * GetActiveShaderTransitionName();

/* Begin a transition from album picture fromPlace to album picture toPlace.
   Does nothing unless transition mode 3 is selected and a shader is loaded. */
void StartShaderTransition(int fromPlace,int toPlace);

/* 1 while one is playing and DrawShaderTransition has something to draw */
unsigned int ShaderTransitionIsPlaying();

/* Draw the transition over everything that has been rendered so far.
   Call after the scene and before the HUD. retval 0 = nothing was drawn. */
int DrawShaderTransition();

void CloseShaderTransitions();

#endif // SHADER_TRANSITIONS_H_INCLUDED
