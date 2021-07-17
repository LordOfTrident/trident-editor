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

#define SCBL_TABSIZE 0
#define SCBL_COLOR   1

#define SCBL_BG    100
#define SCBL_FG    101
#define SCBL_FRAME 102

#define SCBL_KEYWORD  103
#define SCBL_STRING   104
#define SCBL_NUMBER   105
#define SCBL_BOOLEAN  106
#define SCBL_DATATYPE 107
#define SCBL_STD      108
#define SCBL_COMMENT  109

extern i8 editor_fg, 
          editor_bg;

using namespace LUIC;

class ceditor {
public:

    ceditor ();
    ceditor (iohandle *p_ioh, str p_ttl, str p_txt);

    void cleanup ();
    
    frame  *getfrm   ();
    editor *gettxtbx ();

    void setfname (str p_ttl);
    str  getfname ();

private:

    frame  *frm;
    editor *txtbx;
};