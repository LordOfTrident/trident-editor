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
#include "ceditor.hh"
#include "scbl.hh"

#define SCBL_TABSIZE 0
#define SCBL_COLOR   1

#define SCBL_BG    100
#define SCBL_FG    101
#define SCBL_FRAME 102

#define SCBL_KEYWORD  103
#define SCBL_STRING   104
#define SCBL_NUMBER   105
#define SCBL_BOOLEAN  106
#define SCBL_DATATYPE 107
#define SCBL_STD      108
#define SCBL_COMMENT  109
#define SCBL_PREPROC  110

extern str scblerr;

class app {
public: 

    app ();
    app (i32 argc, ch* argv[]);

    void start ();

private:

    bool dexists (str p_name);
    void maked   (str p_name);

    bool fexists (str p_name);
    str  ftostr  (str p_fname);
    void strtof  (str p_txt, str p_fname);

    bool running;
    i8   scblok;

    SCBL::SCBL_Interpreter SCBLi;

    str homedir;

    LUIC::iohandle ioh;
    LUIC::topbar   menubar;

    LUIC::frame   filemenu;
    LUIC::textbar filename;
    LUIC::button  fileclose,
                  fileok;
    LUIC::label   filemsg;

    bool sfowagr;
    LUIC::frame   sfilemenu;
    LUIC::textbar sfilename;
    LUIC::button  sfileclose,
                  sfileok;
    LUIC::label   sfilemsg; 

    LUIC::frame  crdtsframe;
    LUIC::label  crdtslabel;
    LUIC::button crdtsok;

    LUIC::frame atblframe;
    LUIC::label atbllabel,
                atblline;
    vector <LUIC::component*> atblbtns;

    LUIC::frame scblframe;
    LUIC::label scblmsg;

    vector <ceditor> ceditors;
    i8               idx_linfcs;
};