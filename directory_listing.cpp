#include "directory_listing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void GetDirectoryList(char * thedirectory)
{
  int linux_system=0;

  #ifdef __WIN32__
        linux_system=0;
  #else
        linux_system=1;
  #endif

 if (linux_system==1)
  {
  /* PLAY SOUND VIA MPLAYER*/
  char command[512]={0};
  sprintf(command,"find . -maxdepth 1 -type f > filelist");
  int i=system((const char *)command);
  if ( i != 0 ) fprintf(stderr,"Error (%d) listing directory \n",i);
  /*----------------------------------------*/

  /* TODO */

  }  else
  fprintf(stderr,"GetDirectoryList not implemented for platform :( \n");
}


int GetTotalFilesInDirectory()
{
    /* TODO */
    return 0;
}


int GetTotalViewableFilesInDirectory()
{
    /* TODO */
    return 0;
}

int GetViewableFilenameforFile(int file_id,char *filename)
{
    /* TODO */
    return 0;
}
