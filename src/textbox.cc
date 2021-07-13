/*#include "textbox.hh"

LUIC::textbox::textbox() {};

LUIC::textbox::textbox(str p_ttl, ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy):
    ttl    (p_ttl),
    curpos (0    ),
    curx   (0    ),
    cury   (0    ),
    scrx   (0    ),
    scry   (0    )
{
    type = LUIC_TYPE_FRAME;
    
    posx = p_posx;
    posy = p_posy;

    wnd = window (posx, posy, p_szx, p_szy);

    if      (strendsw(ttl, ".cpp") || strendsw(ttl, ".cc")) ext = "C++";
    else if (strendsw(ttl, ".c")                          ) ext = "C";
    else if (strendsw(ttl, ".js")                         ) ext = "JS";
    else if (strendsw(ttl, ".tm")                         ) ext = "Temple";
    else if (strendsw(ttl, ".pas")                        ) ext = "Pascal";
    else if (strendsw(ttl, ".txt")                        ) ext = "Text";
    else                                                    ext = "?";
};

void LUIC::textbox::settxt(str p_txt) {
    txt    = p_txt;
    curpos = 0;
    curx   = 0;
    cury   = 0;
};

str LUIC::textbox::gettxt() {
    return txt;
};

void LUIC::textbox::setttl(str p_ttl) {
    ttl = p_ttl;

    if      (strendsw(ttl, ".cpp") || strendsw(ttl, ".cc")) ext = "C++";
    else if (strendsw(ttl, ".c")                          ) ext = "C";
    else if (strendsw(ttl, ".js")                         ) ext = "JS";
    else if (strendsw(ttl, ".tm")                         ) ext = "Temple";
    else if (strendsw(ttl, ".pas")                        ) ext = "Pascal";
    else if (strendsw(ttl, ".txt")                        ) ext = "Text";
    else                                                    ext = "?";
};

str LUIC::textbox::txtcrp (str p_txt, ui16 p_sx, ui16 p_sy, ui16 p_ex, ui16 p_ey) {
    str  crpedstr = "";
    ui16 x        = 0, 
         y        = 0;

    for (ui32 i = 0; i < (ui32)p_txt.length(); ++ i) {
        if (x >= p_sx && y >= p_sy && x <= p_ex && y <= p_ey) {
            crpedstr += p_txt[i];

            if (x == p_ex && p_txt[i] != 10) 
                crpedstr += 10;
        };

        if (p_txt[i] == 10) {
            if (x < p_sx && y >= p_sy && y <= p_ey) 
                crpedstr += 10;

            x = 0;
            ++ y;

            continue;
        };

        ++ x;
    };

    return crpedstr;
};

bool LUIC::textbox::strendsw(str p_orig, str p_trg) {
    if (p_trg.length() > p_orig.length()) return false;

    if (p_orig.substr(p_orig.length() - p_trg.length(), p_trg.length()) == p_trg) 
        return true;
    
    return false;
};

void LUIC::textbox::draw() {
    ui8 offst = wnd->_maxx - ext.length() - 1;

    werase(wnd);

    str rendertxt = txtcrp(txt, scrx, scry, wnd->_maxx - 2 + scrx, wnd->_maxy - 2 + scry),
        token = "";

    wmove(wnd, 1, 1);

    for (ui32 i = 0; i < (ui32)rendertxt.length(); ++ i) {
        switch (rendertxt[i]) {
            case 32: {
                token = "";

                break;
            };

            case 10: {
                token = "";

                ui16 cx, cy;
                getyx(wnd, cy, cx);

                wmove(wnd, cy + 1, 1);

                continue;
            };

            default: {
                token += rendertxt[i];

                break;
            };
        };

        waddch(wnd, rendertxt[i]);
    };

    box       (wnd, 0, 0                              );

    mvwprintw (wnd, 0, 1, ttl.c_str()                 );
    mvwprintw (wnd, wnd->_maxy, 1, "%i:%i", cury, curx);
    mvwprintw (wnd, wnd->_maxy, offst, ext.c_str()    );

    wrefresh  (wnd                                    );
};

void LUIC::textbox::input() {
    MEVENT evt;
    i16   in = wgetch(wnd);

    wresize(wnd, getmaxy(stdscr) - 1, getmaxx(stdscr));

    switch (in) {
        case KEY_MOUSE: {
            if (getmouse(&evt) == OK) {
                if (evt.bstate & BUTTON1_CLICKED) {
                    if ((evt.y <= wnd->_maxy + 1 && evt.y >= wnd->_begy)
                        &&
                        (evt.x <= wnd->_maxx && evt.x >= wnd->_begx)) 
                    {
                        if ((evt.y <= wnd->_maxy && evt.y > wnd->_begy)
                            &&
                            (evt.x < wnd->_maxx && evt.x > wnd->_begx)) 
                        {

                            ui16 ix = curx, 
                                 iy = cury;

                            curx = evt.x - wnd->_begx - 1;
                            cury = evt.y - wnd->_begy - 1;

                            if (iy > cury || (ix > curx && iy == cury))
                                for (ui32 i = curpos; i > 0; -- i) {
                                    if (ix == curx && iy == cury) break;

                                    if (txt[i] == 10) {
                                        ui32 idx = txt.substr(0, curpos - 1).find_last_of(10);
                                        if (idx == txt.npos) idx = 0;

                                        ix = curpos - idx - 2;

                                        -- iy;
                                    } else
                                        -- ix;

                                    -- curpos;
                                };

                            if (curpos == 0) {
                                curx = 0;
                                cury = 0;
                            };
                        };
                    } else infcs = false;
                };
            };

            break;
        };

        case 27: {
            infcs = false;

            break;
        };

        case 10: {
            txt  .insert(curpos, "\n");
            curx =       0;

            ++ cury;
            ++ curpos;

            break;
        };

        case 9: {
            txt   .insert(curpos, "    ");
            curx   +=     4;
            curpos +=     4;

            break;
        };

        case KEY_BACKSPACE: {
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
            if (in < 32 || in > 126) break;

            str chr =       "";
                chr += (i8) in;

            txt.insert(curpos, chr);

            ++ curx;
            ++ curpos;

            break;
        };
    };

    if ((i16)curx - (i16)scrx > wnd->_maxx - 1) 
        scrx = curx - (wnd->_maxx - 1);
    
    if ((i16)curx - (i16)scrx < 0) 
        scrx = curx;
};*/