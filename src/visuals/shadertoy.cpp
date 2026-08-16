/** @file shadertoy.cpp
 *  @brief see shadertoy.h
 *  @author Ammar Qammaz (AmmarkoV)
 */

/* GLEW has to come before any other GL header , it refuses to load after gl.h */
#include <GL/glew.h>
#include <GL/gl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "shadertoy.h"

unsigned int shadertoyAvailiable=0;

/* Everything a shadertoy.com shader expects to already be declared for it.
   iProgress is not part of ShaderToy , it is what the GL-Transitions style
   transition shaders use , and an unused uniform costs nothing. */
static const char * shadertoyPreamble =
"#version 130\n"
"uniform vec3  iResolution;\n"
"uniform float iTime;\n"
"uniform float iTimeDelta;\n"
"uniform int   iFrame;\n"
"uniform vec4  iMouse;\n"
"uniform vec4  iDate;\n"
"uniform float iSampleRate;\n"
"uniform sampler2D iChannel0;\n"
"uniform sampler2D iChannel1;\n"
"uniform sampler2D iChannel2;\n"
"uniform sampler2D iChannel3;\n"
"uniform vec3  iChannelResolution[4];\n"
"uniform float iProgress;\n"
"#define iGlobalTime iTime\n"
"#line 1\n";

static const char * shadertoyFooter =
"\nvoid main()\n"
"{\n"
"  vec4 color = vec4(0.0);\n"
"  mainImage(color,gl_FragCoord.xy);\n"
"  gl_FragColor = vec4(color.rgb,1.0);\n"
"}\n";

static const char * fullscreenVertexShader =
"#version 130\n"
"in vec2 vPos;\n"
"void main() { gl_Position = vec4(vPos,0.0,1.0); }\n";

static GLuint quadVAO = 0;
static GLuint quadVBO = 0;


/* Slurp a whole file into a malloc'ed , zero terminated buffer.
   BabyKeySmash borrows loadShaderFileToMem from its renderer library , there is no
   such library here so this is the same job done locally */
static char * loadFileToMemory(const char * filename,int * length)
{
  *length=0;
  FILE * fp = fopen(filename,"rb");
  if (fp==0) { return 0; }

  fseek(fp,0,SEEK_END);
  long size = ftell(fp);
  fseek(fp,0,SEEK_SET);
  if (size<=0) { fclose(fp); return 0; }

  char * buffer = (char *) malloc(size+1);
  if (buffer==0) { fclose(fp); return 0; }

  size_t bytesRead = fread(buffer,1,size,fp);
  fclose(fp);
  buffer[bytesRead]=0;

  *length=(int) bytesRead;
  return buffer;
}


/* ShaderToy iDate is ( year , month-1 , day , seconds since midnight ).
   Kept out of shadertoy_draw because its "time" argument shadows time() */
static void getShaderToyDate(float * date)
{
  time_t rawTime = time(0);
  struct tm * local = localtime(&rawTime);
  if (local==0) { date[0]=0; date[1]=0; date[2]=0; date[3]=0; return; }
  date[0] = (float) (local->tm_year+1900);
  date[1] = (float) local->tm_mon;
  date[2] = (float) local->tm_mday;
  date[3] = (float) (local->tm_hour*3600 + local->tm_min*60 + local->tm_sec);
}


static GLuint compileOneShader(GLenum type,const char * source,const char * label)
{
  GLuint sh = glCreateShader(type);
  glShaderSource(sh,1,(const GLchar **) &source,0);
  glCompileShader(sh);

  GLint isok=0;
  glGetShaderiv(sh, GL_COMPILE_STATUS, &isok);
  if (!isok)
  {
    GLchar info[2048]; GLsizei length=0;
    glGetShaderInfoLog(sh,2048,&length,info);
    fprintf(stderr,"Could not compile %s shader :\n%s\n",label,info);
    glDeleteShader(sh);
    return 0;
  }
  return sh;
}

unsigned int shadertoy_compileProgram(const char * vertexSource,const char * fragmentSource)
{
  GLuint vs = compileOneShader(GL_VERTEX_SHADER  ,vertexSource  ,"vertex");
  if (!vs) { return 0; }
  GLuint fs = compileOneShader(GL_FRAGMENT_SHADER,fragmentSource,"fragment");
  if (!fs) { glDeleteShader(vs); return 0; }

  GLuint program = glCreateProgram();
  glAttachShader(program,vs);
  glAttachShader(program,fs);
  glBindAttribLocation(program,0,"vPos");
  glLinkProgram(program);

  GLint isok=0;
  glGetProgramiv(program, GL_LINK_STATUS, &isok);
  if (!isok)
  {
    GLchar info[2048]; GLsizei length=0;
    glGetProgramInfoLog(program,2048,&length,info);
    fprintf(stderr,"Could not link shaders :\n%s\n",info);
    glDeleteProgram(program); program=0;
  }
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}


int shadertoy_init()
{
  shadertoyAvailiable=0;

  /* An OpenGL 1.1 driver , or a GLEW that never initialized , leaves these null and
     calling through them would segfault instead of politely doing nothing */
  if ( (glCreateShader==0) || (glGenVertexArrays==0) || (glGenBuffers==0) )
   {
     fprintf(stderr,"This driver has no shader pipeline , dynamic backgrounds are off\n");
     return 0;
   }

  static const float quad[] = { -1.0f,-1.0f,  1.0f,-1.0f,  1.0f,1.0f,
                                -1.0f,-1.0f,  1.0f, 1.0f, -1.0f,1.0f };
  glGenVertexArrays(1,&quadVAO);
  glBindVertexArray(quadVAO);
  glGenBuffers(1,&quadVBO);
  glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(quad),quad,GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,0);

  shadertoyAvailiable=1;
  return 1;
}


