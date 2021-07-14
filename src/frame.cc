#include "frame.hh"

LUIC::frame::frame() {};

LUIC::frame::frame(str p_ttl, ui16 p_posx, ui16 p_posy, ui16 p_szx, ui16 p_szy, flags p_flags):
    ttl    (" " + p_ttl + " "),
    prsd   (false),
    clse   (false),
    drg    (false),
    rsz    (false)
{
    type = LUIC_TYPE_FRAME;
    flgs = p_flags;

    colorscheme = {
        __LUIC__SHDWCLR,
        __LUIC__WBACLR,
        __LUIC__WBBCLR,
        __LUIC__WNDGRN,
        __LUIC__SYSCLR,
        __LUIC__WNDCLR
    };
 
    posx = p_posx;
    posy = p_posy;

    wnd = window (posx, posy, p_szx, p_szy);
};

void LUIC::frame::setttl(str p_ttl) {
    ttl = " " + p_ttl + " ";
};

void LUIC::frame::draw() {
    if (ioh == NULL || !vsble) return;

    if (parent == NULL) wnd .drawshdw (colorscheme[0]);

    wnd .setbgclr (colorscheme[5]);

    if (prsd) wnd .setdbbrdr (0, 0, wnd.getszx() - 1, wnd.getszy() - 1, colorscheme[1], colorscheme[2]);
    else      wnd .setbrdr   (0, 0, wnd.getszx() - 1, wnd.getszy() - 1, colorscheme[1], colorscheme[2]);

    str fttl = (i32)ttl.length() > (wnd .getszx() - 13)? ttl.substr(0, 13) : ttl;
    wnd .outat (wnd.getszx() / 2 - fttl.length() / 2, 0, fttl);

    if (flgs & LUIC_FLAGS_FRM_CLOSABLE) {
        wnd .outatclr (wnd .getszx() - 4, 0, "[ ]", colorscheme[1]);
        if (!clse) wnd .setattr (A_BLINK, true);
        if (clse)  wnd .wchoutatclr (wnd .getszx() - 3, 0, WACS_S7,      colorscheme[3]);
        else       wnd .wchoutatclr (wnd .getszx() - 3, 0, WACS_DIAMOND, colorscheme[3]);
        if (!clse) wnd .setattr (A_BLINK, false);
    };

    if (flgs & LUIC_FLAGS_FRM_RESIZABLE) {
        if (rsz) wnd .setclr (colorscheme[4]);
        else     wnd .setclr (colorscheme[5]);

        wnd .outat (wnd .getszx () - 1, wnd .getszy () - 1, "+");
    };

    for (ui16 i = 0; i < (ui16)children.size(); ++ i) {
        children[i]->draw();
    };
};

void LUIC::frame::input(i16 p_in, MEVENT* p_evt) {
    if (ioh == NULL || !vsble) return;

    for (i16 i = (i16)children.size() - 1; i >= 0; -- i) {
        component* child = children[i];
        child->input (p_in, p_evt);

        if (child != children[i]) -- i;

        if (child->getflags() & LUIC_FLAGS_COMPONENT_SCALED) {} else {
            if (child->getszx() + 2 > wnd.getszx())
                wnd.setsz(child->getszx() + 2, wnd.getszy());

            if (child->getszy() + 2 > wnd.getszy())
                wnd.setsz(wnd.getszx(), child->getszy() + 2);
        };

        if (child->isinfcs ()) {
            if (ioh->__gchldfcsed ()) child->setfcs (false);
            else ioh->__schldfcsed(true);
        };
    };

    switch (p_in) {
        case KEY_RESIZE: {
            __fxps ();

            break;
        };
        
        case KEY_MOUSE: {
            if (p_evt->bstate & BUTTON1_RELEASED) {
                if (prsd) {
                    prsd  = false;
                    infcs = false;

                    if (flgs & LUIC_FLAGS_FRM_DRAGGABLE) {
                        if (parent != NULL) setpos (p_evt->x - parent->getposx () - offstx, p_evt->y - parent->getposy () - offsty);
                        else                setpos (p_evt->x - offstx, p_evt->y - offsty);

                        __fxps ();
                    };
                } else if (clse) {
                    clse  = false;
                    vsble = false;
                    infcs = false;
                } else if (rsz) {
                    rsz   = false;
                    infcs = false;

                    i16 newszx = p_evt->x - wnd.getposx() + 1, 
                        newszy = p_evt->y - wnd.getposy() + 1;

                    if (newszx < 20) newszx = 20;
                    if (newszy < 5)  newszy = 5;

                    setsz (newszx, newszy);
                };
            } else if (p_evt->bstate & BUTTON1_PRESSED) {
                if (!(p_evt->x >= wnd .getposx () && 
                      p_evt->y >= wnd .getposy () && 
                    
                      p_evt->x < wnd .getszx () + wnd .getposx () && 
                      p_evt->y < wnd .getszy () + wnd .getposy ())) {
                    linfcs = false;
                    
                    break;
                };

                if (ioh->__gchldfcsed()) return;

                if (flgs & LUIC_FLAGS_FRM_CLOSABLE) {
                    if (p_evt->x == wnd .getszx () + wnd .getposx () - 3 &&
                        p_evt->y == wnd .getposy ()) {
                        clse   = true;
                        infcs  = true;
                        linfcs = true;

                        break;
                    };
                };

                if (flgs & LUIC_FLAGS_FRM_RESIZABLE) {
                    if (p_evt->x == wnd .getszx () + wnd .getposx () - 1 &&
                        p_evt->y == wnd .getszy () + wnd .getposy () - 1) {
                        rsz    = true;
                        infcs  = true;
                        linfcs = true;

                        break;
                    };
                };
                
                prsd  = true;
                infcs = false;
                linfcs = true;

                ioh->__schldfcsed (true);
                __fcsprnts ();

                if (flgs & LUIC_FLAGS_FRM_DRAGGABLE) {
                    offstx = p_evt->x - wnd .getposx ();
                    offsty = p_evt->y - wnd .getposy ();
                };

                if (parent == NULL) ioh->pbchild(this);
                else parent->pbchild(this);
            };

            break;
        };
    };

    if (parent == NULL)
        if (posy < 1)
            setpos (getposx (), 1);
};