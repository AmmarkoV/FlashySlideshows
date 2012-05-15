#ifndef LAYOUT_HANDLER_H_INCLUDED
#define LAYOUT_HANDLER_H_INCLUDED

#include "../load_images.h"

enum layout_list
{
   LAYOUT_BASIC=0,
   LAYOUT_EXPO ,
   TOTAL_LAYOUTS
};

void GetLayoutCoordinatesForXY(unsigned int X,unsigned int Y,float *x,float *y,float *z);

int HandleLayout();
int HandlePictureLayout(struct Picture * pic,unsigned int place);

int ChangeLayout();

char * GetLayoutName();

float GetLayoutMinimumX();
float GetLayoutMaximumX();
float GetLayoutMinimumY();
float GetLayoutMaximumY();
float GetLayoutMinimumZ();
float GetLayoutMaximumZ();

int DrawLayoutBackground();
int LayoutMoving();
#endif // LAYOUT_HANDLER_H_INCLUDED