struct shadertoyEffect * shadertoy_load(const char * fragmentFile)
{
  if (!shadertoyAvailiable) { return 0; }

  int fileLength=0;
  char * body = loadFileToMemory(fragmentFile,&fileLength);
  if (body==0) { fprintf(stderr,"Could not load shadertoy file %s \n",fragmentFile); return 0; }

  unsigned int fullLength = strlen(shadertoyPreamble) + fileLength + strlen(shadertoyFooter) + 1;
  char * full = (char *) malloc(fullLength);
  if (full==0) { free(body); return 0; }
  snprintf(full,fullLength,"%s%s%s",shadertoyPreamble,body,shadertoyFooter);
  free(body);

  unsigned int program = shadertoy_compileProgram(fullscreenVertexShader,full);
  free(full);
  if (program==0) { fprintf(stderr,"Failed building shadertoy effect %s \n",fragmentFile); return 0; }

  struct shadertoyEffect * fx = (struct shadertoyEffect *) malloc(sizeof(struct shadertoyEffect));
  if (fx==0) { glDeleteProgram(program); return 0; }

  fx->program             = program;
  fx->locResolution       = glGetUniformLocation(program,"iResolution");
  fx->locTime             = glGetUniformLocation(program,"iTime");
  fx->locTimeDelta        = glGetUniformLocation(program,"iTimeDelta");
  fx->locMouse            = glGetUniformLocation(program,"iMouse");
  fx->locFrame            = glGetUniformLocation(program,"iFrame");
  fx->locChannel0         = glGetUniformLocation(program,"iChannel0");
  fx->locChannel1         = glGetUniformLocation(program,"iChannel1");
  fx->locChannelResolution= glGetUniformLocation(program,"iChannelResolution");
  fx->locDate             = glGetUniformLocation(program,"iDate");
  fx->locProgress         = glGetUniformLocation(program,"iProgress");

  fprintf(stderr,"Loaded shader effect %s \n",fragmentFile);
  return fx;
}


void shadertoy_draw(struct shadertoyEffect * fx,
                    float time,float timeDelta,int frameNumber,
                    float width,float height,
                    float mouseX,float mouseY,
                    unsigned int chan0Tex,unsigned int chan1Tex,
                    float progress)
{
  if ( (fx==0) || (!shadertoyAvailiable) ) { return; }

  /* The rest of the application lives in the fixed function pipeline and expects to
     find its state where it left it , so everything touched here is put back below */
  GLboolean cullingWasOn = glIsEnabled(GL_CULL_FACE);
  GLboolean blendingWasOn = glIsEnabled(GL_BLEND);
  GLboolean depthTestWasOn = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  glUseProgram(fx->program);

  if (fx->locResolution>=0) { glUniform3f(fx->locResolution,width,height,1.0f); }
  if (fx->locTime>=0)       { glUniform1f(fx->locTime,time); }
  if (fx->locTimeDelta>=0)  { glUniform1f(fx->locTimeDelta,timeDelta); }
  if (fx->locFrame>=0)      { glUniform1i(fx->locFrame,frameNumber); }
  if (fx->locProgress>=0)   { glUniform1f(fx->locProgress,progress); }
  //ShaderToy mouse coordinates have y going up
  if (fx->locMouse>=0)      { glUniform4f(fx->locMouse,mouseX,height-mouseY,mouseX,height-mouseY); }
  if (fx->locDate>=0)
  {
    float date[4];
    getShaderToyDate(date);
    glUniform4f(fx->locDate,date[0],date[1],date[2],date[3]);
  }
  if (fx->locChannelResolution>=0)
  {
    float channelResolution[12];
    unsigned int i=0;
    for (i=0; i<4; i++)
     { channelResolution[i*3+0]=width; channelResolution[i*3+1]=height; channelResolution[i*3+2]=1.0; }
    glUniform3fv(fx->locChannelResolution,4,channelResolution);
  }

  if (chan0Tex)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,chan0Tex);
    if (fx->locChannel0>=0) { glUniform1i(fx->locChannel0,0); }
  }
  if (chan1Tex)
  {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,chan1Tex);
    if (fx->locChannel1>=0) { glUniform1i(fx->locChannel1,1); }
  }

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES,0,6);
  glBindVertexArray(0);

  /* Leaving texture unit 1 selected would send every following fixed function
     glBindTexture to the wrong unit and the whole slideshow would go untextured */
  if (chan1Tex) { glBindTexture(GL_TEXTURE_2D,0); }
  glActiveTexture(GL_TEXTURE0);
  glUseProgram(0);

  if (cullingWasOn)   { glEnable(GL_CULL_FACE);  }
  if (blendingWasOn)  { glEnable(GL_BLEND);      }
  if (depthTestWasOn) { glEnable(GL_DEPTH_TEST); }
}


void shadertoy_unload(struct shadertoyEffect * fx)
{
  if (fx==0) { return; }
  glDeleteProgram(fx->program);
  free(fx);
}
