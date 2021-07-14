#include "topbar.hh"

str __gcurdt() {
    time_t    now = time(0);
    struct tm tstruct;
    char      buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), " %d/%m/%Y %X ", &tstruct);

    return buf;
};

LUIC::option::option() {};

LUIC::option::option(str p_name, vector <suboption> p_subopts):
    name    (p_name),
    subopts (p_subopts)
{};;

LUIC::suboption::suboption() {};

LUIC::suboption::suboption(str p_name, str p_kb):
    name    (p_name),
    keybind (p_kb) 
{
    if (name.length() > 9) {
        name =  name.substr(0, 9);
        name += "... " + keybind;;
    } else
        name += str (13 - name.length(), ' ') + keybind;
};

LUIC::topbar::topbar() {};

LUIC::topbar::topbar(str p_ttl, vector <option> p_options):
    ttl     (" " + p_ttl + " "),
    prsd    (false),
    ovr     (0),
    subovr  (0),
    chc     (-1),
    subchc  (0),
    options (p_options) 
{
    type = LUIC_TYPE_TOPBAR;
    
    posx = 0;
    posy = 0;

    wnd  = window (posx, posy, 5, 1);
    ownd = window (0,    0,    5, 5);

    walker_sprites[0] = '/';
    walker_sprites[1] = '-';
    walker_sprites[2] = '\\';
    walker_sprites[3] = '|';
    walker_state = 0;
    walker = 0;
};

void LUIC::topbar::setttl(str p_ttl) {
    ttl = " " + p_ttl + " ";
};

i8 LUIC::topbar::getsboptst(ui8 p_idx) {
    if (chc != p_idx) return LUIC_TOPBAR_OPTNOTCHOSEN;

    return subchc;
};

void LUIC::topbar::draw() {
    if (ioh == NULL || !vsble) return;

    if (parent == NULL) {
        if (ioh->getwsizex() != wnd.getszx())
            wnd .setsz (ioh->getwsizex(), 1);
    } else {
        if (parent->getszx() != wnd.getszx()) 
            wnd .setsz (parent->getszx(), 1);
    };

    str date = __gcurdt ();

    wnd .setbgclr (__LUIC__SYSCLR);
    wnd .setattr  (A_BLINK, true);
    wnd .outatclr (wnd .getszx() - ttl.length(), 0, ttl.c_str(), __LUIC__HGHLGHT);
    wnd .setattr  (A_BLINK, false);
    wnd .outatclr (wnd .getszx() - ttl.length() - date.length(), 0, date.c_str(), __LUIC__COMMON);

    ui16 pos = 0;

    wnd .setclr (__LUIC__COMMON);
    for (ui16 i = 0; i < options.size(); ++ i) {
        if (i == ovr && infcs) {
            wnd  .setclr (__LUIC__HGHLGHT);
            ownd .setpos  (pos, 1);
        };

        wnd .outat (pos, 0, " " + options[i].name + " ");
        pos += options[i].name.length() + 2;

        if (i == ovr && infcs)
            wnd .setclr (__LUIC__COMMON);
    };

    //wnd .outatclr (pos, 0, std::to_string (ioh->gettick ()), __LUIC__SYSCLR);

    if (walker_state != -1) 
        wnd .outatclr (walker + pos, 0, str(1, walker_sprites[walker_state]), __LUIC__SYSCLR);
        //wnd .wchoutatclr (walker +pos, 0, WACS_LANTERN, __LUIC__SYSCLR);

    if (infcs) {
        ui16 szx = 0;

        for (ui16 i = 0; i < options[ovr].subopts.size(); ++ i) {
            if (options[ovr].subopts[i].name.length() > szx)
                szx = options[ovr].subopts[i].name.length();
        };

        ownd .setsz  (szx + 4, options[ovr].subopts.size() + 2);
        ownd .setbgclr (__LUIC__COMMON);

        for (ui16 i = 0; i < options[ovr].subopts.size(); ++ i) {
            if (i == subovr)
                ownd .setclr (__LUIC__HGHLGHT);

            ownd .outat (1, i + 1, " " + options[ovr].subopts[i].name + str (szx - options[ovr].subopts[i].name.length() + 2, ' '));

            if (i == subovr)
                ownd .setclr (__LUIC__COMMON);
        };

        ownd .drawshdw (__LUIC__SHDWCLR);
        ownd .setbrdr  (0, 0, ownd.getszx() - 1, ownd.getszy() - 1, __LUIC__WBACLR, __LUIC__WBBCLR);
    };

    if (ioh->gettick() % 8 == 0) {
        ++ walker;
        i32 max = wnd. getszx () - pos - date .length () - ttl .length () - 1;

        if (walker_state == -1 && max >= 0) walker_state = 0;
        if (max < 0) walker_state = -1;
        if (walker > max) walker = 0;
    };

    if (ioh->gettick() % 16 == 0 && walker_state != -1) {
        ++ walker_state;
        if (walker_state > 3) walker_state = 0;
    };
};

