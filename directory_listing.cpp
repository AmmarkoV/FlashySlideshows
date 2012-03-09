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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wxwidgets_stuff.h"
#include "wx/dir.h"
#include "wx/filename.h"
#include "wx/dirdlg.h"
#include <wx/filefn.h>


unsigned int MAX_RECURSION=100; // 100 levels deep is very deep :p

struct FilenameHolder * list=0;

unsigned int list_size=0;
unsigned int pictures_count=0;


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
 for ( i=0; i<requested_size; i++) { strcpy(list[i].filename,"\0"); }
 if ( list != 0 ) { fprintf(stderr,"Allocated %u records of filename_holders\n",requested_size); }
 list_size=requested_size;

 pictures_count=0;
}



inline wxString _U2(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}

int ExtensionIsPicture(wxString *extension)
{
      unsigned int is_a_picture=0;
      if ( extension->CmpNoCase(wxT("JPG"))==0 )       {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("JPEG"))==0)  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("PNG"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("PNM"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("BMP"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("GIF"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("PCX"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("XPM"))==0 )  {is_a_picture=1;}
      else if ( extension->CmpNoCase(wxT("TIF"))==0 )  {is_a_picture=1;}

  return is_a_picture;
}

int AddFileIfItIsAPicture(char *thedirectory,char *subdir,wxString *filename,unsigned int count_only)
{
      wxString dir_and_filename; dir_and_filename.clear();
      dir_and_filename<<_U2(thedirectory);
      dir_and_filename<<_U2("/");
      dir_and_filename<<*filename;

      wxFileName fname(dir_and_filename);
      wxString extension=fname.GetExt();
      wxString fullname =fname.GetFullName();

      if ( ExtensionIsPicture(&extension) )
        {
          if (!count_only)
          {
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

            wxULongLong size_long_long = fname.GetSize();
            list[pictures_count].filesize=(unsigned int) size_long_long.ToULong();

            if ( pictures_count >= list_size )  { /*Our list cannot acommodate any more data error*/ return 0; }
            ++pictures_count;
          }
          return 1;
        }
  return 0;
}




unsigned int GetDirectoryList(char * thedirectory,char *subdir,unsigned int space_to_allocate,unsigned int comp_func,unsigned int asc_desc,unsigned int recursive)
{
  if (recursive>MAX_RECURSION) { fprintf(stderr,"Recursion has reached a depth of 100 levels , stopping it :P \n"); }


  wxDir dir(_U2(thedirectory));
  if ( !dir.IsOpened() )  { return 0; }

  unsigned int this_list_total_pictures_count=0;
  unsigned int count_only=0;
  if(space_to_allocate==0) {count_only=1;}
  if ((space_to_allocate!=0)&&(list==0)) { fprintf(stderr,"List not allocated for GetDirectoryList call\n"); return 0; }

  wxString filename,extension,fullname;
  wxString filespec = wxT("*.*");
  int cont = dir.GetFirst(&filename,filespec,wxDIR_FILES|wxDIR_HIDDEN);
  while ( cont )
   {
      if ( AddFileIfItIsAPicture(thedirectory,subdir,&filename,count_only) ) { ++this_list_total_pictures_count; }
      /* WE CONTINUE THE LOOP UNTIL THE END!*/
      cont = dir.GetNext(&filename);
   }

if (recursive>0)
{ /* RECURSIVE FUNCTIONALITY */
  if ( recursive == 1 ) { fprintf(stderr,"Starting Recursive dir listing @ %s \n",thedirectory); }
  char recursive_dir[1024];
  char new_subdir[1024];
  wxDir dir2(_U2(thedirectory));
  if ( !dir2.IsOpened() )  { return 0; }
  filespec.clear();
  int cont = dir2.GetFirst(&filename,filespec,wxDIR_DIRS);
  while ( cont )
   {
      sprintf(recursive_dir,"%s%s/",thedirectory,(const char*) filename.mb_str(wxConvUTF8));
      if ( subdir == 0 ) { sprintf(new_subdir,"%s/",(const char*) filename.mb_str(wxConvUTF8)); } else
                         { sprintf(new_subdir,"%s%s/",subdir,(const char*) filename.mb_str(wxConvUTF8)); }

      fprintf(stderr,"Running Recursive (level %u) addition to dir %s ",recursive,recursive_dir);
      fprintf(stderr,"Subdir is %s ",new_subdir);
      unsigned int recursive_pictures_added=GetDirectoryList(recursive_dir,new_subdir,space_to_allocate,comp_func,asc_desc,recursive+1);
      fprintf(stderr," gave %u pictures\n",recursive_pictures_added);

      this_list_total_pictures_count+=recursive_pictures_added;
      /* WE CONTINUE THE LOOP UNTIL THE END!*/
      cont = dir2.GetNext(&filename);
   }
   if ( recursive == 1 ) { fprintf(stderr,"Recursive dir listing is done\n"); }
}



  if (count_only)
    {
        return this_list_total_pictures_count;
    } else
    {
      if ( (this_list_total_pictures_count>1)&&(recursive <= 1))
       {
        fprintf(stderr,"Sorting directory modification time.. ");
        SortDirectoryList(0,this_list_total_pictures_count,comp_func,asc_desc);
        fprintf(stderr,"done\n");
        //PrintDirectoryList();
       }
    }

  return this_list_total_pictures_count;
}

unsigned int CountPicturesInDirectory(char * thedirectory,int recursive)
{
  return GetDirectoryList(thedirectory,0,0,0,0,recursive); /*Calling GetDirectoryList with 0 size only counts files!*/
}


unsigned int LoadPicturesOfDirectory(char * thedirectory,unsigned int comp_func,unsigned int asc_desc,unsigned int recursive)
{
    unsigned int total_pictures_in_dir=CountPicturesInDirectory(thedirectory,recursive);

    AllocateDirectoryList(total_pictures_in_dir+4);
    fprintf(stderr,"Album directory has %u pictures inside \n",total_pictures_in_dir);
    if (total_pictures_in_dir==0)
      {
         fprintf(stderr,"Could not find any pictures to display \n");
         return 0;
      }

    total_pictures_in_dir=GetDirectoryList((char*)thedirectory,0,total_pictures_in_dir,comp_func,asc_desc,recursive); /* Load Directory List */
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

unsigned int GetViewableFilenameforFile(unsigned int file_id,char *directory,char *filename)
{
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;
    if ( directory == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 2 parameter ? \n"); return 0; }
    if ( filename == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 3 parameter ? \n"); return 0; }

//    if (PrintPictureLoadingMsg()) fprintf(stderr,"Copying picture %u = `%s` \n",file_id,list[file_id].filename);
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

unsigned int GetItemDate(unsigned int file_id,unsigned int data)
{
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;

    if ( data == 2 ) { return list[file_id].year; } else
    if ( data == 1 ) { return list[file_id].month; } else
    if ( data == 0 ) { return list[file_id].day; } else
    if ( data == 3 ) { return list[file_id].hour; } else
    if ( data == 4 ) { return list[file_id].minute; } else
    if ( data == 5 ) { return list[file_id].second; }

    return 0;
}



