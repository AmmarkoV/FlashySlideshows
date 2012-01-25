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
#include <wx/spinctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/statbox.h>
#include <wx/frame.h>
#include <wx/dirctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
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
        void OnButtonStartClick(wxCommandEvent& event);
        void OnButtonBrowseFolderClick(wxCommandEvent& event);
        void OnButtonQuitClick(wxCommandEvent& event);
        void OnPathTextCtrlText(wxCommandEvent& event);
        //*)
        void OnRefreshDir(wxTreeEvent& event);

        //(*Identifiers(FlashySlideShowStarterFrame)
        static const long ID_STATICBOX2;
        static const long ID_STATICBOX1;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL1;
        static const long ID_CHECKBOX1;
        static const long ID_CHECKBOX2;
        static const long ID_COMBOBOX1;
        static const long ID_STATICTEXT1;
        static const long ID_GENERICDIRCTRL1;
        static const long ID_CHECKBOX3;
        static const long ID_SPINCTRL1;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_CHECKBOX4;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(FlashySlideShowStarterFrame)
        wxTextCtrl* PathTextCtrl;
        wxStatusBar* StatusBar1;
        wxGenericDirCtrl* PictureFolder;
        wxCheckBox* CheckBoxSound;
        wxCheckBox* CheckBox1;
        wxSpinCtrl* DelaySpinCtrl;
        wxStaticText* StaticText1;
        wxStaticBox* StaticBox2;
        wxStaticText* StaticText3;
        wxCheckBox* CheckBox2;
        wxStaticBox* StaticBox1;
        wxComboBox* ComboBoxTransitions;
        wxCheckBox* CheckBoxFaceDetection;
        wxStaticText* StaticText4;
        wxStaticText* StaticText2;
        wxButton* ButtonStart;
        wxButton* ButtonQuit;
        //*)

        wxTreeCtrl* PathTreeCtrl;

        DECLARE_EVENT_TABLE()
};

#endif // FLASHYSLIDESHOWSTARTERMAIN_H
