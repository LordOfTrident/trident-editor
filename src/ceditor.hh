#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "iohandle.hh"
#include "frame.hh"
#include "editor.hh"

#define __CEDIT_COL__DEF     100
#define __CEDIT_COL__DEF_FRM 101
#define __CEDIT_COL__DEF_GRN 102

extern i8 editor_fg, 
          editor_bg;

class ceditor {
public:

    ceditor ();
    ceditor (LUIC::iohandle *p_ioh, str p_ttl, str p_txt);

    void cleanup ();
    
    LUIC::frame  *getfrm   ();
    LUIC::editor *gettxtbx ();

    void setfname (str p_ttl);
    str  getfname ();

private:

    LUIC::frame  *frm;
    LUIC::editor *txtbx;
};