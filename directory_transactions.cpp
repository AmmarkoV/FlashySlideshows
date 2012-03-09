#include "directory_transactions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "slideshow.h"
#include "visuals.h"
#include "directory_listing.h"
/*
 *
 *
 *      THIS IS THE IMPLEMENTATION OF THE MOVE , COPY , RESIZE
 *      CAPABILITIES
 *
 *
*/

int escape_str(char *in_str,char *out_str)
{
  unsigned int i=0,o=0;
    while (i<strlen(in_str))
     {
        if ( in_str[i]=='\'' ) { out_str[o]='\\'; ++o; out_str[o]='\''; } else
        if ( in_str[i]=='"' ) { out_str[o]='\\'; ++o; out_str[o]='"'; } else
        if ( in_str[i]=='(' ) { out_str[o]='\\'; ++o; out_str[o]='('; } else
        if ( in_str[i]==')' ) { out_str[o]='\\'; ++o; out_str[o]=')'; } else
        if ( in_str[i]=='*' ) { out_str[o]='\\'; ++o; out_str[o]='*'; } else
        if ( in_str[i]=='!' ) { out_str[o]='\\'; ++o; out_str[o]='!'; } else
        if ( in_str[i]=='?' ) { out_str[o]='\\'; ++o; out_str[o]='\?'; } else
          {
            out_str[o]=in_str[i];
          }
      ++o;
      ++i;
     }
    out_str[o]=0; //Null terminator , hasta la vista baby :P
  return 1;
}

int CreateDir(char * dir)
{
  return ( mkdir(dir,S_IRWXU) == 0 ) ;
}

int RescaleFileToDir(unsigned int file_id,char * dir)
{
  //  fprintf(stderr,"This code segment is buggy , TOOD remove the ~ and issue a mkdir call to create a subdirectory for rescaled files..!\n");
  //  return 0;

    if (!frame.allow_operation_resize) { return 0; }
    /*
    sprintf(rescale_operation,"convert %s%s -resize \"%s>\" -size \"%s\" xc:white +swap -gravity center -composite %s%s-out.jpg",
            (char*)frame.album_directory,list[file_id].filename,
             frame.rescale_resolution_string,frame.rescale_resolution_string,
            (char*)frame.album_directory,list[file_id].filename);*/

    char rescale_operation[MAX_PATH]={0};
    char raw_filename[MAX_PATH]={0};
    sprintf(raw_filename,"%s%s",(char*)frame.album_directory,list[file_id].filename);

/*
    char escaped_filename[MAX_PATH];
    escape_str(raw_filename,escaped_filename);*/

    sprintf(rescale_operation,"convert \"%s\" -resize \"%s>^\" \"%s%s-resized.jpg\"&",
             raw_filename,
             frame.rescale_resolution_string,
             dir,
             list[file_id].filename);

    fprintf(stderr,"Executing %s \n",rescale_operation);
    int i=system(rescale_operation);

  return (i==0);
}

int MoveFileToDir(unsigned int file_id,char * dir)
{
  //  fprintf(stderr,"This code segment is buggy , TOOD remove the ~ and issue a mkdir call to create a subdirectory for moved files..!\n");
  //  return 0;

    if (!frame.allow_operation_move) { return 0; }

    char raw_filename[MAX_PATH]={0};
    sprintf(raw_filename,"%s%s",(char*)frame.album_directory,list[file_id].filename);
//    char escaped_filename[2048];
//    escape_str(raw_filename,escaped_filename);

    char move_operation[MAX_PATH]={0};
    strcpy(move_operation,"mv \"");
    strcat(move_operation,raw_filename);
    strcat(move_operation,"\" \"");
    strcat(move_operation,dir);
    strcat(move_operation,"\"&");
    fprintf(stderr,"Executing %s \n",move_operation);
    int i=system(move_operation);

  return (i==0);
}

