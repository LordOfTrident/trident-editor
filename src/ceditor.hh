#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "iohandle.hh"
#include "frame.hh"
#include "editor.hh"

using namespace LUIC;

class ceditor {
public:

    ceditor ();
    ceditor (iohandle *p_ioh, str p_ttl, str p_txt);

    void cleanup ();
    
    frame   *getfrm   ();
    editor *gettxtbx ();

private:

    frame   *frm;
    editor *txtbx;
};