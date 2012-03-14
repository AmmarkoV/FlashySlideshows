#include "layout_handler.h"
#include "expo_layout.h"
#include "../slideshow.h"

/*
  This is the layout handler for albums
  It is one of the few cases which should probably be implemented in an object oriented paradigm :P
  BUT! :P

*/

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
  pic->position.yaw=0;
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
    return -14.0*(frame.images_per_line-1)-4.0;
}

float GetLayoutMaximumX()
{
    return 20;
}


float GetLayoutMinimumY()
{
    return 0;
}

float GetLayoutMaximumY()
{
    return 0;
}


float GetLayoutMinimumZ()
{
    return -4.0;
}

float GetLayoutMaximumZ()
{
    return 20;
}
