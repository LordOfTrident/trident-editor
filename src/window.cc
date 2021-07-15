#include "window.hh"

LUIC::window::window() {};

LUIC::window::window(i16 p_posx, i16 p_posy, ui16 p_szx, ui16 p_szy):
    posx (p_posx < 0? 0 : p_posx),
    posy (p_posy < 0? 0 : p_posy),
    szx  (p_szx),
    szy  (p_szy)
{
    if (szx > getmaxx(stdscr)) szx = getmaxx(stdscr);
    if (szy > getmaxy(stdscr)) szy = getmaxy(stdscr);

    if (posx > getmaxx(stdscr)) posx = getmaxx(stdscr);
    if (posy > getmaxy(stdscr)) posy = getmaxy(stdscr);
};

void LUIC::window::setpos(i16 p_posx, i16 p_posy) {
    if (p_posx > getmaxx(stdscr)) p_posx = getmaxx(stdscr);
    if (p_posy > getmaxy(stdscr)) p_posy = getmaxy(stdscr);

    if (p_posx < 0) p_posx = 0;
    if (p_posy < 0) p_posy = 0;

    posx = p_posx;
    posy = p_posy;
};

ui16 LUIC::window::getposx() {
    return posx;
}

ui16 LUIC::window::getposy() {
    return posy;
};

void  LUIC::window::setsz(ui16 p_szx, ui16 p_szy) {
    szx = p_szx;
    szy = p_szy;

    if (szx > getmaxx(stdscr)) szx = getmaxx(stdscr);
    if (szy > getmaxy(stdscr)) szy = getmaxy(stdscr);
};

ui16 LUIC::window::getszx() {
    return szx;
};

ui16 LUIC::window::getszy() {
    return szy;
};

void LUIC::window::setbrdr(ui16 p_posxa, ui16 p_posya, ui16 p_posxb, ui16 p_posyb, color p_clra, color p_clrb) {
    color prevclr = screen::lastclr;

    screen::lastattr = A_ALTCHARSET;
    screen::lastclr  = p_clra;

    screen::outch_box (p_posxa, p_posya, posx, posy, szx, szy, ACS_ULCORNER);
    screen::outch_box (p_posxa, p_posyb, posx, posy, szx, szy, ACS_LLCORNER);

    for (ui16 i = p_posxa + 1; i < p_posxb; ++ i) 
        screen::outch_box (i, p_posya, posx, posy, szx, szy, ACS_HLINE);
    
    for (ui16 i = p_posya + 1; i < p_posyb; ++ i)
        screen::outch_box (p_posxa, i, posx, posy, szx, szy, ACS_VLINE);

    screen::lastclr  = p_clrb;

    screen::outch_box (p_posxb, p_posya, posx, posy, szx, szy, ACS_URCORNER);
    screen::outch_box (p_posxb, p_posyb, posx, posy, szx, szy, ACS_LRCORNER);

    for (ui16 i = p_posxa + 1; i < p_posxb; ++ i) 
        screen::outch_box (i, p_posyb, posx, posy, szx, szy, ACS_HLINE);
    
    for (ui16 i = p_posya + 1; i < p_posyb; ++ i)
        screen::outch_box (p_posxb, i, posx, posy, szx, szy, ACS_VLINE);

    screen::lastattr = 0;
    screen::lastclr = prevclr;
};

void LUIC::window::setdbbrdr(ui16 p_posxa, ui16 p_posya, ui16 p_posxb, ui16 p_posyb, color p_clra, color p_clrb) {
    color prevclr = screen::lastclr;

    screen::lastattr = A_ALTCHARSET;
    screen::lastclr  = p_clra;

    screen::outwch_box (p_posxa, p_posya, posx, posy, szx, szy, WACS_D_ULCORNER);
    screen::outwch_box (p_posxa, p_posyb, posx, posy, szx, szy, WACS_D_LLCORNER);

    for (ui16 i = p_posxa + 1; i < p_posxb; ++ i) 
        screen::outwch_box (i, p_posya, posx, posy, szx, szy, WACS_D_HLINE);
    
    for (ui16 i = p_posya + 1; i < p_posyb; ++ i)
        screen::outwch_box (p_posxa, i, posx, posy, szx, szy, WACS_D_VLINE);

    screen::lastclr  = p_clrb;

    screen::outwch_box (p_posxb, p_posya, posx, posy, szx, szy, WACS_D_URCORNER);
    screen::outwch_box (p_posxb, p_posyb, posx, posy, szx, szy, WACS_D_LRCORNER);

    for (ui16 i = p_posxa + 1; i < p_posxb; ++ i) 
        screen::outwch_box (i, p_posyb, posx, posy, szx, szy, WACS_D_HLINE);
    
    for (ui16 i = p_posya + 1; i < p_posyb; ++ i)
        screen::outwch_box (p_posxb, i, posx, posy, szx, szy, WACS_D_VLINE);

    screen::lastattr = 0;
    screen::lastclr = prevclr;
};

