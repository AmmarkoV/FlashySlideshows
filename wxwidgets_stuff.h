#ifndef WXWIDGETS_STUFF_H_INCLUDED
#define WXWIDGETS_STUFF_H_INCLUDED

#ifdef _WXMSW_
#include <windows.h>
#endif


#include <wx/app.h>

class WxWidgetsContext : public wxApp
{
    public:
        virtual bool OnInit();
        virtual bool OnClose();


};

#endif // WXWIDGETS_STUFF_H_INCLUDED
