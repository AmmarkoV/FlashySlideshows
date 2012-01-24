/***************************************************************
 * Name:      FlashySlideShowStarterMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2012-01-24
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#ifndef FLASHYSLIDESHOWSTARTERMAIN_H
#define FLASHYSLIDESHOWSTARTERMAIN_H

//(*Headers(FlashySlideShowStarterFrame)
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/statbox.h>
#include <wx/frame.h>
#include <wx/stattext.h>
//*)

class FlashySlideShowStarterFrame: public wxFrame
{
    public:

        FlashySlideShowStarterFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~FlashySlideShowStarterFrame();

    private:

        //(*Handlers(FlashySlideShowStarterFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(FlashySlideShowStarterFrame)
        static const long ID_STATICBOX2;
        static const long ID_STATICBOX1;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_STATICTEXT1;
        static const long ID_BUTTON3;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(FlashySlideShowStarterFrame)
        wxStatusBar* StatusBar1;
        wxStaticText* StaticText1;
        wxStaticBox* StaticBox2;
        wxStaticBox* StaticBox1;
        wxButton* ButtonStart;
        wxButton* ButtonChooseFolder;
        wxButton* ButtonQuit;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // FLASHYSLIDESHOWSTARTERMAIN_H
