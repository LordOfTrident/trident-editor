#include "screen.hh"

vector <vector <LUIC::screen::pixel>> LUIC::screen::pixels   = {};
color                                 LUIC::screen::lastclr  = 0;
i32                                   LUIC::screen::lastattr = 0;

LUIC::screen::pixel::pixel() {};

void LUIC::screen::outch(ui16 p_posx, ui16 p_posy, ch p_ch) {
    if (p_posx >= getmaxx(stdscr)) return;
    if (p_posy >= getmaxy(stdscr)) return;
    if (p_ch < 32) p_ch = 32;

    screen::pixels[p_posy][p_posx].pch  = p_ch;
    screen::pixels[p_posy][p_posx].pcch = NULL;
    screen::pixels[p_posy][p_posx].pclr = screen::lastclr;
    screen::pixels[p_posy][p_posx].pattr = screen::lastattr;
};


void LUIC::screen::outwch(ui16 p_posx, ui16 p_posy, cch* p_ch) {
    if (p_posx >= getmaxx(stdscr)) return;
    if (p_posy >= getmaxy(stdscr)) return;

    screen::pixels[p_posy][p_posx].pcch  = p_ch;
    screen::pixels[p_posy][p_posx].pclr  = screen::lastclr;
    screen::pixels[p_posy][p_posx].pattr = screen::lastattr;
};

void LUIC::screen::outstr(ui16 p_posx, ui16 p_posy, str p_str) {
    for (ui16 i = 0; i < p_str.length(); ++ i) {
        if (p_posx > getmaxx(stdscr)) {
            ++ p_posy;

            p_posx = 0;
        };

        if (p_str[i] == 10) {
            ++ p_posy;

            p_posx = -1;
        };

        if (p_posy >= getmaxy(stdscr))
            return;

        if (!(p_str[i] == 10)) {
            screen::pixels[p_posy][p_posx].pch   = p_str[i];
            screen::pixels[p_posy][p_posx].pcch  = NULL;
            screen::pixels[p_posy][p_posx].pclr  = screen::lastclr;
            screen::pixels[p_posy][p_posx].pattr = screen::lastattr;
        };

        ++ p_posx;
    };
};

void LUIC::screen::outch_box(ui16 p_posx, ui16 p_posy, ui16 p_minposx, ui16 p_minposy, ui16 p_maxposx, ui16 p_maxposy, ch p_ch) {
    if (p_posx + p_minposx >= getmaxx(stdscr)) return;
    if (p_posy + p_minposy >= getmaxy(stdscr)) return;

    screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pch  = p_ch;
    screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pcch = NULL;
    screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pclr = screen::lastclr;
    screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pattr = screen::lastattr;
};

void LUIC::screen::outwch_box(ui16 p_posx, ui16 p_posy, ui16 p_minposx, ui16 p_minposy, ui16 p_maxposx, ui16 p_maxposy, cch* p_ch) {    
    if (p_posx + p_minposx >= getmaxx(stdscr)) return;
    if (p_posy + p_minposy >= getmaxy(stdscr)) return;

    screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pcch  = p_ch;
    screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pclr  = screen::lastclr;
    screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pattr = screen::lastattr;
};

void LUIC::screen::outstr_box(ui16 p_posx, ui16 p_posy, ui16 p_minposx, ui16 p_minposy, ui16 p_maxposx, ui16 p_maxposy, str p_str) {
    for (ui16 i = 0; i < p_str.length(); ++ i) {
        if (p_posx > p_maxposx) {
            ++ p_posy;

            p_posx = 0;
        };

        if (p_posy > p_maxposy)
            return;
        
        if (p_str[i] == 10) {
            ++ p_posy;

            p_posx = -1;
        };

        if (p_posy + p_minposy >= getmaxy(stdscr))
            return;


        if (!(p_posx + p_minposx >= getmaxx(stdscr)) && !(p_str[i] == 10)) {
            screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pch   = p_str[i];
            screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pcch  = NULL;
            screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pclr  = screen::lastclr;
            screen::pixels[p_posy + p_minposy][p_posx + p_minposx].pattr = screen::lastattr;
        };

        ++ p_posx;
    };
};

void LUIC::screen::update() {
    move (0, 0);

    i32 prevattr = 0;
    for (ui16 i = 0; i < (ui16)screen::pixels.size(); ++ i)
        for (ui16 j = 0; j < (ui16)screen::pixels[i].size(); ++ j) {
            if (prevattr != screen::pixels[i][j].pattr) {
                attroff (prevattr);

                prevattr = screen::pixels[i][j].pattr;
                attron (prevattr);
            };

            attron (COLOR_PAIR (screen::pixels[i][j].pclr));

            if (screen::pixels[i][j].pch < 32)
                screen::pixels[i][j].pch = 32;

            if (screen::pixels[i][j].pcch == NULL)
                addch   (screen::pixels[i][j].pch);
            else 
                add_wch (screen::pixels[i][j].pcch);
        };
    
    attroff (prevattr);
    
    refresh ();
};

void LUIC::screen::fillch_box(ui16 p_minposx, ui16 p_minposy, ui16 p_maxposx, ui16 p_maxposy, ch p_ch) {
    for (ui16 i = p_minposy; i < p_maxposy + p_minposy; ++ i)
        for (ui16 j = p_minposx; j < p_maxposx + p_minposx; ++ j)
            outch (j, i, p_ch);
};