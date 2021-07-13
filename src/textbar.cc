#include "textbar.hh"

LUIC::textbar::textbar() {};

LUIC::textbar::textbar(ui16 p_posx, ui16 p_posy, ui16 p_szx):
    txt    (""),
    prsd   (false),
    curpos (0),
    scrx   (0)
{
    type = LUIC_TYPE_TEXTBAR;
    
    posx = p_posx;
    posy = p_posy;

    wnd = window (posx, posy, p_szx, 3);
};

void LUIC::textbar::settxt(str p_txt) {
    txt = p_txt;

    curpos = 0;
    scrx = 0;
};

str LUIC::textbar::gettxt() {
    return txt;
};

void LUIC::textbar::draw() {
    if (ioh == NULL || !vsble) return;

    if (parent == NULL) wnd .drawshdw (__LUIC__SHDWCLR);

    if (prsd)       wnd .setbgclr (__LUIC__SYSCLR);
    else if (infcs) wnd .setbgclr (__LUIC__TXTCLR);
    else            wnd .setbgclr (__LUIC__TXTNFCS);

    wnd .outat (1, 1, txt.substr(scrx, wnd.getszx() - 2));
   
    if (infcs) {
        wnd .setdbbrdr (0, 0, wnd.getszx() - 1, wnd.getszy() - 1, __LUIC__WBACLR, __LUIC__WBBCLR);
        
        wnd .setattr (A_REVERSE, true);
        wnd .setclr  (__LUIC__SYSCLR);

        if (curpos == txt.length())
            wnd .outat (curpos - scrx + 1, 1, " ");
        else
            wnd .outat (curpos - scrx + 1, 1, str(1, txt[curpos]));
        
        wnd .setattr (A_REVERSE, false);
    } else       wnd .setbrdr   (0, 0, wnd.getszx() - 1, wnd.getszy() - 1, __LUIC__WBACLR, __LUIC__WBBCLR);
};

void LUIC::textbar::input(i16 p_in, MEVENT* p_evt) {
    if (ioh == NULL || !vsble) return;

    switch (p_in) {
        case KEY_MOUSE: {
            if (p_evt->bstate & BUTTON1_RELEASED) {
                if (prsd)
                    prsd  = false;
            } else if (p_evt->bstate & BUTTON1_PRESSED) {
                if (ioh->__gchldfcsed()) return;

                if (!(p_evt->x >= wnd .getposx () && 
                      p_evt->y >= wnd .getposy () && 
                    
                      p_evt->x < wnd .getszx () + wnd .getposx () && 
                      p_evt->y < wnd .getszy () + wnd .getposy ())) {
                    infcs = false;
                    
                    break;
                };
                
                prsd  = true;
                infcs = true;

                ioh->__schldfcsed (false);
                __fcsprnts ();
            };

            break;
        };

        case 10: {
            infcs = false;

            break;
        };

        case KEY_BACKSPACE: {
            if (!infcs)      break;
            if (curpos == 0) break;

            txt.erase(curpos - 1, 1);
            -- curpos;

            break;
        };

        case KEY_LEFT: {
            if (!infcs)      break;
            if (curpos == 0) break;

            -- curpos;

            break;
        };

        case KEY_RIGHT: {
            if (!infcs)                 break;
            if (curpos == txt.length()) break;

            ++ curpos;

            break;
        };

        default: {
            if (!infcs)                  break;
            if (p_in < 32 || p_in > 126) break;

            txt.insert(curpos, str(1, p_in));

            ++ curpos;

            break;
        };
    };

    if ((i16)curpos - (i16)scrx >= wnd.getszx() - 3) 
        scrx = curpos - (wnd.getszx() - 3);
    
    if ((i16)curpos - (i16)scrx < 0) 
        scrx =  curpos;
};