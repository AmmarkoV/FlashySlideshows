/***************************************************************
 * Name:      FlashySlideShowStarterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2012-01-24
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#include "FlashySlideShowStarterMain.h"
#include <wx/msgdlg.h>
#include <wx/filename.h>

//(*InternalHeaders(FlashySlideShowStarterFrame)
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/font.h>
//*)
#include <wx/treectrl.h>

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild;

    wxbuild.clear();


    wxbuild << _T("Another quality \"product\" by Ammar Qammaz a.k.a. AmmarkoV --- http://ammar.gr\n\n");

    wxbuild << _T("GUI Compiled using wxWidgets\n");
    wxbuild << wxVERSION_STRING;

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }


    return wxbuild;
}

//(*IdInit(FlashySlideShowStarterFrame)
const long FlashySlideShowStarterFrame::ID_STATICBOX2 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICBOX1 = wxNewId();
const long FlashySlideShowStarterFrame::ID_BUTTON1 = wxNewId();
const long FlashySlideShowStarterFrame::ID_BUTTON2 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT2 = wxNewId();
const long FlashySlideShowStarterFrame::ID_TEXTCTRL1 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX1 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX2 = wxNewId();
const long FlashySlideShowStarterFrame::ID_COMBOBOX1 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT1 = wxNewId();
const long FlashySlideShowStarterFrame::ID_GENERICDIRCTRL1 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX3 = wxNewId();
const long FlashySlideShowStarterFrame::ID_SPINCTRL1 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT3 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT4 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX4 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT5 = wxNewId();
const long FlashySlideShowStarterFrame::ID_COMBOBOX2 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX5 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT6 = wxNewId();
const long FlashySlideShowStarterFrame::ID_BUTTON3 = wxNewId();
const long FlashySlideShowStarterFrame::idMenuQuit = wxNewId();
const long FlashySlideShowStarterFrame::idMenuAbout = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATUSBAR1 = wxNewId();
//*)


BEGIN_EVENT_TABLE(FlashySlideShowStarterFrame,wxFrame)
    //(*EventTable(FlashySlideShowStarterFrame)
    //*)

END_EVENT_TABLE()

FlashySlideShowStarterFrame::FlashySlideShowStarterFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(FlashySlideShowStarterFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, _("Flashy SlideShow Starter "), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(663,456));
    StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("Options"), wxPoint(448,16), wxSize(200,264), 0, _T("ID_STATICBOX2"));
    StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Folder Preview"), wxPoint(24,16), wxSize(416,312), 0, _T("ID_STATICBOX1"));
    ButtonStart = new wxButton(this, ID_BUTTON1, _("Start!"), wxPoint(448,288), wxSize(200,72), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    wxFont ButtonStartFont(36,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    ButtonStart->SetFont(ButtonStartFont);
    ButtonQuit = new wxButton(this, ID_BUTTON2, _("Quit"), wxPoint(584,368), wxSize(61,29), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Directory :"), wxPoint(32,334), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    PathTextCtrl = new wxTextCtrl(this, ID_TEXTCTRL1, _("~/Pictures"), wxPoint(104,330), wxSize(336,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    CheckBoxSound = new wxCheckBox(this, ID_CHECKBOX1, _("Sound Effects"), wxPoint(456,240), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBoxSound->SetValue(false);
    CheckBoxFaceDetection = new wxCheckBox(this, ID_CHECKBOX2, _("Face Detection"), wxPoint(456,208), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBoxFaceDetection->SetValue(false);
    ComboBoxTransitions = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxPoint(456,112), wxSize(176,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    ComboBoxTransitions->SetSelection( ComboBoxTransitions->Append(_("Random Transitions")) );
    ComboBoxTransitions->Append(_("Movement"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Transitions"), wxPoint(456,96), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    PictureFolder = new wxGenericDirCtrl(this, ID_GENERICDIRCTRL1, _T("~/Pictures"), wxPoint(40,48), wxSize(384,256), 0, wxEmptyString, 0, _T("ID_GENERICDIRCTRL1"));
    CheckBoxVisuals = new wxCheckBox(this, ID_CHECKBOX3, _("Visual Effects"), wxPoint(456,224), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBoxVisuals->SetValue(false);
    DelaySpinCtrl = new wxSpinCtrl(this, ID_SPINCTRL1, _T("1"), wxPoint(496,152), wxSize(88,27), 0, 100, 10000, 1, _T("ID_SPINCTRL1"));
    DelaySpinCtrl->SetValue(_T("1"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Delay"), wxPoint(456,160), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("ms"), wxPoint(592,160), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    CheckBoxIncludeSubfolders = new wxCheckBox(this, ID_CHECKBOX4, _("Include Subfolders"), wxPoint(456,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBoxIncludeSubfolders->SetValue(false);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Picture Quality"), wxPoint(456,40), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    ComboBoxQuality = new wxComboBox(this, ID_COMBOBOX2, wxEmptyString, wxPoint(456,56), wxSize(176,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX2"));
    ComboBoxQuality->Append(_("Very High"));
    ComboBoxQuality->SetSelection( ComboBoxQuality->Append(_("High")) );
    ComboBoxQuality->Append(_("Medium"));
    ComboBoxQuality->Append(_("Low"));
    ComboBoxQuality->Append(_("Very Low"));
    CheckBoxMipmap = new wxCheckBox(this, ID_CHECKBOX5, _("Use Mipmaping"), wxPoint(456,192), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    CheckBoxMipmap->SetValue(false);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Date of Creation : 00-00-0000 , Pictures Inside : 0000"), wxPoint(32,368), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    ButtonControls = new wxButton(this, ID_BUTTON3, _("Controls"), wxPoint(448,368), wxSize(128,29), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnButtonStartClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnButtonQuitClick);
    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnPathTextCtrlText);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnButtonControlsClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnAbout);
    //*)


    PathTreeCtrl=PictureFolder->GetTreeCtrl();

    Connect(PathTreeCtrl->GetId(),wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnRefreshDir);
}

FlashySlideShowStarterFrame::~FlashySlideShowStarterFrame()
{
    //(*Destroy(FlashySlideShowStarterFrame)
    //*)
}

void FlashySlideShowStarterFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void FlashySlideShowStarterFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("FlashySlideshows!!"));
}


void FlashySlideShowStarterFrame::OnRefreshDir(wxCommandEvent& event)
{
    PathTextCtrl->SetValue(PictureFolder->GetPath());
}


int FileExists(const char *fname)
{
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void FlashySlideShowStarterFrame::OnButtonStartClick(wxCommandEvent& event)
{
    wxString raw_path=PictureFolder->GetPath();
    PathTextCtrl->SetValue(raw_path);

    wxFileName path_converter(raw_path);



    wxString path;


     if ( path_converter.DirExists(raw_path) )
       {
         path=raw_path;
       } else
       {
         path=path_converter.GetPath();
       }



    wxString what_to_call;
    what_to_call.clear();

    if ( FileExists("bin/Release/FlashySlideShow")) { /*Dev Build*/ what_to_call<< wxT("bin/Release/FlashySlideShow "); } else
    if ( FileExists("/usr/bin/FlashySlideShow"))    { /*Normal Deployment */what_to_call<< wxT("/usr/bin/FlashySlideShow "); }

    if ( CheckBoxIncludeSubfolders->IsChecked() ) { what_to_call<< wxT(" -r"); }
    if ( CheckBoxSound->IsChecked() ) { what_to_call<< wxT(" -s"); }
    if ( CheckBoxFaceDetection->IsChecked() ) { what_to_call<< wxT(" -fd"); }
    if ( CheckBoxVisuals->IsChecked() ) { what_to_call<< wxT(" -v"); }
    if ( CheckBoxMipmap->IsChecked() ) { what_to_call<< wxT(" -m"); }

    if ( ComboBoxQuality->GetCurrentSelection()!=1 ) { what_to_call<< wxT(" -q "); what_to_call<<ComboBoxQuality->GetCurrentSelection(); }
    if ( ComboBoxTransitions->GetCurrentSelection()!=0 ) { what_to_call<< wxT(" -t ");  what_to_call<<ComboBoxTransitions->GetCurrentSelection();  }
    if ( DelaySpinCtrl->GetValue()!=100 ) { what_to_call<< wxT(" -d ");  what_to_call<<DelaySpinCtrl->GetValue(); }


    what_to_call<< wxT(" \"");
    what_to_call<< path;
    what_to_call<< wxT("\"");

   // wxMessageBox(what_to_call,wxT("What will be executed")); // DEBUG : P
    wxExecute(what_to_call);
}

void FlashySlideShowStarterFrame::OnButtonBrowseFolderClick(wxCommandEvent& event)
{
}

void FlashySlideShowStarterFrame::OnButtonQuitClick(wxCommandEvent& event)
{
    Close();
}

void FlashySlideShowStarterFrame::OnPathTextCtrlText(wxCommandEvent& event)
{
    PictureFolder->SetPath(PathTextCtrl->GetValue());
}

void FlashySlideShowStarterFrame::OnButtonControlsClick(wxCommandEvent& event)
{
    wxString controls_text;
    controls_text.clear();

    controls_text<< wxT(" W = Up , S = Down , A = Left , D = Right \n");
    controls_text<< wxT("---------------------------------------------\n");
    controls_text<< wxT(" Arrow Keys = Change Picture , F = Zoom out , R = Zoom in \n");
    controls_text<< wxT("---------------------------------------------\n");
    controls_text<< wxT(" Enter = Automatic Slideshow \n M = Change Transition Mode \n B = Change Hover Effect \n");
    wxMessageBox(controls_text, _("FlashySlideShow Controls list..!"));

}
