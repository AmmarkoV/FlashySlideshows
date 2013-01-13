/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammar.gr
Written by Ammar Qammaz a.k.a. AmmarkoV 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
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
 *      THIS IS THE IMPLEMENTATION OF THE MOVE , COPY , RESIZE , LINK
 *      CAPABILITIES
 *
 *
*/

enum DirectoryTransactionOperations
{
   NONE = 0 ,
   MOVE_TO_DIR ,
   COPY_TO_DIR ,
   RESIZE_TO_DIR ,
   LINK_TO_DIR
};


int escape_str(char *in_str,char *out_str)
{
  unsigned int i=0,o=0;
    while (i<strlen(in_str))
     {
        if ( in_str[i]=='\'') { out_str[o]='\\'; ++o; out_str[o]='\''; } else
        if ( in_str[i]=='"' ) { out_str[o]='\\'; ++o; out_str[o]='"';  } else
        if ( in_str[i]=='(' ) { out_str[o]='\\'; ++o; out_str[o]='(';  } else
        if ( in_str[i]==')' ) { out_str[o]='\\'; ++o; out_str[o]=')';  } else
        if ( in_str[i]=='*' ) { out_str[o]='\\'; ++o; out_str[o]='*';  } else
        if ( in_str[i]=='!' ) { out_str[o]='\\'; ++o; out_str[o]='!';  } else
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

    if (!frame.allow_operation_resize) { fprintf(stderr,"LinkFileToDir but frame.allow_operation_resize is not enabled\n"); return 0; }
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

    sprintf(rescale_operation,"nice -n 19 convert \"%s\" -resize \"%s>^\" \"%s%s-resized.jpg\"&",
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

    if (!frame.allow_operation_move) {  fprintf(stderr,"LinkFileToDir but frame.allow_operation_move is not enabled\n"); return 0; }

    char raw_filename[MAX_PATH]={0};
    sprintf(raw_filename,"%s%s",(char*)frame.album_directory,list[file_id].filename);
//    char escaped_filename[2048];
//    escape_str(raw_filename,escaped_filename);

    char move_operation[MAX_PATH]={0};
    strcpy(move_operation,"ionice -c 3 mv \"");
    strcat(move_operation,raw_filename);
    strcat(move_operation,"\" \"");
    strcat(move_operation,dir);
    strcat(move_operation,"\"&");
    fprintf(stderr,"Executing %s \n",move_operation);
    int i=system(move_operation);

  return (i==0);
}


int LinkFileToDir(unsigned int file_id,char * dir)
{
    fprintf(stderr,"LinkFileToDir not implemented yet \n");
    return 0;

    if (!frame.allow_operation_link) { fprintf(stderr,"LinkFileToDir but frame.allow_operation_link is not enabled\n"); return 0; }

    char raw_filename[MAX_PATH]={0};
    sprintf(raw_filename,"%s%s",(char*)frame.album_directory,list[file_id].filename);
//    char escaped_filename[2048];
//    escape_str(raw_filename,escaped_filename);

    char move_operation[MAX_PATH]={0};
    strcpy(move_operation,"ln -s \"");
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
    strcpy(move_operation,"ionice -c 3 cp \"");
    strcat(move_operation,raw_filename);
    strcat(move_operation,"\" \"");
    strcat(move_operation,dir);
    strcat(move_operation,"\"&");
    fprintf(stderr,"Executing %s \n",move_operation);
    int i=system(move_operation);

  return (i==0);
}


int CreateDirsForTransaction(char * dir,unsigned int trans_type,unsigned int sort_id)
{
  switch (trans_type)
  {
    //-----------------------------------------------
    case MOVE_TO_DIR :

      sprintf(dir,"%s%s",(char*)frame.album_directory,(char*)frame.move_directory);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Base Dir %s\n",dir); }
      sprintf(dir,"%s%s/Category%u",(char*)frame.album_directory,(char*)frame.move_directory,sort_id);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Category Dir %s for id %u \n",dir,sort_id); }
      strcat(dir,"/");

    break;

    //-----------------------------------------------
    case COPY_TO_DIR :

      sprintf(dir,"%s%s",(char*)frame.album_directory,(char*)frame.copy_directory);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Base Dir %s\n",dir); }
      sprintf(dir,"%s%s/Category%u",(char*)frame.album_directory,(char*)frame.copy_directory,sort_id);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Category Dir %s for id %u \n",dir,sort_id); }
      strcat(dir,"/");

    break;


    //-----------------------------------------------
    case RESIZE_TO_DIR :

      sprintf(dir,"%s%s",(char*)frame.album_directory,(char*)frame.resize_directory);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Base Dir %s\n",dir); }
      sprintf(dir,"%s%s/Category%u",(char*)frame.album_directory,(char*)frame.resize_directory,sort_id);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Category Dir %s for id %u \n",dir,sort_id); }
      strcat(dir,"/");

    break;


    //-----------------------------------------------
    case LINK_TO_DIR :

      sprintf(dir,"%s%s",(char*)frame.album_directory,(char*)frame.resize_directory);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Base Dir %s\n",dir); }
      sprintf(dir,"%s%s/Category%u",(char*)frame.album_directory,(char*)frame.resize_directory,sort_id);
      if ( CreateDir(dir) ) { fprintf(stderr,"Created Category Dir %s for id %u \n",dir,sort_id); }
      strcat(dir,"/");

    break;
  };

  return 1;
}


int PerformDirectoryTransaction(unsigned int file_id,unsigned int sort_id)
{
  char dir[MAX_PATH]={0};
  char msg[512]={0};

  int check_opts = 0;
  if (frame.allow_operation_move) { ++check_opts; }
  if (frame.allow_operation_copy) { ++check_opts; }
  if (frame.allow_operation_resize) { ++check_opts; }
  if (frame.allow_operation_link) { ++check_opts; }


  if (check_opts == 0 ) { fprintf(stderr,"No options enabled for directory transaction , doing nothing \n"); return 0; }

  if (frame.allow_operation_resize )
   {
     sprintf(msg,"Resizing Category %u",sort_id);
     NewLabel(frame.vx,frame.vy,msg);

     CreateDirsForTransaction(dir,RESIZE_TO_DIR,sort_id);
     return RescaleFileToDir(file_id,dir);
    } else
  if (frame.allow_operation_copy)
    {
     sprintf(msg,"Copying Category %u",sort_id);
     NewLabel(frame.vx,frame.vy,msg);

     CreateDirsForTransaction(dir,COPY_TO_DIR,sort_id);
     return CopyFileToDir(file_id,dir);
    } else
  if (frame.allow_operation_move)
    {
     sprintf(msg,"Moving Category %u",sort_id);
     NewLabel(frame.vx,frame.vy,msg);

     CreateDirsForTransaction(dir,MOVE_TO_DIR,sort_id);
     return MoveFileToDir(file_id,dir);
    } else
  if (frame.allow_operation_link)
    {
     sprintf(msg,"Linking Category %u",sort_id);
     NewLabel(frame.vx,frame.vy,msg);

     CreateDirsForTransaction(dir,LINK_TO_DIR,sort_id);
     return LinkFileToDir(file_id,dir);
    }



 return 0;
}

