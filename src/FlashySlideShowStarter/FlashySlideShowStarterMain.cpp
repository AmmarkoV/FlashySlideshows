/***************************************************************
 * Name:      FlashySlideShowStarterMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ammar Qammaz (ammarkov@gmail.com)
 * Created:   2012-01-24
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#include "FlashySlideShowStarterMain.h"
#include "../filesystem/directory_listing.h"
#include "../version.h"

#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/wx.h>
#include <wx/utils.h>
#include <wx/dir.h>
#include "EmptyThumbnail.h"
#include "FlashyLogo.h"

//(*InternalHeaders(FlashySlideShowStarterFrame)
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/font.h>
//*)
#include <wx/treectrl.h>



unsigned int display_logo=1;
unsigned int ONLY_DISPLAY_LOGO=1;
wxBitmap *flashy_bmp=0,*default_bmp_thumb=0,*bmp_thumb1=0,*bmp_thumb2=0,*bmp_thumb3=0,*bmp_thumb4=0,*bmp_thumb5=0;
wxImage   flashy_img,default_img_thumb,img_thumb1,img_thumb2,img_thumb3,img_thumb4,img_thumb5;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild;

    wxbuild.clear();


    wxbuild << _T("Running FlashySlideshows v");
    wxString ourVersion(AutoVersion::FULLVERSION_STRING, wxConvUTF8);
    wxbuild << ourVersion;
    wxbuild << _T("\n");

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
const long FlashySlideShowStarterFrame::ID_COMBOBOX4 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX8 = wxNewId();
const long FlashySlideShowStarterFrame::ID_SPINCTRL2 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT8 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT9 = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATICTEXT10 = wxNewId();
const long FlashySlideShowStarterFrame::ID_TEXTCTRL2 = wxNewId();
const long FlashySlideShowStarterFrame::ID_CHECKBOX9 = wxNewId();
const long FlashySlideShowStarterFrame::idMenuQuit = wxNewId();
const long FlashySlideShowStarterFrame::idMenuPlay = wxNewId();
const long FlashySlideShowStarterFrame::idMenuEconomy = wxNewId();
const long FlashySlideShowStarterFrame::idMenuFullscreenCommand = wxNewId();
const long FlashySlideShowStarterFrame::idMenuWebInterface = wxNewId();
const long FlashySlideShowStarterFrame::idMenuHUD = wxNewId();
const long FlashySlideShowStarterFrame::idMenuSimpleBackground = wxNewId();
const long FlashySlideShowStarterFrame::idMenuViewCommand = wxNewId();
const long FlashySlideShowStarterFrame::idMenuGithub = wxNewId();
const long FlashySlideShowStarterFrame::idMenuAmmarkoVWebsite = wxNewId();
const long FlashySlideShowStarterFrame::idMenuCheckNewVersion = wxNewId();
const long FlashySlideShowStarterFrame::idMenuAbout = wxNewId();
const long FlashySlideShowStarterFrame::ID_STATUSBAR1 = wxNewId();
//*)


BEGIN_EVENT_TABLE(FlashySlideShowStarterFrame,wxFrame)
    //(*EventTable(FlashySlideShowStarterFrame)
    //*)
        EVT_PAINT(FlashySlideShowStarterFrame::OnPaint)
END_EVENT_TABLE()




FlashySlideShowStarterFrame::FlashySlideShowStarterFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(FlashySlideShowStarterFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuItem* MenuItem3;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem10;
    wxMenuItem* MenuItem4;
    wxMenu* Menu2;
    wxMenuItem* MenuItem8;

    Create(parent, wxID_ANY, _("Flashy SlideShow Starter "), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(685,551));
    StaticBox2 = new wxStaticBox(this, ID_STATICBOX2, _("Slideshow Options"), wxPoint(448,16), wxSize(224,384), 0, _T("ID_STATICBOX2"));
    StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Folder Preview"), wxPoint(32,16), wxSize(408,360), 0, _T("ID_STATICBOX1"));
    ButtonStart = new wxButton(this, ID_BUTTON1, _("Start!"), wxPoint(448,408), wxSize(216,48), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    wxFont ButtonStartFont(32,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    ButtonStart->SetFont(ButtonStartFont);
    ButtonQuit = new wxButton(this, ID_BUTTON2, _("Quit"), wxPoint(600,464), wxSize(61,29), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Directory :"), wxPoint(32,388), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    PathTextCtrl = new wxTextCtrl(this, ID_TEXTCTRL1, _("~/Pictures"), wxPoint(104,384), wxSize(336,27), wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    CheckBoxSound = new wxCheckBox(this, ID_CHECKBOX1, _("Sound Effects"), wxPoint(456,280), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBoxSound->SetValue(true);
    CheckBoxFaceDetection = new wxCheckBox(this, ID_CHECKBOX2, _("Face Detection"), wxPoint(456,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBoxFaceDetection->SetValue(true);
    ComboBoxTransitions = new wxComboBox(this, ID_COMBOBOX1, wxEmptyString, wxPoint(456,102), wxSize(128,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    ComboBoxTransitions->SetSelection( ComboBoxTransitions->Append(_("3D Seek")) );
    ComboBoxTransitions->Append(_("Immediate"));
    ComboBoxTransitions->Append(_("Transparency"));
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Transitions"), wxPoint(456,86), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    PictureFolder = new wxGenericDirCtrl(this, ID_GENERICDIRCTRL1, _T("~/Pictures"), wxPoint(40,48), wxSize(384,320), 0, wxEmptyString, 0, _T("ID_GENERICDIRCTRL1"));
    CheckBoxVisuals = new wxCheckBox(this, ID_CHECKBOX3, _("Visual Effects"), wxPoint(456,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBoxVisuals->SetValue(true);
    DelaySpinCtrl = new wxSpinCtrl(this, ID_SPINCTRL1, _T("5000"), wxPoint(496,138), wxSize(64,27), 0, 100, 10000, 5000, _T("ID_SPINCTRL1"));
    DelaySpinCtrl->SetValue(_T("5000"));
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Delay"), wxPoint(456,142), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("ms"), wxPoint(560,144), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    CheckBoxIncludeSubfolders = new wxCheckBox(this, ID_CHECKBOX4, _("Include Subfolders"), wxPoint(456,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBoxIncludeSubfolders->SetValue(false);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Picture Quality"), wxPoint(456,40), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    ComboBoxQuality = new wxComboBox(this, ID_COMBOBOX2, wxEmptyString, wxPoint(456,56), wxSize(128,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX2"));
    ComboBoxQuality->Append(_("Very Low"));
    ComboBoxQuality->Append(_("Low"));
    ComboBoxQuality->Append(_("Medium"));
    ComboBoxQuality->SetSelection( ComboBoxQuality->Append(_("Good")) );
    ComboBoxQuality->Append(_("High"));
    ComboBoxQuality->Append(_("Very High"));
    ComboBoxQuality->Append(_("Ultra"));
    CheckBoxMipmap = new wxCheckBox(this, ID_CHECKBOX5, _("Use Mipmaping"), wxPoint(456,232), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    CheckBoxMipmap->SetValue(false);
    DateText = new wxStaticText(this, ID_STATICTEXT6, _("Select a directory and then click Start to begin Slideshow"), wxPoint(32,416), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    ButtonControls = new wxButton(this, ID_BUTTON3, _("Controls"), wxPoint(448,464), wxSize(144,29), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Sort Pictures By"), wxPoint(456,168), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    ComboBoxSort = new wxComboBox(this, ID_COMBOBOX3, wxEmptyString, wxPoint(456,186), wxSize(200,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX3"));
    ComboBoxSort->SetSelection( ComboBoxSort->Append(_("Names Ascending")) );
    ComboBoxSort->Append(_("Dates Ascending"));
    ComboBoxSort->Append(_("Sizes Ascending"));
    ComboBoxSort->Append(_("Names Descending"));
    ComboBoxSort->Append(_("Dates Descending"));
    ComboBoxSort->Append(_("Sizes Descending"));
    ComboBoxSort->Append(_("Random"));
    CheckBoxFileMove = new wxCheckBox(this, ID_CHECKBOX6, _("File Move"), wxPoint(456,296), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
    CheckBoxFileMove->SetValue(false);
    CheckBoxFileMove->SetToolTip(_("Selecting this option will make the buttons 1,2,3,4,5,6,7,8,9,0 move thecurrently viewed picture inside the Moved/CategoryX/ folder where X is the number pressed"));
    CheckBoxFileMove->SetHelpText(_("Selecting this option will make the buttons 1,2,3,4,5,6,7,8,9,0 move thecurrently viewed picture inside the Moved/CategoryX/ folder where X is the number pressed"));
    CheckBoxFileResize = new wxCheckBox(this, ID_CHECKBOX7, _("File Resize"), wxPoint(456,312), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
    CheckBoxFileResize->SetValue(false);
    CheckBoxFileResize->SetToolTip(_("Selecting this option will make the buttons 1,2,3,4,5,6,7,8,9,0 make a resized copy of the currently viewed picture inside the Resized/CategoryX/ folder where X is the number pressed"));
    CheckBoxFileResize->SetHelpText(_("Selecting this option will make the buttons 1,2,3,4,5,6,7,8,9,0 make a resized copy of the currently viewed picture inside the Resized/CategoryX/ folder where X is the number pressed"));
    ComboBoxResizeResolution = new wxComboBox(this, ID_COMBOBOX4, wxEmptyString, wxPoint(552,320), wxSize(104,29), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX4"));
    ComboBoxResizeResolution->Append(_("320x240"));
    ComboBoxResizeResolution->Append(_("640x480"));
    ComboBoxResizeResolution->Append(_("1024x768"));
    ComboBoxResizeResolution->Append(_("1280x720"));
    ComboBoxResizeResolution->SetSelection( ComboBoxResizeResolution->Append(_("1920x1080")) );
    ComboBoxResizeResolution->Append(_("30%"));
    ComboBoxResizeResolution->Append(_("50%"));
    ComboBoxResizeResolution->Disable();
    wxFont ComboBoxResizeResolutionFont(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
    ComboBoxResizeResolution->SetFont(ComboBoxResizeResolutionFont);
    CheckBoxFileCopy = new wxCheckBox(this, ID_CHECKBOX8, _("File Copy"), wxPoint(456,328), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
    CheckBoxFileCopy->SetValue(false);
    ImagesPerLine = new wxSpinCtrl(this, ID_SPINCTRL2, _T("3"), wxPoint(600,102), wxSize(56,27), 0, 1, 10, 3, _T("ID_SPINCTRL2"));
    ImagesPerLine->SetValue(_T("3"));
    StaticText6 = new wxStaticText(this, ID_STATICTEXT8, _("Images"), wxPoint(600,56), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    StaticText8 = new wxStaticText(this, ID_STATICTEXT9, _("per line :"), wxPoint(600,72), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    StaticText9 = new wxStaticText(this, ID_STATICTEXT10, _("Custom Args :"), wxPoint(456,368), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    Arguments = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxPoint(552,368), wxSize(104,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    CheckBoxFileLink = new wxCheckBox(this, ID_CHECKBOX9, _("File Link"), wxPoint(456,344), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX9"));
    CheckBoxFileLink->SetValue(false);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    MenuItem8 = new wxMenuItem(Menu3, idMenuPlay, _("Immediate Playback"), wxEmptyString, wxITEM_CHECK);
    Menu3->Append(MenuItem8);
    MenuItem12 = new wxMenuItem(Menu3, idMenuEconomy, _("Economy"), wxEmptyString, wxITEM_CHECK);
    Menu3->Append(MenuItem12);
    MenuItem11 = new wxMenuItem(Menu3, idMenuFullscreenCommand, _("Fullscreen"), wxEmptyString, wxITEM_CHECK);
    Menu3->Append(MenuItem11);
    MenuItem11->Check(true);
    MenuItem5 = new wxMenuItem(Menu3, idMenuWebInterface, _("Web Interface"), wxEmptyString, wxITEM_CHECK);
    Menu3->Append(MenuItem5);
    MenuItem7 = new wxMenuItem(Menu3, idMenuHUD, _("Heads Up Display"), wxEmptyString, wxITEM_CHECK);
    Menu3->Append(MenuItem7);
    MenuItem6 = new wxMenuItem(Menu3, idMenuSimpleBackground, _("Simple Background"), wxEmptyString, wxITEM_CHECK);
    Menu3->Append(MenuItem6);
    MenuItem9 = new wxMenuItem(Menu3, idMenuViewCommand, _("View Command before startup"), wxEmptyString, wxITEM_CHECK);
    Menu3->Append(MenuItem9);
    MenuBar1->Append(Menu3, _("More Options"));
    Menu2 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu2, idMenuGithub, _("Visit Github Repository"), _("Click to open the github repository of FlashySlideshows on your browser"), wxITEM_NORMAL);
    Menu2->Append(MenuItem3);
    MenuItem4 = new wxMenuItem(Menu2, idMenuAmmarkoVWebsite, _("Visit Author\'s Website"), _("Click to open the website of the author of the program"), wxITEM_NORMAL);
    Menu2->Append(MenuItem4);
    MenuItem10 = new wxMenuItem(Menu2, idMenuCheckNewVersion, _("Check for a new version"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItem10);
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
    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxSoundClick);
    Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxFaceDetectionClick);
    Connect(ID_CHECKBOX3,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxVisualsClick);
    Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxIncludeSubfoldersClick);
    Connect(ID_CHECKBOX5,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxMipmapClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnButtonControlsClick);
    Connect(ID_CHECKBOX6,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxFileMoveClick);
    Connect(ID_CHECKBOX7,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxFileResizeClick);
    Connect(ID_CHECKBOX8,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxFileCopyClick);
    Connect(ID_CHECKBOX9,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnCheckBoxFileLinkClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnAbout);
    //*)

    //TA KOUMPIA TOU MENU
    Connect(idMenuGithub,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OpenGithubSite);
    Connect(idMenuAmmarkoVWebsite,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OpenAmmarkoVSite);
    Connect(idMenuCheckNewVersion,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::CheckNewVersion);

    default_img_thumb.SetData((unsigned char *)EmptyThumbnail.pixel_data,EmptyThumbnail.width ,EmptyThumbnail.height,true);
    default_bmp_thumb = new wxBitmap(default_img_thumb);

    flashy_img.SetData((unsigned char *)FlashyLogo.pixel_data,FlashyLogo.width ,FlashyLogo.height,true);
    flashy_bmp = new wxBitmap(flashy_img);
   // free(pixel);


    StatusBar1->SetStatusText(wxT("Welcome to Flashy Slideshows!"));

    PathTreeCtrl=PictureFolder->GetTreeCtrl();

    Connect(PathTreeCtrl->GetId(),wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&FlashySlideShowStarterFrame::OnRefreshDir);

    ComboBoxResizeResolution->Disable();

    ButtonStart->SetLabel(wxT("Click"));
    ButtonStart->SetLabel(wxT("to"));
    ButtonStart->SetLabel(wxT("Start!"));

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
    file = fopen(fname, "r");
    if (file!=0)
    {
        fclose(file);
        return 1;
    }
    return 0;
}


void ComplainAboutInstallation()
{
  wxString error_message; error_message.clear();
  error_message<<wxT("FlashySlideshowStarter was unable to locate the main executable of FlashySlideshows.\n");
  error_message<<wxT("This means a bad installation , execution from a weird directory or possibly that there are\n");
  error_message<<wxT("unresolved OpenGL dependencies and the main executable couldnt be compiled\n\n");
  error_message<<wxT("You can visit the github repository using the help menu and create an issue ticket\n");
  error_message<<wxT("describing your problem , or mail ammarkov+flashy@gmail.com..!\n");
  error_message<<wxT("\n\n");
  error_message<<wxT("Sorry for the inconvinience :)\n");
  wxMessageBox(error_message,wxT("Cannot find required executable files.."));
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

    if ( FileExists("flashyslideshows")) { /*Dev Build*/ what_to_call<< wxT("./flashyslideshows "); } else
    if ( FileExists("bin/Release/flashyslideshows")) { /*Dev Build*/ what_to_call<< wxT("bin/Release/flashyslideshows "); } else
    if ( FileExists("/usr/bin/flashyslideshows"))    { /*Normal Deployment */what_to_call<< wxT("/usr/bin/flashyslideshows "); } else
                                                    { /*Bad installation ? Weird System ? */
                                                      ComplainAboutInstallation();
                                                      return;
                                                    }


    /* Menu Options */
    if (!Menu3->IsChecked(idMenuEconomy))  { what_to_call<< wxT(" -alwaysRedraw"); }
    if (Menu3->IsChecked(idMenuPlay))  { what_to_call<< wxT(" -play"); }
    if (Menu3->IsChecked(idMenuWebInterface))  { what_to_call<< wxT(" -web"); }
    if (!Menu3->IsChecked(idMenuHUD))  { what_to_call<< wxT(" -no_hud"); }
    if (Menu3->IsChecked(idMenuSimpleBackground))  { what_to_call<< wxT(" -no_background"); }
    if (Menu3->IsChecked(idMenuFullscreenCommand))  { what_to_call<< wxT(" -fullscreen"); }

    /* Options from side panel*/
    if ( CheckBoxIncludeSubfolders->IsChecked() ) { what_to_call<< wxT(" -r"); }
    if ( CheckBoxSound->IsChecked() ) { what_to_call<< wxT(" -sfx"); }
    if ( CheckBoxFaceDetection->IsChecked() ) { what_to_call<< wxT(" -fd"); }
    if ( CheckBoxVisuals->IsChecked() ) { what_to_call<< wxT(" -v"); }
    if ( CheckBoxMipmap->IsChecked() ) { what_to_call<< wxT(" -m"); }
    if ( CheckBoxFileMove->IsChecked() ) { what_to_call<< wxT(" -file_move"); }
    if ( CheckBoxFileCopy->IsChecked() ) { what_to_call<< wxT(" -file_copy"); }
    if ( CheckBoxFileLink->IsChecked() ) { what_to_call<< wxT(" -file_link"); }
    if ( CheckBoxFileResize->IsChecked() ) { what_to_call<< wxT(" -file_resize ");
                                             what_to_call<<ComboBoxResizeResolution->GetValue();
                                           }




   what_to_call<< wxT(" -images_per_line ");
   what_to_call<<ImagesPerLine->GetValue();

    if ( ComboBoxQuality->GetCurrentSelection()!=1 ) { what_to_call<< wxT(" -q "); what_to_call<<ComboBoxQuality->GetCurrentSelection(); }
    if ( ComboBoxTransitions->GetCurrentSelection()!=0 ) { what_to_call<< wxT(" -t ");  what_to_call<<ComboBoxTransitions->GetCurrentSelection();  }
    if ( DelaySpinCtrl->GetValue()!=100 ) { what_to_call<< wxT(" -d ");  what_to_call<<DelaySpinCtrl->GetValue(); }

    if ( ComboBoxSort->GetCurrentSelection()!=0 )
    {
      if ( ComboBoxSort->GetCurrentSelection()==6 )
        { what_to_call<< wxT(" -s r"); /*RANDOM SORTING*/  } else
        { what_to_call<< wxT(" -s "); what_to_call<<ComboBoxSort->GetCurrentSelection(); }
    }

    what_to_call<< wxT(" ");
    what_to_call<< Arguments->GetValue();
    what_to_call<< wxT(" ");



    what_to_call<< wxT(" \"");
    what_to_call<< path;
    what_to_call<< wxT("\"");

    //wxMessageBox(what_to_call,wxT("What will be executed")); // DEBUG : P

    if (Menu3->IsChecked(idMenuViewCommand))  { wxMessageBox(what_to_call,wxT("What will be executed"));  }

    StatusBar1->SetStatusText(wxT("Starting FlashySlideshows..!!!"));
    ButtonStart->SetLabel(wxT("Running"));
    long result=wxExecute(what_to_call,wxEXEC_SYNC);
    ButtonStart->SetLabel(wxT("Start!"));
    StatusBar1->SetStatusText(wxT("FlashySlideshows terminated.."));
    if ( result != 0 )
      {  //Non Zero return from FlashySlideshows
          StatusBar1->SetStatusText(wxT("FlashySlideshows terminated with an error.."));
          wxString error_message; error_message.clear();
          error_message<<wxT("Failed to execute FlashySlideshows , it returned the following error identifier (");
          error_message<<result;
          error_message<<wxT(")\n\n");
          error_message<<wxT("The execution string was : \n");
          error_message<<what_to_call;
          wxMessageBox(error_message,wxT("Execution Error"));
      }

}


