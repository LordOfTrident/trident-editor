#include "app.hh"

App::App(i32 argc, ch* argv[]) {
	IOH = LUIC::IOHandle(
		"Trident Editor",
		LUIC_FLAGS_IOH_NOESCDELAY | LUIC_FLAGS_IOH_CLEARSCR
	);

	IOH.InitTheme(true);

	TopBar = LUIC::TopBar (
		"TR-ED",
		{
			LUIC::Option("File", {
				LUIC::SubOption("New",     "CTRL+N"),
				LUIC::SubOption("Load",    "CTRL+L"),
				LUIC::SubOption("Save",    "CTRL+S"),
				LUIC::SubOption("Save As", ""),
				LUIC::SubOption("Exit",    "CTRL+Q")
			}),

			LUIC::Option("Edit", {
				LUIC::SubOption("Copy",  "CTRL+C"),
				LUIC::SubOption("Cut",   "CTRL+X"),
				LUIC::SubOption("Paste", "CTRL+V")
			}),

			LUIC::Option("Tools", {
				LUIC::SubOption("ASCII Table",  "CTRL+A"),
				LUIC::SubOption("Reset Config", "CTRL+R")
			}),

			LUIC::Option("Help", {
				LUIC::SubOption("About", ""),
				LUIC::SubOption("Credits", "")
			})
		}
	);

	IOH.AddChild(&TopBar);

	LFileMenu = LUIC::Frame(
		"Load File",
		IOH.GetWindowSizeX() / 2 - 20,
		IOH.GetWindowSizeY() / 2 - 5,
		40,
		10,
		LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
	);
	LFileMenu.SetVisible(false);
	IOH.AddChild(&LFileMenu);

	LFileName = LUIC::TextBar(
		1,
		1,
		38,
		0
	);
	LFileMenu.AddChild(&LFileName);

	LFileMsg = LUIC::Label(
		"",
		2,
		4,
		0
	);
	LFileMenu.AddChild(&LFileMsg);

	LFileClose = LUIC::Button(
		"Close",
		LFileMenu.GetSizeX() - 9,
		LFileMenu.GetSizeY() - 2,
		9,
		1,
		0
	);
	LFileMenu.AddChild(&LFileClose);

	LFileOk = LUIC::Button(
		"Ok",
		LFileMenu.GetSizeX() - 19,
		LFileMenu.GetSizeY() - 2,
		8,
		1,
		0
	);
	LFileMenu.AddChild(&LFileOk);

	SFileMenu = LUIC::Frame(
		"Save File As",
		IOH.GetWindowSizeX() / 2 - 20,
		IOH.GetWindowSizeY() / 2 - 5,
		40,
		10,
		LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
	);
	SFileMenu.SetVisible(false);
	IOH.AddChild(&SFileMenu);

	SFileName = LUIC::TextBar(
		1,
		1,
		38,
		0
	);
	SFileMenu.AddChild(&SFileName);

	SFileMsg = LUIC::Label(
		"",
		2,
		4,
		0
	);
	SFileMenu.AddChild(&SFileMsg);

	SFileClose = LUIC::Button(
		"Close",
		SFileMenu.GetSizeX() - 9,
		SFileMenu.GetSizeY() - 2,
		9,
		1,
		0
	);
	SFileMenu.AddChild(&SFileClose);

	SFileOk = LUIC::Button(
		"Ok",
		SFileMenu.GetSizeX() - 19,
		SFileMenu.GetSizeY() - 2,
		8,
		1,
		0
	);
	SFileMenu.AddChild(&SFileOk);

	CreditsFrame = LUIC::Frame(
		"Credits",
		IOH.GetWindowSizeX() / 2 - 15,
		IOH.GetWindowSizeY() / 2 - 4,
		30,
		8,
		LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
	);
	CreditsFrame.SetVisible(false);
	IOH.AddChild(&CreditsFrame);

	CreditsLabel = LUIC::Label(
		"    Made By LordOfTrident\n        Using NCurses",
		1,
		2,
		0
	);
	CreditsFrame.AddChild(&CreditsLabel);

	CreditsOk = LUIC::Button(
		"Ok",
		CreditsFrame.GetSizeX() / 2 - 4,
		CreditsFrame.GetSizeY() - 2,
		8,
		1,
		0
	);
	CreditsFrame.AddChild(&CreditsOk);

	ASCIITableFrame = LUIC::Frame(
		"ASCII Table",
		IOH.GetWindowSizeX() / 2 - 25,
		IOH.GetWindowSizeY() / 2 - 11,
		34,
		8,
		LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
	);

	ASCIITableFrame.SetVisible(false);
	IOH.AddChild(&ASCIITableFrame);

	ui8 posx = 0, posy = 0;

	for (ui8 i = 0; i < 128; ++ i) {
		if (i % 32 == 0) {
			posx = 0;

			++ posy;
		};

		LUIC::Button* btn = new LUIC::Button(
			i < 32 or i > 126? " " : str(1, i),
			1 + posx,
			posy,
			1,
			1,
			0
		);

		btn->SetColorscheme({
			__LUIC__SHDWCLR,
			__LUIC__WNDCLR,
			__LUIC__SYSCLR,
			__LUIC__WBACLR,
			__LUIC__WBBCLR,
			__LUIC__TXTCLR,
			__LUIC__COMMON
		});

		ASCIITableButtons.push_back(btn);
		ASCIITableFrame.AddChild(btn);

		++ posx;
	};

	ASCIITableLine = LUIC::Label(
		str(32, ACS_HLINE),
		1,
		5,
		LUIC_FLAGS_LBL_ALTCHARSET
	);

	ASCIITableLine.SetColorscheme({
		__LUIC__WBACLR,
		__LUIC__SHDWCLR
	});

	ASCIITableFrame.AddChild(&ASCIITableLine);

	ASCIITableLabel = LUIC::Label(
		"CH: - | CNTRL: - | DEC: -",
		2,
		6,
		0
	);

	ASCIITableFrame.AddChild(&ASCIITableLabel);

	SCBLFrame = LUIC::Frame(
		"SCBL",
		IOH.GetWindowSizeX() / 2 - 20,
		IOH.GetWindowSizeY() / 2 - 4,
		40,
		8,
		LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_CLOSABLE
	);

	IOH.AddChild(&SCBLFrame);

	SCBLMsg = LUIC::Label(
		"",
		1,
		1,
		0
	);

	SCBLFrame.AddChild(&SCBLMsg);

	HomeDir = getenv("HOME");
	IdxLastInFocus = -1;
	SFile = false;

	Defaults();
	InitSCBL();
	ReadConfig();

	i16 cex = argc - 2, cey = 1;
	for (ui8 i = 1; i < argc; ++ i) {
		str txt = FS::Exists(argv[i]) and not FS::IsADirectory(argv[i])? FS::ReadFile(argv[i]) : "";

		CEditors.push_back(CEditor(
			&IOH,
			argv[i],
			txt,
			cex,
			cey,
			IOH.GetWindowSizeX() - cex,
			IOH.GetWindowSizeY() - cey,
			Settings
		));
		LFileMenu.SetVisible(false);

		-- cex;
		++ cey;

		if (cex < 0)
			cex = argc - 2;

		if (cey > IOH.GetWindowSizeY())
			cey = 1;

		IdxLastInFocus = i - 1;
	};

	if (IdxLastInFocus != -1)
		CEditors[IdxLastInFocus].GetEditor()->SetFocus(true);

	if (SCBLFrame.GetVisible())
		SCBLFrame.ToTop();

	erase();
};

