#include "ceditor.hh"

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

    frm->addchild (txtbx);
    frm->setcolschm({
        __LUIC__SHDWCLR,
        __LUIC__SYSCLR,
        __LUIC__SYSCLR,
        __LUIC__SYSGRN,
        __LUIC__SYSGRN,
        __LUIC__SYSCLR
    });
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