void FlashySlideShowStarterFrame::OpenAmmarkoVSite(wxCommandEvent& event)
{
  StatusBar1->SetStatusText(wxT("Launching http://ammar.gr/ in the default internet browser.."));
  wxLaunchDefaultBrowser(wxT("http://ammar.gr/"));
}

void FlashySlideShowStarterFrame::OpenGithubSite(wxCommandEvent& event)
{
  StatusBar1->SetStatusText(wxT("Launching https://github.com/AmmarkoV/FlashySlideshows in the default internet browser.."));
  wxLaunchDefaultBrowser(wxT("https://github.com/AmmarkoV/FlashySlideshows"));
}


int UnixTrimEndingNewLine(char * buffer)
{
  int len = strlen(buffer);
  if (len > 0 && buffer[len-1] == '\n')
  {
   buffer[len-1] = 0;
   return 1;
  }
  return 0;
}

void FlashySlideShowStarterFrame::CheckNewVersion(wxCommandEvent& event)
{
   fprintf(stderr,"Checking for a new Version of Flashy Slideshows ( running %s ) \n ",AutoVersion::FULLVERSION_STRING);

   int i=0;
   //Inform the user that the program will block for a while..!
   i=system("notify-send \"Please Wait while checking for a new version.. \n It might take a while..\"&");
   if (i!=0) { fprintf(stderr,"Could not inform user in a nice GUI friendly way that it might take a while to check for a new verison\n"); }
   StatusBar1->SetStatusText(wxT("Please Wait while checking for a newer version ( this might take a while ).."));
   fprintf(stderr,"Please Wait while checking for a newer version ( this might take a while )..\n ");

   //Thanks @c00kiemon5ter , the next commandline one liner will put the FULLVERSION_STRING of the git repo inside char * output
   //FILE * fp = popen("wget -qO- https://raw.github.com/AmmarkoV/FlashySlideshows/master/src/version.h | grep FULLVERSION_STRING | awk -F\\\" '/\"/{ print $2 }'", "r");
    int size_of_output = 512;   char output[512];
    FILE * fp = popen("wget -qO- https://raw.github.com/AmmarkoV/FlashySlideshows/master/src/version.h | sed -n '/FULLVERSION_STRING/s,.*\"\\(.\\+\\)\".*,\\1,p'", "r");
    if (fp == 0 )
      {
        fprintf(stderr,"Failed to run check for new version \n");
        wxMessageBox(wxT("Failed to run check for new version \n"),wxT("New Version exists"),wxICON_ASTERISK);
        return;
      }

  i=0;
  while (fgets(output, size_of_output , fp) != 0)
    {
      //We should trim the trailing new line character ( if it exists )
      UnixTrimEndingNewLine(output);
      wxString ourVersion(AutoVersion::FULLVERSION_STRING, wxConvUTF8);
      wxString remoteVersion(output, wxConvUTF8);

        ++i;
        fprintf(stderr,"Our Version : `%s` , Remote Version : `%s` \n",AutoVersion::FULLVERSION_STRING,output);
        if (strcmp(AutoVersion::FULLVERSION_STRING,output)!=0)
          {
            wxString outmsg;
            outmsg<<wxT("You are currently running version ");
            outmsg<<ourVersion;
            outmsg<<wxT(", there is a newer version ");
            outmsg<<remoteVersion;
            outmsg<<wxT(" you can download at https://www.github.com/AmmarkoV/FlashySlideshows");

            wxMessageBox(outmsg,wxT("A New Version is availiable"),wxICON_WARNING);
            fprintf(stderr,"A new version of flashy slideshows exists..");
            StatusBar1->SetStatusText(wxT("A new version of flashy slideshows exists.."));
          } else
          {
            wxString outmsg;
            outmsg<<wxT("You already have the latest version (");
            outmsg<<ourVersion;
            outmsg<<wxT(") of FlashySlideshows");
            wxMessageBox(outmsg,wxT("Version up to date"),wxICON_INFORMATION);
            fprintf(stderr,"Our version of flashy slideshows is up to date..");
            StatusBar1->SetStatusText(wxT("Your version of flashy slideshows is up to date.."));
          }
        break;
    }
  pclose(fp);
}


