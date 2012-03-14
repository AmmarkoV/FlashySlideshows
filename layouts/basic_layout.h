#ifndef BASIC_LAYOUT_H_INCLUDED
#define BASIC_LAYOUT_H_INCLUDED

#include "../load_images.h"

enum layout_list
{
   LAYOUT_BASIC=0,
   LAYOUT_EXPO ,
   TOTAL_LAYOUTS
};


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


#endif // BASIC_LAYOUT_H_INCLUDED
