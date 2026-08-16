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
#include <glob.h>

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
"  vec4 color = vec4(0.0,0.0,0.0,1.0);\n"
"  mainImage(color,gl_FragCoord.xy);\n"
"  gl_FragColor = color;\n"
"}\n";

/* A shader that declares transition(vec2) instead of mainImage(out vec4,in vec2) is a
   GL Transitions ( gl-transitions.com ) shader , and expects progress / ratio /
   getFromColor / getToColor to already exist. Providing them here means the ~80
   shaders in that collection can be dropped into shaders/ unmodified.
   Their uv has ( 0,0 ) at the bottom left and covers the whole viewport , while our
   photos are stored with row 0 at the top and are not the shape of the window , so
   the two samplers letterbox the picture and flip it on the way in. */
static const char * transitionBindings =
"\n#define progress iProgress\n"
"#define ratio (iResolution.x/iResolution.y)\n"
"vec4 shadertoy_fitted(sampler2D img,vec3 res,vec2 uv)\n"
"{\n"
"  if (res.x<1.0) { return vec4(0.0); }\n"
"  float pictureAspect = res.x/res.y;\n"
"  float viewAspect    = iResolution.x/iResolution.y;\n"
"  vec2 scale = vec2(1.0);\n"
"  if (pictureAspect>viewAspect) { scale.y = pictureAspect/viewAspect; }\n"
"  else                         { scale.x = viewAspect/pictureAspect; }\n"
"  vec2 p = (uv-0.5)*scale + 0.5;\n"
"  if ( (p.x<0.0)||(p.x>1.0)||(p.y<0.0)||(p.y>1.0) ) { return vec4(0.0); }\n"
"  return texture(img,vec2(p.x,1.0-p.y));\n"
"}\n"
"#define getFromColor(uv) shadertoy_fitted(iChannel0,iChannelResolution[0],uv)\n"
"#define getToColor(uv)   shadertoy_fitted(iChannel1,iChannelResolution[1],uv)\n"
"#line 1\n";

static const char * transitionFooter =
"\nvoid main()\n"
"{\n"
"  vec2 uv = gl_FragCoord.xy / iResolution.xy;\n"
"  gl_FragColor = transition(uv);\n"
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


/* Does the shader source actually contain this token , as opposed to merely mentioning
   it in a comment ? Which dialect a file is written in is decided by whether it
   declares mainImage or transition , and a header comment that explains "this file has
   no mainImage" would otherwise put it in the wrong bucket. */
static int sourceMentions(const char * source,const char * token)
{
  unsigned int tokenLength = strlen(token);
  const char * p = source;

  while (*p!=0)
  {
    if ( (p[0]=='/') && (p[1]=='/') )
     { while ( (*p!=0) && (*p!='\n') ) { ++p; } continue; }

    if ( (p[0]=='/') && (p[1]=='*') )
     {
       p+=2;
       while ( (*p!=0) && !( (p[0]=='*') && (p[1]=='/') ) ) { ++p; }
       if (*p!=0) { p+=2; }
       continue;
     }

    if (strncmp(p,token,tokenLength)==0) { return 1; }
    ++p;
  }
  return 0;
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

  /* Which dialect this file is written in. A GL Transitions shader has no mainImage ,
     it declares vec4 transition(vec2 uv) instead. */
  const char * preamble = shadertoyPreamble;
  const char * bindings = "";
  const char * footer   = shadertoyFooter;
  if ( (!sourceMentions(body,"mainImage")) && (sourceMentions(body,"transition(")) )
   {
     bindings = transitionBindings;
     footer   = transitionFooter;
   }

  unsigned int fullLength = strlen(preamble) + strlen(bindings) + fileLength + strlen(footer) + 1;
  char * full = (char *) malloc(fullLength);
  if (full==0) { free(body); return 0; }
  snprintf(full,fullLength,"%s%s%s%s",preamble,bindings,body,footer);
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
  fx->locChannelResolution[0]= glGetUniformLocation(program,"iChannelResolution[0]");
  fx->locChannelResolution[1]= glGetUniformLocation(program,"iChannelResolution[1]");
  fx->locDate             = glGetUniformLocation(program,"iDate");
  fx->locProgress         = glGetUniformLocation(program,"iProgress");

  fprintf(stderr,"Loaded shader effect %s \n",fragmentFile);
  return fx;
}


/* ------------------------------------------------------------------------------
   Finding and loading a whole directory of effects , shared by the animated
   backgrounds and the transitions since both are "every .frag with this prefix"
   ------------------------------------------------------------------------------ */

int shadertoy_findShadersDirectory(char * directory,unsigned int directorySize)
{
  const char * candidates[] = { "shaders",
                                "../shaders",
                                "/usr/share/flashyslideshows/shaders",
                                0 };
  unsigned int i=0;
  for (i=0; candidates[i]!=0; i++)
   {
     char probe[1024]={0};
     snprintf(probe,1024,"%s/background_aurora.frag",candidates[i]);
     FILE * fp = fopen(probe,"rb");
     if (fp!=0)
      {
        fclose(fp);
        snprintf(directory,directorySize,"%s",candidates[i]);
        return 1;
      }
   }
  return 0;
}

