#include "topbar.hh"

#define CTRL(key) ((key) & 0x1f)

str __gcurdt() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), " %d/%m/%Y %X ", &tstruct);

	return buf;
};

LUIC::Option::Option() {};

LUIC::Option::Option(str p_Name, vector <SubOption> p_SubOptions):
	Name(p_Name),
	SubOptions(p_SubOptions)
{};;

LUIC::SubOption::SubOption() {};

LUIC::SubOption::SubOption(str p_Name, str p_kb):
	Name(p_Name),
	Keybind(p_kb)
{
	if (Name.length() > 9) {
		Name = Name.substr(0, 9);
		Name += "... " + Keybind;;
	} else
		Name += str(13 - Name.length(), ' ') + Keybind;
};

LUIC::TopBar::TopBar() {};

LUIC::TopBar::TopBar(str p_Title, vector <Option> p_Options):
	Title(" " + p_Title + " "),
	Pressed(false),
	Over(0),
	SubOver(0),
	Choice(-1),
	SubChoice(0),
	Options(p_Options)
{
	Type = LUIC_TYPE_TOPBAR;

	PosX = 0;
	PosY = 0;

	Wnd = Window(PosX, PosY, 5, 1);
	OWnd = Window(0, 0, 5, 5);

	WalkerSprites[0] = '/';
	WalkerSprites[1] = '-';
	WalkerSprites[2] = '\\';
	WalkerSprites[3] = '|';
	WalkerState = 0;
	Walker = 0;
};

void LUIC::TopBar::SetTitle(str p_Title) {
	Title = " " + p_Title + " ";
};

i8 LUIC::TopBar::GetChoice() {
	return Choice;
};

i8 LUIC::TopBar::GetSubChoice() {
	return SubChoice;
};

void LUIC::TopBar::Draw() {
	if (IOH == NULL or not Visible)
		return;

	if (Parent == NULL) {
		if (IOH->GetWindowSizeX() != Wnd.GetSizeX())
			Wnd.SetSize(IOH->GetWindowSizeX(), 1);
	} else
		if (Parent->GetSizeX() != Wnd.GetSizeX())
			Wnd.SetSize(Parent->GetSizeX(), 1);

	str date = __gcurdt();

	Wnd.SetBackgroundColor(__LUIC__SYSCLR);
	Wnd.SetAttribute(A_BLINK, true);
	Wnd.OutAt(Wnd.GetSizeX() - Title.length(), 0, Title.c_str(), __LUIC__HGHLGHT);
	Wnd.SetAttribute(A_BLINK, false);
	Wnd.OutAt(Wnd.GetSizeX() - Title.length() - date.length(), 0, date.c_str(), __LUIC__COMMON);

	ui16 pos = 0;

	Wnd.SetColor(__LUIC__COMMON);
	for (ui16 i = 0; i < Options.size(); ++ i) {
		if (i == Over and InFocus) {
			Wnd.SetColor(__LUIC__HGHLGHT);
			OWnd.SetPos(pos, 1);
		};

		Wnd.OutAt(pos, 0, " " + Options[i].Name + " ");
		pos += Options[i].Name.length() + 2;

		if (i == Over and InFocus)
			Wnd.SetColor(__LUIC__COMMON);
	};

	if (WalkerState != -1)
		Wnd.OutAt(Walker + pos, 0, str(1, WalkerSprites[WalkerState]), __LUIC__SYSCLR);

	if (InFocus) {
		ui16 szx = 0;

		for (ui16 i = 0; i < Options[Over].SubOptions.size(); ++ i) {
			if (Options[Over].SubOptions[i].Name.length() > szx)
				szx = Options[Over].SubOptions[i].Name.length();
		};

		OWnd.SetSize(szx + 4, Options[Over].SubOptions.size() + 2);
		OWnd.SetBackgroundColor(__LUIC__COMMON);

		for (ui16 i = 0; i < Options[Over].SubOptions.size(); ++ i) {
			if (i == SubOver)
				OWnd.SetColor(__LUIC__HGHLGHT);

			OWnd.OutAt(1, i + 1, " " + Options[Over].SubOptions[i].Name + str (szx - Options[Over].SubOptions[i].Name.length() + 2, ' '));

			if (i == SubOver)
				OWnd.SetColor(__LUIC__COMMON);
		};

		OWnd.DrawShadow(__LUIC__SHDWCLR);
		OWnd.SetBorder(0, 0, OWnd.GetSizeX() - 1, OWnd.GetSizeY() - 1, __LUIC__WBACLR, __LUIC__WBBCLR);
	};

	if (IOH->GetTick() % 8 == 0) {
		++ Walker;
		i32 max = Wnd.GetSizeX() - pos - date.length() - Title.length() - 1;

		if (WalkerState == -1 and max >= 0) WalkerState = 0;
		if (max < 0) WalkerState = -1;
		if (Walker > max) Walker = 0;
	};

	if (IOH->GetTick() % 16 == 0 and WalkerState != -1) {
		++ WalkerState;
		if (WalkerState > 3) WalkerState = 0;
	};
};

