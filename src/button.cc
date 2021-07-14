#include "button.hh"

LUIC::button::button() {};

LUIC::button::button(str p_txt, ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy, flags p_flags):
    txt    (p_txt),
    prsd   (false),
    clck   (false),
    linfcs (false)
{
    type = LUIC_TYPE_BUTTON;
    flgs = p_flags;
    
    posx = p_posx;
    posy = p_posy;

    wnd = window (posx, posy, p_szx, p_szy);
};

void LUIC::button::settxt(str p_txt) {
    txt = p_txt;
};

bool LUIC::button::isclicked() {
    if (clck) {
        clck = false;
        return true;
    } else
        return false;
};

void LUIC::button::draw() {
    if (ioh == NULL || !vsble) return;

    if (parent == NULL) wnd .drawshdw (__LUIC__SHDWCLR);

    if (wnd.getszy() > 2) {
        wnd .setbgclr (__LUIC__WNDCLR);

        if (prsd)
            wnd .setdbbrdr (0, 0, wnd.getszx() - 1, wnd.getszy() - 1, __LUIC__WBACLR, __LUIC__WBBCLR);
        else 
            wnd .setbrdr   (0, 0, wnd.getszx() - 1, wnd.getszy() - 1, __LUIC__WBACLR, __LUIC__WBBCLR);

        if (linfcs)
            wnd .setclr (__LUIC__SYSCLR);

        wnd .outat (1, wnd.getszy() / 2, str(wnd.getszx() - 2, ' '));
        wnd .outat (wnd.getszx() / 2 - txt.length() / 2, wnd.getszy() / 2, txt);
    } else {
        if (prsd)
            wnd .setbgclr (__LUIC__SYSCLR);
        else if (linfcs)
            wnd .setbgclr (__LUIC__BTNPCLR);
        else 
            wnd .setbgclr (__LUIC__HGHLGHT);

        wnd .outat (wnd.getszx() / 2 - txt.length() / 2, wnd.getszy() / 2, txt);
    };
};

void LUIC::button::input(i16 p_in, MEVENT* p_evt) {
    if (ioh == NULL || !vsble) return;
    
    clck = false;

    switch (p_in) {
        case KEY_MOUSE: {
            if (p_evt->bstate & BUTTON1_RELEASED) {
                if (prsd) {
                    prsd  = false;
                    clck  = true;
                    infcs = false;
                };
            } else if (p_evt->bstate & BUTTON1_PRESSED) {
                if (ioh->__gchldfcsed()) return;

                if (!(p_evt->x >= wnd .getposx () && 
                      p_evt->y >= wnd .getposy () && 
                    
                      p_evt->x < wnd .getszx () + wnd .getposx () && 
                      p_evt->y < wnd .getszy () + wnd .getposy ())) {
                    linfcs = false;
                    
                    break;
                };
                     
                prsd   = true;
                infcs  = false;
                linfcs = true;

                ioh->__schldfcsed (true);
                __fcsprnts ();

                if (parent == NULL) ioh->pbchild(this);
                else parent->pbchild(this);
            };

            break;
        };
    };
};