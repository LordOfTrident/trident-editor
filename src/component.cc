#include "component.hh"

LUIC::component::component():
    infcs  (false),
    vsble  (true),
    linfcs (false)
{};

void LUIC::component::setparent(component* p_parent) {
    parent = p_parent;

    if (parent == NULL)
        wnd .setpos (posx, posy);
    else {
        if (posx < 1) posx = 1;
        if (posy < 1) posy = 1;

        if (posx > parent->getszx () - wnd .getszx () - 1) posx = parent->getszx () - wnd .getszx () - 1;
        if (posy > parent->getszy () - wnd .getszy () - 1) posy = parent->getszy () - wnd .getszy () - 1;

        wnd .setpos (parent->__gttlpsx () + posx, parent->__gttlpsy () + posy);

        if (flgs & LUIC_FLAGS_COMPONENT_SCALED) {
            setsz  (parent->getszx() - 2, parent->getszy() - 2);
            setpos (1, 1);
        };
    };
};

void LUIC::component::setcolschm(colors p_colorscheme) {
    colorscheme = p_colorscheme;
};

colors LUIC::component::getcolschm() {
    return colorscheme;
};

void LUIC::component::setioh(iohandle* p_ioh) {
    ioh = p_ioh;
};

void LUIC::component::setflags(flags p_flags) {
    flgs = p_flags;
};

flags LUIC::component::getflags() {
    return flgs;
};

void LUIC::component::addchild(component* p_child) {
    p_child->setparent (this);
    p_child->setioh    (ioh);

    children .push_back (p_child);
};

i16 LUIC::component::getchild(component* p_child) {
    for (ui16 i = 0; i < (ui16)children.size(); ++ i) {
        if (children[i] == p_child) return i;
    };

    return -1;
};

void LUIC::component::remchild(ui16 p_idx) {
    children.erase(children.begin() + p_idx);
};

void LUIC::component::pbchild(component* p_child) {
    remchild (getchild (p_child));
    addchild (p_child);
};

void LUIC::component::setpos(i16 p_posx, i16 p_posy) {
    if (p_posx < 0) p_posx = 0;
    if (p_posy < 0) p_posy = 0;

    posx = p_posx;
    posy = p_posy;

    if (parent == NULL)
        wnd .setpos (posx, posy);
    else {
        if (posx < 1) posx = 1;
        if (posy < 1) posy = 1;

        if (posx > parent->getszx () - wnd .getszx () - 1) posx = parent->getszx () - wnd .getszx () - 1;
        if (posy > parent->getszy () - wnd .getszy () - 1) posy = parent->getszy () - wnd .getszy () - 1;

        wnd .setpos (parent->__gttlpsx () + posx, parent->__gttlpsy () + posy);
    };

    __fxps ();
};

ui16 LUIC::component::getposx() {
    return posx;
};

ui16 LUIC::component::getposy() {
    return posy;
};

void LUIC::component::setsz(ui16 p_szx, ui16 p_szy) {
    wnd.setsz(p_szx, p_szy);

    for (i16 i = (i16)children.size() - 1; i >= 0; -- i) {
        component *child = children[i];

        if (child->getflags () & LUIC_FLAGS_COMPONENT_SCALED) {
            child->setsz(p_szx - 2, p_szy - 2);
            child->setpos(1, 1);
        };
    };
};

ui16 LUIC::component::getszx() {
    return wnd.getszx();
};

ui16 LUIC::component::getszy() {
    return wnd.getszy();
};

void LUIC::component::setfcs(bool p_fcs) {
    infcs = p_fcs;
};

bool LUIC::component::isinfcs() {
    return infcs;
};

void LUIC::component::totop() {
    if (parent == NULL) ioh->pbchild(this);
    else parent->pbchild(this);
};

void LUIC::component::setvsble(bool p_vsble) {
    vsble = p_vsble;
};

bool LUIC::component::getvsble() {
    return vsble;
};

str LUIC::component::gettype() {
    return type;
};

bool LUIC::component::lstinfcs() {
    return linfcs;
};

LUIC::component* LUIC::component::getparent() {
    return parent;
};

LUIC::iohandle* LUIC::component::getioh() {
    return ioh;
};

void LUIC::component::__sttlps(ui16 p_posx, ui16 p_posy) {
    wnd.setpos(p_posx, p_posy);
};

ui16 LUIC::component::__gttlpsx() {
    return wnd.getposx();
};

ui16 LUIC::component::__gttlpsy() {
    return wnd.getposy();
};

void LUIC::component::__fcsprnts() {
    if (parent != NULL) {
        parent->pbchild (this);
        parent->__fcsprnts ();
    } else {
        ioh->pbchild (this);

        infcs  = true;
        linfcs = true;
    };
};

void LUIC::component::__fxps() {
    for (i16 i = (i16)children.size() - 1; i >= 0; -- i) {
        children[i]->setpos (children[i]->getposx (), children[i]->getposy ());
        children[i]->__fxps ();
    };
};

LUIC::window* LUIC::component::__gwndacs() {
    return &wnd;
};