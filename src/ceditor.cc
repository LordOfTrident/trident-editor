#include "ceditor.hh"

str __gext(str p_Str) {
	sword lastdotpos = -1;

	for (word i = 0; i < p_Str.length(); ++ i)
		if (p_Str[i] == '.')
			lastdotpos = i;

	return lastdotpos == -1? "?" : p_Str.substr(lastdotpos);
};

CEditor::CEditor() {};

CEditor::CEditor(LUIC::IOHandle *p_IOH, str p_Title, str p_Text, std::vector <str> p_Settings) {
	Frame = new LUIC::Frame (
		p_Title,
		0,
		1,
		p_IOH->GetWindowSizeX(),
		p_IOH->GetWindowSizeY() - 1,
		LUIC_FLAGS_FRM_CLOSABLE | LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_RESIZABLE
	);

	p_IOH->AddChild(Frame);

	Editor = new LUIC::Editor(
		1,
		1,
		20,
		5,
		(bool)p_Settings[SETTING_CURBLINK][0]? LUIC_FLAGS_EDITOR_CURSORBLINK : 0
	);

	Editor->SetText(p_Text);
	Editor->SetColorscheme({
		__LUIC__SHDWCLR,
		__CEDIT_COL__DEF_FRM,
		__CEDIT_COL__DEF
	});

	Frame->AddChild(Editor);
	Frame->SetColorscheme({
		__LUIC__SHDWCLR,
		__CEDIT_COL__DEF_FRM,
		__CEDIT_COL__DEF_FRM,
		__CEDIT_COL__DEF_GRN,
		__LUIC__SYSCLR,
		__CEDIT_COL__DEF_FRM
	});

	SetTabSize((ui8)p_Settings[SETTING_TABSIZE][0]);
	SetFrameName(p_Title);
};

CEditor::CEditor(LUIC::IOHandle *p_IOH, str p_Title, str p_Text, i16 p_PosX, i16 p_PosY, ui16 p_SizeX, ui16 p_SizeY, std::vector <str> p_Settings) {
	Frame = new LUIC::Frame(
		p_Title,
		p_PosX,
		p_PosY,
		p_SizeX,
		p_SizeY,
		LUIC_FLAGS_FRM_CLOSABLE | LUIC_FLAGS_FRM_DRAGGABLE | LUIC_FLAGS_FRM_RESIZABLE
	);

	p_IOH->AddChild(Frame);

	Editor = new LUIC::Editor(
		1,
		1,
		20,
		5,
		(bool)p_Settings[SETTING_CURBLINK][0]? LUIC_FLAGS_EDITOR_CURSORBLINK : 0
	);

	Editor->SetText(p_Text);
	Editor->SetColorscheme({
		__LUIC__SHDWCLR,
		__CEDIT_COL__DEF_FRM,
		__CEDIT_COL__DEF
	});

	Frame->AddChild(Editor);
	Frame->SetColorscheme({
		__LUIC__SHDWCLR,
		__CEDIT_COL__DEF_FRM,
		__CEDIT_COL__DEF_FRM,
		__CEDIT_COL__DEF_GRN,
		__LUIC__SYSCLR,
		__CEDIT_COL__DEF_FRM
	});

	SetTabSize((ui8)p_Settings[SETTING_TABSIZE][0]);
	SetFrameName(p_Title);
};

void CEditor::SetFrameName(str p_Title) {
	Frame->SetTitle(p_Title);
	Editor->SetTitle(p_Title);

	str FT = __gext(p_Title);

	Editor->SetFT(FT);
};

str CEditor::GetFrameName() {
	return Editor->GetTitle();
};

void CEditor::Cleanup() {
	if (Frame != NULL) {
		Frame->RemoveChild(Frame->GetChild(Editor));

		LUIC::IOHandle* IOH = Frame->GetIOHandle();
		IOH->RemoveChild(IOH->GetChild(Frame));

		delete Editor;
		delete Frame;
	};
};

LUIC::Frame *CEditor::GetFrame() {
	return Frame;
};

LUIC::Editor *CEditor::GetEditor() {
	return Editor;
};

void CEditor::SetTabSize(ui8 p_tabsize) {
	Editor->SetTabSize(p_tabsize);
};
