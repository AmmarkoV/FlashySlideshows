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
  bool wxsOK = true;
  wxInitAllImageHandlers();
  if ( wxsOK )
    {
  //    ClinicDBFrame* Frame = new ClinicDBFrame(0);
  //    if (  gui_friendly_mode == 1 ) ShowStartupMenu();
  //    Frame->Show();
   //   SetTopWindow(Frame);
    }
  //*)
  return wxsOK;

}


bool WxWidgetsContext::OnClose()
{
  fprintf(stderr,"WxWidgets closing down\n");
  return 0;
}
