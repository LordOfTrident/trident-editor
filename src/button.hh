#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
    class button: public component {
    public:

        button ();
        button (str p_txt, ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy);

        virtual void draw  ();
        virtual void input (i16 p_in, MEVENT* p_evt);

        void settxt    (str p_txt);
        bool isclicked ();

    private:

        str   txt;
        bool  prsd,
              clck,
              linfcs;
    };
};