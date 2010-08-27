#include "directory_listing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "InputParser_C.h"

struct FilenameHolder
{
     char filename[512];
};

struct FilenameHolder * list=0;

unsigned int list_size=0;
unsigned int last_list_total_count=0;
unsigned int last_list_total_pictures_count=0;

void AllocateDirectoryList(unsigned int requested_size)
{
 if ( list != 0 ) { fprintf(stderr,"List already contained data , freeing memory \n"); free(list); }
 list = ( struct FilenameHolder * ) malloc(   sizeof(struct FilenameHolder) * (requested_size+5)  );

 unsigned int i=0;
 for ( i=0; i<requested_size; i++)
  {
    strcpy(list[i].filename,"\0");
  }

 list_size=requested_size;

 if ( list != 0 )
  {
    fprintf(stderr,"Allocated %u records of filename_holders\n",requested_size);
  }
}

void FreeDirectoryList()
{
  if ( list != 0 ) { fprintf(stderr,"Freeing  list memory \n"); free(list); }
  list=0;
}


unsigned int GetDirectoryList(char * thedirectory,unsigned int store_results_in_space)
{
  fprintf(stderr,"This code section is a little sloppy , may segfault \n");

  int linux_system=0;

  #ifdef __WIN32__
        linux_system=0;
  #else
        linux_system=1;
  #endif
 if (linux_system==0)
  {
    /* WINDOWS MAC ETC. */ fprintf(stderr,"GetDirectoryList not implemented for platform :( \n");
  }
    else
 if (linux_system==1)
  {
  char command[512]={0};

  /* crazy r.e by c00kiemon5ter <-> http://github.com/c00kiemon5ter \/*/
  sprintf(command,"for file in $(find \"%s\" -maxdepth 1 -type f ! -name \".*\" -exec file {} + | grep \": \\s*\\w* image data\" | sed \"s/\\(.*\\):.*/\\1/\"  ); do basename $file; done > filelist.dat",thedirectory);

  int i=system((const char *)command);
  if ( i != 0 ) fprintf(stderr,"Error (%d) listing directory using system command \n",i);
  /*----------------------------------------*/

  /* the file filelist.dat should contain the list of all the files in folder*/

    /*Tokenizing it using InputParser_C :) --------------------- */
    struct InputParserC * ipc=0;
    ipc = InputParser_Create(2048,5);
    InputParser_SetDelimeter(ipc,1,' ');
    InputParser_SetDelimeter(ipc,2,10);
    InputParser_SetDelimeter(ipc,3,13);
    InputParser_SetDelimeter(ipc,4,0);

    struct InputParserC * ipc2=0;
    ipc2 = InputParser_Create(1024,3);
    InputParser_SetDelimeter(ipc2,1,'.');
    /*---------------------------------------------------------- */

    FILE * fp;
    fp = fopen("filelist.dat","r");
    if (fp == 0) { return 0; }

    if ( fp != 0 )
       {
          char line[2048]={0};
          char filename[2048]={0};
          char extension[512]={0};
          unsigned int linelen=2048;

          unsigned int this_list_total_count=0;
          unsigned int this_list_total_pictures_count=0;

          if ( store_results_in_space > 0 )
            {
               /* We will create space to store up to "store_results_in_space" results*/
               AllocateDirectoryList(store_results_in_space);
            }

          while (!feof(fp) )
           {
            if (fgets(line,linelen,fp)!=0)
             {
                 /* START of Read loop that fills in line variable */
                 int res = InputParser_SeperateWords(ipc,line,linelen);
                 InputParser_GetWord(ipc,0,filename,2048);
                 printf("%u arguments <--> %s / %s  \n ",res,line,filename);

                 res = InputParser_SeperateWords(ipc2,filename,linelen);
                 if ( res > 1 )
                 {
                  ++this_list_total_count;
                  InputParser_GetWord(ipc2,1,extension,512);
                  printf( (char*) "EXTENSION : %s \n",extension);

                  if ( InputParser_WordCompareNoCase(ipc2,1,(char *)"JPG",3)==1 )
                      {
                       if ( store_results_in_space > 0 )
                       {
                         if ( list_size <= this_list_total_pictures_count )
                          {
                              fprintf(stderr,"Our initial size (%u) was mistaken (%u) , while accessing %u stoping procedure \n",store_results_in_space,list_size,this_list_total_pictures_count);
                              break;
                          } else
                          {
                             strncpy(list[this_list_total_pictures_count].filename,filename,512);
                             printf("Filename Coming from memory %s ",list[this_list_total_pictures_count].filename);
                          }
                       }
                        ++this_list_total_pictures_count;
                      }

                 }
                 /* END of Read loop that fills in line variable */
             }
           }
           fclose (fp);

           last_list_total_count=this_list_total_count;
           last_list_total_pictures_count=this_list_total_pictures_count;

           fprintf(stderr," %u of total %u files are pictures \n",last_list_total_pictures_count,last_list_total_count);
       }

     InputParser_Destroy(ipc);

    return 1;
  }
  return 0;
}


unsigned int GetTotalFilesInDirectory()
{
    return last_list_total_count;
}


unsigned int GetTotalViewableFilesInDirectory()
{
    return last_list_total_pictures_count;
}

unsigned int GetViewableFilenameforFile(unsigned int file_id,char *directory,char *filename)
{
  //  fprintf(stderr,"This code section is a little sloppy , may segfault \n");
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;
    if ( directory == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 2 parameter ? \n"); return 0; }
    if ( filename == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 2 parameter ? \n"); return 0; }

    fprintf(stderr,"Copying `%s` \n",list[file_id].filename);
    strcpy(filename,directory);
    strcat(filename,list[file_id].filename);

    return 1;
}
