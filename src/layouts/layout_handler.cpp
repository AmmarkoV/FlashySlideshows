#include "layout_handler.h"
#include "expo_layout.h"
#include "../slideshow.h"
#include "../hypervisor/load_images.h"
#include "../visuals/background.h"
#include <stdlib.h>
#include <stdio.h>

/*
  This is the layout handler for albums
  It is one of the few cases which should probably be implemented in an object oriented paradigm :P
  BUT! :P

*/

// Some layout ideas
// http://www.youtube.com/watch?v=iDnRjOSslsg
// http://www.youtube.com/watch?v=9gTeMcfRoAI
// http://www.youtube.com/watch?v=cLdjn91WSM8

void GetLayoutCoordinatesForXY(unsigned int X,unsigned int Y,float *x,float *y,float *z)
{
  *x=(X*14);
  *x=*x-14;
  *x=(-1)* (*x);

  *y=(Y*12);
  *y= *y-12;

  *z=-5;
}


int GetPictureGeometry(unsigned int place,float *x,float *y,float *z,float *sizeX,float *sizeY)
{
  struct Picture * pic=0;
  if ( (album!=0) && (place<frame.total_images) ) { pic=album[place]; }

  /* Anything not loaded yet shares one of the loading / loading_texture / failed
     placeholder structures , and that structure's position member holds wherever it was
     last drawn rather than where this slot belongs. Asking it gives the answer for some
     other picture entirely , so only a picture that is really its own answers for itself
     and everything else is told where the layout is going to put it. */
  if ( PictureIsItsOwn(pic) && (pic->position.ok!=0) )
   {
     *x=pic->position.x;      *y=pic->position.y;      *z=pic->position.z;
     *sizeX=pic->position.size_x; *sizeY=pic->position.size_y;
     return 1;
   }

  GetLayoutCoordinatesForXY(PictureIDtoX(place),PictureIDtoY(place),x,y,z);
  /* The biggest a picture is ever sized to , see FixOpenGLPictureSize */
  *sizeX=6.0; *sizeY=4.5;
  return 0;
}


int HandleBasicPictureLayout(struct Picture * pic,unsigned int place)
{
  unsigned int x,y;
  if ( frame.images_per_line == 0 ) { return 0; }


  PictureIDtoXY(&x,&y,place);

  pic->position.ok=1;

  pic->position.x=(x*14);
  pic->position.x=pic->position.x-14;
  pic->position.x=(-1)*pic->position.x;

  pic->position.y=(y*12);
  pic->position.y=pic->position.y-12;

  pic->position.z=-5;

  pic->position.heading=0;
  pic->position.roll=0;
  pic->position.pitch=0;
  return 1;
}


int HandlePictureLayout(struct Picture * pic,unsigned int place)
{
  switch (frame.layout.layout_mode)
   {
      case    LAYOUT_EXPO  : return HandleExpoPictureLayout(pic,place); break;
      case    LAYOUT_BASIC : return HandleBasicPictureLayout(pic,place);break;
      default : return HandleBasicPictureLayout(pic,place); break;
   };
 return 0;
}


int HandleLayout()
{

  return 0;
}


int ChangeLayout()
{
  ++frame.layout.layout_mode;
  if (frame.layout.layout_mode>=TOTAL_LAYOUTS) { frame.layout.layout_mode=0; }

  if ( (frame.total_images > 0)&&(album!=0) )
   {
     HandlePictureLayout(album[0],0);
     HandlePictureLayout(album[frame.total_images-1],frame.total_images-1);
   }


  return 1;
}

/* Anything past this and the grid is wider than the camera can usefully travel , one is
   the narrowest that still is a grid */
#define MAXIMUM_IMAGES_PER_LINE 20

int ChangeImagesPerLine(int howmany)
{
  if ( (frame.total_images==0)||(album==0) ) { return 0; }

  int new_images_per_line = (int) frame.images_per_line + howmany;
  if (new_images_per_line<1)                       { new_images_per_line=1; }
  if (new_images_per_line>MAXIMUM_IMAGES_PER_LINE) { new_images_per_line=MAXIMUM_IMAGES_PER_LINE; }
  if ( (unsigned int) new_images_per_line == frame.images_per_line ) { return 0; }

  /* A picture keeps its place in the album , only the grid the layout hands it changes ,
     so the picture the camera is over is remembered by place and everything is rebuilt
     around it. Doing it the other way round ( keeping x,y ) would leave the camera over a
     different photo every time the grid was resized. */
  unsigned int stay_over = frame.active_image_place;

  fprintf(stderr,"Images per line %u -> %d , staying over picture %u\n",frame.images_per_line,new_images_per_line,stay_over);
  frame.images_per_line = (unsigned int) new_images_per_line;

  frame.active_image_x = PictureIDtoX(stay_over);
  frame.active_image_y = PictureIDtoY(stay_over);
  frame.active_image_place = stay_over;
  frame.last_image_x = frame.active_image_x;
  frame.last_image_y = frame.active_image_y;
  frame.last_image_place = stay_over;

  /* Every picture that owns its structure is carrying coordinates the old grid gave it ,
     the shared placeholders answer from the layout anyway ( see GetPictureGeometry ) */
  unsigned int album_traveler=0;
  for (album_traveler=0; album_traveler<frame.total_images; album_traveler++)
   {
     if ( PictureIsItsOwn(album[album_traveler]) ) { PositionPicture(album[album_traveler],album_traveler); }
   }

  /* .. and the camera follows the picture it was looking at to wherever it landed */
  float x=0.0,y=0.0,z=0.0;
  GetLayoutCoordinatesForXY(frame.active_image_x,frame.active_image_y,&x,&y,&z);
  frame.vx=x;         frame.vy=y;
  frame.desired_x=x;  frame.desired_y=y;
  frame.transitions.seek_move_activated=0;
  frame.forceDrawOneMoreTime=1;

  return 1;
}


char * GetLayoutName()
{
  switch (frame.layout.layout_mode)
   {
      case    LAYOUT_EXPO  : return (char*) "Expo";         break;
      case    LAYOUT_BASIC : return (char*) "Basic";        break;
      default :              return (char*) "Basic";        break;
   };
 return (char*) "Not set";
}




float GetLayoutMinimumX()
{
    return -14.0*(frame.images_per_line-2)-4.0;
}

float GetLayoutMaximumX()
{
    return 20;
}


float GetLayoutMinimumY()
{
    if ( (frame.total_images==0)||(album==0) ) { fprintf(stderr,"Returning Failsafe GetLayoutMinimumY()\n");return -1000; }
    return album[0]->position.y-21;
}

float GetLayoutMaximumY()
{
    if ( (frame.total_images==0)||(album==0) ) { fprintf(stderr,"Returning Failsafe GetLayoutMaximumY()\n"); return 1000; }
    return album[frame.total_images-1]->position.y+21;
}


float GetLayoutMinimumZ()
{
    return -4.7;
}

float GetLayoutMaximumZ()
{
    return 3.0;
}

int DrawLayoutBackground()
{
  DrawBackground();
  return 0;
}


int LayoutMoving()
{
  return 0;
}