void FlashySlideShowStarterFrame::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);

  int height = 440;

  if ((display_logo)||(ONLY_DISPLAY_LOGO))
  {
      if ( flashy_bmp != 0 ) { dc.DrawBitmap(*flashy_bmp,32+0*(67+10),height,true); } else {   }
  } else
  {
   if ( bmp_thumb1 != 0 ) { dc.DrawBitmap(*bmp_thumb1,32+0*(67+10),height,true); } else { dc.DrawBitmap(*default_bmp_thumb,32+0*(67+10),height,true); }
   if ( bmp_thumb2 != 0 ) { dc.DrawBitmap(*bmp_thumb2,32+1*(67+10),height,true); } else { dc.DrawBitmap(*default_bmp_thumb,32+1*(67+10),height,true); }
   if ( bmp_thumb3 != 0 ) { dc.DrawBitmap(*bmp_thumb3,32+2*(67+10),height,true); } else { dc.DrawBitmap(*default_bmp_thumb,32+2*(67+10),height,true); }
   if ( bmp_thumb4 != 0 ) { dc.DrawBitmap(*bmp_thumb4,32+3*(67+10),height,true); } else { dc.DrawBitmap(*default_bmp_thumb,32+3*(67+10),height,true); }
   if ( bmp_thumb5 != 0 ) { dc.DrawBitmap(*bmp_thumb5,32+4*(67+10),height,true); } else { dc.DrawBitmap(*default_bmp_thumb,32+4*(67+10),height,true); }
  }

}




