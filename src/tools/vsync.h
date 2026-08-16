#ifndef VSYNC_H_INCLUDED
#define VSYNC_H_INCLUDED

/* Vertical sync lives in its own file because asking GLX for a swap interval means
   including GL/glx.h , and X11/X.h defines DisableScreenSaver / EnableScreenSaver as
   macros which collide with the functions of the same name in tools/environment.h */

/* Ask the driver to hold the buffer swap until the vertical blank.
   Has to be called after a window and a GL context exist. retval 0 = could not */
int EnableVerticalSync();

/* 1 once EnableVerticalSync has succeeded , the software framerate limiter
   steps out of the way when this is set */
extern unsigned int verticalSyncEnabled;

#endif // VSYNC_H_INCLUDED
