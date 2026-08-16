#include "vsync.h"

#include <stdio.h>

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glx.h>
#endif

unsigned int verticalSyncEnabled=0;

int EnableVerticalSync()
{
  /* Tearing comes from handing the screen a new buffer half way through its scanout ,
     it is not caused by clearing the buffer. Asking GLX for a swap interval of one
     makes the swap wait for the vertical blank and the tearing goes away for real ,
     which is what lets main.cpp glClear() again like a normal application.
     __GL_SYNC_TO_VBLANK ( see main() ) only ever worked on the NVidia blob , and only
     when it was exported before the process started.. */
#ifndef __APPLE__
  typedef void (*swapIntervalEXTPtr)  (Display *,GLXDrawable,int);
  typedef int  (*swapIntervalMESAPtr) (unsigned int);
  typedef int  (*swapIntervalSGIPtr)  (int);

  swapIntervalEXTPtr  swapEXT  = (swapIntervalEXTPtr)  glXGetProcAddressARB((const GLubyte *) "glXSwapIntervalEXT");
  swapIntervalMESAPtr swapMESA = (swapIntervalMESAPtr) glXGetProcAddressARB((const GLubyte *) "glXSwapIntervalMESA");
  swapIntervalSGIPtr  swapSGI  = (swapIntervalSGIPtr)  glXGetProcAddressARB((const GLubyte *) "glXSwapIntervalSGI");

  if ( swapEXT != 0 )
   {
     Display * display    = glXGetCurrentDisplay();
     GLXDrawable drawable = glXGetCurrentDrawable();
     if ( (display!=0) && (drawable!=0) )
      {
        swapEXT(display,drawable,1);
        fprintf(stderr,"VSync enabled using GLX_EXT_swap_control\n");
        verticalSyncEnabled=1;
        return 1;
      }
   }
  if ( swapMESA != 0 ) { swapMESA(1); fprintf(stderr,"VSync enabled using GLX_MESA_swap_control\n"); verticalSyncEnabled=1; return 1; }
  if ( swapSGI  != 0 ) { swapSGI(1);  fprintf(stderr,"VSync enabled using GLX_SGI_swap_control\n");  verticalSyncEnabled=1; return 1; }
#endif

  fprintf(stderr,"Could not enable VSync , falling back to the software framerate limiter\n");
  return 0;
}
