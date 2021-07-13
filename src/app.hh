#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "iohandle.hh"
#include "frame.hh"
#include "button.hh"
#include "label.hh"
#include "topbar.hh"
#include "textbar.hh"
#include "editor.hh"

using namespace LUIC;

class app {
public: 

    app ();
    app (i32 argc, ch* argv[]);

    void start ();

private:

    bool fexists(str p_name);
    str  ftostr(str p_fname);
    void strtof(str p_txt, str p_fname);

    bool     running;

    iohandle ioh;
    topbar   menubar;

    frame   filemenu;
    textbar filename;
    button  fileclose,
            fileok;
    label   filemsg;

    bool    sfowagr;
    frame   sfilemenu;
    textbar sfilename;
    button  sfileclose,
            sfileok;
    label   sfilemsg;

    frame  crdtsframe;
    label  crdtslabel;
    button crdtsok;

    frame atblframe;
    label atbllabel;

    vector <editor> editors;
    i8             idx_linfcs;
};