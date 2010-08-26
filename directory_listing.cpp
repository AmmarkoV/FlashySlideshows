#include "directory_listing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "InputParser_C.h"

int GetDirectoryList(char * thedirectory)
{
  int linux_system=0;

  #ifdef __WIN32__
        linux_system=0;
  #else
        linux_system=1;
  #endif

 if (linux_system==1)
  {
  char command[512]={0};

  /* crazy r.e by c00kiemon5ter \/*/
  sprintf(command,"find \"%s\" -maxdepth 1 -type f ! -name \".*\" -exec file {} + -exec grep \": \\s*\\w* image\" {} + | sed \"s/.\\/\\(.*\\):.*/\\1/\" > filelist.dat",thedirectory);

  //sprintf(command,"find %s -type f -printf \"%%P\\t%%t\\n\"  > filelist.dat",thedirectory);
  //sprintf(command,"for file in $(find ~/downloads/ -maxdepth 1 -type f ! -name ".*" -exec file {} + | grep ": \\s*\\w* image data" | sed "s/\(.*\):.*/\1/"); do basename $file; done > imagelist");
  int i=system((const char *)command);
  if ( i != 0 ) fprintf(stderr,"Error (%d) listing directory \n",i);
  /*----------------------------------------*/

  /* the file filelist.dat should contain the list of all the files in folder*/

    /*Tokenizing it using InputParser_C :) */
    struct InputParserC * ipc=0;
    ipc = InputParser_Create(1024,5);
    InputParser_SetDelimeter(ipc,1,' ');

    struct InputParserC * ipc2=0;
    ipc2 = InputParser_Create(1024,3);
    InputParser_SetDelimeter(ipc2,1,'.');

    FILE * fp;
    fp = fopen("filelist.dat","r");
    if (fp == 0) { return 0; }

    if ( fp != 0 )
       {
          char line[2048]={0};
          char filename[2048]={0};
          char extension[512]={0};
          unsigned int linelen=2048;

          while (!feof(fp) )
           {
            if (fgets(line,linelen,fp)!=0)
             {
                 int res = InputParser_SeperateWords(ipc,line,linelen);
                 printf("%u arguments\n ",res);
                 printf("%s ",line);
                 InputParser_GetWord(ipc,0,filename,2048);

                 res = InputParser_SeperateWords(ipc2,filename,linelen);
                 if ( res > 1 )
                 {
                  InputParser_GetWord(ipc2,1,extension,512);
                  printf("EXTENSION : %s \n",extension);
                 }

             }
           }
           fclose (fp);

       }

     InputParser_Destroy(ipc);

    return 1;
  }  else
  {
    /* WINDOWS MAC ETC. */
   fprintf(stderr,"GetDirectoryList not implemented for platform :( \n");
  }
  return 0;
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
