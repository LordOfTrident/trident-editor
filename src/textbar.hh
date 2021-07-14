#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
    class textbar: public component {
    public:

        textbar ();
        textbar (ui16 p_posx, ui16 p_posy, ui16 p_szx, flags p_flags);

        virtual void draw  ();
        virtual void input (i16 p_in, MEVENT* p_evt);

        void settxt (str p_txt);
        str  gettxt ();

    private:

        str  txt;
        bool prsd;
        ui16 curpos,
             scrx;
    };
};