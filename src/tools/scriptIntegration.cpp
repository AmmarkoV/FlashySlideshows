#include "scriptIntegration.h"

#include "../filesystem/directory_listing.h"
#include "../slideshow.h"

#include <stdlib.h>
#include <stdio.h>

unsigned int UploadPhotoToMyloader(char * photo)
{
if (photo==0) { fprintf(stderr,"Erroneous photograph in UploadPhotoToMyloader\n"); return 0; }
fprintf(stderr,"Trying to UploadPhotoToMyloader %s \n",photo);
char line[512];
char command[512];
sprintf(command,"scripts/myloader \"%s\"",photo);

FILE *fpipe;
 if ( !(fpipe = (FILE*)popen(command,"r")) )
   {
       fprintf(stderr,"Error UploadPhotoToMyloader\n");
       return 0;
   } else
   {
      while ( fgets( line, sizeof line, fpipe))
       {
         fprintf(stderr,"UploadPhotoToMyloader returned link %s \n",line);
       }
   }
 return 1;
}

unsigned int UploadCurrentPhotoToMyloader()
{
  fprintf(stderr,"Feature deactivated :P");
  return 0;

  char photo_filename[1024];
  GetFullFilenameforFile(frame.active_image_place,photo_filename);
  return  UploadPhotoToMyloader(photo_filename);
}