void App::Defaults() {
	__ssttng(SETTING_TABSIZE, {4});
	__ssttng(SETTING_CURBLINK, {1});
	__ssttng(SETTING_THEMEFILE, "");

	__sclr(SETTINGCOLOR_EDITORFG, COLOR_WHITE);
	__sclr(SETTINGCOLOR_EDITORBG, COLOR_BLACK);
	__sclr(SETTINGCOLOR_EDITORFRAMEFG, COLOR_WHITE);
	__sclr(SETTINGCOLOR_EDITORFRAMEBG, COLOR_BLACK);

	init_pair(__CEDIT_COL__DEF,     COLOR_WHITE,       COLOR_BLACK);
	init_pair(__CEDIT_COL__DEF_GRN, COLOR_BRIGHTGREEN, COLOR_BLACK);
	init_pair(__CEDIT_COL__DEF_FRM, COLOR_WHITE,       COLOR_BLACK);

	init_pair(__CEDIT_COL__KEYWORD, COLOR_WHITE, COLOR_BRIGHTMAGENTA);
	init_pair(__CEDIT_COL__STRING,  COLOR_WHITE, COLOR_BRIGHTGREEN);
	init_pair(__CEDIT_COL__NUMBER,  COLOR_WHITE, COLOR_CYAN);
	init_pair(__CEDIT_COL__BOOLEAN, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(__CEDIT_COL__DATATYP, COLOR_WHITE, COLOR_BRIGHTCYAN);
	init_pair(__CEDIT_COL__STD,     COLOR_WHITE, COLOR_BRIGHTYELLOW);
	init_pair(__CEDIT_COL__COMMENT, COLOR_WHITE, COLOR_GREY);
	init_pair(__CEDIT_COL__PREPROC, COLOR_WHITE, COLOR_BRIGHTRED);

	CreateOrFixPath(HomeDir + "/.config/tr-ed");
	CreateOrFixPath(HomeDir + "/.config/tr-ed/themes");
	CreateOrFixPath(HomeDir + "/.config/tr-ed/syntax");

	CreateOrFixPath(HomeDir + "/.config/tr-ed/settings.scbl", STTNGS_DEFAULT);
	CreateOrFixPath(HomeDir + "/.config/tr-ed/themes/default.scbl",   THEME_DEFAULT);
	CreateOrFixPath(HomeDir + "/.config/tr-ed/themes/classic.scbl",   THEME_CLASSIC);
	CreateOrFixPath(HomeDir + "/.config/tr-ed/themes/light.scbl",     THEME_LIGHT);
	CreateOrFixPath(HomeDir + "/.config/tr-ed/themes/bubblegum.scbl", THEME_BUBBLEGUM);
};

void App::InitSCBL() {
	SCBLi.SetUserData(this);

	SCBLi.AddConstant(SCBL::Constant("true",  1, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("false", 0, SCBL_CONSTANT_1BYTE));

	SCBLi.AddConstant(SCBL::Constant("tabsize",     SCBL_TABSIZE, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("color",       SCBL_COLOR,   SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("theme",       SCBL_THEME,   SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("cursorblink", SCBL_CURBLNK, SCBL_CONSTANT_1BYTE));

	SCBLi.AddConstant(SCBL::Constant("background",      SCBL_BG,       SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("foreground",      SCBL_FG,       SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("frameforeground", SCBL_FRAME,    SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("framebackground", SCBL_FRAMEBG,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("keyword",         SCBL_KEYWORD,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("string",          SCBL_STRING,   SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("number",          SCBL_NUMBER,   SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("boolean",         SCBL_BOOLEAN,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("datatype",        SCBL_DATATYPE, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("std",             SCBL_STD,      SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("comment",         SCBL_COMMENT,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("preproc",         SCBL_PREPROC,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("operators",       SCBL_OPERATOR, SCBL_CONSTANT_1BYTE));

	SCBLi.AddConstant(SCBL::Constant("black",   COLOR_BLACK,   SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("red",     COLOR_RED,     SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("green",   COLOR_GREEN,   SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("yellow",  COLOR_YELLOW,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("blue",    COLOR_BLUE,    SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("magenta", COLOR_MAGENTA, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("cyan",    COLOR_CYAN,    SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("white",   COLOR_WHITE,   SCBL_CONSTANT_1BYTE));

	SCBLi.AddConstant(SCBL::Constant("grey",          COLOR_GREY,          SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("brightred",     COLOR_BRIGHTRED,     SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("brightgreen",   COLOR_BRIGHTGREEN,   SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("brightyellow",  COLOR_BRIGHTYELLOW,  SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("brightblue",    COLOR_BRIGHTBLUE,    SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("brightmagenta", COLOR_BRIGHTMAGENTA, SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("brightcyan",    COLOR_BRIGHTCYAN,    SCBL_CONSTANT_1BYTE));
	SCBLi.AddConstant(SCBL::Constant("brightwhite",   COLOR_BRIGHTWHITE,   SCBL_CONSTANT_1BYTE));
};

void App::ReadConfig() {
	SCBLError = "";
	SCBLOk = SCBL_RUNTIME_OK;

    SCBLi.SetFunction(SCBL::Function("set", [] (std::vector <ui8> Parameters, void* UserData) {
        SCBL::ParameterHandler phnd(Parameters);
        App *app = (App*)UserData;

        ui8 arg = phnd.GetNextParam8();

        if (phnd.IsOutOfParams()) {
            app->__sscblerr("Not enough parameters for 'set'");

            return;
        };

        switch (arg) {
            case SCBL_TABSIZE: {
                arg = phnd.GetNextParamInt();

				if (arg < 1)
					arg = 1;

                if (phnd.IsOutOfParams()) {
                    app->__sscblerr("Not enough parameters for 'set tabsize'");

                    return;
                };

                app->__ssttng(SETTING_TABSIZE, {(ch)arg});

                break;
            };

			case SCBL_CURBLNK: {
                arg = phnd.GetNextParamInt();

                if (phnd.IsOutOfParams()) {
                    app->__sscblerr("Not enough parameters for 'set cursorblink'");

                    return;
                };

				app->__ssttng(SETTING_CURBLINK, {(bool)arg});

				break;
			};

			case SCBL_THEME: {
				str arg = phnd.GetNextParamStr();

				if (phnd.IsOutOfParams()) {
                    app->__sscblerr("Not enough parameters for 'set theme'");

                    return;
                };

				str fullpath = app->__ghmdir() + "/.config/tr-ed/themes/" + arg + ".scbl";

				if (not FS::Exists(fullpath) or FS::IsADirectory(fullpath)) {
                    app->__sscblerr("'set theme' theme file\n'" + fullpath + "'\nnot found");

                    return;
                };

				app->__ssttng(SETTING_THEMEFILE, fullpath);

				break;
			};
        };
    }));

	RunSCBL(HomeDir + "/.config/tr-ed/settings.scbl");

	SCBLi.SetFunction(SCBL::Function("set", [] (std::vector <ui8> Parameters, void* UserData) {
        SCBL::ParameterHandler phnd(Parameters);
        App *app = (App*)UserData;

        ui8 arg = phnd.GetNextParam8();

        if (phnd.IsOutOfParams()) {
            app->__sscblerr("Not enough parameters for 'set'");

            return;
        };

        switch (arg) {
            case SCBL_COLOR: {
                arg = phnd.GetNextParam8();
                ui8 color = phnd.GetNextParam8();

                if (phnd.IsOutOfParams()) {
                    app->__sscblerr("Not enough parameters for 'set color'");

                    return;
                };

                switch (arg) {
                    case SCBL_BG: {
						app->__sclr(SETTINGCOLOR_EDITORBG, color);

                        init_pair(__CEDIT_COL__DEF, app->__gclr(SETTINGCOLOR_EDITORFG), app->__gclr(SETTINGCOLOR_EDITORBG));

                        break;
                    };

                    case SCBL_FG: {
						app->__sclr(SETTINGCOLOR_EDITORFG, color);

                        init_pair(__CEDIT_COL__DEF, app->__gclr(SETTINGCOLOR_EDITORFG), app->__gclr(SETTINGCOLOR_EDITORBG));

                        break;
                    };

                    case SCBL_FRAME: {
						app->__sclr(SETTINGCOLOR_EDITORFRAMEFG, color);

                        init_pair(__CEDIT_COL__DEF_FRM, app->__gclr(SETTINGCOLOR_EDITORFRAMEFG), app->__gclr(SETTINGCOLOR_EDITORFRAMEBG));

                        break;
                    };

					case SCBL_FRAMEBG: {
						app->__sclr(SETTINGCOLOR_EDITORFRAMEBG, color);

                        init_pair(__CEDIT_COL__DEF_FRM, app->__gclr(SETTINGCOLOR_EDITORFRAMEFG), app->__gclr(SETTINGCOLOR_EDITORFRAMEBG));
                        init_pair(__CEDIT_COL__DEF_GRN, COLOR_BRIGHTGREEN, app->__gclr(SETTINGCOLOR_EDITORFRAMEBG));

                        break;
                    };
                };

                break;
            };
        };
    }));

	RunSCBL(Settings[SETTING_THEMEFILE]);
};

void App::Start() {
	running = true;

	while (running) {
		IOH.Draw();
		IOH.Input();

		switch (TopBar.GetChoice()) {
			case 0: {
				switch (TopBar.GetSubChoice()) {
					case 0: {
						CEditors.push_back(CEditor(
							&IOH,
							"Untitled",
							"",
							Settings
						));

						IdxLastInFocus = CEditors.size() - 1;

						CEditors[IdxLastInFocus].GetFrame()->SetFocus(true);
						CEditors[IdxLastInFocus].GetEditor()->SetFocus(true);

						break;
					};

					case 1: {
						LFileMsg.SetText("Type in the file name");
						LFileName.SetText("");

						LFileMenu.SetPos(IOH.GetWindowSizeX() / 2 - LFileMenu.GetSizeX() / 2, IOH.GetWindowSizeY() / 2 - LFileMenu.GetSizeY() / 2);
						LFileMenu.ToTop();

						LFileMenu.SetVisible(true);

						break;
					};

					case 2: {
						if (IdxLastInFocus != -1) {
							str fname = CEditors[IdxLastInFocus].GetFrameName();

							if (FS::Exists(fname)) {
								if (FS::IsADirectory(fname))
									FS::Delete(fname);

								FS::WriteFile(CEditors[IdxLastInFocus].GetEditor()->GetText(), fname);
								CEditors[IdxLastInFocus].GetEditor()->SetModif(false);
							} else {
								SFile = false;

								SFileMsg.SetText("Type in the file name");
								SFileName.SetText("");

								SFileMenu.SetPos(IOH.GetWindowSizeX() / 2 - SFileMenu.GetSizeX() / 2, IOH.GetWindowSizeY() / 2 - SFileMenu.GetSizeY() / 2);
								SFileMenu.ToTop();

								SFileMenu.SetVisible(true);
							};
						};

						break;
					};

					case 3: {
						SFile = false;

						SFileMsg.SetText("Type in the file name");
						SFileName.SetText("");

						SFileMenu.SetPos(IOH.GetWindowSizeX() / 2 - SFileMenu.GetSizeX() / 2, IOH.GetWindowSizeY() / 2 - SFileMenu.GetSizeY() / 2);
						SFileMenu.ToTop();

						SFileMenu.SetVisible(true);

						break;
					};

					case 4: {
						running = false;

						break;
					};
				};

				break;
			};

			case 1: {
				switch (TopBar.GetSubChoice()) {

				};

				break;
			};

			case 2: {
				switch (TopBar.GetSubChoice()) {
					case 0: {
						ASCIITableFrame.SetPos(IOH.GetWindowSizeX() / 2 - ASCIITableFrame.GetSizeX() / 2, IOH.GetWindowSizeY() / 2 - ASCIITableFrame.GetSizeY() / 2);
						ASCIITableFrame.ToTop();

						ASCIITableFrame.SetVisible(true);

						break;
					};

					case 1: {
						Defaults();
						ReadConfig();

						break;
					};
				};

				break;
			};

			case 3: {
				switch (TopBar.GetSubChoice()) {
					case 0: {

						break;
					};

					case 1: {
						CreditsFrame.SetPos(IOH.GetWindowSizeX() / 2 - CreditsFrame.GetSizeX() / 2, IOH.GetWindowSizeY() / 2 - CreditsFrame.GetSizeY() / 2);
						CreditsFrame.ToTop();

						CreditsFrame.SetVisible(true);

						break;
					};
				};

				break;
			}
		};

		if (LFileClose.IsClicked())
			LFileMenu.SetVisible(false);
		else if (SFileClose.IsClicked())
			SFileMenu.SetVisible(false);
		else if (LFileOk.IsClicked()) {
			str fname = LFileName.GetText();

			if (fname != "") {
				if (fname[0] == '~')
					fname = HomeDir + fname.substr(1);

				if (FS::Exists(fname) and not FS::IsADirectory(fname)) {
					CEditors.push_back(CEditor(
						&IOH,
						fname,
						FS::ReadFile(fname),
						Settings
					));
					LFileMenu.SetVisible(false);
				} else
					LFileMsg.SetText("File '" + (fname.length() > 9? fname.substr(0, 9) + "..." : fname) + "' not found.\nType in the file name");
			} else
				LFileMsg.SetText("Type in the file name");
		} else if (SFileOk.IsClicked()) {
			if (IdxLastInFocus != -1) {
				str fname = SFileName.GetText();

				if (fname != "") {
					if (fname[0] == '~')
						fname = HomeDir + fname.substr(1);

					bool error = true;
					for (ui16 i = 0; i < fname.length(); ++ i)
						if (fname[i] != ' ' and fname[i] != '*')
							error = false;

					if (not error) {
						if (FS::Exists(fname)) {
							if (SFile) {
								FS::WriteFile(CEditors[IdxLastInFocus].GetEditor()->GetText(), fname);
								CEditors[IdxLastInFocus].GetEditor() ->SetModif (false);

								CEditors[IdxLastInFocus].SetFrameName(fname);

								SFileMenu.SetVisible(false);

								SFile = false;
							} else {
								SFileMsg.SetText("File already exists, press ok if\nyou want to overwrite.");

								SFile = true;
							};
						} else {
							FS::WriteFile(CEditors[IdxLastInFocus].GetEditor()->GetText(), fname);
							CEditors[IdxLastInFocus].GetEditor() ->SetModif(false);
							CEditors[IdxLastInFocus].SetFrameName(fname);

							SFileMenu.SetVisible(false);
						};
					} else
						SFileMsg.SetText("File name contains invalid symbols");
				} else
					SFileMsg.SetText("Type in the file name");
			} else
				SFileMsg.SetText("Select the editor of which the\ncontents you want to save.");
		} else if (CreditsOk.IsClicked())
			CreditsFrame.SetVisible(false);

		ASCIITableLabel.SetText("CH: - | CNTRL: - | DEC: -");
		for (ui8 i = 0; i < (ui8)ASCIITableButtons.size(); ++ i) {
			LUIC::Button* btn = (LUIC::Button*)ASCIITableButtons[i];

			if (btn->IsLastInFocus()) {
				ASCIITableLabel.SetText("CH: " + (i < 32 or i > 126? "-" : str(1, i)) + " | CNTRL: " + (i < 32 or i > 126? "Y" : "N") + " | DEC: " + std::to_string(i));

				break;
			};
		};

		for (i16 i = 0; i < (i16)CEditors.size(); ++ i) {
			LUIC::Frame* ceditfrm = CEditors[i].GetFrame();
			ceditfrm->SetFlags(LUIC_FLAGS_FRM_CLOSABLE | LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_RESIZABLE);

			if (ceditfrm->GetPosX() + ceditfrm->GetSizeX() == IOH.GetWindowSizeX())
				ceditfrm->SetFlags(ceditfrm->GetFlags() | LUIC_FLAGS_COMPONENT_SCALEDX);

			if (ceditfrm->GetPosY() + ceditfrm->GetSizeY() == IOH.GetWindowSizeY())
				ceditfrm->SetFlags(ceditfrm->GetFlags() | LUIC_FLAGS_COMPONENT_SCALEDY);

			if (not CEditors[i].GetFrame()->GetVisible()) {
				CEditors[i].Cleanup();
				CEditors.erase(CEditors.begin() + i);

				if (IdxLastInFocus == i)
					IdxLastInFocus = -1;

				-- i;

				continue;
			};

			if (CEditors[i].GetFrame()->IsLastInFocus())
				IdxLastInFocus = i;
		};
	};

	for (i16 i = 0; i < (i16)CEditors.size(); ++ i)
		CEditors[i].Cleanup();

	for (ui8 i = 0; i < (ui8)ASCIITableButtons.size(); ++ i)
		delete (LUIC::Button*)ASCIITableButtons[i];
};

void App::CreateOrFixPath(str p_Path) {
	if (not FS::IsADirectory(p_Path))
		FS::Delete(p_Path);

	if (not FS::Exists(p_Path))
		FS::CreateDirectory(p_Path);
};

void App::CreateOrFixPath(str p_Path, str p_Contents) {
	if (FS::IsADirectory(p_Path))
		FS::Delete(p_Path);

	if (not FS::Exists(p_Path))
		FS::WriteFile(p_Contents, p_Path);
};

void App::RunSCBL(str p_Path) {
	if (SCBLOk != SCBL_RUNTIME_OK)
		return;

	SCBLOk = SCBLi.Parse(FS::ReadFile(p_Path));

	if (SCBLOk == SCBL_PARSER_OK)
		SCBLOk = SCBLi.Run();

	str scblframedesc = "SCBL: ";
	bool showscblfrm = false;

	if (SCBLError != "") {
		scblframedesc += SCBLError + "\n";
		showscblfrm = true;
	};

	if (SCBLOk != SCBL_RUNTIME_OK) {
		scblframedesc += SCBLi.GetErrorMsg();
		showscblfrm = true;
	};

	if (showscblfrm) {
		SCBLMsg.SetText(scblframedesc);

		SCBLFrame.SetVisible(true);
		SCBLFrame.ToTop();
	} else
		SCBLFrame.SetVisible(false);
};

void App::__sscblerr(str p_SCBLError) {
	SCBLError = p_SCBLError;
};

str App::__ghmdir() {
	return HomeDir;
};

void App::__ssttng(ui8 p_SettingIdx, str p_Value) {
	while (p_SettingIdx >= (ui8)Settings.size())
		Settings.push_back("");

	Settings[p_SettingIdx] = p_Value;
};
;
void App::__sclr(ui8 p_ColorIdx, ui8 p_Value) {
	while (p_ColorIdx >= (ui8)Colors.size())
		Colors.push_back(0);

	Colors[p_ColorIdx] = p_Value;
};

ui8 App::__gclr(ui8 p_ColorIdx) {
	if (p_ColorIdx > (ui8)Colors.size())
		return 0;

	return Colors[p_ColorIdx];
};