void LUIC::window::wchoutat(ui16 p_posx, ui16 p_posy, cch* p_ch) {
    screen::outwch_box (p_posx, p_posy, posx, posy, szx, szy, p_ch);
};

void LUIC::window::wchoutatclr (ui16 p_posx, ui16 p_posy, cch* p_ch, color p_clr) {
    color prevclr = screen::lastclr;

    screen::lastclr = p_clr;
    screen::outwch_box (p_posx, p_posy, posx, posy, szx, szy, p_ch);

    screen::lastclr = prevclr;
};

void LUIC::window::outatclr(ui16 p_posx, ui16 p_posy, str p_txt, color p_clr) {
    color prevclr = screen::lastclr;

    screen::lastclr = p_clr;
    screen::outstr_box(p_posx, p_posy, posx, posy, szx, szy, p_txt);

    screen::lastclr = prevclr;
};

void LUIC::window::outat(ui16 p_posx, ui16 p_posy, str p_txt) {
    screen::outstr_box(p_posx, p_posy, posx, posy, szx, szy, p_txt);
};

void LUIC::window::setbgclr(color p_clr) {
    screen::lastclr = p_clr;
    screen::fillch_box (posx, posy, szx, szy, ' ');
};

void LUIC::window::setclr(color p_clr) {
    screen::lastclr = p_clr;
};

void LUIC::window::setattr(i32 p_attr, bool p_on) {
    screen::lastattr = p_on? p_attr : 0;
};

void LUIC::window::drawshdw(color p_clr) {
    if (szy > 1) {
        color prevclr = screen::lastclr;

        screen::lastclr = p_clr;

        ui16 minposx = posx + szx,
              maxposx = 2,
              minposy = posy + 1,
              maxposy = szy;

        for (ui16 i = minposy; i < maxposy + minposy; ++ i)
            for (ui16 j = minposx; j < maxposx + minposx; ++ j) {
                if (j >= getmaxx(stdscr)) continue;
                if (i >= getmaxy(stdscr)) continue;
                
                screen::pixels[i][j].pclr = p_clr;
            };
        
        screen::lastclr = prevclr;
    };
    
    if (szx > 1) {
        color prevclr = screen::lastclr;

        screen::lastclr = p_clr;

        ui16 minposx = posx + 2,
              maxposx = szx,
              minposy = posy + szy,
              maxposy = 1;

        for (ui16 i = minposy; i < maxposy + minposy; ++ i)
            for (ui16 j = minposx; j < maxposx + minposx; ++ j) {
                if (j >= getmaxx(stdscr)) continue;
                if (i >= getmaxy(stdscr)) continue;

                screen::pixels[i][j].pclr = p_clr;
            };
        
        screen::lastclr = prevclr;
    };
};

void LUIC::window::clear() {
    screen::fillch_box (posx, posy, szx, szy, ' ');
};

void LUIC::window::__swsz(ui16 p_szx, ui16 p_szy) {
    if (p_szx > getmaxx(stdscr)) p_szx = getmaxx(stdscr);
    if (p_szy > getmaxy(stdscr)) p_szy = getmaxy(stdscr);
};

void LUIC::window::__swps(ui16 p_posx, ui16 p_posy) {
    if (p_posx > getmaxx(stdscr)) p_posx = getmaxx(stdscr);
    if (p_posy > getmaxy(stdscr)) p_posy = getmaxy(stdscr);

    if (p_posx < 0) p_posx = 0;
    if (p_posy < 0) p_posy = 0;
};