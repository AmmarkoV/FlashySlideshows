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
#include "slideshow.h"
#include "environment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wxwidgets_stuff.h"
#include "wx/dir.h"
#include "wx/filename.h"
#include "wx/dirdlg.h"
#include <wx/filefn.h>



struct FilenameHolder
{
     unsigned int year;
     unsigned int month;
     unsigned int day;
     unsigned int day_of_year;
     unsigned int hour;
     unsigned int minute;
     unsigned int second;

     unsigned int filesize;

     char filename[512];
};


struct FilenameHolder * list=0;

unsigned int list_size=0;
unsigned int last_list_total_countNOTUSEDANYWHERE=0; // We dont actually care about the total count of files , only about the pictures thus NOTUSEDANYWHERE :P
unsigned int last_list_total_pictures_count=0;



unsigned int FirstItemSmallerOrEqualToSecond(unsigned int item_a,unsigned int item_b,unsigned int comp_func,unsigned int asc_desc)
{
    unsigned int true_value=1;
    unsigned int false_value=0;

    if (!asc_desc) { /*IF descending , we swap the result!*/ true_value=0; false_value=1;}

    if (comp_func == 0)
     { //Compare by name
       unsigned int size_of_item_a_filename = strlen(list[item_a].filename);
       unsigned int size_of_item_b_filename = strlen(list[item_b].filename);
       unsigned int length=size_of_item_b_filename;
       if (size_of_item_a_filename<size_of_item_b_filename) { length=size_of_item_a_filename; }

       unsigned int i=0;
       for (i=0; i<length; i++)
       {
          if ( list[item_a].filename[i]<list[item_b].filename[i] ) { return true_value;  }
          if ( list[item_a].filename[i]>list[item_b].filename[i] ) { return false_value;  }
       }

       if (size_of_item_a_filename<size_of_item_b_filename) { return true_value; }
     } else
    if (comp_func == 1)
     { // Compare by date
       if (list[item_a].year < list[item_b].year) { return true_value; }
       if (list[item_a].year > list[item_b].year) { return false_value; }

       if (list[item_a].month < list[item_b].month) { return true_value; }
       if (list[item_a].month > list[item_b].month) { return false_value; }

       if (list[item_a].day < list[item_b].day) { return true_value; }
       if (list[item_a].day > list[item_b].day) { return false_value; }

       if (list[item_a].hour < list[item_b].hour) { return true_value; }
       if (list[item_a].hour > list[item_b].hour) { return false_value; }

       if (list[item_a].minute < list[item_b].minute) { return true_value; }
       if (list[item_a].minute > list[item_b].minute) { return false_value; }

       if (list[item_a].second < list[item_b].second) { return true_value; }
       if (list[item_a].second > list[item_b].second) { return false_value; }
     } else
    if (comp_func == 2 )
     { // Compare by size
       if (list[item_a].filesize <= list[item_b].filesize) { return true_value; } else
                                                           { return false_value; }
     }

    return false_value;
}

void SwapListItems(unsigned int item_a,unsigned int item_b)
{
  struct FilenameHolder temp;
  temp=list[item_a];
  list[item_a]=list[item_b];
  list[item_b]=temp;
}


void RemoveListItem(unsigned int item_num)
{
  //Since the list is always sorted , we will have to move all items one step back
  // TODO , this could be a linked list to make things easier , typically remove list items
  // is not called in sorting so it will not have a performance impact

  unsigned int item_iterator=item_num+1;
  while (item_iterator<last_list_total_pictures_count)
   {
      list[item_iterator-1]=list[item_iterator];
   }
   --last_list_total_pictures_count;
}



void SortDirectoryList(unsigned int beg,unsigned int end,unsigned int comp_func,unsigned int asc_desc)
{
  if (end > beg + 1)
  {
    unsigned int piv=beg, l = beg + 1, r = end;
    while (l < r)
    {
      if ( FirstItemSmallerOrEqualToSecond(l,piv,comp_func,asc_desc) )
        {
          l++;
        }
      else
        {
          SwapListItems(l,--r);
        }
    }
    SwapListItems(--l,beg);

    SortDirectoryList(beg,l,comp_func,asc_desc);
    SortDirectoryList(r,end,comp_func,asc_desc);
  }
}



void PrintDirectoryList()
{
  if (!PrintPictureLoadingMsg())   { return; }
  unsigned int i=0;
  for (i=0; i<last_list_total_pictures_count; i++)
   {
     fprintf(stderr,"%u - %s\n",i,list[i].filename);
   }

}


void PrintDirectoryListItem(unsigned int item)
{
  if (list==0) {return;}
  if (item>=last_list_total_pictures_count) {return; }
  if (PrintPictureLoadingMsg()) fprintf(stderr,"%u - %s\n",item,list[item].filename);
}












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

inline wxString _U2(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}

//ls -lrt sorted directory list

