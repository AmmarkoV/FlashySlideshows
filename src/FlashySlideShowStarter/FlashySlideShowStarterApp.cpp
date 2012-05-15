/***************************************************************
 * Name:      FlashySlideShowStarterApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2012-01-24
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#include "FlashySlideShowStarterApp.h"

//(*AppHeaders
#include "FlashySlideShowStarterMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(FlashySlideShowStarterApp);

bool FlashySlideShowStarterApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	FlashySlideShowStarterFrame* Frame = new FlashySlideShowStarterFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
