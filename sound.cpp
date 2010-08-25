#include "sound.h"
#include <stdlib.h>
#include <stdio.h>

void PlaySound(char * thesound)
{
  int linux_sound=0;

  #ifdef __WIN32__
        linux_sound=0;
  #else
        linux_sound=1;
  #endif

 if (linux_sound==1)
  {
  /* PLAY SOUND VIA MPLAYER*/
  char command[512]={0};
  sprintf(command,"mpg123 -q %s",thesound);
  int i=system((const char *)command);
  if ( i != 0 ) fprintf(stderr,"Error (%d) playing sound\n",i);
  /*----------------------------------------*/
  }  else
  fprintf(stderr,"PlaySound not implemented for platform :( \n");
}
