#include "frame.hh"

LUIC::Frame::Frame() {};

LUIC::Frame::Frame(str p_Title, ui16 p_PosX, ui16 p_PosY, ui16 p_SizeX, ui16 p_SizeY, flags p_Flags):
	Title(" " + p_Title + " "),
	Pressed(false),
	Close(false),
	Drag(false),
	Resize(false)
{
	Type = LUIC_TYPE_FRAME;
	Flags = p_Flags;

	ColorScheme = {
		__LUIC__SHDWCLR,
		__LUIC__WBACLR,
		__LUIC__WBBCLR,
		__LUIC__WNDGRN,
		__LUIC__SYSCLR,
		__LUIC__WNDCLR
	};

	PosX = p_PosX;
	PosY = p_PosY;

	Wnd = Window(PosX, PosY, p_SizeX, p_SizeY);
};

void LUIC::Frame::SetTitle(str p_Title) {
	Title = " " + p_Title + " ";
};

str LUIC::Frame::GetTitle() {
	return Title.substr(1, Title.length() - 2);
};

void LUIC::Frame::Draw() {
	if (IOH == NULL or not Visible)
		return;

	if (Parent == NULL)
		Wnd.DrawShadow(ColorScheme[0]);

	Wnd.SetBackgroundColor(ColorScheme[5]);

	if (Pressed)
		Wnd.SetDBBorder(0, 0, Wnd.GetSizeX() - 1, Wnd.GetSizeY() - 1, ColorScheme[1], ColorScheme[2]);
	else
		Wnd.SetBorder(0, 0, Wnd.GetSizeX() - 1, Wnd.GetSizeY() - 1, ColorScheme[1], ColorScheme[2]);

	str fTitle = (i32)Title.length() > (Wnd .GetSizeX() - 8)? Title.substr(0, Wnd .GetSizeX() - 11) + "... " : Title;
	Wnd.OutAt(Wnd.GetSizeX() / 2 - fTitle.length() / 2, 0, fTitle);

	if (Flags & LUIC_FLAGS_FRM_CLOSABLE) {
		Wnd.OutAt(Wnd.GetSizeX() - 4, 0, "[ ]", ColorScheme[1]);

		if (Close)
			Wnd.OutAt(Wnd.GetSizeX() - 3, 0, WACS_S7, ColorScheme[3]);
		else {
			Wnd.SetAttribute(A_BLINK, true);
			Wnd.OutAt(Wnd.GetSizeX() - 3, 0, WACS_DIAMOND, ColorScheme[3]);
			Wnd.SetAttribute(A_BLINK, false);
		};
	};

	if (Flags & LUIC_FLAGS_FRM_RESIZABLE) {
		if (Resize)
			Wnd.SetColor(ColorScheme[4]);
		else
			Wnd.SetColor(ColorScheme[5]);

		Wnd.OutAt(Wnd.GetSizeX() - 1, Wnd.GetSizeY() - 1, "+");
	};

	for (ui16 i = 0; i < (ui16)Children.size(); ++ i)
		Children[i]->Draw();
};

void LUIC::Frame::Input(i16 p_Input, MEVENT* p_Event) {
	if (IOH == NULL or not Visible)
		return;

	for (i16 i = (i16)Children.size() - 1; i >= 0; -- i) {
		Component* Child = Children[i];
		Child->Input(p_Input, p_Event);

		if (Child != Children[i])
			-- i;

		bool Scaled = Child->GetFlags() & LUIC_FLAGS_COMPONENT_SCALED;

		if (not Scaled) {
			if (Child->GetSizeX() + 2 > Wnd.GetSizeX())
				Wnd.SetSize(Child->GetSizeX() + 2, Wnd.GetSizeY());

			if (Child->GetSizeY() + 2 > Wnd.GetSizeY())
				Wnd.SetSize(Wnd.GetSizeX(), Child->GetSizeY() + 2);
		};

		if (Child->IsInFocus()) {
			if (IOH->__gchldfcsed ())
				Child->SetFocus(false);
			else
				IOH->__schldfcsed(true);
		};
	};

	switch (p_Input) {
		case KEY_RESIZE: {
			__fxps();

			break;
		};

		case KEY_MOUSE: {
			if (p_Event->bstate & BUTTON1_RELEASED) {
				if (Pressed) {
					Pressed = false;
					InFocus = false;

					if (Flags & LUIC_FLAGS_FRM_DRAGGABLE) {
						if (Parent != NULL)
							SetPos(p_Event->x - Parent->GetPosX() - OffsetX, p_Event->y - Parent->GetPosY() - OffsetY);
						else
							SetPos(p_Event->x - OffsetX, p_Event->y - OffsetY);

						__fxps();
					};
				} else if (Close) {
					Close = false;
					Visible = false;
					InFocus = false;
				} else if (Resize) {
					Resize = false;
					InFocus = false;

					i16 newSizeX = p_Event->x - Wnd.GetPosX() + 1;
					i16 newSizeY = p_Event->y - Wnd.GetPosY() + 1;

					if (newSizeX < 20)
						newSizeX = 20;

					if (newSizeY < 5)
						newSizeY = 5;

					SetSize(newSizeX, newSizeY);
				};
			} else if (p_Event->bstate & BUTTON1_PRESSED) {
				bool PosCheck = p_Event->x >= Wnd.GetPosX() and p_Event->y >= Wnd.GetPosY();
				bool BRCornerCheck = p_Event->x < Wnd.GetSizeX() + Wnd.GetPosX() and p_Event->y < Wnd.GetSizeY() + Wnd.GetPosY();

				if (not (PosCheck and BRCornerCheck)) {
					LastInFocus = false;

					break;
				};

				if (IOH->__gchldfcsed()) return;

				if (Flags & LUIC_FLAGS_FRM_CLOSABLE) {
					if (p_Event->x == Wnd.GetSizeX() + Wnd.GetPosX() - 3 and p_Event->y == Wnd.GetPosY()) {
						Close = true;
						InFocus = true;
						LastInFocus = true;

						break;
					};
				};

				if (Flags & LUIC_FLAGS_FRM_RESIZABLE) {
					if (p_Event->x == Wnd.GetSizeX() + Wnd.GetPosX() - 1 and p_Event->y == Wnd.GetSizeY() + Wnd.GetPosY() - 1) {
						Resize = true;
						InFocus = true;
						LastInFocus = true;

						break;
					};
				};

				Pressed = true;
				InFocus = false;
				LastInFocus = true;

				IOH->__schldfcsed(true);
				__fcsprnts();

				if (Flags & LUIC_FLAGS_FRM_DRAGGABLE) {
					OffsetX = p_Event->x - Wnd.GetPosX();
					OffsetY = p_Event->y - Wnd.GetPosY();
				};

				if (Parent == NULL)
					IOH->PushBackChild(this);
				else
					Parent->PushBackChild(this);
			};

			break;
		};
	};

	if (Parent == NULL)
		if (PosY < 1)
			SetPos(GetPosX(), 1);
};
