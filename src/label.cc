#include "label.hh"

LUIC::label::label() {};

LUIC::label::label(str p_txt, ui16 p_posx, ui16 p_posy, flags p_flags):
    txt (p_txt)
{
    type = LUIC_TYPE_LABEL;
    flgs = p_flags;
    colorscheme = {
        __LUIC__COMMON,
        __LUIC__SHDWCLR
    };
    
    posx = p_posx;
    posy = p_posy;
    
    ui16 szx = 0;
    ui16 szy = 1;

    ui16 pos = 0;
    for (ui16 i = 0; i < txt.length(); ++ i) {
        if (txt[i] == 10) {
            ++ szy;

            if (szx < i - pos + 1) szx = i - pos;

            pos = i;
        };
    };

    if (szx < txt.length() - pos + 1) szx = txt.length() - pos;

    wnd = window (posx, posy, szx, szy);
};


void LUIC::label::settxt(str p_txt) {
    txt = p_txt;

    ui16 szx = 0;
    ui16 szy = 1;

    ui16 pos = 0;
    for (ui16 i = 0; i < txt.length(); ++ i) {
        if (txt[i] == 10) {
            ++ szy;

            if (szx < i - pos + 1) szx = i - pos;

            pos = i;
        };
    };

    if (szx < txt.length() - pos + 1) szx = txt.length() - pos;

    wnd .setsz (szx, szy);
};

void LUIC::label::draw() {
    if (ioh == NULL || !vsble) return;

    if (parent == NULL) wnd .drawshdw (colorscheme[1]);

    wnd .setbgclr (colorscheme[0]);

    bool altcharset = false;
    
    if (flgs & LUIC_FLAGS_LBL_ALTCHARSET) {
        altcharset = true;

        wnd .setattr (A_ALTCHARSET, true);
    };
    
    wnd .outat (0, 0, txt);
    
    if (altcharset)
        wnd .setattr (A_ALTCHARSET, false);
};

void LUIC::label::input(i16 p_in, MEVENT* p_evt) {};