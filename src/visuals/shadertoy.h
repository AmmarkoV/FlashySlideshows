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
  /* Per element , because a GLSL compiler is free to shrink a uniform array down to
     the highest index the shader actually reads , and then one glUniform3fv covering
     all four elements is out of range */
  int locChannelResolution[2];
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


#define MAX_SHADERTOY_EFFECTS 32

/* A directory worth of effects , named after the part of the filename that follows
   the prefix : shaders/background_aurora.frag becomes "aurora" */
struct shadertoyEffectList
{
  struct
  {
    struct shadertoyEffect * fx;
    char name[64];
  } entry[MAX_SHADERTOY_EFFECTS];
  unsigned int count;
};

/* Where the .frag files are : the directory the binary was started from , one level up
   from it ( the binary lands in src/ in a build tree ) , or the installed location.
   retval 0 = there is no shaders directory anywhere */
int shadertoy_findShadersDirectory(char * directory,unsigned int directorySize);

/* Compile every shaders/<prefix>*.frag into list , retval = how many compiled.
   Files that fail to compile are reported and skipped , they do not abort the rest. */
unsigned int shadertoy_loadEffectDirectory(const char * prefix,struct shadertoyEffectList * list);

/* Index of the effect called name , or -1 */
int shadertoy_findEffectByName(struct shadertoyEffectList * list,const char * name);

/* Print " a b c" , the names in the list , to stderr */
void shadertoy_printEffectNames(struct shadertoyEffectList * list);

void shadertoy_unloadEffectList(struct shadertoyEffectList * list);

/* Everything a shader can be fed for one frame. Zero the struct and fill in what the
   effect actually needs , the uniforms it does not declare are skipped anyway.
   This is a struct rather than a long argument list because the transition shaders of
   shaders/transition_*.frag need both channels , their sizes and the progress , and
   that had already grown to ten positional arguments. */
struct shadertoyInputs
{
  float time;        /* iTime , seconds */
  float timeDelta;   /* iTimeDelta , seconds since the previous draw */
  int   frameNumber; /* iFrame */

  float width,height; /* the viewport , goes out as iResolution */
  float mouseX,mouseY;

  /* iProgress , 0.0 -> 1.0 , read by anything that fades between the two channels */
  float progress;

  /* 1 = respect the alpha the shader writes , blending the pass over whatever is
     already on screen. The transitions use it to leave the letterbox bars around a
     photo transparent so the animated background keeps showing there.
     0 = draw opaque , which is what a background wants and what keeps a pasted
     shadertoy.com shader safe when it writes a meaningless alpha. */
  unsigned int useAlpha;

  /* GL texture ids bound to iChannel0 / iChannel1 , 0 = leave that channel unbound.
     The matching size lands in iChannelResolution[n] and stays ( 0,0,0 ) for an
     unbound channel , which is how a shader can tell whether it has a picture. */
  unsigned int channel[2];
  float channelWidth[2],channelHeight[2];
};

/* Draw the effect over the whole viewport.
   The quad is in normalized device coordinates so it ignores the modelview and the
   projection completely , which is exactly what a background wants. */
void shadertoy_draw(struct shadertoyEffect * fx,struct shadertoyInputs * inputs);

void shadertoy_unload(struct shadertoyEffect * fx);

/* Compile a program from in-memory sources , binds attribute "vPos" to location 0 ,
   retval 0 = Failure */
unsigned int shadertoy_compileProgram(const char * vertexSource,const char * fragmentSource);

#endif
