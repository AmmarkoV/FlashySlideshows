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
    wxString wxbuild(wxVERSION_STRING);

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
    ButtonQuit = new wxButton(this, ID_BUTTON2, _("Quit"), wxPoint(560,368), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Directory :"), wxPoint(32,334), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    PathTextCtrl = new wxTextCtrl(this, ID_TEXTCTRL1, _("~/Pictures"), wxPoint(104,330), wxSize(336,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    CheckBoxSound = new wxCheckBox(this, ID_CHECKBOX1, _("Sound Effects"), wxPoint(480,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBoxSound->SetValue(false);
    CheckBoxFaceDetection = new wxCheckBox(this, ID_CHECKBOX2, _("Face Detection"), wxPoint(480,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBoxFaceDetection->SetValue(false);
    ComboBoxTransitions = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxPoint(456,64), wxSize(176,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    ComboBoxTransitions->SetSelection( ComboBoxTransitions->Append(_("Random Transitions")) );
    ComboBoxTransitions->Append(_("Movement"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Transitions"), wxPoint(456,40), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    PictureFolder = new wxGenericDirCtrl(this, ID_GENERICDIRCTRL1, _T("~/Pictures"), wxPoint(40,48), wxSize(384,256), 0, wxEmptyString, 0, _T("ID_GENERICDIRCTRL1"));
    CheckBox1 = new wxCheckBox(this, ID_CHECKBOX3, _("Visual Effects"), wxPoint(480,192), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBox1->SetValue(false);
    DelaySpinCtrl = new wxSpinCtrl(this, ID_SPINCTRL1, _T("1"), wxPoint(512,98), wxSize(88,27), 0, 100, 10000, 1, _T("ID_SPINCTRL1"));
    DelaySpinCtrl->SetValue(_T("1"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Delay"), wxPoint(464,104), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("ms"), wxPoint(608,104), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    CheckBox2 = new wxCheckBox(this, ID_CHECKBOX4, _("Include Subfolders"), wxPoint(480,144), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBox2->SetValue(false);
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
    wxMessageBox(msg, _("Welcome to..."));
}


void FlashySlideShowStarterFrame::OnRefreshDir(wxCommandEvent& event)
{
    PathTextCtrl->SetValue(PictureFolder->GetPath());
}

void FlashySlideShowStarterFrame::OnButtonStartClick(wxCommandEvent& event)
{
    PathTextCtrl->SetValue(PictureFolder->GetPath());

    wxString what_to_call;
    what_to_call.clear();

    what_to_call<< wxT("bin/Release/FlashySlideShow \"");
    what_to_call<< PathTextCtrl->GetValue();
    what_to_call<< wxT("\"");
    wxMessageBox(what_to_call,wxT("What will be executed"));
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
}