unsigned int GetDirectoryList(char * thedirectory,unsigned int store_results_in_space,unsigned int comp_func,unsigned int asc_desc)
{
  unsigned int COUNT_FILES_ONLY=0;
  unsigned int this_list_total_count=0;
  unsigned int this_list_total_pictures_count=0;


  if (store_results_in_space == 0 ) { COUNT_FILES_ONLY = 1; }

  wxDir dir(_U2(thedirectory)); /*wxGetCwd()*/
  if ( !dir.IsOpened() )  { return 0; }

  wxString filename,extension,fullname;
  wxString filespec = wxT("*.*");
  int flags=wxDIR_FILES;

  if (!COUNT_FILES_ONLY) { AllocateDirectoryList(store_results_in_space); }


  wxDateTime mod_time;
  int cont = dir.GetFirst(&filename,filespec,flags);
  while ( cont )
   {
      wxString dir_and_filename;
      dir_and_filename.clear();
      dir_and_filename<<_U2(thedirectory);
      dir_and_filename<<_U2("/");
      dir_and_filename<<filename;

      wxFileName fname(dir_and_filename);
     // fname.Normalize(wxPATH_NORM_LONG|wxPATH_NORM_DOTS|wxPATH_NORM_TILDE|wxPATH_NORM_ABSOLUTE);
     // fname.MakeRelativeTo(_U2(thedirectory));

      extension=fname.GetExt();
      fullname=fname.GetFullName();

      unsigned int is_a_picture=0;
      if ( extension.CmpNoCase(wxT("JPG"))==0 ) {is_a_picture=1;}
      else if ( extension.CmpNoCase(wxT("JPEG"))==0 ) {is_a_picture=1;}
      else if ( extension.CmpNoCase(wxT("PNG"))==0 ) {is_a_picture=1;}
//      else if ( extension.CmpNoCase(wxT("BMP")==0) ) {is_a_picture=1;}

      if ( is_a_picture )
        {
          if (!COUNT_FILES_ONLY)
          {
            strncpy(list[this_list_total_pictures_count].filename,(const char*) fullname.mb_str(wxConvUTF8),512);


            mod_time=fname.GetModificationTime();
            list[this_list_total_pictures_count].year=mod_time.GetYear();
            list[this_list_total_pictures_count].month=mod_time.GetMonth();
            list[this_list_total_pictures_count].day=mod_time.GetDay();
            list[this_list_total_pictures_count].day_of_year=mod_time.GetDayOfYear();
            list[this_list_total_pictures_count].hour=mod_time.GetHour();
            list[this_list_total_pictures_count].minute=mod_time.GetMinute();
            list[this_list_total_pictures_count].second=mod_time.GetSecond();

            wxULongLong size_long_long = fname.GetSize();
            list[this_list_total_pictures_count].filesize=(unsigned int) size_long_long.ToULong();

            if ( this_list_total_pictures_count >= list_size )  { /*Our list cannot acommodate any more data error*/ return 0; }
          }
          ++this_list_total_pictures_count;
        }
      ++this_list_total_count;
      /* WE CONTINUE THE LOOP UNTIL THE END!*/
      cont = dir.GetNext(&filename);
   }


  if (COUNT_FILES_ONLY)
    {
        last_list_total_countNOTUSEDANYWHERE=this_list_total_count;
        last_list_total_pictures_count=this_list_total_pictures_count;
        return this_list_total_pictures_count;
    } else
    {
      if (this_list_total_count>0)
       {
        fprintf(stderr,"Sorting directory modification time.. ");
        SortDirectoryList(0,this_list_total_pictures_count,comp_func,asc_desc);
        fprintf(stderr,"done\n");
        PrintDirectoryList();
       }
    }

  return 1;
}


unsigned int CountPicturesInDirectory(char * thedirectory)
{
  return GetDirectoryList(thedirectory,0,0,0); /*Calling GetDirectoryList with 0 size only counts files!*/
}

unsigned int GetTotalFilesInDirectory()
{
    return last_list_total_countNOTUSEDANYWHERE;
}


unsigned int GetTotalViewableFilesInDirectory()
{
    return last_list_total_pictures_count;
}

unsigned int GetViewableFilenameforFile(unsigned int file_id,char *directory,char *filename)
{
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;
    if ( directory == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 2 parameter ? \n"); return 0; }
    if ( filename == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 3 parameter ? \n"); return 0; }

    if (PrintPictureLoadingMsg()) fprintf(stderr,"Copying picture %u = `%s` \n",file_id,list[file_id].filename);
    strcpy(filename,directory);
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


unsigned int LaunchGUI_PickDir()
{
  return 0;
  WxWidgetsContext* Frame = new WxWidgetsContext();

  //Frame->Show();
  //SetTopWindow(Frame);

 wxString defaultPath=wxT("/");
 wxDirDialog dialog((wxWindow *) Frame,wxT("Please select a directory to view files"),defaultPath,wxDD_NEW_DIR_BUTTON);
 if (dialog.ShowModal() == wxID_OK )
  {
    wxString path = dialog.GetPath();
    strncpy((char*)frame.album_directory,(const char*) path.mb_str(wxConvUTF8),MAX_PATH);
  }

  delete Frame;

 return 0;
}


int MoveFileToDir(unsigned int file_id,char * dir)
{
    if (!frame.allow_mv_operation_sorting) { return 0; }
    char move_operation[2048];
    strcpy(move_operation,"mv ");

    strcat(move_operation,(char*)frame.album_directory);
    strcat(move_operation,list[file_id].filename);
    strcat(move_operation," ");
    strcat(move_operation,dir);
    fprintf(stderr,"Executing %s \n",move_operation);
    int i=system(move_operation);

  return (i==0);
}

