/** @file shadertoy.h
 *  @brief Compile ShaderToy-style fragment shaders ( files containing a
 *         mainImage(out vec4,in vec2) function ) behind a common preamble that
 *         declares iResolution / iTime / iMouse / iDate / iChannel0..3 , and draw
 *         them on a fullscreen quad.
 *         Any single-pass shader from shadertoy.com can be dropped in a file under
 *         shaders/ and loaded with shadertoy_load() without touching any C code.
 *
 *         Ported from BabyKeySmash ( src/shadertoy.c ) , with the file loader
 *         inlined so that FlashySlideshows does not grow another dependency.
 *  @author Ammar Qammaz (AmmarkoV)
 */

#ifndef SHADERTOY_H_INCLUDED
#define SHADERTOY_H_INCLUDED

struct shadertoyEffect
{
  unsigned int program;
  int locResolution;
  int locTime;
  int locTimeDelta;
  int locMouse;
  int locFrame;
  int locChannel0;
  int locChannel1;
  int locChannelResolution;
  int locDate;
  int locProgress;
};

/* Create the shared fullscreen quad , call once after the GL context and GLEW are up.
   retval 0 = this machine cannot run shaders , the caller should stay on the
   fixed function path instead of drawing nothing */
int shadertoy_init();

/* 1 once shadertoy_init has succeeded */
extern unsigned int shadertoyAvailiable;

/* Load + compile a ShaderToy style fragment shader file , retval 0 = Failure */
struct shadertoyEffect * shadertoy_load(const char * fragmentFile);

/* Draw the effect over the whole viewport.
   The quad is in normalized device coordinates so it ignores the modelview and the
   projection completely , which is exactly what a background wants.
   chan0Tex / chan1Tex are GL texture ids bound to iChannel0 / iChannel1 , 0 = none ,
   progress feeds the iProgress uniform used by the transition shaders ( 0.0 -> 1.0 ) */
void shadertoy_draw(struct shadertoyEffect * fx,
                    float time,float timeDelta,int frameNumber,
                    float width,float height,
                    float mouseX,float mouseY,
                    unsigned int chan0Tex,unsigned int chan1Tex,
                    float progress);

void shadertoy_unload(struct shadertoyEffect * fx);

/* Compile a program from in-memory sources , binds attribute "vPos" to location 0 ,
   retval 0 = Failure */
unsigned int shadertoy_compileProgram(const char * vertexSource,const char * fragmentSource);

#endif