void LUIC::topbar::input(i16 p_in, MEVENT* p_evt) {
    if (ioh == NULL || !vsble) return;

    chc    = -1;
    subchc = -1;

    switch (p_in) {
        case KEY_MOUSE: {
            if (p_evt->bstate & BUTTON1_RELEASED) {
                if (prsd) {
                    chc    = ovr;
                    subchc = subovr;
                    infcs  = false;
                    prsd   = false;
                };
            } else if (p_evt->bstate & BUTTON1_PRESSED) {
                if (p_evt->y == posy && 
                    p_evt->x >= posx && 
                    p_evt->x < posx + wnd.getszx()) {
                    ioh->__schldfcsed (false);
                    infcs = true;

                    ui16 pos = 0;

                    for (ui16 i = 0; i < options.size(); ++ i) {
                        if (p_evt->x >= pos && p_evt->x < pos + 2 + (i32)options[i].name.length()) {
                            ovr = i;
                            subovr = 0;

                            break;
                        };

                        pos += options[i].name.length() + 2;
                    };
                } else if (p_evt->x >= ownd.getposx() &&
                           p_evt->x < ownd.getposx() + ownd.getszx() &&
                           p_evt->y < ownd.getposy() + ownd.getszy()) {
                    if (!infcs) break;
                    
                    if (p_evt->x > ownd.getposx() &&
                        p_evt->x < ownd.getposx() + ownd.getszx() - 1 && 
                        p_evt->y > ownd.getposy() && 
                        p_evt->y < ownd.getposy() + ownd.getszy() - 1) {
                        for (ui16 i = 0; i < options[ovr].subopts.size(); ++ i) {
                            if (p_evt->y == i + ownd.getposy() + 1) {
                                subovr = i;
                                prsd   = true;

                                break;
                            };
                        };
                    };
                } else infcs = false;

                /*if (!(p_evt->x >= wnd .getposx () && 
                      p_evt->y >= wnd .getposy () && 
                    
                      p_evt->x < wnd .getszx () + wnd .getposx () && 
                      p_evt->y < wnd .getszy () + wnd .getposy ()))
                    break;
                
                if (flgs & LUIC_FLAGS_FRM_CLOSABLE) {
                    if (p_evt->x == wnd .getszx () + wnd .getposx () - 3 &&
                        p_evt->y == wnd .getposy ()) {
                        clse = true;
                        infcs = true;

                        break;
                    };
                };
                
                prsd  = true;
                infcs = false;

                ioh->__schldfcsed (true);
                __fcsprnts ();

                if (flgs & LUIC_FLAGS_FRM_DRAGGABLE) {
                    offstx = p_evt->x - wnd .getposx ();
                    offsty = p_evt->y - wnd .getposy ();
                };

                if (parent == NULL) ioh->pbchild(this);
                else parent->pbchild(this);*/
            };

            break;
        };

        case 27: {
            if (ioh->__gchldfcsed ()) break;

            if (infcs) 
                infcs = false;
            else {
                ioh->__schldfcsed (false);
                infcs = true;
            };

            break;
        };

        case KEY_RIGHT: {
            if (!infcs) break;

            if (ovr == (i16)options.size() - 1) ovr = -1;
            ++ ovr;

            subovr = 0;

            break;
        };

        case KEY_LEFT: {
            if (!infcs) break;

            if (ovr == 0) ovr = options.size();
            -- ovr;

            subovr = 0;

            break;
        };

        case KEY_DOWN: {
            if (!infcs) break;

            if (subovr == (i16)options[ovr].subopts.size() - 1) subovr = -1;
            ++ subovr;

            break;
        };

        case KEY_UP: {
            if (!infcs) break;

            if (subovr == 0) subovr = options[ovr].subopts.size();
            -- subovr;

            break;
        };

        case 10: {
            if (!infcs) break;
            
            chc    = ovr;
            subchc = subovr;
            infcs  = false;

            break;
        };
    };
};