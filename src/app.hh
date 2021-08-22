#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_DEFAULTS"
#include "_SCBL"
#include "_SETTINGS"
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
#include "fs.hh"

class App {
public:
	App();
	App(i32 argc, ch* argv[]);

	void Start();

	void __sscblerr(str p_SCBLError);
	str __ghmdir();
	void __ssttng(ui8 p_SettingIdx, str p_Value);
	void __sclr(ui8 p_ColorIdx, ui8 p_Value);
	ui8 __gclr(ui8 p_ColorIdx);

private:
	void CreateOrFixPath(str p_Path);
	void CreateOrFixPath(str p_Path, str p_Contents);
	void RunSCBL(str p_Path);

	void Defaults();
	void InitSCBL();
	void ReadConfig();

	bool running;

	i8 SCBLOk;
	str SCBLError;

	SCBL::SCBL_Interpreter SCBLi;

	str HomeDir;

	std::vector <str> Settings;
	std::vector <ui8> Colors;

	LUIC::IOHandle IOH;
	LUIC::TopBar TopBar;

	LUIC::Frame LFileMenu;
	LUIC::TextBar LFileName;
	LUIC::Button LFileClose, LFileOk;
	LUIC::Label LFileMsg;

	bool SFile;
	LUIC::Frame SFileMenu;
	LUIC::TextBar SFileName;
	LUIC::Button SFileClose, SFileOk;
	LUIC::Label SFileMsg;

	LUIC::Frame CreditsFrame;
	LUIC::Label CreditsLabel;
	LUIC::Button CreditsOk;

	LUIC::Frame ASCIITableFrame;
	LUIC::Label ASCIITableLabel, ASCIITableLine;
	vector <LUIC::Component*> ASCIITableButtons;

	LUIC::Frame SCBLFrame;
	LUIC::Label SCBLMsg;

	vector <CEditor> CEditors;
	i8 IdxLastInFocus;
};