void FlashySlideShowStarterFrame::OnButtonBrowseFolderClick(wxCommandEvent& event)
{
}

void FlashySlideShowStarterFrame::OnButtonQuitClick(wxCommandEvent& event)
{
    Close();
}

unsigned int FlashySlideShowStarterFrame::RefreshThumbnails()
{
    display_logo=0;
    unsigned int recursive=0;
    //if ( CheckBoxIncludeSubfolders->IsChecked() ) { recursive=1; }
    char thedirectory[1024];
    strncpy(thedirectory,(const char*) PathTextCtrl->GetValue().mb_str(wxConvUTF8),1024);
    unsigned int total_pictures = GetDirectoryList(thedirectory,0,0,0,0,0,recursive);

   return total_pictures;
}


void FlashySlideShowStarterFrame::OnPathTextCtrlText(wxCommandEvent& event)
{
    wxDir dir(PathTextCtrl->GetValue());

    if ( !dir.IsOpened() )
      {
        StatusBar1->SetStatusText(wxT("Dir does not exist.."));
        return ; //DIR does not exist..
      }

    StatusBar1->SetStatusText(wxT("Please Wait while accessing filesystem.."));
    PictureFolder->SetPath(PathTextCtrl->GetValue());

    wxFileName fname(PathTextCtrl->GetValue());
    wxDateTime mod_time=fname.GetModificationTime();
    wxDateTime now_time=wxDateTime::Now();
    StatusBar1->SetStatusText(wxT(" "));

    if ( mod_time.IsEarlierThan(now_time) )
     {
       StatusBar1->SetStatusText(wxT("Please Wait while thumbnails are generated.."));
       unsigned int total_pictures = RefreshThumbnails();
       StatusBar1->SetStatusText(wxT(" "));

       wxTimeSpan difference = now_time.Subtract(mod_time);
       wxString value;

       value<<wxT("Modified ");
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


       if ( (years_diff>0)||(months_diff>0)||(days_diff>0) ) { value << wxT("ago - "); }

       value << mod_time.GetDay();
       value << wxT("/");
       value << mod_time.GetMonth();
       value << wxT("/");
       value << mod_time.GetYear();

       if (total_pictures>0)
        {
          value << wxT(" - ");
          value << total_pictures;
          value << wxT(" picture");
          if (total_pictures>1) { value << wxT("s"); }
        }

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

    controls_text<< wxT(" \n");
    controls_text<< wxT(" W = Up , S = Down , A = Left , D = Right \n");
    controls_text<< wxT("---------------------------------------------\n");
    controls_text<< wxT(" Arrow Keys = Change Picture \n F = Zoom out , R = Zoom in \n");
    controls_text<< wxT(" Q = Rotate Counter Clockwise \n E = Rotate Clockwise \n");
    controls_text<< wxT("---------------------------------------------\n");
    controls_text<< wxT(" Keys 1,2,3,4,5,6,7,8,9,0 control Image Transactions\n");
    controls_text<< wxT(" This functionality of FlashySlideshows is particularly handy for sorting your images\n\n");
    controls_text<< wxT(" If `File Move` is checked , active Image will be moved to Moved/CategoryX\n");
    controls_text<< wxT(" If `File Resize` is checked , active Image will be resized to Resized/CategoryX using the resolution supplied\n");
    controls_text<< wxT(" If `File Copy` is checked , active Image will be copy to Copied/CategoryX\n");
    controls_text<< wxT(" If `File Link` is checked , active Image will be linked to Linked/CategoryX\n\n");
    controls_text<< wxT(" The X part of CategoryX takes the value of the key pressed\n");
    controls_text<< wxT("---------------------------------------------\n");
    controls_text<< wxT(" Enter = Automatic Slideshow \n M = Cycle Transition Modes \n B = Change Hover Effect \n");
    controls_text<< wxT(" N = Cycle Layouts \n J = Toggle Fullscreen \n Escape = Quit \n");
    wxMessageBox(controls_text, _("FlashySlideShow control list"));

}


/* Checkboxes Move , Resize and Copy are alternated so they are close to each other */
void FlashySlideShowStarterFrame::OnCheckBoxFileMoveClick(wxCommandEvent& event)
{
  CheckBoxFileCopy->SetValue(0);
  CheckBoxFileResize->SetValue(0);
  CheckBoxFileLink->SetValue(0);
  if ( CheckBoxFileResize->IsChecked() ) { ComboBoxResizeResolution->Enable(); } else
                                         { ComboBoxResizeResolution->Disable(); }
  StatusBar1->SetStatusText(wxT("Buttons 0 to 9 will move the currently viewed picture inside the Moved/CategoryX/ folder where X is the number pressed"));

}

void FlashySlideShowStarterFrame::OnCheckBoxFileResizeClick(wxCommandEvent& event)
{
  CheckBoxFileCopy->SetValue(0);
  CheckBoxFileMove->SetValue(0);
  CheckBoxFileLink->SetValue(0);
  StatusBar1->SetStatusText(wxT("Buttons 0 to 9 will make a resized copy of the currently viewed picture inside the Resized/CategoryX/ folder where X is the number pressed"));
  if ( CheckBoxFileResize->IsChecked() ) { ComboBoxResizeResolution->Enable(); } else
                                         { ComboBoxResizeResolution->Disable(); }
}


void FlashySlideShowStarterFrame::OnCheckBoxFileCopyClick(wxCommandEvent& event)
{
  CheckBoxFileResize->SetValue(0);
  CheckBoxFileMove->SetValue(0);
  CheckBoxFileLink->SetValue(0);
  if ( CheckBoxFileResize->IsChecked() ) { ComboBoxResizeResolution->Enable(); } else
                                         { ComboBoxResizeResolution->Disable(); }

  StatusBar1->SetStatusText(wxT("Buttons 0 to 9 will copy the currently viewed picture inside the Copied/CategoryX/ folder where X is the number pressed"));
}



void FlashySlideShowStarterFrame::OnCheckBoxFileLinkClick(wxCommandEvent& event)
{
  CheckBoxFileCopy->SetValue(0);
  CheckBoxFileResize->SetValue(0);
  CheckBoxFileMove->SetValue(0);
  StatusBar1->SetStatusText(wxT("Buttons 0 to 9 will make a link of the currently viewed picture inside the Linked/CategoryX/ folder where X is the number pressed"));
  if ( CheckBoxFileResize->IsChecked() ) { ComboBoxResizeResolution->Enable(); } else
                                         { ComboBoxResizeResolution->Disable(); }
}

/* --------------------------------------------------------------------------------*/

void FlashySlideShowStarterFrame::OnCheckBoxSoundClick(wxCommandEvent& event)
{
  if ( CheckBoxSound->IsChecked() ) { StatusBar1->SetStatusText(wxT("Sound effects enabled..!")); } else
                                    { StatusBar1->SetStatusText(wxT("Sound effects disabled..!")); }
}

void FlashySlideShowStarterFrame::OnCheckBoxVisualsClick(wxCommandEvent& event)
{
  if ( CheckBoxVisuals->IsChecked() ) { StatusBar1->SetStatusText(wxT("Visual effects enabled..!")); } else
                                      { StatusBar1->SetStatusText(wxT("Visual effects disabled..!")); }
}

void FlashySlideShowStarterFrame::OnCheckBoxFaceDetectionClick(wxCommandEvent& event)
{
   if ( CheckBoxFaceDetection->IsChecked() ) { StatusBar1->SetStatusText(wxT("Face Detection enabled..!")); } else
                                             { StatusBar1->SetStatusText(wxT("Face Detection disabled..!")); }
}

void FlashySlideShowStarterFrame::OnCheckBoxMipmapClick(wxCommandEvent& event)
{
   if ( CheckBoxMipmap->IsChecked() ) { StatusBar1->SetStatusText(wxT("OpenGL Texture Mipmaping enabled..!")); } else
                                      { StatusBar1->SetStatusText(wxT("OpenGL Texture Mipmaping disabled..!")); }
}

void FlashySlideShowStarterFrame::OnCheckBoxIncludeSubfoldersClick(wxCommandEvent& event)
{
   if ( CheckBoxIncludeSubfolders->IsChecked() ) { StatusBar1->SetStatusText(wxT("Pictures found in subfolders will be included to your slideshow enabled..!")); } else
                                                 { StatusBar1->SetStatusText(wxT("Subfolder's will not be included in slideshow..!")); }
}
