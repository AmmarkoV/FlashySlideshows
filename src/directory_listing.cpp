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

#include "directory_listing.h"
#include "directory_sorting.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wx/msgdlg.h>
#include "wxwidgets_stuff.h"
#include "wx/dir.h"
#include "wx/filename.h"
#include "wx/dirdlg.h"
#include <wx/filefn.h>


unsigned int FORCE_STOP_LISTING=0; // EMERGENCY SWITCH :P
unsigned int DONT_ASK_USER_FOR_STOP=0; // DONT BUG WITH QUESTIONS IF THE USER ANSWERED :P
unsigned int MAX_TIME_FOR_OPERATION_MS=3000; // this is considered a sane amount of time for access to the file system :p
struct timeval start_time,current_time,difference_time;

char root_directory[2048]={0};
struct FilenameHolder * list=0;

unsigned long last_list_time_ms=0;
unsigned int list_size=0;
unsigned int pictures_count=0;



unsigned long timeval_diff2 ( struct timeval *difference, struct timeval *end_time, struct timeval *start_time )
{
   struct timeval temp_diff;

   if(difference==0) { difference=&temp_diff; }
   difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
   difference->tv_usec=end_time->tv_usec-start_time->tv_usec;
  // Using while instead of if below makes the code slightly more robust.

  while(difference->tv_usec<0)
  {
    difference->tv_usec+=1000000;
    difference->tv_sec -=1;
  }
  return 1000000LL*difference->tv_sec+ difference->tv_usec;
}

void PrintDirectoryList()
{
  return ;
  unsigned int i=0;
  for (i=0; i<pictures_count; i++) { fprintf(stderr,"%u - %s\n",i,list[i].filename); }
}

void PrintDirectoryListItem(unsigned int item)
{
  return ;
  if (list==0) {return;}
  if (item>=pictures_count) {return; }
  fprintf(stderr,"%u - %s\n",item,list[item].filename);
}

void FreeDirectoryList()
{
  if ( list != 0 ) { fprintf(stderr,"Freeing list memory \n"); free(list); }
  list=0;
}

void AllocateDirectoryList(unsigned int requested_size)
{
 if ( list != 0 ) { fprintf(stderr,"List already contained data , freeing memory \n"); free(list); }
 list = ( struct FilenameHolder * ) malloc(   sizeof(struct FilenameHolder) * (requested_size+5)  );

 unsigned int i=0;
 for ( i=0; i<requested_size; i++)
 {
    list[i].year=0;
    list[i].month=0;
    list[i].day=0;
    list[i].day_of_year=0;
    list[i].hour=0;
    list[i].minute=0;
    list[i].second=0;
    list[i].filesize=0;
    strcpy(list[i].subdir,"\0");
    strcpy(list[i].filename,"\0");
 }


 if ( list != 0 ) { fprintf(stderr,"Allocated %u records of filename_holders\n",requested_size); }
 list_size=requested_size;

 pictures_count=0;
}

void ensure_ending_with_slash(char * str)
{
  if (str==0) { return ; }
  unsigned int string_length = strlen(str);
  if (string_length==0) { return ; }
  if (str[strlen(str)-1]=='/') { return; }
  strcat(str,"/");
}

inline wxString _U2(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}

int ExtensionIsJPEGPicture(wxString *extension)
{
      unsigned int is_a_picture=0;
      if ( extension->CmpNoCase(wxT("JPG"))==0 )       {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("JPEG"))==0)  {is_a_picture=1;}

  return is_a_picture;
}

