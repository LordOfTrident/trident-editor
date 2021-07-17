#include "./app.hh"

str scblerr = "";

bool app::dexists(str p_name) {
    struct stat info;
    
    if (stat(p_name.c_str(), &info) != 0)
        return false;
    
    return (info.st_mode & S_IFDIR) != 0;
};

void app::maked(str p_name) {
    mode_t mode = 0755;

    mkdir(p_name.c_str(), mode);
};

bool app::fexists(str p_name) {
    return access(p_name.c_str(), F_OK) != -1;
};

str app::ftostr(str p_fname) {
    str           fstr = "";
    std::ifstream fhnd (p_fname.c_str());

    if (fhnd.is_open()) {
        str fln = "";

        while (getline(fhnd, fln)) {
            for (i32 i = 0; i < (i32)fln.length(); ++ i) {
                if (fln[i] == 9) {
                    fln[i] = 32;
                    fln.insert(i + 1, "   ");
                };
            };

            fstr += fln + '\n';
        };
    };

    fhnd.close();

    return fstr;
};

void app::strtof(str p_txt, str p_fname) {
    std::ofstream fhnd(p_fname.c_str());

    fhnd << p_txt;
};

app::app(i32 argc, ch* argv[]) {
    ioh = iohandle (
        "Trident Editor", 
        LUIC_FLAGS_IOH_NOESCDELAY | LUIC_FLAGS_IOH_CLEARSCR
    );

    ioh .initthm (true);
    //ioh .setttl  ("Trident Editor");
    //ioh .setbody ("Made by LordOfTrident using NCurses\n\nControls:\n* Mouse - Everything\n* ESC - Menu bar\n* Arrows - Change navigate cursor/menu bar");

    menubar = topbar (
        "TR-ED",
        {
            option ("File", {
                suboption("New",     "CTRL+N"), 
                suboption("Load",    "CTRL+L"), 
                suboption("Save",    "CTRL+S"),
                suboption("Save As", ""),
                suboption("Exit",    "CTRL+Q")
            }),

            option ("Edit", {
                suboption("Copy",  "CTRL+C"), 
                suboption("Cut",   "CTRL+X"),
                suboption("Paste", "CTRL+V")
            }),

            option ("Tools", {
                suboption("ASCII Table", "CTRL+A")
            }),

            option ("Help", {
                suboption("About", ""),
                suboption("Credits", "")
            })
        }
    );

    ioh.addchild(&menubar);

    filemenu = frame (
        "Load File",
        ioh.getwsizex() / 2 - 20,
        ioh.getwsizey() / 2 - 5,
        40,
        10,
        LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
    );
    filemenu .setvsble (false);
    ioh .addchild (&filemenu);

    filename = textbar (
        1, 
        1, 
        38,
        0
    );
    filemenu .addchild (&filename);

    filemsg = label (
        "",
        2,
        4,
        0
    );
    filemenu .addchild (&filemsg);

    fileclose = button (
        "Close",
        filemenu.getszx() - 9,
        filemenu.getszy() - 2,
        9,
        1,
        0
    );
    filemenu .addchild (&fileclose);
    
    fileok = button (
        "Ok",
        filemenu.getszx() - 19,
        filemenu.getszy() - 2,
        8,
        1,
        0
    );
    filemenu .addchild (&fileok);

    sfilemenu = frame (
        "Save File As",
        ioh.getwsizex() / 2 - 20,
        ioh.getwsizey() / 2 - 5,
        40,
        10,
        LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
    );
    sfilemenu .setvsble (false);
    ioh .addchild (&sfilemenu);

    sfilename = textbar (
        1, 
        1, 
        38,
        0
    );
    sfilemenu .addchild (&sfilename);

    sfilemsg = label (
        "",
        2,
        4,
        0
    );
    sfilemenu .addchild (&sfilemsg);

    sfileclose = button (
        "Close",
        sfilemenu.getszx() - 9,
        sfilemenu.getszy() - 2,
        9,
        1,
        0
    );
    sfilemenu .addchild (&sfileclose);
    
    sfileok = button (
        "Ok",
        sfilemenu.getszx() - 19,
        sfilemenu.getszy() - 2,
        8,
        1,
        0
    );
    sfilemenu .addchild (&sfileok);

    crdtsframe = frame (
        "Credits",
        ioh.getwsizex() / 2 - 15,
        ioh.getwsizey() / 2 - 4,
        30,
        8,
        LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
    );
    crdtsframe .setvsble (false);
    ioh .addchild (&crdtsframe);

    crdtslabel = label (
        "    Made By LordOfTrident\n        Using NCurses",
        1,
        2,
        0
    );
    crdtsframe .addchild (&crdtslabel);

    crdtsok = button (
        "Ok",
        crdtsframe.getszx() / 2 - 4,
        crdtsframe.getszy() - 2,
        8,
        1,
        0
    );
    crdtsframe .addchild (&crdtsok);

    atblframe = frame(
        "ASCII Table",
        ioh.getwsizex() / 2 - 25,
        ioh.getwsizey() / 2 - 11,
        34,
        8,
        LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
    );

    atblframe .setvsble (false);
    ioh .addchild (&atblframe);

    ui8 posx = 0, posy = 0;
    for (ui8 i = 0; i < 128; ++ i) {
        if (i % 32 == 0) {
            posx = 0;

            ++ posy;
        };

        button* btn = new button(
            i < 32 || i > 126? " " : str(1, i),
            1 + posx,
            posy,
            1,
            1,
            0
        );

        btn->setcolschm ({
            __LUIC__SHDWCLR,
            __LUIC__WNDCLR,
            __LUIC__SYSCLR,
            __LUIC__WBACLR,
            __LUIC__WBBCLR,
            __LUIC__TXTCLR,
            __LUIC__COMMON
        });

        atblbtns  .push_back (btn);
        atblframe .addchild  (btn);

        ++ posx;
    };

    atblline = label (
        str(30, ACS_HLINE),
        1,
        5,
        LUIC_FLAGS_LBL_ALTCHARSET
    );

    atblline .setcolschm ({
        __LUIC__WBACLR,
        __LUIC__SHDWCLR
    });

    atblframe .addchild (&atblline);

    atbllabel = label(
        "CH: - | CNTRL: - | DEC: -",
        2,
        6,
        0
    );

    atblframe .addchild (&atbllabel);

    scblframe = frame(
        "SCBL",
        ioh.getwsizex() / 2 - 20,
        ioh.getwsizey() / 2 - 4,
        40,
        8,
        LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
    );

    ioh .addchild (&scblframe);

    scblmsg = label(
        "",
        1,
        1,
        0
    );

    scblframe. addchild (&scblmsg);

    idx_linfcs = -1;
    sfowagr    = false;

    init_pair(__CEDIT_COL__DEF,     COLOR_WHITE,      COLOR_BLACK);
    init_pair(__CEDIT_COL__DEF_GRN, COLOR_LIGHTGREEN, COLOR_BLACK);
    init_pair(__CEDIT_COL__DEF_FRM, COLOR_WHITE,      COLOR_BLACK);

    homedir = getenv("HOME");
    #define STTNGS_DEFAULT "set tabsize 4;\nset color background black;\nset color foreground white;\nset color frame white;\nset color comment grey;\nset color keyword brightmagenta;\nset color string brightgreen;\nset color number cyan;\nset color boolean magenta;\nset color datatype brightcyan;\nset color std brightyellow;"

    if (!dexists (homedir + "/.config/tr-ed")) {
        maked  (homedir + "/.config/tr-ed");
        strtof (STTNGS_DEFAULT, homedir + "/.config/tr-ed/settings.scbl");
    } else {
        if (!fexists (homedir + "/.config/tr-ed/settings.scbl"))
            strtof (STTNGS_DEFAULT, homedir + "/.config/tr-ed/settings.scbl");
    };

	SCBL::SCBL_Interpreter SCBLi;

	SCBLi.AddConstant(SCBL::Constant("tabsize", SCBL_TABSIZE));
	SCBLi.AddConstant(SCBL::Constant("color",   SCBL_COLOR));

	SCBLi.AddConstant(SCBL::Constant("background", SCBL_BG));
	SCBLi.AddConstant(SCBL::Constant("foreground", SCBL_FG));
	SCBLi.AddConstant(SCBL::Constant("frame",      SCBL_FRAME));
	SCBLi.AddConstant(SCBL::Constant("keyword",    SCBL_KEYWORD));
	SCBLi.AddConstant(SCBL::Constant("string",     SCBL_STRING));
	SCBLi.AddConstant(SCBL::Constant("number",     SCBL_NUMBER));
	SCBLi.AddConstant(SCBL::Constant("boolean",    SCBL_BOOLEAN));
	SCBLi.AddConstant(SCBL::Constant("datatype",   SCBL_DATATYPE));
	SCBLi.AddConstant(SCBL::Constant("std",        SCBL_STD));
	SCBLi.AddConstant(SCBL::Constant("comment",    SCBL_COMMENT));

	SCBLi.AddConstant(SCBL::Constant("black",   COLOR_BLACK));
	SCBLi.AddConstant(SCBL::Constant("red",     COLOR_RED));
	SCBLi.AddConstant(SCBL::Constant("green",   COLOR_GREEN));
	SCBLi.AddConstant(SCBL::Constant("yellow",  COLOR_YELLOW));
	SCBLi.AddConstant(SCBL::Constant("blue",    COLOR_BLUE));
	SCBLi.AddConstant(SCBL::Constant("magenta", COLOR_MAGENTA));
	SCBLi.AddConstant(SCBL::Constant("cyan",    COLOR_CYAN));
	SCBLi.AddConstant(SCBL::Constant("white",   COLOR_WHITE));

	SCBLi.AddConstant(SCBL::Constant("grey",          COLOR_GREY));
	SCBLi.AddConstant(SCBL::Constant("brightred",     COLOR_LIGHTRED));
	SCBLi.AddConstant(SCBL::Constant("brightgreen",   COLOR_LIGHTGREEN));
	SCBLi.AddConstant(SCBL::Constant("brightyellow",  COLOR_LIGHTYELLOW));
	SCBLi.AddConstant(SCBL::Constant("brightblue",    COLOR_LIGHTBLUE));
	SCBLi.AddConstant(SCBL::Constant("brightmagenta", COLOR_LIGHTMAGENTA));
	SCBLi.AddConstant(SCBL::Constant("brightcyan",    COLOR_LIGHTCYAN));
	SCBLi.AddConstant(SCBL::Constant("brightwhite",   COLOR_LIGHTWHITE));

    SCBLi.AddFunction(SCBL::Function("set", [] (std::vector <uint32_t> Parameters) {
        if (Parameters.size() < 2) {
            scblerr = "Not enough parameters for 'set'";

            return;
        };

	    switch (Parameters[0]) {
            case 0: {
                // TODO: Change tab size

                break;
            };

            case 1: {
                if (Parameters.size() < 3) {
                    scblerr = "Not enough parameters for 'set color'";

                    return;
                };

                switch (Parameters[1]) {
                    case SCBL_BG: {
                        editor_bg = Parameters[2];

                        init_pair(__CEDIT_COL__DEF,     editor_fg,        editor_bg);
                        init_pair(__CEDIT_COL__DEF_GRN, COLOR_LIGHTGREEN, editor_bg);

                        break;
                    };

                    case SCBL_FG: {
                        editor_fg = Parameters[2];

                        init_pair(__CEDIT_COL__DEF, editor_fg, editor_bg);
                        
                        break;
                    };

                    case SCBL_FRAME: {
                        init_pair(__CEDIT_COL__DEF_FRM, Parameters[2], editor_bg);

                        break;
                    };
                };

                break;
            };
        };
	}));

    for (ui8 i = 1; i < argc; ++ i) {
        if (fexists(argv[i])) {
            ceditors .push_back (ceditor (&ioh, argv[i], ftostr(argv[i])));
            filemenu .setvsble  (false);
        };
    };

    scblok = SCBLi .Parse (ftostr(homedir + "/.config/tr-ed/settings.scbl"));
    
    if (scblok == SCBL_PARSER_OK) 
        scblok = SCBLi .Run ();

    if (scblok != SCBL_PARSER_OK) {
        scblframe .setvsble (true);
        scblframe .totop    ();

        scblmsg.settxt("SCBL: " + scblerr + "\n" + SCBLi.GetErrorMsg());
    } else 
        scblframe .setvsble (false);

    erase ();
};

