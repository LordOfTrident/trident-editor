#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
    class textbox: public component {
    public:

        textbox ();
        textbox (str p_ttl, ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy);

        virtual void draw  ();
        virtual void input (i16 p_in, MEVENT* p_evt);

        str  gettxt ();
        void settxt (str p_txt);

        str  getttl ();
        void setttl (str p_ttl);

    private:

        str  txtcrp   (str p_txt, ui16 p_sx, ui16 p_sy, ui16 p_ex, ui16 p_ey);
        bool strendsw (str p_orig, str p_trg);

        str txt,
            ext,
            ttl;

        ui16 curpos,
             curx,
             cury,
             scrx,
             scry;
    };
};