void LUIC::TopBar::Input(i16 p_Input, MEVENT* p_Event) {
	if (IOH == NULL or not Visible)
		return;

	Choice = -1;
	SubChoice = -1;

	switch (p_Input) {
		case KEY_MOUSE: {
			if (p_Event->bstate & BUTTON1_RELEASED) {
				if (Pressed) {
					Choice = Over;
					SubChoice = SubOver;
					InFocus = false;
					Pressed = false;
				};
			} else if (p_Event->bstate & BUTTON1_PRESSED) {
				if (p_Event->y == PosY and p_Event->x >= PosX and p_Event->x < PosX + Wnd.GetSizeX()) {
					IOH->__schldfcsed(false);
					InFocus = true;

					ui16 pos = 0;

					for (ui16 i = 0; i < Options.size(); ++ i) {
						if (p_Event->x >= pos and p_Event->x < pos + 2 + (i32)Options[i].Name.length()) {
							Over = i;
							SubOver = 0;

							break;
						};

						pos += Options[i].Name.length() + 2;
					};
				} else if (p_Event->x >= OWnd.GetPosX() and p_Event->x < OWnd.GetPosX() + OWnd.GetSizeX() and p_Event->y < OWnd.GetPosY() + OWnd.GetSizeY()) {
					if (not InFocus)
						break;

					ui16 BRCornerPosX = OWnd.GetPosX() + OWnd.GetSizeX() - 1;
					ui16 BRCornerPosY = OWnd.GetPosY() + OWnd.GetSizeY() - 1;

					bool XCheck = p_Event->x > OWnd.GetPosX() and p_Event->x < BRCornerPosX;
					bool YCheck = p_Event->y > OWnd.GetPosY() and p_Event->y < BRCornerPosY;

					if (XCheck and YCheck) {
						for (ui16 i = 0; i < Options[Over].SubOptions.size(); ++ i) {
							if (p_Event->y == i + OWnd.GetPosY() + 1) {
								SubOver = i;
								Pressed = true;

								break;
							};
						};
					};
				} else
					InFocus = false;
			};

			break;
		};

		case 27: {
			if (IOH->__gchldfcsed())
				break;

			if (InFocus)
				InFocus = false;
			else {
				IOH->__schldfcsed(false);
				InFocus = true;
			};

			break;
		};

		case KEY_RIGHT: {
			if (not InFocus)
				break;

			if (Over == (i16)Options.size() - 1)
				Over = -1;

			++ Over;
			SubOver = 0;

			break;
		};

		case KEY_LEFT: {
			if (not InFocus)
				break;

			if (Over == 0)
				Over = Options.size();

			-- Over;
			SubOver = 0;

			break;
		};

		case KEY_DOWN: {
			if (not InFocus)
				break;

			if (SubOver == (i16)Options[Over].SubOptions.size() - 1)
				SubOver = -1;

			++ SubOver;

			break;
		};

		case KEY_UP: {
			if (not InFocus)
				break;

			if (SubOver == 0)
				SubOver = Options[Over].SubOptions.size();

			-- SubOver;

			break;
		};

		case 10: {
			if (not InFocus)
				break;

			Choice = Over;
			SubChoice = SubOver;
			InFocus = false;

			break;
		};

		default: {
			for (ui16 i = 0; i < Options.size(); ++ i) {
				for (ui16 j = 0; j < Options[i].SubOptions.size(); ++ j) {
					str Keybind = Options[i].SubOptions[j].Keybind;

					if (Keybind.length() >= 6 and Keybind.substr(0, 5) == "CTRL+") {
						if (p_Input == CTRL(Keybind[5])) {
							Over = i;
							SubOver = j;

							Choice = Over;
							SubChoice = SubOver;

							break;
						};
					};
				};
			};

			break;
		};
	};
};
