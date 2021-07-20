#include "ceditor.hh"

i8 editor_fg = COLOR_WHITE, 
   editor_bg = COLOR_BLACK;

bool __endsw(str p_str, str p_sub) {
    if (p_sub.length() > p_str.length() || p_str.substr(p_str.length() - p_sub.length(), p_sub.length()) != p_sub) return false;

    return true;
};

ceditor::ceditor() {};

ceditor::ceditor(LUIC::iohandle *p_ioh, str p_ttl, str p_txt) {
    frm = new LUIC::frame (
        p_ttl, 
        0, 
        1, 
        p_ioh->getwsizex(),
        p_ioh->getwsizey() - 1,
        LUIC_FLAGS_FRM_CLOSABLE | LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_RESIZABLE
    );

    p_ioh->addchild (frm);

    txtbx = new LUIC::editor(
        1,
        1,
        20,
        5,
        0
    );

    txtbx->settxt (p_txt);
    txtbx->setcolschm ({
        __LUIC__SHDWCLR,
        __CEDIT_COL__DEF_FRM,
        __CEDIT_COL__DEF
    });

    frm->addchild (txtbx);
    frm->setcolschm ({
        __LUIC__SHDWCLR,
        __CEDIT_COL__DEF_FRM,
        __CEDIT_COL__DEF_FRM,
        __CEDIT_COL__DEF_GRN,
        __LUIC__SYSCLR,
        __CEDIT_COL__DEF_FRM
    });

    setfname(p_ttl);
};

void ceditor::setfname(str p_ttl) {
    frm->setttl  (p_ttl);
    txtbx->setttl (p_ttl);

    str ft = "txt";

    if (__endsw(p_ttl, ".cpp") || __endsw(p_ttl, ".cc") || __endsw(p_ttl, ".cxx") || __endsw(p_ttl, ".c++") ||
        __endsw(p_ttl, ".hpp") || __endsw(p_ttl, ".hh") || __endsw(p_ttl, ".hxx") || __endsw(p_ttl, ".h++")) ft = "C++";
    else if (__endsw(p_ttl, ".pas") || __endsw(p_ttl, ".inc")) ft = "Pascal";
    else if (__endsw(p_ttl, ".tm")) ft = "Temple";
    else if (__endsw(p_ttl, ".lua")) ft = "Lua";
    else if (__endsw(p_ttl, ".js")) ft = "JavaScript";
    else if (__endsw(p_ttl, ".c") || __endsw(p_ttl, ".h")) ft = "C";
    else if (__endsw(p_ttl, ".cs")) ft = "C#";
    else if (__endsw(p_ttl, ".py")) ft = "Python";
    else if (__endsw(p_ttl, ".atm")) ft = "Atoment";
    else if (__endsw(p_ttl, ".rs")) ft = "Rust";
    else if (__endsw(p_ttl, ".sh") || __endsw(p_ttl, ".bash")) ft = "Bash";
    else if (__endsw(p_ttl, ".bat")) ft = "Batch";
    else if (__endsw(p_ttl, ".asm")) ft = "Assembly";
    else if (__endsw(p_ttl, ".scbl")) ft = "SCBL";
    else if (__endsw(p_ttl, ".rb")) ft = "Ruby";
    else if (__endsw(p_ttl, ".go")) ft = "Go";
    else if (__endsw(p_ttl, ".java")) ft = "Java";
    else if (__endsw(p_ttl, ".f") || __endsw(p_ttl, ".for") || __endsw(p_ttl, ".fpp")) ft = "Fortran";
    else if (__endsw(p_ttl, ".json")) ft = "Json";
    else if (__endsw(p_ttl, ".xml")) ft = "XML";
    else if (__endsw(p_ttl, ".css")) ft = "CSS";
    else if (__endsw(p_ttl, ".html")) ft = "HTML";
 
    txtbx->setft (ft);
};

str ceditor::getfname() {
    return txtbx->getttl ();
};

void ceditor::cleanup () {
    if (frm != NULL) {
        frm->remchild(frm->getchild(txtbx));

        LUIC::iohandle* ioh = frm->getioh();
        ioh->remchild(ioh->getchild(frm));

        delete txtbx;
        delete frm;
    };
};

LUIC::frame *ceditor::getfrm() {
    return frm;
};

LUIC::editor *ceditor::gettxtbx() {
    return txtbx;
};