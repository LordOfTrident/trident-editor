#include "./app.hh"

bool app::fexists(str p_name) {
    return access(p_name.c_str(), F_OK) != -1;
};

str app::ftostr(str p_fname) {
    str           fstr = "";
    std::ifstream fhnd (p_fname.c_str());

    if (fhnd.is_open()) {
        str fln = "";

        while (getline(fhnd, fln))
            fstr += fln + '\n';
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
        LUIC_FLAGS_IOH_CBREAK | LUIC_FLAGS_IOH_NOESCDELAY | LUIC_FLAGS_IOH_CLEARSCR
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
                suboption("Save",    "CTRL+U"),
                suboption("Save As", ""), 
                suboption("Exit",    "CTRL+C")
            }),

            option ("Edit", {
                suboption("Copy",  "CTRL+K"), 
                suboption("Cut",   "CTRL+X"),
                suboption("Paste", "CTRL+V")
            }),

            option ("Help", {
                suboption("ASCII Table", ""),
                suboption("About", "CTRL+A"),
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
        38
    );
    filemenu .addchild (&filename);

    filemsg = label (
        "",
        2,
        4
    );
    filemenu .addchild (&filemsg);

    fileclose = button (
        "Close",
        filemenu.getszx() - 9,
        filemenu.getszy() - 2,
        9,
        1 
    );
    filemenu .addchild (&fileclose);
    
    fileok = button (
        "Ok",
        filemenu.getszx() - 19,
        filemenu.getszy() - 2,
        8,
        1
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
        38
    );
    sfilemenu .addchild (&sfilename);

    sfilemsg = label (
        "",
        2,
        4
    );
    sfilemenu .addchild (&sfilemsg);

    sfileclose = button (
        "Close",
        sfilemenu.getszx() - 9,
        sfilemenu.getszy() - 2,
        9,
        1 
    );
    sfilemenu .addchild (&sfileclose);
    
    sfileok = button (
        "Ok",
        sfilemenu.getszx() - 19,
        sfilemenu.getszy() - 2,
        8,
        1
    );
    sfilemenu .addchild (&sfileok);

    /*fileclose = button (
        "Close",
        filemenu.getszx() - 13,
        filemenu.getszy() - 4,
        13,
        3 
    );
    filemenu .addchild (&fileclose);
    
    fileok = button (
        "Ok",
        filemenu.getszx() - 27,
        filemenu.getszy() - 4,
        12,
        3
    );
    filemenu .addchild (&fileok);*/

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
        2
    );
    crdtsframe .addchild (&crdtslabel);

    crdtsok = button (
        "Ok",
        crdtsframe.getszx() / 2 - 4,
        crdtsframe.getszy() - 2,
        8,
        1
    );
    crdtsframe .addchild (&crdtsok);

    atblframe = frame(
        "ASCII Table",
        ioh.getwsizex() / 2 - 25,
        ioh.getwsizey() / 2 - 11,
        50,
        22,
        LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
    );

    atblframe .setvsble (false);
    ioh .addchild (&atblframe);

    str tbl = "";
    i16 strl = 0;
    for (ch i = 0; (ui8)i < 128; ++ i) {
        str toadd = "";
        toadd += str(3 - std::to_string(i).length(), ' ');
        toadd += std::to_string(i) + ": ";
        toadd += (i < 32 || i > 126? " " : str(1, i));
        toadd += "|";

        strl += toadd.length();

        if ((i32)toadd.length() + strl > atblframe.getszx() - 4) {
            strl = 0; 
            tbl  += '\n';
        };

        tbl += toadd;
    };

    atbllabel = label(
        tbl,
        1,
        1
    );

    atblframe .addchild (&atbllabel);

    idx_linfcs = -1;
    sfowagr    = false;

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
            filemsg  .settxt   ("Type in the file name");
            filename .settxt   ("");

            filemenu .setpos   (ioh.getwsizex() / 2 - filemenu.getszx() / 2, ioh.getwsizey() / 2 - filemenu.getszy() / 2);
            filemenu .totop    ();

            filemenu .setvsble (true);
        } else if (menubar.getsboptst(0) == 3) {
            sfowagr = false;

            sfilemsg  .settxt   ("Type in the file name");
            sfilename .settxt   ("");

            sfilemenu .setpos   (ioh.getwsizex() / 2 - sfilemenu.getszx() / 2, ioh.getwsizey() / 2 - sfilemenu.getszy() / 2);
            sfilemenu .totop    ();

            sfilemenu .setvsble (true);
        } else if (menubar.getsboptst(2) == 0) {
            atblframe .setpos   (ioh.getwsizex() / 2 - atblframe.getszx() / 2, ioh.getwsizey() / 2 - atblframe.getszy() / 2);
            atblframe .totop    ();

            atblframe .setvsble (true);
        } else if (menubar.getsboptst(2) == 2) {
            crdtsframe .setpos   (ioh.getwsizex() / 2 - crdtsframe.getszx() / 2, ioh.getwsizey() / 2 - crdtsframe.getszy() / 2);
            crdtsframe .totop    ();
            
            crdtsframe .setvsble (true);
        } else if (menubar.getsboptst(0) == 0) {
            editors.push_back (editor (&ioh, "Untitled", ""));

            idx_linfcs = editors.size() - 1;
        } else if (fileclose.isclicked())
            filemenu .setvsble (false);
        else if (sfileclose.isclicked())
            sfilemenu .setvsble (false);
        else if (fileok.isclicked()) {
            str fname = filename .gettxt ();

            if (fname != "") {
                if (fexists(fname)) {
                    editors  .push_back (editor (&ioh, fname, ""));
                    filemenu .setvsble  (false);
                } else
                    filemsg  .settxt    ("File '" + (
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
                                strtof ("Generated by Trident Editor (TR-ED)", fname);
                                editors[idx_linfcs] .getfrm ()->setttl (fname);

                                sfilemenu .setvsble (false);

                                sfowagr = false;
                            } else {
                                sfilemsg .settxt ("File already exists, press ok if\nyou want to overwrite.");

                                sfowagr = true;
                            };
                        } else {
                            strtof ("Generated by Trident Editor (TR-ED)", fname);
                            editors[idx_linfcs] .getfrm ()->setttl (fname);

                            sfilemenu .setvsble (false);
                        };
                    } else
                        sfilemsg .settxt ("File name contains invalid symbols");
                } else 
                    sfilemsg .settxt ("Type in the file name");
            } else
                sfilemsg .settxt ("Select the editor of which the\ncontents you want to save.");
        } else if (crdtsok.isclicked())
            crdtsframe .setvsble (false);

        for (i16 i = 0; i < (i16)editors.size(); ++ i) {
            if (!editors[i].getfrm()->getvsble()) {
                editors[i] .cleanup ();
                editors.erase(editors.begin() + i);

                if (idx_linfcs == i) 
                    idx_linfcs = -1;

                -- i;

                continue;
            };

            if (editors[i].getfrm()->lstinfcs())
                idx_linfcs = i;
        };
    };

    for (i16 i = 0; i < (i16)editors.size(); ++ i) {
        editors[i] .cleanup ();
    };
};