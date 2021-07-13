#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "iohandle.hh"
#include "frame.hh"

using namespace LUIC;

class editor {
public:

    editor ();
    editor (iohandle *p_ioh, str p_ttl, str p_txt);

    void cleanup ();
    
    frame *getfrm ();

private:

    frame *frm;
};