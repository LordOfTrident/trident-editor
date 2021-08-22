#include "button.hh"

LUIC::Button::Button() {};

LUIC::Button::Button(str p_Text, ui16 p_PosX, ui16 p_PosY, ui16 p_SizeX, ui16 p_SizeY, flags p_Flags):
	Text(p_Text),
	Pressed(false),
	Click(false)
{
	Type = LUIC_TYPE_BUTTON;
	Flags = p_Flags;
	ColorScheme = {
		__LUIC__SHDWCLR,
		__LUIC__WNDCLR,
		__LUIC__SYSCLR,
		__LUIC__WBACLR,
		__LUIC__WBBCLR,
		__LUIC__BTNPCLR,
		__LUIC__HGHLGHT
	};

	PosX = p_PosX;
	PosY = p_PosY;

	Wnd = Window(PosX, PosY, p_SizeX, p_SizeY);
};

void LUIC::Button::SetText(str p_Text) {
	Text = p_Text;
};

bool LUIC::Button::IsClicked() {
	if (Click) {
		Click = false;

		return true;
	} else
		return false;
};

void LUIC::Button::Draw() {
	if (IOH == NULL or not Visible)
		return;

	if (Parent == NULL)
		Wnd.DrawShadow(ColorScheme[0]);

	if (Wnd.GetSizeY() > 2) {
		Wnd.SetBackgroundColor(ColorScheme[1]);

		if (Pressed)
			Wnd.SetDBBorder(0, 0, Wnd.GetSizeX() - 1, Wnd.GetSizeY() - 1, ColorScheme[3], ColorScheme[4]);
		else
			Wnd.SetBorder(0, 0, Wnd.GetSizeX() - 1, Wnd.GetSizeY() - 1, ColorScheme[3], ColorScheme[4]);

		if (LastInFocus)
			Wnd.SetColor(ColorScheme[2]);

		Wnd.OutAt(1, Wnd.GetSizeY() / 2, str(Wnd.GetSizeX() - 2, ' '));
		Wnd.OutAt(Wnd.GetSizeX() / 2 - Text.length() / 2, Wnd.GetSizeY() / 2, Text);
	} else {
		if (Pressed)
			Wnd.SetBackgroundColor(ColorScheme[2]);
		else if (LastInFocus)
			Wnd.SetBackgroundColor(ColorScheme[5]);
		else
			Wnd.SetBackgroundColor(ColorScheme[6]);

		Wnd.OutAt(Wnd.GetSizeX() / 2 - Text.length() / 2, Wnd.GetSizeY() / 2, Text);
	};
};

void LUIC::Button::Input(i16 p_Input, MEVENT* p_Event) {
	if (IOH == NULL or not Visible)
		return;

	Click = false;

	switch (p_Input) {
		case KEY_MOUSE: {
			if (p_Event->bstate & BUTTON1_RELEASED) {
				if (Pressed) {
					Pressed = false;
					Click = true;
					InFocus = false;
				};
			} else if (p_Event->bstate & BUTTON1_PRESSED) {
				if (IOH->__gchldfcsed()) return;

				bool PosCheck = p_Event->x >= Wnd.GetPosX() and p_Event->y >= Wnd.GetPosY();
				bool BRCornerCheck = p_Event->x < Wnd.GetSizeX() + Wnd.GetPosX() and p_Event->y < Wnd.GetSizeY() + Wnd.GetPosY();

				if (not (PosCheck and BRCornerCheck)) {
					LastInFocus = false;

					break;
				};

				Pressed = true;
				InFocus = false;
				LastInFocus = true;

				IOH->__schldfcsed(true);
				__fcsprnts();

				if (Parent == NULL)
					IOH->PushBackChild(this);
				else
					Parent->PushBackChild(this);
			};

			break;
		};
	};
};
