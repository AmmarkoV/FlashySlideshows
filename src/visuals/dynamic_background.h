/** @file dynamic_background.h
 *  @brief Animated backgrounds drawn by a ShaderToy style fragment shader instead of
 *         the static app_clipart/backgroundN.jpg quad.
 *
 *         Every shaders/background_*.frag found at startup becomes a selectable
 *         background , so a new one can be added by dropping a file in that
 *         directory , without recompiling anything.
 *  @author Ammar Qammaz (AmmarkoV)
 */

#ifndef DYNAMIC_BACKGROUND_H_INCLUDED
#define DYNAMIC_BACKGROUND_H_INCLUDED

/* Find the shaders directory , compile every shaders/background_*.frag it holds.
   Call once , after the GL context , GLEW and shadertoy_init() are up.
   retval = how many backgrounds were compiled ( 0 = stay on the static quad ) */
unsigned int InitDynamicBackgrounds();

/* How many are loaded , and the name ( "aurora" , "plasma" .. ) of one of them */
unsigned int GetNumberOfDynamicBackgrounds();
const char * GetDynamicBackgroundName(unsigned int index);

/* Pick one by name , retval 0 = no such background , the selection is unchanged */
int SelectDynamicBackgroundByName(const char * name);

/* Move to the next background , wrapping around , and past the last one back to the
   static picture background so the old look is still reachable from the keyboard */
void CycleDynamicBackground();

/* 1 when a shader background is selected and DrawDynamicBackground will draw it */
unsigned int DynamicBackgroundIsActive();

/* Name of the selected shader , "" when the static picture background is in use */
const char * GetActiveDynamicBackgroundName();

/* Draw the selected shader over the whole viewport , retval 0 = nothing was drawn
   and the caller has to fall back to the static background */
int DrawDynamicBackground();

void CloseDynamicBackgrounds();

#endif
