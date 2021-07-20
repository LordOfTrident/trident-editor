#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
    class editor: public component {
    public:

        editor ();
        editor (ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy, flags p_flags);

        virtual void draw  ();
        virtual void input (i16 p_in, MEVENT* p_evt);

        void settxt (str p_txt);
        str  gettxt ();

        void setft (str p_ft);
        str  getft ();

        void setmodif (bool p_m);
        bool getmodif ();

        void setttl (str p_ttl);
        str  getttl ();

    private:

        str txt,
            ft,
            ttl;

        bool prsd,
             modif;

        ui32 curpos,
             curx,
             cury,
             scrx,
             scry;
    };
};