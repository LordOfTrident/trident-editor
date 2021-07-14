#include "ceditor.hh"

bool __endsw(str p_str, str p_sub) {
    if (p_sub.length() > p_str.length() || p_str.substr(p_str.length() - p_sub.length(), p_sub.length()) != p_sub) return false;

    return true;
};

ceditor::ceditor() {};

ceditor::ceditor(iohandle *p_ioh, str p_ttl, str p_txt) {
    frm = new frame (
        p_ttl, 
        0, 
        1, 
        p_ioh->getwsizex(),
        p_ioh->getwsizey() - 1,
        LUIC_FLAGS_FRM_CLOSABLE | LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_RESIZABLE
    );

    p_ioh->addchild (frm);

    txtbx = new editor(
        1,
        1,
        20,
        5,
        0
    );

    txtbx->settxt (p_txt);

    frm->addchild (txtbx);
    frm->setcolschm ({
        __LUIC__SHDWCLR,
        __LUIC__TXTCLR,
        __LUIC__TXTCLR,
        __LUIC__BCKGRN,
        __LUIC__SYSCLR,
        __LUIC__TXTCLR
    });

    setfname(p_ttl);
};

void ceditor::setfname(str p_ttl) {
    frm->setttl (p_ttl);

    str ft = "txt";

    if (__endsw(p_ttl, ".cpp") || __endsw(p_ttl, ".cc") || __endsw(p_ttl, ".cxx") || __endsw(p_ttl, ".c++") ||
        __endsw(p_ttl, ".hpp") || __endsw(p_ttl, ".hh") || __endsw(p_ttl, ".hxx") || __endsw(p_ttl, ".h++")) ft = "cpp";
    else if (__endsw(p_ttl, ".pas") || __endsw(p_ttl, ".inc")) ft = "pas";
    else if (__endsw(p_ttl, ".tm")) ft = "tm";
    else if (__endsw(p_ttl, ".lua")) ft = "lua";
    else if (__endsw(p_ttl, ".js")) ft = "js";
    else if (__endsw(p_ttl, ".c") || __endsw(p_ttl, ".h")) ft = "c";
    else if (__endsw(p_ttl, ".cs")) ft = "cs";
    else if (__endsw(p_ttl, ".py")) ft = "py";
    else if (__endsw(p_ttl, ".atm")) ft = "atoment";
    else if (__endsw(p_ttl, ".rs")) ft = "rust";
    else if (__endsw(p_ttl, ".sh") || __endsw(p_ttl, ".bash")) ft = "bash";
    else if (__endsw(p_ttl, ".bat")) ft = "bat";
    else if (__endsw(p_ttl, ".asm")) ft = "asm";
    else if (__endsw(p_ttl, ".scbl")) ft = "scbl";
    else if (__endsw(p_ttl, ".rb")) ft = "ruby";
    else if (__endsw(p_ttl, ".go")) ft = "go";
    else if (__endsw(p_ttl, ".java")) ft = "java";
    else if (__endsw(p_ttl, ".f") || __endsw(p_ttl, ".for") || __endsw(p_ttl, ".fpp")) ft = "fortran";
    else if (__endsw(p_ttl, ".json")) ft = "json";
    else if (__endsw(p_ttl, ".xml")) ft = "xml";
    else if (__endsw(p_ttl, ".css")) ft = "css";
    else if (__endsw(p_ttl, ".html")) ft = "html";
 
    txtbx->setft(ft);
};

void ceditor::cleanup () {
    if (frm != NULL) {
        frm->remchild(frm->getchild(txtbx));

        iohandle* ioh = frm->getioh();
        ioh->remchild(ioh->getchild(frm));

        delete txtbx;
        delete frm;
    };
};

frame *ceditor::getfrm() {
    return frm;
};

editor *ceditor::gettxtbx() {
    return txtbx;
};