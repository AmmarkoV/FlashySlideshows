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
const long FlashySlideShowStarterFrame::ID_STATICTEXT7 = wxNewId();
const long FlashySlideShowStarterFrame::ID_COMBOBOX3 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX6 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX7 = wxNewId();
const long FlashySlideShowStarterFrame::ID_TEXTCTRL2 = wxNewId();
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

    Create(parent, wxID_ANY, _("Flashy SlideShow Starter "), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(663,495));
    StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("Slideshow Options"), wxPoint(448,16), wxSize(200,320), 0, _T("ID_STATICBOX2"));
    StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Folder Preview"), wxPoint(24,16), wxSize(416,312), 0, _T("ID_STATICBOX1"));
    ButtonStart = new wxButton(this, ID_BUTTON1, _("Start!"), wxPoint(448,344), wxSize(200,64), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    wxFont ButtonStartFont(36,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    ButtonStart->SetFont(ButtonStartFont);
    ButtonQuit = new wxButton(this, ID_BUTTON2, _("Quit"), wxPoint(584,408), wxSize(61,29), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Directory :"), wxPoint(32,334), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    PathTextCtrl = new wxTextCtrl(this, ID_TEXTCTRL1, _("~/Pictures"), wxPoint(104,330), wxSize(336,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    CheckBoxSound = new wxCheckBox(this, ID_CHECKBOX1, _("Sound Effects"), wxPoint(456,280), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBoxSound->SetValue(true);
    CheckBoxFaceDetection = new wxCheckBox(this, ID_CHECKBOX2, _("Face Detection"), wxPoint(456,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBoxFaceDetection->SetValue(true);
    ComboBoxTransitions = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxPoint(456,102), wxSize(176,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    ComboBoxTransitions->SetSelection( ComboBoxTransitions->Append(_("Random Transitions")) );
    ComboBoxTransitions->Append(_("Movement"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Transitions"), wxPoint(456,86), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    PictureFolder = new wxGenericDirCtrl(this, ID_GENERICDIRCTRL1, _T("~/Pictures"), wxPoint(40,48), wxSize(384,256), 0, wxEmptyString, 0, _T("ID_GENERICDIRCTRL1"));
    CheckBoxVisuals = new wxCheckBox(this, ID_CHECKBOX3, _("Visual Effects"), wxPoint(456,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBoxVisuals->SetValue(true);
    DelaySpinCtrl = new wxSpinCtrl(this, ID_SPINCTRL1, _T("5000"), wxPoint(496,138), wxSize(88,27), 0, 100, 10000, 5000, _T("ID_SPINCTRL1"));
    DelaySpinCtrl->SetValue(_T("5000"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Delay"), wxPoint(456,142), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("ms"), wxPoint(592,142), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    CheckBoxIncludeSubfolders = new wxCheckBox(this, ID_CHECKBOX4, _("Include Subfolders"), wxPoint(456,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBoxIncludeSubfolders->SetValue(false);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Picture Quality"), wxPoint(456,40), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    ComboBoxQuality = new wxComboBox(this, ID_COMBOBOX2, wxEmptyString, wxPoint(456,56), wxSize(176,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX2"));
    ComboBoxQuality->Append(_("Very Low"));
    ComboBoxQuality->Append(_("Low"));
    ComboBoxQuality->SetSelection( ComboBoxQuality->Append(_("Medium")) );
    ComboBoxQuality->Append(_("Very High"));
    ComboBoxQuality->Append(_("High"));
    ComboBoxQuality->Append(_("Ultra"));
    CheckBoxMipmap = new wxCheckBox(this, ID_CHECKBOX5, _("Use Mipmaping"), wxPoint(456,232), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    CheckBoxMipmap->SetValue(false);
    DateText = new wxStaticText(this, ID_STATICTEXT6, _("Select a directory and then click Start to begin Slideshow"), wxPoint(32,368), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    ButtonControls = new wxButton(this, ID_BUTTON3, _("Controls"), wxPoint(448,408), wxSize(128,29), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Sort Pictures By"), wxPoint(456,168), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    ComboBoxSort = new wxComboBox(this, ID_COMBOBOX3, wxEmptyString, wxPoint(456,186), wxSize(176,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX3"));
    ComboBoxSort->SetSelection( ComboBoxSort->Append(_("Names Ascending")) );
    ComboBoxSort->Append(_("Dates Ascending"));
    ComboBoxSort->Append(_("Sizes Ascending"));
    ComboBoxSort->Append(_("Names Descending"));
    ComboBoxSort->Append(_("Dates Descending"));
    ComboBoxSort->Append(_("Sizes Descending"));
    CheckBoxFileMove = new wxCheckBox(this, ID_CHECKBOX6, _("Move Func"), wxPoint(456,296), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
    CheckBoxFileMove->SetValue(false);
    CheckBoxFileResize = new wxCheckBox(this, ID_CHECKBOX7, _("Resize Func"), wxPoint(456,312), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
    CheckBoxFileResize->SetValue(false);
    TextCtrlResizeResolution = new wxTextCtrl(this, ID_TEXTCTRL2, _("1024x768"), wxPoint(554,300), wxSize(88,27), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
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
    Connect(ID_CHECKBOX6,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxFileMoveClick);
    Connect(ID_CHECKBOX7,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxFileResizeClick);
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
    if ( CheckBoxSound->IsChecked() ) { what_to_call<< wxT(" -sfx"); }
    if ( CheckBoxFaceDetection->IsChecked() ) { what_to_call<< wxT(" -fd"); }
    if ( CheckBoxVisuals->IsChecked() ) { what_to_call<< wxT(" -v"); }
    if ( CheckBoxMipmap->IsChecked() ) { what_to_call<< wxT(" -m"); }
    if ( CheckBoxFileMove->IsChecked() ) { what_to_call<< wxT(" -mv_sort"); }
    if ( CheckBoxFileResize->IsChecked() ) { what_to_call<< wxT(" -mv_resize ");
                                             what_to_call<<TextCtrlResizeResolution->GetValue();
                                           }

    if ( ComboBoxQuality->GetCurrentSelection()!=1 ) { what_to_call<< wxT(" -q "); what_to_call<<ComboBoxQuality->GetCurrentSelection(); }
    if ( ComboBoxTransitions->GetCurrentSelection()!=0 ) { what_to_call<< wxT(" -t ");  what_to_call<<ComboBoxTransitions->GetCurrentSelection();  }
    if ( DelaySpinCtrl->GetValue()!=100 ) { what_to_call<< wxT(" -d ");  what_to_call<<DelaySpinCtrl->GetValue(); }

    if ( ComboBoxSort->GetCurrentSelection()!=0 )
       { what_to_call<< wxT(" -s "); what_to_call<<ComboBoxSort->GetCurrentSelection(); }

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


    wxFileName fname(PathTextCtrl->GetValue());
    wxDateTime mod_time=fname.GetModificationTime();
    wxDateTime now_time=wxDateTime::Now();

    if ( mod_time.IsEarlierThan(now_time) )
     {
       wxTimeSpan difference = now_time.Subtract(mod_time);
       wxString value;

       value<<wxT("Modification ");
       //THIS IS KIND OF BAD CODE :P , NEEDS REVISION
       unsigned int days_difference_full = difference.GetDays();
       unsigned int days_difference_subtracted = difference.GetDays();
       unsigned int years_diff =  (unsigned int) difference.GetDays()/365;
       days_difference_subtracted = days_difference_full-years_diff*365;
       unsigned int months_diff = (unsigned int) (days_difference_subtracted)/30 ;
       days_difference_subtracted -= months_diff*30;
       unsigned int days_diff =  days_difference_subtracted;

       if (years_diff>0)
         {
           if (years_diff==1) { value << wxT("1 year "); } else
                              { value << years_diff; value << wxT(" years "); }
         }
       if (months_diff>0)
         {
           if (months_diff==1) { value << wxT("1 month "); } else
                               { value << months_diff; value << wxT(" months "); }
         }
       if (days_diff>0)
         {
           if (days_diff==1) { value << wxT("1 day "); } else
                             { value << days_diff; value << wxT(" days "); }
         }


       if ( (years_diff>0)||(months_diff>0)||(days_diff>0) ) { value << wxT("ago at "); }

       value << mod_time.GetDay();
       value << wxT("/");
       value << mod_time.GetMonth();
       value << wxT("/");
       value << mod_time.GetYear();

       DateText->SetLabel(value);

     } else
     {
       //This is weird , date appears to be in the future!
       wxString value;
       value << mod_time.GetDay();
       value << wxT("/");
       value << mod_time.GetMonth();
       value << wxT("/");
       value << mod_time.GetYear();
       DateText->SetLabel(value);
     }

}

void FlashySlideShowStarterFrame::OnButtonControlsClick(wxCommandEvent& event)
{
    wxString controls_text;
    controls_text.clear();

    controls_text<< wxT(" W = Up , S = Down , A = Left , D = Right \n");
    controls_text<< wxT("---------------------------------------------\n");
    controls_text<< wxT(" Arrow Keys = Change Picture \n F = Zoom out , R = Zoom in \n");
    controls_text<< wxT(" Q = Rotate Counter Clockwise \n E = Rotate Clockwise \n");
    controls_text<< wxT("---------------------------------------------\n");
    controls_text<< wxT(" Enter = Automatic Slideshow \n M = Change Transition Mode \n B = Change Hover Effect \n");
    controls_text<< wxT(" N = Cycle Effects \n J = Toggle Fullscreen \n Escape = Quit \n");
    wxMessageBox(controls_text, _("FlashySlideShow Controls list..!"));

}

void FlashySlideShowStarterFrame::OnCheckBoxFileMoveClick(wxCommandEvent& event)
{
  CheckBoxFileResize->SetValue(0);
}

void FlashySlideShowStarterFrame::OnCheckBoxFileResizeClick(wxCommandEvent& event)
{
  CheckBoxFileMove->SetValue(0);
}
