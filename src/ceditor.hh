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
#define __CEDIT_COL__KEYWORD 103
#define __CEDIT_COL__STRING  104
#define __CEDIT_COL__NUMBER  105
#define __CEDIT_COL__BOOLEAN 106
#define __CEDIT_COL__DATATYP 107
#define __CEDIT_COL__STD     108
#define __CEDIT_COL__COMMENT 109
#define __CEDIT_COL__PREPROC 110

extern i8 editor_fg, 
          editor_bg;

class ceditor {
public:

    ceditor ();
    ceditor (LUIC::iohandle *p_ioh, str p_ttl, str p_txt);
    ceditor (LUIC::iohandle *p_ioh, str p_ttl, str p_txt, i16 p_posx, i16 p_posy, ui16 p_szx, ui16 p_szy);

    void cleanup ();
    
    LUIC::frame  *getfrm   ();
    LUIC::editor *gettxtbx ();

    void setfname (str p_ttl);
    str  getfname ();

private:

    LUIC::frame  *frm;
    LUIC::editor *txtbx;
};