#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
    class label: public component {
    public:

        label ();
        label (str p_txt, ui16 p_posx, ui16 p_posy, flags p_flags);

        virtual void draw  ();
        virtual void input (i16 p_in, MEVENT* p_evt);

        void settxt   (str p_txt);

    private:

        str   txt;
    };
};