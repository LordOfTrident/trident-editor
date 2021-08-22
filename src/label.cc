#include "label.hh"

LUIC::Label::Label() {};

LUIC::Label::Label(str p_Text, ui16 p_PosX, ui16 p_PosY, flags p_Flags):
	Text(p_Text)
{
	Type = LUIC_TYPE_LABEL;
	Flags = p_Flags;
	ColorScheme = {
		__LUIC__COMMON,
		__LUIC__SHDWCLR
	};

	PosX = p_PosX;
	PosY = p_PosY;

	ui16 SizeX = 0;
	ui16 SizeY = 1;

	ui16 pos = 0;
	for (ui16 i = 0; i < Text.length(); ++ i) {
		if (Text[i] == 10) {
			++ SizeY;

			if (SizeX < i - pos + 1) SizeX = i - pos;

			pos = i;
		};
	};

	if (SizeX < Text.length() - pos + 1)
		SizeX = Text.length() - pos;

	Wnd = Window(PosX, PosY, SizeX, SizeY);
};

void LUIC::Label::SetText(str p_Text) {
	Text = p_Text;

	ui16 SizeX = 0;
	ui16 SizeY = 1;

	ui16 pos = 0;

	for (ui16 i = 0; i < Text.length(); ++ i) {
		if (Text[i] == 10) {
			++ SizeY;

			if (SizeX < i - pos + 1) SizeX = i - pos;

			pos = i;
		};
	};

	if (SizeX < Text.length() - pos + 1)
		SizeX = Text.length() - pos;

	Wnd.SetSize(SizeX, SizeY);
};

void LUIC::Label::Draw() {
	if (IOH == NULL or not Visible)
		return;

	if (Parent == NULL)
		Wnd.DrawShadow(ColorScheme[1]);

	Wnd.SetBackgroundColor(ColorScheme[0]);

	bool altcharset = false;

	if (Flags & LUIC_FLAGS_LBL_ALTCHARSET) {
		altcharset = true;

		Wnd.SetAttribute(A_ALTCHARSET, true);
	};

	Wnd.OutAt(0, 0, Text);

	if (altcharset)
		Wnd.SetAttribute(A_ALTCHARSET, false);
};

void LUIC::Label::Input(i16 p_Input, MEVENT* p_Event) {};
