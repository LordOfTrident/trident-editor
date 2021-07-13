#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"

namespace LUIC {
    namespace screen {
        class pixel {
        public:

            pixel ();

            color pclr;
            i32   pattr;
            ch    pch;
            cch*  pcch;
        };

        extern vector <vector <pixel>> pixels;
        extern color                   lastclr;
        extern i32                     lastattr;

        void outch  (ui16 p_posx, ui16 p_posy, ch p_ch);
        void outwch (ui16 p_posx, ui16 p_posy, cch* p_ch);
        void outstr (ui16 p_posx, ui16 p_posy, str p_str);

        void outch_box  (ui16 p_posx, ui16 p_posy, ui16 p_minposx, ui16 p_minposy, ui16 p_maxposx, ui16 p_maxposy, ch p_ch);
        void outwch_box (ui16 p_posx, ui16 p_posy, ui16 p_minposx, ui16 p_minposy, ui16 p_maxposx, ui16 p_maxposy, cch* p_ch);
        void outstr_box (ui16 p_posx, ui16 p_posy, ui16 p_minposx, ui16 p_minposy, ui16 p_maxposx, ui16 p_maxposy, str p_str);

        void fillch_box (ui16 p_minposx, ui16 p_minposy, ui16 p_maxposx, ui16 p_maxposy, ch p_ch);

        void update ();
    };
};