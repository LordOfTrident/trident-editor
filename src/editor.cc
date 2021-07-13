#include "editor.hh"

editor::editor() {};

editor::editor(iohandle *p_ioh, str p_ttl, str p_txt) {
    frm = new frame (
        p_ttl, 
        0, 
        1, 
        p_ioh->getwsizex(),
        p_ioh->getwsizey() - 1,
        LUIC_FLAGS_FRM_CLOSABLE | LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_RESIZABLE
    );

    p_ioh->addchild (frm);
};

void editor::cleanup () {
    if (frm != NULL) {
        iohandle* ioh = frm->getioh();
        
        ioh->remchild(ioh->getchild(frm));

        delete frm;
    };
};

frame *editor::getfrm() {
    return frm;
};