int ExtensionIsPicture(wxString *extension)
{
      if ( extension->Length()<=2 )  { return 0; } //No way it is a picture extension :P

      unsigned int is_a_picture=0;
      if      ( extension->CmpNoCase(wxT("PNG"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("PNM"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("BMP"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("GIF"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("PCX"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("XPM"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("TIF"))==0 )  {is_a_picture=1;}

      if ( ExtensionIsJPEGPicture(extension) ) { is_a_picture=1; }

  return is_a_picture;
}

int AddFileIfItIsAPicture(char *thedirectory,char *subdir,wxString *filename,unsigned int count_only)
{
      if (filename->Length()<=4) { fprintf(stderr,"Filenames must be bigger than .jpg for example\n"); return 0; }

       //char filename_str[1024];
       //strncpy(filename_str,(const char*) filename->mb_str(wxConvUTF8),1024);
       //fprintf(stderr,"AddFileIfItIsAPicture called for %s , %s , %s \n",thedirectory,subdir,filename_str);

      wxString dir_and_filename; dir_and_filename.clear();
      dir_and_filename<<_U2(thedirectory);
      if (thedirectory[strlen(thedirectory)-1]!='/') { dir_and_filename<<_U2("/"); }
      dir_and_filename<<*filename;

      wxFileName fname(dir_and_filename);
      wxString extension=fname.GetExt();
      wxString fullname =fname.GetFullName();

      if (!fname.FileExists( )) { fprintf(stderr,"Not a file ..!\n"); }
          else
     // if (fname.DirExists( )) { fprintf(stderr," Not a directory ..!\n"); }
     //     else
      if ( ExtensionIsPicture(&extension) )
        {
          if (!count_only)
          {
            if ( pictures_count >= list_size )  { /*Our list cannot acommodate any more data error*/ return 0; }

            strncpy(list[pictures_count].filename,(const char*) fullname.mb_str(wxConvUTF8),256);
            if (subdir==0) { strncpy(list[pictures_count].subdir,"\0",1024);   } else
                           { strncpy(list[pictures_count].subdir,subdir,1024); }

            wxDateTime mod_time=fname.GetModificationTime();
            list[pictures_count].year=mod_time.GetYear();
            list[pictures_count].month=mod_time.GetMonth();
            list[pictures_count].day=mod_time.GetDay();
            list[pictures_count].day_of_year=mod_time.GetDayOfYear();
            list[pictures_count].hour=mod_time.GetHour();
            list[pictures_count].minute=mod_time.GetMinute();
            list[pictures_count].second=mod_time.GetSecond();

            if (ExtensionIsJPEGPicture(&extension)) { list[pictures_count].is_jpeg=1; } else
                                                    { list[pictures_count].is_jpeg=0; }

            wxULongLong size_long_long = fname.GetSize();
            list[pictures_count].filesize=(unsigned int) size_long_long.ToULong();

            ++pictures_count;
          }
          return 1;
        }
  return 0;
}


int UserWantsToStop()
{
  if (DONT_ASK_USER_FOR_STOP) { return 1; }
  char command[512]={0};
  sprintf(command,"gdialog --title \"Operation taking too long\" --yesno \"Directory listing operation is taking too long (%u secs)\nWould you like to stop it , next warning at %u secs ?\"",MAX_TIME_FOR_OPERATION_MS/1000,2*MAX_TIME_FOR_OPERATION_MS/1000);
  int i=system(command);
  if (i==0) { fprintf(stderr,"User Wants to Stop script\n"); DONT_ASK_USER_FOR_STOP=1; return 1; } else
            { fprintf(stderr,"User Doesnt want to Stop script\n"); return 0; }
return 1;
}

unsigned int GetDirectoryList(char * thedirectory,char *subdir,unsigned int space_to_allocate,unsigned int comp_func,unsigned int asc_desc,unsigned int randomized,unsigned int recursive)
{
  if (FORCE_STOP_LISTING) { fprintf(stderr,"Emergency stop listing\n"); return 0; }

  unsigned int this_list_total_pictures_count=0;
  unsigned int count_only=0,bottom_level=0;
  if ( recursive <= 1 ) { bottom_level=1; }
  if (space_to_allocate==0) { count_only=1; } else
  if ((space_to_allocate!=0)&&(list==0)) { fprintf(stderr,"List not allocated for GetDirectoryList call\n"); return 0; }

  //if (recursive>MAX_RECURSION) { fprintf(stderr,"Recursion has reached a depth of 100 levels , stopping it :P \n"); return 0; } else
  if (bottom_level)
   {
     gettimeofday(&start_time,0x0);
     strcpy(root_directory,thedirectory);
   }

  wxDir dir(_U2(thedirectory));
  if ( !dir.IsOpened() )  {  fprintf(stderr,"Cannot open dir `%s`\n",thedirectory);  return 0; }
  wxString dirname,filename,extension,fullname,filespec = wxT("*.*");
  int cont = dir.GetFirst(&filename,filespec,wxDIR_FILES|wxDIR_HIDDEN);
  while ( cont )
   {
      if ( AddFileIfItIsAPicture(thedirectory,subdir,&filename,count_only) ) { ++this_list_total_pictures_count; }
      /* WE CONTINUE THE LOOP UNTIL THE END!*/
      cont = dir.GetNext(&filename);
   }



     gettimeofday(&current_time,0x0);
     unsigned int curtime=timeval_diff2(&difference_time,&current_time,&start_time)/1000;
     //If recursion is enabled , and we timed out , lets ask the user if he wants to continue
     if ( (curtime>MAX_TIME_FOR_OPERATION_MS) && (recursive!=0) )
      {
        if (UserWantsToStop()) {
                                 /*To stop directory listing , we stop recursion here and now..! and set the force stop flag*/
                                 FORCE_STOP_LISTING=1;
                                 recursive=0;
                               } else
                               {
                                 //Compensate for the time lost waiting for user input.. :P
                                 gettimeofday(&current_time,0x0);
                                 curtime=(timeval_diff2(&difference_time,&current_time,&start_time)/1000 )-curtime;
                                 MAX_TIME_FOR_OPERATION_MS=curtime+=(MAX_TIME_FOR_OPERATION_MS*2);
                               }
      }

if (recursive>0) //Recursion Enabled
{ /* RECURSIVE FUNCTIONALITY START*/
  filespec.clear();
  if ( recursive == 1 ) { fprintf(stderr,"Starting Recursive dir listing @ %s \n",thedirectory); }
  char recursive_dir[1024]={0},new_subdir[1024]={0},dirname_cstr[1024]={0};
  wxDir dir2(_U2(thedirectory));
  if (dir2.IsOpened())
  {
   int cont = dir2.GetFirst(&dirname,filespec,wxDIR_DIRS);
   while ( cont )
   {
      strncpy(dirname_cstr,(const char*) dirname.mb_str(wxConvUTF8),1024);
      if ( ( strlen(dirname_cstr)==1 )&&(dirname_cstr[0]=='/') ) { } else
      if ( ( strlen(dirname_cstr)==1 )&&(dirname_cstr[0]=='.') ) { } else
      if ( ( strlen(dirname_cstr)==2 )&&(dirname_cstr[0]=='.')&&(dirname_cstr[1]=='.') ) { } else
      if ( strlen(dirname_cstr)>0 )
      {//NO POINT IN RECURSING IN THE SAME (PRACTICLY) DIR :P
       ensure_ending_with_slash(dirname_cstr);

       //fprintf(stderr,"WAS The Directory is `%s\n` ",thedirectory);
       //fprintf(stderr,"WAS Running Recursive (level %u) addition to dir `%s`\n",recursive,recursive_dir);
       //fprintf(stderr,"WAS Subdir is `%s`\n",new_subdir);

       sprintf(recursive_dir,"%s%s",thedirectory,dirname_cstr);

       if ( subdir == 0 )
       { sprintf(new_subdir,"%s",dirname_cstr); } else
       {
         ensure_ending_with_slash(subdir);
         sprintf(new_subdir,"%s%s",subdir,dirname_cstr);
       }

       //fprintf(stderr,"Running Recursive (level %u) addition to dir `%s`\n",recursive,recursive_dir);
       //fprintf(stderr,"Subdir is `%s`\n",new_subdir);
       unsigned int recursive_pictures_added=GetDirectoryList(recursive_dir,new_subdir,space_to_allocate,comp_func,asc_desc,randomized,recursive+1);
       //fprintf(stderr," returned %u ( %u until now ) pictures\n",recursive_pictures_added,this_list_total_pictures_count);

       this_list_total_pictures_count+=recursive_pictures_added;
      }
      /* WE CONTINUE THE LOOP UNTIL THE END!*/
      cont = dir2.GetNext(&dirname);
   }
  }
   if (bottom_level) { fprintf(stderr,"Recursive dir listing is done\n"); }
  /* RECURSIVE FUNCTIONALITY END */
}

   if (bottom_level)
   {
     gettimeofday(&current_time,0x0);
     last_list_time_ms=timeval_diff2(&difference_time,&current_time,&start_time)/1000;
     fprintf(stderr,"Getting directory list took %u ms\n",(unsigned int) last_list_time_ms);
   }

  if (count_only)
    {
        return this_list_total_pictures_count;
    } else
    {
      if ( (this_list_total_pictures_count>1)&&(bottom_level))
       {
         if (randomized)
          {
             fprintf(stderr,"Randomizing directory list.. ");
             RandomizeDirectoryListSorting(0,this_list_total_pictures_count);
          }   else
          {
             fprintf(stderr,"Sorting directory modification time.. ");
             SortDirectoryList(0,this_list_total_pictures_count,comp_func,asc_desc);
          }
         fprintf(stderr,"done\n");

        //PrintDirectoryList();
       }
    }

  return this_list_total_pictures_count;
}

unsigned int CountPicturesInDirectory(char * thedirectory,int recursive)
{
  FORCE_STOP_LISTING=0;
  return GetDirectoryList(thedirectory,0,0,0,0,0,recursive); /*Calling GetDirectoryList with 0 size only counts files!*/
}


unsigned int LoadPicturesOfDirectory(char * thedirectory,unsigned int comp_func,unsigned int asc_desc,unsigned int randomize,unsigned int recursive)
{

    printf("Trying to load directory list for %s .. ",thedirectory);
    unsigned int total_pictures_in_dir=CountPicturesInDirectory(thedirectory,recursive);
    printf(" %u total pictures inside \n ",total_pictures_in_dir);

    AllocateDirectoryList(total_pictures_in_dir+4);
    fprintf(stderr,"Album directory has %u pictures inside \n",total_pictures_in_dir);
    if (total_pictures_in_dir==0)
      {
         fprintf(stderr,"Could not find any pictures to display \n");
         return 0;
      }

    FORCE_STOP_LISTING=0;
    total_pictures_in_dir=GetDirectoryList((char*)thedirectory,0,total_pictures_in_dir,comp_func,asc_desc,randomize,recursive); /* Load Directory List */
 return 1;
}


/*
 *  ----------------------------
 *  Getters & Setters following
 *  ----------------------------
*/
unsigned int GetTotalViewableFilesInDirectory()
{
    return pictures_count;
}


unsigned int GetFullFilenameforFile(unsigned int file_id,char *filename)
{
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;
    if ( filename == 0 )  { fprintf(stderr,"GetViewableFilenameforFile called with wrong 2 parameter ? \n"); return 0; }

    strcpy(filename,root_directory);
    if (strlen(list[file_id].subdir)!=0) { strcat(filename,list[file_id].subdir); }
    strcat(filename,list[file_id].filename);

    return 1;
}

unsigned int GetViewableFilenameforFile(unsigned int file_id,char *directory,char *filename)
{
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;
    if ( directory == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 2 parameter ? \n"); return 0; }
    if ( filename == 0 )  { fprintf(stderr,"GetViewableFilenameforFile called with wrong 3 parameter ? \n"); return 0; }

    strcpy(filename,directory);
    if (strlen(list[file_id].subdir)!=0) { strcat(filename,list[file_id].subdir); }
    strcat(filename,list[file_id].filename);

    return 1;
}

char * GetFilename(unsigned int file_id)
{
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;
    return list[file_id].filename;
}

char * GetSubdir(unsigned int file_id)
{
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;
    return list[file_id].subdir;
}

unsigned int GetItemDate(unsigned int file_id,unsigned int data)
{
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;

    if ( data == 2 ) { return list[file_id].year;  } else
    if ( data == 1 ) { return list[file_id].month; } else
    if ( data == 0 ) { return list[file_id].day;   } else
    if ( data == 3 ) { return list[file_id].hour;  } else
    if ( data == 4 ) { return list[file_id].minute;} else
    if ( data == 5 ) { return list[file_id].second;}

    return 0;
}



