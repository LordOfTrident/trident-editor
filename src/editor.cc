#include "editor.hh"

LUIC::Editor::Editor() {};

LUIC::Editor::Editor(ui16 p_PosX, ui16 p_PosY, ui16 p_szx, ui16 p_szy, flags p_flags):
	Text(""),
	Pressed(false),
	Modif(false),
	CurRender(true),
	CurPos (0),
	CurX(0),
	CurY(0),
	ScrollX(0),
	ScrollY(0),
	CurOff(0),
	HigherLinePos(0)
{
	Type = LUIC_TYPE_EDITOR;
	Flags = p_flags | LUIC_FLAGS_COMPONENT_SCALED;
	CurOff = 0;

	ColorScheme = {
		__LUIC__SHDWCLR,
		__LUIC__TXTCLR,
		__LUIC__TXTCLR
	};

	PosX = p_PosX;
	PosY = p_PosY;

	Wnd = Window(PosX, PosY, p_szx, p_szy);
};

void LUIC::Editor::SetText(str p_Text) {
	Text = p_Text;

	CurPos = 0;
	ScrollX = 0;
	ScrollY = 0;
};

str LUIC::Editor::GetText() {
	return Text;
};

void LUIC::Editor::SetFT(str p_FT) {
	FT = p_FT;
};

str LUIC::Editor::GetFT() {
	return Text;
};

void LUIC::Editor::SetModif(bool p_Modif) {
	Modif = p_Modif;
};

bool LUIC::Editor::GetModif() {
	return Modif;
};

void LUIC::Editor::SetTitle(str p_Title) {
	Title = " " + p_Title + " ";
};

str LUIC::Editor::GetTitle() {
	return Title.substr(1, Title.length() - 2);
};

void LUIC::Editor::SetTabSize(ui8 p_TabSize) {
	TabSize = p_TabSize;
};

ui8 LUIC::Editor::GetTabSize() {
	return TabSize;
};

void LUIC::Editor::Draw() {
	if (IOH == NULL or !Visible)
		return;

	if (Parent == NULL)
		Wnd.DrawShadow(ColorScheme[0]);

	Wnd.SetBackgroundColor(ColorScheme[2]);

	str fText = __shptxtbuf(Text, ScrollX, ScrollY, GetSizeX() + ScrollX, GetSizeY() + ScrollY);
	ui16 rndrx = 0, rndry = 0;
	bool currendered = false;

	for (ui32 i = 0; i <= (ui32)fText.length(); ++ i) {
		bool curon = false;

		if (rndrx == CurX + CurOff - ScrollX and rndry == CurY - ScrollY and InFocus and not currendered) {
			if (CurRender)
				Wnd.SetAttribute(A_REVERSE, true);

			curon = true;
			currendered = true;
		};

		if (i < fText.length() and fText[i] != 10) {
			Wnd.OutAt(rndrx, rndry, fText[i] > 31 and fText[i] < 127? str(1, fText[i]) : " ");

			++ rndrx;
		} else {
			if (curon)
				Wnd.OutAt(rndrx, rndry, " ");

			if (fText[i] == 10) {
				rndrx = 0;

				++ rndry;
			};
		};

		if (curon)
			Wnd.SetAttribute(A_REVERSE, false);
	};

	if (Parent != NULL) {
		str footer = std::to_string(CurY) + ":" + std::to_string(CurX) + " | Mem: " + std::to_string(Text.length()) + "B | FT: " + FT + " | EN: ASCII";
		footer = footer.substr(0, Parent->GetSizeX() - 2);

		Parent->__gwndacs()->OutAt(1, Parent->GetSizeY () - 1, footer, ColorScheme[1]);

		if (Modif) {
			str fttl = (i32) Title.length() > (Parent->GetSizeX() - 8)? Title.substr(0, Parent->GetSizeX() - 7) : Title;
			Parent->__gwndacs()->OutAt (Wnd.GetSizeX() / 2 - fttl.length() / 2 + 1, 0, "*", ColorScheme[1]);
		};
	};
};

