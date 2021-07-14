#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

#define LUIC_FLAGS_FRM_DRAGGABLE 10
#define LUIC_FLAGS_FRM_CLOSABLE  11
#define LUIC_FLAGS_FRM_RESIZABLE 52

namespace LUIC {
    class frame: public component {
    public:

        frame ();
        frame (str p_ttl, ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy, flags p_flags);

        virtual void draw  ();
        virtual void input (i16 p_in, MEVENT* p_evt);

        void setttl   (str p_ttl);

        bool lstinfcs ();

    private:

        str   ttl;
        bool  prsd,
              clse,
              drg,
              rsz,
              linfcs;

        ui16 offstx, 
             offsty;
    };
};