/* "some/path/background_aurora.frag" -> "aurora" */
static void nameFromShaderPath(const char * path,const char * prefix,char * name,unsigned int nameSize)
{
  const char * base = strrchr(path,'/');
  base = (base!=0) ? base+1 : path;
  snprintf(name,nameSize,"%s",base+strlen(prefix));
  char * dot = strrchr(name,'.');
  if (dot!=0) { *dot=0; }
}

unsigned int shadertoy_loadEffectDirectory(const char * prefix,struct shadertoyEffectList * list)
{
  if (list==0) { return 0; }
  memset(list,0,sizeof(struct shadertoyEffectList));
  if (!shadertoyAvailiable) { return 0; }

  char directory[1024]={0};
  if (!shadertoy_findShadersDirectory(directory,1024))
   {
     fprintf(stderr,"Unable to locate a shaders/ directory , %s effects are off\n",prefix);
     return 0;
   }

  char pattern[1088]={0};
  snprintf(pattern,1088,"%s/%s*.frag",directory,prefix);

  glob_t files;
  memset(&files,0,sizeof(files));
  if (glob(pattern,0,0,&files)!=0) { return 0; }

  unsigned int i=0;
  for (i=0; (i<files.gl_pathc) && (list->count<MAX_SHADERTOY_EFFECTS); i++)
   {
     struct shadertoyEffect * fx = shadertoy_load(files.gl_pathv[i]);
     if (fx!=0)
      {
        list->entry[list->count].fx = fx;
        nameFromShaderPath(files.gl_pathv[i],prefix,list->entry[list->count].name,64);
        ++list->count;
      }
   }
  globfree(&files);

  return list->count;
}

int shadertoy_findEffectByName(struct shadertoyEffectList * list,const char * name)
{
  if ( (list==0) || (name==0) ) { return -1; }
  unsigned int i=0;
  for (i=0; i<list->count; i++)
   {
     if (strcmp(list->entry[i].name,name)==0) { return (int) i; }
   }
  return -1;
}

void shadertoy_printEffectNames(struct shadertoyEffectList * list)
{
  if (list==0) { return; }
  unsigned int i=0;
  for (i=0; i<list->count; i++) { fprintf(stderr," %s",list->entry[i].name); }
}

void shadertoy_unloadEffectList(struct shadertoyEffectList * list)
{
  if (list==0) { return; }
  unsigned int i=0;
  for (i=0; i<list->count; i++)
   {
     shadertoy_unload(list->entry[i].fx);
     list->entry[i].fx=0;
   }
  list->count=0;
}


void shadertoy_draw(struct shadertoyEffect * fx,struct shadertoyInputs * inputs)
{
  if ( (fx==0) || (inputs==0) || (!shadertoyAvailiable) ) { return; }

  /* The rest of the application lives in the fixed function pipeline and expects to
     find its state where it left it , so everything touched here is put back below */
  GLboolean cullingWasOn = glIsEnabled(GL_CULL_FACE);
  GLboolean blendingWasOn = glIsEnabled(GL_BLEND);
  GLboolean depthTestWasOn = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  if (inputs->useAlpha)
       { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); } else
       { glDisable(GL_BLEND); }

  glUseProgram(fx->program);

  float width=inputs->width , height=inputs->height;

  if (fx->locResolution>=0) { glUniform3f(fx->locResolution,width,height,1.0f); }
  if (fx->locTime>=0)       { glUniform1f(fx->locTime,inputs->time); }
  if (fx->locTimeDelta>=0)  { glUniform1f(fx->locTimeDelta,inputs->timeDelta); }
  if (fx->locFrame>=0)      { glUniform1i(fx->locFrame,inputs->frameNumber); }
  if (fx->locProgress>=0)   { glUniform1f(fx->locProgress,inputs->progress); }
  //ShaderToy mouse coordinates have y going up
  if (fx->locMouse>=0)      { glUniform4f(fx->locMouse,inputs->mouseX,height-inputs->mouseY,inputs->mouseX,height-inputs->mouseY); }
  if (fx->locDate>=0)
  {
    float date[4];
    getShaderToyDate(date);
    glUniform4f(fx->locDate,date[0],date[1],date[2],date[3]);
  }
  /* An unbound channel keeps a resolution of ( 0,0,0 ) , which is how a shader tells
     that there is no picture there. Channels 2 and 3 are never bound by this
     application and are simply left at whatever the shader initialized them to. */
  unsigned int channelIndex=0;
  for (channelIndex=0; channelIndex<2; channelIndex++)
   {
     if (fx->locChannelResolution[channelIndex]<0) { continue; }
     if (inputs->channel[channelIndex]==0)
          { glUniform3f(fx->locChannelResolution[channelIndex],0.0,0.0,0.0); } else
          { glUniform3f(fx->locChannelResolution[channelIndex],
                        inputs->channelWidth[channelIndex],
                        inputs->channelHeight[channelIndex],1.0); }
   }

  if (inputs->channel[0])
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,inputs->channel[0]);
    if (fx->locChannel0>=0) { glUniform1i(fx->locChannel0,0); }
  }
  if (inputs->channel[1])
  {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,inputs->channel[1]);
    if (fx->locChannel1>=0) { glUniform1i(fx->locChannel1,1); }
  }

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES,0,6);
  glBindVertexArray(0);

  /* Leaving texture unit 1 selected would send every following fixed function
     glBindTexture to the wrong unit and the whole slideshow would go untextured */
  if (inputs->channel[1]) { glBindTexture(GL_TEXTURE_2D,0); }
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