void app::start() {
    running = true;

    while (running) {
        ioh .draw  ();
        ioh .input ();

        if (menubar.getsboptst(0) == 4) 
            running = false;
        else if (menubar.getsboptst(0) == 1) {
            filemsg  .settxt ("Type in the file name");
            filename .settxt ("");

            filemenu .setpos (ioh.getwsizex() / 2 - filemenu.getszx() / 2, ioh.getwsizey() / 2 - filemenu.getszy() / 2);
            filemenu .totop  ();

            filemenu .setvsble (true);
        } else if (menubar.getsboptst(0) == 3) {
            sfowagr = false;

            sfilemsg  .settxt ("Type in the file name");
            sfilename .settxt ("");

            sfilemenu .setpos (ioh.getwsizex() / 2 - sfilemenu.getszx() / 2, ioh.getwsizey() / 2 - sfilemenu.getszy() / 2);
            sfilemenu .totop  ();

            sfilemenu .setvsble (true);
        } else if (menubar.getsboptst(0) == 2) {
            if (idx_linfcs != -1) {
                str fname = ceditors[idx_linfcs].getfname();

                if (fexists(fname)) {
                    strtof (ceditors[idx_linfcs].gettxtbx()->gettxt(), fname);
                    ceditors[idx_linfcs] .gettxtbx () ->setmodif (false);
                } else {
                    sfowagr = false;

                    sfilemsg  .settxt ("Type in the file name");
                    sfilename .settxt ("");

                    sfilemenu .setpos (ioh.getwsizex() / 2 - sfilemenu.getszx() / 2, ioh.getwsizey() / 2 - sfilemenu.getszy() / 2);
                    sfilemenu .totop  ();

                    sfilemenu .setvsble (true);
                };
            };
        } else if (menubar.getsboptst(2) == 0) {
            atblframe .setpos   (ioh.getwsizex() / 2 - atblframe.getszx() / 2, ioh.getwsizey() / 2 - atblframe.getszy() / 2);
            atblframe .totop    ();

            atblframe .setvsble (true);
        } else if (menubar.getsboptst(3) == 1) {
            crdtsframe .setpos   (ioh.getwsizex() / 2 - crdtsframe.getszx() / 2, ioh.getwsizey() / 2 - crdtsframe.getszy() / 2);
            crdtsframe .totop    ();
            
            crdtsframe .setvsble (true);
        } else if (menubar.getsboptst(0) == 0) {
            ceditors.push_back (ceditor (&ioh, "Untitled", ""));

            idx_linfcs = ceditors.size() - 1;

            ceditors[idx_linfcs] .getfrm   ()->setfcs (true);
            ceditors[idx_linfcs] .gettxtbx ()->setfcs (true);
        } else if (fileclose.isclicked())
            filemenu .setvsble (false);
        else if (sfileclose.isclicked())
            sfilemenu .setvsble (false);
        else if (fileok.isclicked()) {
            str fname = filename .gettxt ();

            if (fname != "") {
                if (fexists(fname)) {
                    ceditors .push_back (ceditor (&ioh, fname, ftostr(fname)));
                    filemenu .setvsble  (false);
                } else
                    filemsg .settxt    ("File '" + (
                            fname.length() > 9? 
                                fname.substr(0, 9) + "..." 
                            : 
                                fname
                        ) + "' not found.\nType in the file name");
            } else 
                filemsg .settxt ("Type in the file name");
        } else if (sfileok.isclicked()) {
            if (idx_linfcs != -1) {
                str fname = sfilename .gettxt ();

                if (fname != "") {
                    bool error = true;
                    for (ui16 i = 0; i < fname.length(); ++ i)
                        if (fname[i] != ' ' && fname[i] != '*') 
                            error = false;

                    if (!error) {
                        if (fexists(fname)) {
                            if (sfowagr) {
                                strtof (ceditors[idx_linfcs].gettxtbx()->gettxt(), fname);
                                ceditors[idx_linfcs] .gettxtbx () ->setmodif (false);

                                ceditors[idx_linfcs] .setfname (fname);

                                sfilemenu .setvsble (false);

                                sfowagr = false;
                            } else {
                                sfilemsg .settxt ("File already exists, press ok if\nyou want to overwrite.");

                                sfowagr = true;
                            };
                        } else {
                            strtof (ceditors[idx_linfcs].gettxtbx()->gettxt(), fname);
                            ceditors[idx_linfcs] .gettxtbx () ->setmodif (false);
                            ceditors[idx_linfcs] .setfname (fname);

                            sfilemenu .setvsble (false);
                        };
                    } else
                        sfilemsg .settxt ("File name contains invalid symbols");
                } else 
                    sfilemsg .settxt ("Type in the file name");
            } else
                sfilemsg .settxt ("Select the ceditor of which the\ncontents you want to save.");
        } else if (crdtsok.isclicked())
            crdtsframe .setvsble (false);

        atbllabel .settxt ("CH: - | CNTRL: - | DEC: -");
        for (ui8 i = 0; i < (ui8)atblbtns.size(); ++ i) {
            button* btn = (button*)atblbtns[i];

            if (btn->lstinfcs()) {
                atbllabel .settxt ("CH: " + (i < 32 || i > 126? "-" : str(1, i)) + " | CNTRL: " + (i < 32 || i > 126? "Y" : "N") + " | DEC: " + std::to_string(i));

                break;
            };
        };

        for (i16 i = 0; i < (i16)ceditors.size(); ++ i) {
            if (!ceditors[i].getfrm()->getvsble()) {
                ceditors[i] .cleanup ();
                ceditors.erase(ceditors.begin() + i);

                if (idx_linfcs == i) 
                    idx_linfcs = -1;

                -- i;

                continue;
            };

            if (ceditors[i].getfrm()->lstinfcs())
                idx_linfcs = i;
        };
    };

    for (i16 i = 0; i < (i16)ceditors.size(); ++ i) {
        ceditors[i] .cleanup ();
    };

    for (ui8 i = 0; i < (ui8)atblbtns.size(); ++ i) {
        delete (button*)atblbtns[i];
    };
};