#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "screen.hh"

namespace LUIC {
    class window {
    public:

        window ();
        window (i16 p_posx, i16 p_posy, ui16 p_szx, ui16 p_szy);

        void  setpos  (i16 p_posx, i16 p_posy);
        ui16 getposx ();
        ui16 getposy ();

        void  setsz  (ui16 p_szx, ui16 p_szy);
        ui16 getszx ();
        ui16 getszy ();

        void setbrdr   (ui16 p_posxa, ui16 p_posya, ui16 p_posxb, ui16 p_posyb, color p_clra, color p_clrb);
        void setdbbrdr (ui16 p_posxa, ui16 p_posya, ui16 p_posxb, ui16 p_posyb, color p_clra, color p_clrb);

        void outat       (ui16 p_posx, ui16 p_posy, str p_txt);
        void outatclr    (ui16 p_posx, ui16 p_posy, str p_txt, color p_clr);
        void wchoutat    (ui16 p_posx, ui16 p_posy, cch* p_ch);
        void wchoutatclr (ui16 p_posx, ui16 p_posy, cch* p_ch, color p_clr);

        void setbgclr (color p_clr);
        void setclr   (color p_clr);
        void setattr  (i32 p_attr, bool p_on);

        void drawshdw (color p_clr);
        void clear    ();

        void __swsz (ui16 p_szx, ui16 p_szy);
        void __swps (ui16 p_posx, ui16 p_posy);

    private:

        ui16 posx, posy,
              szx,  szy;
    };
};