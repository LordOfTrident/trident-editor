#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
    class frame: public component {
    public:

        frame ();
        frame (str p_ttl, ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy, flags p_flags);

        virtual void draw  ();
        virtual void input (i16 p_in, MEVENT* p_evt);

        void setttl (str p_ttl);
        str  getttl ();

    private:

        str   ttl;
        bool  prsd,
              clse,
              drg,
              rsz;

        ui16 offstx, 
             offsty;
    };
};