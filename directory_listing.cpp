/* FancySlideShow..
A fancy , OpenGL slideshow application !
URLs: http://ammarkov.ath.cx
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wxwidgets_stuff.h"
#include "wx/dir.h"
#include "wx/filename.h"
#include "wx/dirdlg.h"

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

inline wxString _U2(const char String[] = "")
{
  return wxString(String, wxConvUTF8);
}

//ls -lrt sorted directory list

unsigned int GetDirectoryList(char * thedirectory,unsigned int store_results_in_space)
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

  int cont = dir.GetFirst(&filename,filespec,flags);
  while ( cont )
   {
      wxFileName fname(filename);
     // fname.Normalize(wxPATH_NORM_LONG|wxPATH_NORM_DOTS|wxPATH_NORM_TILDE|wxPATH_NORM_ABSOLUTE);
     // fname.MakeRelativeTo(_U2(thedirectory));

      extension=fname.GetExt();
      fullname=fname.GetFullName();

      unsigned int is_a_picture=0;
      if ( extension.CmpNoCase(wxT("JPG")) ) {is_a_picture=1;}
      else if ( extension.CmpNoCase(wxT("JPEG")) ) {is_a_picture=1;}
      else if ( extension.CmpNoCase(wxT("PNG")) ) {is_a_picture=1;}
//      else if ( extension.CmpNoCase(wxT("BMP")) ) {is_a_picture=1;}

      if ( is_a_picture )
        {
          if (!COUNT_FILES_ONLY)
          {
            strncpy(list[this_list_total_pictures_count].filename,(const char*) fullname.mb_str(wxConvUTF8),512);
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
        last_list_total_count=this_list_total_count;
        last_list_total_pictures_count=this_list_total_pictures_count;
        return this_list_total_pictures_count;
    }

  return 1;
}


unsigned int CountPicturesInDirectory(char * thedirectory)
{
  return GetDirectoryList(thedirectory,0); /*Calling GetDirectoryList with 0 size only counts files!*/
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
    if ( list_size <= file_id ) return 0;
    if ( list == 0 ) return 0;
    if ( directory == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 2 parameter ? \n"); return 0; }
    if ( filename == 0 ) { fprintf(stderr,"GetViewableFilenameforFile called with wrong 3 parameter ? \n"); return 0; }

    fprintf(stderr,"Copying `%s` \n",list[file_id].filename);
    strcpy(filename,directory);
    strcat(filename,list[file_id].filename);

    return 1;
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
