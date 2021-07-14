#include "editor.hh"

str __shptxtbuf(str p_txt, i32 p_x, i32 p_y, i32 p_szx, i32 p_szy) {
    str shpdstr = "";
    i32 posx = 0, posy = 0;
    
    for (ui32 i = 0; i < p_txt.length(); ++ i) {
        if (((posx >= p_x && posx < p_szx) || p_txt[i] == 10) && posy >= p_y && posy < p_szy)
            shpdstr += p_txt[i];

        if (p_txt[i] == 10) {
            posx = -1;

            ++ posy;
        };

        ++ posx;
    };

    return shpdstr;
};

LUIC::editor::editor() {};

LUIC::editor::editor(ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy, flags p_flags):
    txt    (""),
    prsd   (false),
    curpos (0),
    curx   (0),
    cury   (0),
    scrx   (0),
    scry   (0)
{
    type = LUIC_TYPE_EDITOR;
    flgs = p_flags | LUIC_FLAGS_COMPONENT_SCALED;

    colorscheme = {
        __LUIC__SHDWCLR,
        __LUIC__TXTCLR,
        __LUIC__TXTCLR,
        __LUIC__SYSCLR
    };
    
    posx = p_posx;
    posy = p_posy;

    wnd = window (posx, posy, p_szx, p_szy);
};

void LUIC::editor::settxt(str p_txt) {
    txt = p_txt;

    curpos = 0;
    scrx = 0;
    scry = 0;
};

str LUIC::editor::gettxt() {
    return txt;
};

void LUIC::editor::setft(str p_ft) {
    ft = p_ft;
};

str LUIC::editor::getft() {
    return txt;
};

void LUIC::editor::draw() {
    if (ioh == NULL || !vsble) return;

    if (parent == NULL) wnd .drawshdw (colorscheme[0]);

    if (prsd)       wnd .setbgclr (colorscheme[3]);
    else            wnd .setbgclr (colorscheme[2]);

    str ftxt = __shptxtbuf(txt, scrx, scry, getszx() + scrx, getszy() + scry);

    ui16 rndrx = 0, rndry = 0;
    for (ui32 i = 0; i <= (ui32)ftxt.length(); ++ i) {
        bool curon = false;

        if (rndrx == curx - scrx && rndry == cury - scry && infcs) {
            wnd .setattr (A_REVERSE, true);

            curon = true;
        };

        if (i < ftxt.length() && ftxt[i] != 10) {
            if (ftxt[i] > 31 && ftxt[i] < 127) wnd .outat (rndrx, rndry, str(1, ftxt[i]));

            ++ rndrx;
        } else {
            if (curon)
                wnd .outat   (rndrx, rndry, " ");

            if (ftxt[i] == 10) {
                rndrx = 0;

                ++ rndry;
            };
        };

        if (curon)
            wnd .setattr (A_REVERSE, false);
    };

    //wnd .outatclr (0, getszy() - 1, str(getszx(), ' '), colorscheme[1]);

    if (parent != NULL) 
        parent->__gwndacs ()->outatclr (1, parent->getszy () - 1, str("0:0 | ft: " + ft + " | en: ASCII").substr(0, parent->getszx () - 2), colorscheme[1]);
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

        case 27: {
            if (!infcs) break;
            infcs = false;

            break;
        };

        case 10: {
            if (!infcs) break;
            txt  .insert(curpos, "\n");
            curx =       0;

            ++ cury;
            ++ curpos;

            break;
        };

        case 9: {
            if (!infcs) break;
            txt   .insert(curpos, "    ");
            curx   +=     4;
            curpos +=     4;

            break;
        };

        case KEY_BACKSPACE: {
            if (!infcs) break;
            if (curpos == 0) break;

            if (txt[curpos - 1] == 10) {
                ui32 idx = txt.substr(0, curpos - 1).find_last_of(10);
                if (idx == txt.npos) idx = 0;

                curx = curpos - idx - 2;

                -- cury;
            } else 
                -- curx;

            txt.erase(curpos - 1, 1);

            -- curpos;

            break;
        };

        case KEY_LEFT: {
            if (!infcs) break;
            if (curpos == 0) break;

            if (txt[curpos - 1] == 10) {
                ui32 idx = txt.substr(0, curpos - 1).find_last_of(10);
                
                if (idx == txt.npos) idx = 0;

                curx = curpos - idx - 2;

                -- cury;
            } else 
                -- curx;

            -- curpos;

            break;
        };

        case KEY_RIGHT: {
            if (!infcs) break;
            if (curpos == txt.length()) break;

            if (txt[curpos] == 10) {
                curx = 0;

                ++ cury;
            } else
                ++ curx;

            ++ curpos;

            break;
        };

        default: {
            if (!infcs) break;
            if (p_in < 32 || p_in > 126) break;

            str chr =       "";
                chr += (i8) p_in;

            txt.insert(curpos, chr);

            ++ curx;
            ++ curpos;

            break;
        };
    };

    if ((i16)curx - (i16)scrx >= wnd.getszx() - 1) 
        scrx = curx - (wnd.getszx() - 1);
    else if ((i16)curx - (i16)scrx < 0) 
        scrx = curx;

    if ((i16)cury - (i16)scry >= wnd.getszy() - 1) 
        scry = cury - (wnd.getszy() - 1);
    else if ((i16)cury - (i16)scry < 0) 
        scry = cury;
};