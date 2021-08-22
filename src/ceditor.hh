#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_SETTINGS"
#include "types.hh"
#include "iohandle.hh"
#include "frame.hh"
#include "editor.hh"

#define __CEDIT_COL__DEF     100
#define __CEDIT_COL__DEF_FRM 101
#define __CEDIT_COL__DEF_GRN 102
#define __CEDIT_COL__KEYWORD 103
#define __CEDIT_COL__STRING  104
#define __CEDIT_COL__NUMBER  105
#define __CEDIT_COL__BOOLEAN 106
#define __CEDIT_COL__DATATYP 107
#define __CEDIT_COL__STD     108
#define __CEDIT_COL__COMMENT 109
#define __CEDIT_COL__PREPROC 110

extern i8 EditorForeground, EditorBackground;

class CEditor {
public:
	CEditor();
	CEditor(LUIC::IOHandle *p_IOH, str p_Title, str p_Text, std::vector <str> p_Settings);
	CEditor(LUIC::IOHandle *p_IOH, str p_Title, str p_Text, i16 p_PosX, i16 p_PosY, ui16 p_SizeX, ui16 p_SizeY, std::vector <str> p_Settings);

	void Cleanup();

	LUIC::Frame *GetFrame();
	LUIC::Editor *GetEditor();

	void SetFrameName(str p_Title);
	str GetFrameName();

	void SetTabSize(ui8 p_TabSize);

private:
	LUIC::Frame *Frame;
	LUIC::Editor *Editor;
};
