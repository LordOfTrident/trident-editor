#include "editor.hh"

LUIC::editor::editor() {};

LUIC::editor::editor(ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy, flags p_flags):
    txt    (""),
    prsd   (false),
    curpos (0),
    scrx   (0)
{
    type = LUIC_TYPE_EDITOR;
    flgs = p_flags | LUIC_FLAGS_COMPONENT_SCALED;

    colorscheme = {
        __LUIC__SHDWCLR,
        __LUIC__SYSCLR,
        __LUIC__TXTCLR,
        __LUIC__TXTNFCS
    };
    
    posx = p_posx;
    posy = p_posy;

    wnd = window (posx, posy, p_szx, p_szy);
};

void LUIC::editor::settxt(str p_txt) {
    txt = p_txt;

    curpos = 0;
    scrx = 0;
};

str LUIC::editor::gettxt() {
    return txt;
};

void LUIC::editor::draw() {
    if (ioh == NULL || !vsble) return;

    if (parent == NULL) wnd .drawshdw (colorscheme[0]);

    if (prsd)       wnd .setbgclr (colorscheme[1]);
    else if (infcs) wnd .setbgclr (colorscheme[2]);
    else            wnd .setbgclr (colorscheme[3]);

    wnd .outat (0, 0, txt.substr(scrx, wnd.getszx() - 2));
   
    if (infcs) {
        wnd .setattr (A_REVERSE, true);
        wnd .setclr  (colorscheme[1]);

        if (curpos == txt.length())
            wnd .outat (curpos - scrx, 0, " ");
        else
            wnd .outat (curpos - scrx, 0, str(1, txt[curpos]));
        
        wnd .setattr (A_REVERSE, false);
    };

    //wnd .outatclr (0, getszy() - 1, str(getszx(), ' '), colorscheme[1]);

    if (parent != NULL) 
        parent->__gwndacs ()->outatclr (1, parent->getszy () - 1, str("0:0 | ft: txt | en: ASCII").substr(0, parent->getszx ()), colorscheme[1]);
};

void LUIC::editor::input(i16 p_in, MEVENT* p_evt) {
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