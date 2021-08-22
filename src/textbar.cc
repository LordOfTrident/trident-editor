#include "textbar.hh"

LUIC::TextBar::TextBar() {};

LUIC::TextBar::TextBar(ui16 p_PosX, ui16 p_PosY, ui16 p_SizeX, flags p_Flags):
	Text(""),
	Pressed(false),
	CurPos(0),
	ScrollX(0)
{
	Type = LUIC_TYPE_TEXTBAR;
	Flags = p_Flags;

	PosX = p_PosX;
	PosY = p_PosY;

	Wnd = Window(PosX, PosY, p_SizeX, 3);
};

void LUIC::TextBar::SetText(str p_Text) {
	Text = p_Text;

	CurPos = 0;
	ScrollX = 0;
};

str LUIC::TextBar::GetText() {
	return Text;
};

void LUIC::TextBar::Draw() {
	if (IOH == NULL or not Visible)
		return;

	if (Parent == NULL)
		Wnd.DrawShadow(__LUIC__SHDWCLR);

	if (Pressed)
		Wnd.SetBackgroundColor(__LUIC__SYSCLR);
	else if (InFocus)
		Wnd.SetBackgroundColor(__LUIC__TXTCLR);
	else
		Wnd.SetBackgroundColor(__LUIC__TXTNFCS);

	Wnd.OutAt(1, 1, Text.substr(ScrollX, Wnd.GetSizeX() - 2));

	if (InFocus) {
		Wnd.SetDBBorder(0, 0, Wnd.GetSizeX() - 1, Wnd.GetSizeY() - 1, __LUIC__WBACLR, __LUIC__WBBCLR);

		Wnd.SetAttribute(A_REVERSE, true);
		Wnd.SetColor(__LUIC__SYSCLR);

		if (CurPos == Text.length())
			Wnd.OutAt(CurPos - ScrollX + 1, 1, " ");
		else
			Wnd.OutAt(CurPos - ScrollX + 1, 1, str(1, Text[CurPos]));

		Wnd.SetAttribute(A_REVERSE, false);
	} else
		Wnd.SetBorder(0, 0, Wnd.GetSizeX() - 1, Wnd.GetSizeY() - 1, __LUIC__WBACLR, __LUIC__WBBCLR);
};

void LUIC::TextBar::Input(i16 p_Input, MEVENT* p_Event) {
	if (IOH == NULL or not Visible)
		return;

	switch (p_Input) {
		case KEY_MOUSE: {
			if (p_Event->bstate & BUTTON1_RELEASED) {
				if (Pressed)
					Pressed = false;
			} else if (p_Event->bstate & BUTTON1_PRESSED) {
				if (IOH->__gchldfcsed())
					return;

				bool PosCheck = p_Event->x >= Wnd.GetPosX() and p_Event->y >= Wnd.GetPosY();
				bool BRCornerCheck = p_Event->x < Wnd.GetSizeX() + Wnd.GetPosX() and p_Event->y < Wnd.GetSizeY() + Wnd.GetPosY();

				if (not (PosCheck and BRCornerCheck)) {
					InFocus = false;

					break;
				};

				Pressed = true;
				InFocus = true;

				IOH->__schldfcsed(false);
				__fcsprnts();
			};

			break;
		};

		case 10: {
			InFocus = false;

			break;
		};

		case 127: case KEY_BACKSPACE: {
			if (CurPos == 0 or not InFocus)
				break;

			Text.erase(CurPos - 1, 1);
			-- CurPos;

			break;
		};

		case KEY_LEFT: {
			if (CurPos == 0 or not InFocus)
				break;

			-- CurPos;

			break;
		};

		case KEY_RIGHT: {
			if (CurPos == Text.length() or not InFocus)
				break;

			++ CurPos;

			break;
		};

		default: {
			if (not InFocus)
				break;

			if (p_Input < 32 or p_Input > 126)
				break;

			Text.insert(CurPos, str(1, p_Input));

			++ CurPos;

			break;
		};
	};

	if ((i16)CurPos - (i16)ScrollX >= Wnd.GetSizeX() - 3)
		ScrollX = CurPos - (Wnd.GetSizeX() - 3);

	if ((i16)CurPos - (i16)ScrollX < 0)
		ScrollX =  CurPos;
};
