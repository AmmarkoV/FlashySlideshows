#include "basic_layout.h"

/*
  This is a basic layout handler for using a ZxN album like
  album outline
*/


/*
 ======================================
 ==================================================
 DEFAULT HANDLERS ======================================
 ==================================================
 ======================================
*/

//SEEK MOVEMENT  ======================================

int HandleLayout()
{

  return 0;
}

int HandlePictureLayout(struct Picture * pic,unsigned int place)
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
