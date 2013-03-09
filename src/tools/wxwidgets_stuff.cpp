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

#include <wx/msgdlg.h>
#include <wx/utils.h>

//(*AppHeaders
#include "wxwidgets_stuff.h"
#include <wx/image.h>
//*)

//IMPLEMENT_APP(WxWidgetsContext);
DECLARE_APP(WxWidgetsContext)


bool WxWidgetsContext::OnInit()
{
  fprintf(stderr,"WxWidgets beeing kickstarted\n");
  //(*AppInitialize
  bool wxsOK = wxInitialize();
  if ( ! wxsOK ) { fprintf(stderr,"Could not kickstart wxWidgets\n"); return wxsOK; }
  wxInitAllImageHandlers();
  if ( ! wxsOK ) { fprintf(stderr,"Could not kickstart image handlers for wxWidgets\n"); return wxsOK; }

  return wxsOK;
}


bool WxWidgetsContext::OnClose()
{
  fprintf(stderr,"WxWidgets closing down\n");
  return 0;
}


unsigned int LaunchGUI_PickDir()
{
  return 0;
  /*
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
*/
 return 0;
}