int CopyFileToDir(unsigned int file_id,char * dir)
{
  //  fprintf(stderr,"This code segment is buggy , TOOD remove the ~ and issue a mkdir call to create a subdirectory for moved files..!\n");
  //  return 0;

    if (!frame.allow_operation_copy) { return 0; }

    char raw_filename[MAX_PATH]={0};
    sprintf(raw_filename,"%s%s",(char*)frame.album_directory,list[file_id].filename);
//    char escaped_filename[2048];
//    escape_str(raw_filename,escaped_filename);

    char move_operation[MAX_PATH]={0};
    strcpy(move_operation,"cp \"");
    strcat(move_operation,raw_filename);
    strcat(move_operation,"\" \"");
    strcat(move_operation,dir);
    strcat(move_operation,"\"&");
    fprintf(stderr,"Executing %s \n",move_operation);
    int i=system(move_operation);

  return (i==0);
}


int CreateDirsForMoveOrRescale(char * dir,unsigned int resize2_copy1_move0,unsigned int sort_id)
{
   if (resize2_copy1_move0==2)
   {
      sprintf(dir,"%s%s",(char*)frame.album_directory,(char*)frame.resize_directory);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Base Dir %s\n",dir); }
      sprintf(dir,"%s%s/Category%u",(char*)frame.album_directory,(char*)frame.resize_directory,sort_id);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Category Dir %s for id %u \n",dir,sort_id); }
      strcat(dir,"/");
   } else
  if (resize2_copy1_move0==1)
   {
      sprintf(dir,"%s%s",(char*)frame.album_directory,(char*)frame.copy_directory);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Base Dir %s\n",dir); }
      sprintf(dir,"%s%s/Category%u",(char*)frame.album_directory,(char*)frame.copy_directory,sort_id);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Category Dir %s for id %u \n",dir,sort_id); }
      strcat(dir,"/");
   } else
   {
      sprintf(dir,"%s%s",(char*)frame.album_directory,(char*)frame.move_directory);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Base Dir %s\n",dir); }
      sprintf(dir,"%s%s/Category%u",(char*)frame.album_directory,(char*)frame.move_directory,sort_id);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Category Dir %s for id %u \n",dir,sort_id); }
      strcat(dir,"/");
   }
  return 1;
}


int MoveOrRescaleOrCopyFileToDir(unsigned int file_id,unsigned int sort_id)
{
  char dir[MAX_PATH]={0};
  char msg[512]={0};

  if ((!frame.allow_operation_resize)&&(!frame.allow_operation_move)&&(!frame.allow_operation_copy) )
    {
      fprintf(stderr,"MoveOrRescaleOrCopyFileToDir Activated but , Moving , Resizing and Copying are disabled , please start program with -file_move or -file_copy or -file_resize\n");
      return 0;
    } else
  if ((frame.allow_operation_resize)&&(frame.allow_operation_move) )
    {
      fprintf(stderr,"Both Moving and Resizing are enabled, using resize function\n");

      sprintf(msg,"Resizing Category %u",sort_id);
      NewLabel(frame.vx,frame.vy,msg);

      CreateDirsForMoveOrRescale(dir,1,sort_id);
      return RescaleFileToDir(file_id,dir);
    } else
  if (frame.allow_operation_resize )
   {
     sprintf(msg,"Resizing Category %u",sort_id);
     NewLabel(frame.vx,frame.vy,msg);

     CreateDirsForMoveOrRescale(dir,2,sort_id);
     return RescaleFileToDir(file_id,dir);
    } else
  if (frame.allow_operation_copy)
    {
     sprintf(msg,"Copying Category %u",sort_id);
     NewLabel(frame.vx,frame.vy,msg);

     CreateDirsForMoveOrRescale(dir,1,sort_id);
     return CopyFileToDir(file_id,dir);
    } else
  if (frame.allow_operation_move)
    {
     sprintf(msg,"Moving Category %u",sort_id);
     NewLabel(frame.vx,frame.vy,msg);

     CreateDirsForMoveOrRescale(dir,0,sort_id);
     return MoveFileToDir(file_id,dir);
    }


 return 0;
}

