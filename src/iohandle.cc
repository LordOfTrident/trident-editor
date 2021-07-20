#include "iohandle.hh"

LUIC::iohandle::iohandle() {};

LUIC::iohandle::iohandle(str p_ttl, flags p_flags):
    chldfcsed (false),
    flgs      (p_flags)
{
    setlocale (LC_CTYPE, "");
    printf    ("\033]0;%s\007", p_ttl.c_str()); // Title
    //printf    ("\033[?1003h\n");                // Enable Mouse Movement Events
    initscr   ();
    
    raw       ();
    noecho    ();
    timeout   (-1);
    nodelay   (stdscr, true);
    keypad    (stdscr, true);
    mousemask (ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    start_color        (); 
    use_default_colors ();
    mouseinterval      (0);

    if (flgs & LUIC_FLAGS_IOH_CBREAK)     cbreak ();
    if (flgs & LUIC_FLAGS_IOH_NOESCDELAY) ESCDELAY = 0;

    wsizex = getmaxx (stdscr);
    wsizey = getmaxy (stdscr);

    screen::pixels.resize(wsizey);
    for (ui16 i = 0; i < (ui16)screen::pixels.size(); ++ i)
        screen::pixels[i].resize(wsizex);

    curs_set (0);
};

LUIC::iohandle::~iohandle() {
    endwin();

    //printf("\033[?1003l\n"); // Disable Mouse Movement Events
};

void LUIC::iohandle::setflgs(flags p_flags) {
    flgs = p_flags;
};

void LUIC::iohandle::setttl(str p_ttl) {
    ttl = p_ttl;
};

void LUIC::iohandle::setbody(str p_body) {
    body = p_body;
};

ui16 LUIC::iohandle::getwsizex() {
    return wsizex;
};

ui16 LUIC::iohandle::getwsizey() {
    return wsizey;
};

ui32 LUIC::iohandle::gettick() {
    return tick;
};

void LUIC::iohandle::draw() {
    if (flgs & LUIC_FLAGS_IOH_CLEARSCR)
        for (ui16 i = 0; i < (ui16)screen::pixels.size(); ++ i)
            for (ui16 j = 0; j < (ui16)screen::pixels[i].size(); ++ j) {
                if (body == "" && ttl == "") {
                    screen::pixels[i][j].pch   = ACS_CKBOARD;
                    screen::pixels[i][j].pattr = A_ALTCHARSET;
                    screen::pixels[i][j].pclr  = __LUIC__ABCKCLR;
                } else {
                    screen::pixels[i][j].pch   = 32;
                    screen::pixels[i][j].pattr = 0;
                    screen::pixels[i][j].pclr  = __LUIC__BCKCLR;
                };

                screen::pixels[i][j].pcch  = NULL;
            };

    screen::lastclr  = __LUIC__BCKCLR;
    screen::lastattr = 0;

    if (ttl != "") {
        screen::outstr (1, 1, ttl);;

        screen::lastattr = A_ALTCHARSET;
        screen::outstr (1, 2, str(wsizex - 2, ACS_HLINE));
        screen::lastattr = 0;
    };

    if (body != "")
        screen::outstr (0, 4, body);

    for (ui16 i = 0; i < (ui16)children.size(); ++ i) {
        children[i]->draw();
    };

    screen::update ();
};

i16 LUIC::iohandle::input() {
    napms(10);
    
    ++ tick;

    MEVENT evt;

    i16 in   = getch();
    chldfcsed = false;

    switch (in) {
        case KEY_MOUSE: {
            getmouse(&evt);

            break;
        };

        case KEY_RESIZE: {
            wsizex = getmaxx (stdscr);
            wsizey = getmaxy (stdscr);

            screen::pixels.resize(wsizey);
            for (ui16 i = 0; i < (ui16)screen::pixels.size(); ++ i)
                screen::pixels[i].resize(wsizex);

            break;
        };
    };

    for (i16 i = (i16)children.size() - 1; i >= 0; -- i) {
        component* child = children[i];
        child->input (in, &evt);

        if (child != children[i]) -- i;

        if (child->isinfcs ()) {
            if (chldfcsed) child->setfcs (false);
            else chldfcsed = true;
        };
    };

    if (wndtopbar != NULL) 
        if (children[children.size() - 1] != wndtopbar) 
            pbchild (wndtopbar);

    return in;
};

void LUIC::iohandle::addclr(ui8 p_id, color p_bg, color p_fg) {
    init_pair(p_id, p_bg, p_fg);
};

void LUIC::iohandle::addchild(component* p_child) {
    p_child->setparent (NULL);
    p_child->setioh    (this);

    if (p_child->gettype() == LUIC_TYPE_TOPBAR)
        wndtopbar = p_child;

    children .push_back (p_child);
};

void LUIC::iohandle::initthm(bool p_shdw) {
    shdw = p_shdw;

    init_pair (__LUIC__SYSCLR, COLOR_LIGHTWHITE, COLOR_BLUE);
    
    init_pair (__LUIC__BCKCLR,  COLOR_LIGHTCYAN,  COLOR_BLUE);
    init_pair (__LUIC__ABCKCLR, COLOR_GREY,       COLOR_LIGHTBLUE);
    init_pair (__LUIC__BCKCLR,  COLOR_LIGHTCYAN,  COLOR_BLUE);
    init_pair (__LUIC__SHDWCLR, COLOR_GREY,       COLOR_BLACK);
    init_pair (__LUIC__HGHLGHT, COLOR_BLACK,      COLOR_GREEN);
    init_pair (__LUIC__BTNPCLR, COLOR_LIGHTWHITE, COLOR_GREEN);

    init_pair (__LUIC__WNDGRN, COLOR_LIGHTGREEN, COLOR_WHITE);
    init_pair (__LUIC__SYSGRN, COLOR_LIGHTGREEN, COLOR_BLUE);
    init_pair (__LUIC__BCKGRN, COLOR_LIGHTGREEN, COLOR_BLACK);
    init_pair (__LUIC__WNDCLR, COLOR_LIGHTBLUE,  COLOR_WHITE);
    init_pair (__LUIC__WNDDEF, COLOR_BLACK,      COLOR_WHITE);
    init_pair (__LUIC__WBACLR, COLOR_LIGHTWHITE, COLOR_WHITE);
    init_pair (__LUIC__WBBCLR, COLOR_GREY,       COLOR_WHITE);
    init_pair (__LUIC__BARCLR, COLOR_CYAN,       COLOR_WHITE);
    init_pair (__LUIC__COMMON, COLOR_BLACK,      COLOR_WHITE);

    init_pair (__LUIC__TXTCLR,  COLOR_WHITE, COLOR_BLACK);
    init_pair (__LUIC__TXTNFCS, COLOR_GREY,  COLOR_BLACK);
};

i16 LUIC::iohandle::getchild(component* p_child) {
    for (ui16 i = 0; i < (ui16)children.size(); ++ i) {
        if (children[i] == p_child) return i;
    };

    return -1;
};

void LUIC::iohandle::remchild(ui16 p_idx) {
    children.erase(children.begin() + p_idx);
};

void LUIC::iohandle::pbchild(component* p_child) {
    remchild(getchild(p_child));
    addchild(p_child);
};

bool LUIC::iohandle::__hasshdw() {
    return shdw;
};

bool LUIC::iohandle::__gchldfcsed() {
    return chldfcsed;
};

void LUIC::iohandle::__schldfcsed(bool p_fcsed) {
    chldfcsed = p_fcsed;
};