void LUIC::Editor::Input(i16 p_Input, MEVENT* p_Event) {
	if (IOH == NULL or not Visible)
		return;

	if (Flags & LUIC_FLAGS_EDITOR_CURSORBLINK) {
		if (IOH->GetTick() % 64 == 0)
			CurRender = not CurRender;

		if (p_Input > 0 and p_Input <= 127)
			CurRender = true;
	};

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

				CurX = p_Event->x - (Parent == NULL? GetPosX() : Parent->GetPosX()) + ScrollX - 1;
				CurY = p_Event->y - (Parent == NULL? GetPosY() : Parent->GetPosY()) + ScrollY - 1;

				CurPos = 0;
				CurRender = true;
				ui32 cx = 0;
				ui32 cy = 0;

				for (ui32 i = 0; i < (ui32)Text.length(); ++ i) {
					if (CurY == cy)
						break;

					if (Text[i] == 10) {
						++ cy;

						CurPos = i + 1;
					};
				};

				if (CurY != cy)
					CurY = cy;

				for (ui32 i = CurPos; i <= (ui32)Text.length(); ++ i) {
					if (Text[i] == 10 or cx >= CurX or i == (ui32)Text.length()) {
						CurX = cx;
						CurPos += CurX;

						break;
					};

					if (Text[i] == 9) {
						if (cx + 2 >= CurX) {
							CurX = cx;
							continue;
						} else if (cx + 4 >= CurX)
							CurX = cx + 1;
						else
							CurX -= TabSize - 1;
					};

					++ cx;
				};

				IOH->__schldfcsed(false);
				__fcsprnts();
			};

			break;
		};

		case 27: {
			if (not InFocus)
				break;

			InFocus = false;

			break;
		};

		case 10: {
			if (not InFocus)
				break;

			Modif = true;

			Text.insert(CurPos, "\n");
			CurX = 0;

			++ CurY;
			++ CurPos;

			break;
		};

		case 127: case KEY_BACKSPACE: {
			if (not InFocus or CurPos == 0)
				break;

			Modif = true;
			CurRender = true;

			if (Text[CurPos - 1] == 10) {
				ui32 idx = Text.substr(0, CurPos - 1).find_last_of(10);

				if (idx == Text.npos)
					idx = 0;

				CurX = CurPos - idx - 2;
				-- CurY;
			} else
				-- CurX;

			Text.erase(CurPos - 1, 1);

			-- CurPos;

			break;
		};

		case KEY_LEFT: {
			if (not InFocus or CurPos == 0)
				break;

			CurRender = true;

			if (Text[CurPos - 1] == 10) {
				ui32 idx = Text.substr(0, CurPos - 1).find_last_of(10);

				if (idx == Text.npos)
					idx = 0;

				CurX = CurPos - idx - 2;
				-- CurY;
			} else
				-- CurX;

			-- CurPos;

			break;
		};

		case KEY_RIGHT: {
			if (not InFocus or CurPos == Text.length())
				break;

			CurRender = true;

			if (Text[CurPos] == 10) {
				CurX = 0;
				++ CurY;
			} else
				++ CurX;

			++ CurPos;

			break;
		};

		case KEY_UP: {
			if (not InFocus)
				break;

			ui32 cx = CurX;
			CurRender = true;

			-- CurY;
			-- CurPos;

			for (i32 i = CurPos; i >= -1; -- i) {
				if (Text[i] == 10) {
					cx = 0;
					i32 j = 0;
					CurX += CurOff;
					-- CurPos;

					for (i32 i = CurPos; i >= -1; -- i) {
						if (Text[i] == 10 or i == -1)
							break;

						if (Text[HigherLinePos + cx] == 9)
							if (cx < CurX) {
								++ j;
								if (cx + 2 >= CurX)
									CurX = cx;
								else if (cx + 4 >= CurX)
									CurX = cx + 1;
								else
									CurX -= TabSize - 1;
							};

						-- CurPos;
						++ cx;
					};

					FT = std::to_string(HigherLinePos);

					if (CurX > cx)
						CurX = cx;

					CurPos += CurX + 1;

					break;
				};

				if (i == -1) {
					CurX = cx;
					++ CurY;
					++ CurPos;

					break;
				};

				-- CurPos;
				-- cx;
			};

			break;
		};

		case KEY_DOWN: {
			if (not InFocus)
				break;

			ui32 cx = CurX;
			CurRender = true;

			++ CurY;

			for (ui32 i = CurPos; i <= (ui32)Text.length(); ++ i) {
				if (Text[i] == 10) {
					CurX += CurOff;
					cx = 0;
					++ CurPos;

					for (ui32 i = CurPos; i <= (ui32)Text.length(); ++ i) {
						if (Text[i] == 10 or i == (ui32)Text.length() or cx == CurX)
							break;

						if (Text[i] == 9)
							if (cx < CurX) {
								if (cx + 2 >= CurX) {
									CurX = cx;

									break;
								} else if (cx + 4 >= CurX)
									CurX = cx + 1;
								else
									CurX -= TabSize - 1;
							};

						++ CurPos;
						++ cx;
					};

					CurX = cx;

					break;
				};

				if (i == Text.length()) {
					CurX = cx;
					-- CurY;

					break;
				};

				++ CurPos;
				++ cx;
			};

			break;
		};

		default: {
			if (not InFocus or ((p_Input < 32 or p_Input > 126) and p_Input != 9))
				break;

			Modif = true;

			Text.insert(CurPos, str(1, p_Input));

			++ CurX;
			++ CurPos;

			break;
		};
	};

	if ((sword)CurX + (sword)CurOff - (sword)ScrollX < 0)
		ScrollX = CurX + CurOff;
	else if (CurX + CurOff - ScrollX >= (ui32)Wnd.GetSizeX() - 1)
		ScrollX = CurX + CurOff - ((ui32)Wnd.GetSizeX() - 1);

	if ((sword)CurY - (sword)ScrollY < 0)
		ScrollY = CurY;
	else if (CurY - ScrollY >= (ui32)Wnd.GetSizeY() - 1)
		ScrollY = CurY - ((ui32)Wnd.GetSizeY() - 1);
};

str LUIC::Editor::__shptxtbuf(str p_Text, ui32 p_X, ui32 p_Y, ui32 p_szx, ui32 p_szy) {
	str shpdstr = "";
	ui32 PosX = 0, PosY = 0;
	CurOff = 0;
	HigherLinePos = 0;
	ui32 HigherLinePosOff = 0;

	for (ui32 i = 0; i < p_Text.length(); ++ i) {
		if (((PosX >= p_X and PosX < p_szx) or p_Text[i] == 10) and PosY >= p_Y and PosY < p_szy)
			shpdstr += p_Text[i];

		switch (p_Text[i]) {
			case 9: {
				p_Text[i] = 32;
				p_Text.insert(i, str(TabSize - 1, 32));

				if (PosY < CurY)
					HigherLinePosOff += TabSize - 1;

				if (PosY != CurY)
					break;

				if (PosX < CurX + CurOff)
					CurOff += TabSize - 1;

				break;
			};

			case 10: {
				PosX = -1;
				++ PosY;

				if (PosY + 1 == CurY)
					HigherLinePos = i - HigherLinePosOff + 1;

				break;
			};
		};

		++ PosX;
	};

	return shpdstr;
};
