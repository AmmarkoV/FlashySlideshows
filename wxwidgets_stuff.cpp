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
