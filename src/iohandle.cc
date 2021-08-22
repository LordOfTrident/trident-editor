#include "iohandle.hh"

LUIC::IOHandle::IOHandle() {};

LUIC::IOHandle::IOHandle(str p_Title, flags p_Flags):
	ChildFocused(false),
	Flags(p_Flags)
{
	setlocale(LC_CTYPE, "");
	printf("\033]0;%s\007", p_Title.c_str());
	initscr();

	raw();
	noecho();
	timeout(-1);
	nodelay(stdscr, true);
	keypad(stdscr, true);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

	start_color();
	use_default_colors();
	mouseinterval(0);

	if (Flags & LUIC_FLAGS_IOH_CBREAK)
		cbreak();

	if (Flags & LUIC_FLAGS_IOH_NOESCDELAY)
		ESCDELAY = 0;

	WindowSizeX = getmaxx(stdscr);
	WindowSizeY = getmaxy(stdscr);

	Screen::Pixels.resize(WindowSizeY);

	for (ui16 i = 0; i < (ui16)Screen::Pixels.size(); ++ i)
		Screen::Pixels[i].resize(WindowSizeX);

	curs_set(0);
};

LUIC::IOHandle::~IOHandle() {
	endwin();
};

void LUIC::IOHandle::SetFlags(flags p_Flags) {
	Flags = p_Flags;
};

void LUIC::IOHandle::SetTitle(str p_Title) {
	Title = p_Title;
};

void LUIC::IOHandle::SetBody(str p_Body) {
	Body = p_Body;
};

ui16 LUIC::IOHandle::GetWindowSizeX() {
	return WindowSizeX;
};

ui16 LUIC::IOHandle::GetWindowSizeY() {
	return WindowSizeY;
};

word LUIC::IOHandle::GetTick() {
	return Tick;
};

void LUIC::IOHandle::Draw() {
	if (Flags & LUIC_FLAGS_IOH_CLEARSCR)
		for (ui16 i = 0; i < (ui16)Screen::Pixels.size(); ++ i)
			for (ui16 j = 0; j < (ui16)Screen::Pixels[i].size(); ++ j) {
				if (Body == "" and Title == "") {
					Screen::Pixels[i][j].Char = ACS_CKBOARD;
					Screen::Pixels[i][j].Attribute = A_ALTCHARSET;
					Screen::Pixels[i][j].Color = __LUIC__ABCKCLR;
				} else {
					Screen::Pixels[i][j].Char = 32;
					Screen::Pixels[i][j].Attribute = 0;
					Screen::Pixels[i][j].Color = __LUIC__BCKCLR;
				};

				Screen::Pixels[i][j].CChar = NULL;
			};

	Screen::LastColor = __LUIC__BCKCLR;
	Screen::LastAttribute = 0;

	if (Title != "") {
		Screen::Out(1, 1, Title);

		Screen::LastAttribute = A_ALTCHARSET;
		Screen::Out(1, 2, str(WindowSizeX - 2, ACS_HLINE));
		Screen::LastAttribute = 0;
	};

	if (Body != "")
		Screen::Out(0, 4, Body);

	for (ui16 i = 0; i < (ui16)Children.size(); ++ i)
		Children[i]->Draw();

	Screen::Update();
};

i16 LUIC::IOHandle::Input() {
	napms(10);

	++ Tick;

	MEVENT evt;
	i16 in = getch();
	ChildFocused = false;

	switch (in) {
		case KEY_MOUSE: {
			getmouse(&evt);

			break;
		};

		case KEY_RESIZE: {
			WindowSizeX = getmaxx (stdscr);
			WindowSizeY = getmaxy (stdscr);

			Screen::Pixels.resize(WindowSizeY);

			for (ui16 i = 0; i < (ui16)Screen::Pixels.size(); ++ i)
				Screen::Pixels[i].resize(WindowSizeX);

			break;
		};
	};

	for (i16 i = (i16)Children.size() - 1; i >= 0; -- i) {
		Component* Child = Children[i];

		bool Scaled = Child->GetFlags() & LUIC_FLAGS_COMPONENT_SCALED;
		bool ScaledX = Child->GetFlags() & LUIC_FLAGS_COMPONENT_SCALEDX;
		bool ScaledY = Child->GetFlags() & LUIC_FLAGS_COMPONENT_SCALEDY;

		if (Scaled or ScaledX)
			if (Child->GetSizeX() + Child->GetPosX() != GetWindowSizeY())
				Child->SetSize((GetWindowSizeX() - Child->GetPosX() < 20)? 20 : GetWindowSizeX() - Child->GetPosX(), Child->GetSizeY());

		if (Scaled or ScaledY)
			if (Child->GetSizeY() + Child->GetPosY() != GetWindowSizeY())
				Child->SetSize(Child->GetSizeX(), (GetWindowSizeY() - Child->GetPosY() < 3)? 3 : GetWindowSizeY() - Child->GetPosY());

		Child->Input(in, &evt);

		if (Child != Children[i])
			-- i;

		if (Child->IsInFocus()) {
			if (ChildFocused)
				Child->SetFocus(false);
			else
				ChildFocused = true;
		};
	};

	if (WindowTopBar != NULL)
		if (Children[Children.size() - 1] != WindowTopBar)
			PushBackChild(WindowTopBar);

	return in;
};

void LUIC::IOHandle::AddColor(ui8 p_id, color p_bg, color p_fg) {
	init_pair(p_id, p_bg, p_fg);
};

void LUIC::IOHandle::AddChild(Component* p_Child) {
	p_Child->SetParent(NULL);
	p_Child->SetIOHandle(this);

	if (p_Child->GetType() == LUIC_TYPE_TOPBAR)
		WindowTopBar = p_Child;

	Children.push_back(p_Child);
};

void LUIC::IOHandle::InitTheme(bool p_Shadow) {
	Shadow = p_Shadow;

	init_pair(__LUIC__SYSCLR, COLOR_BRIGHTWHITE, COLOR_BLUE);

	init_pair(__LUIC__BCKCLR,  COLOR_BRIGHTCYAN,  COLOR_BLUE);
	init_pair(__LUIC__ABCKCLR, COLOR_GREY,        COLOR_BRIGHTBLUE);
	init_pair(__LUIC__BCKCLR,  COLOR_BRIGHTCYAN,  COLOR_BLUE);
	init_pair(__LUIC__SHDWCLR, COLOR_GREY,        COLOR_BLACK);
	init_pair(__LUIC__HGHLGHT, COLOR_BLACK,       COLOR_GREEN);
	init_pair(__LUIC__BTNPCLR, COLOR_BRIGHTWHITE, COLOR_GREEN);

	init_pair(__LUIC__WNDGRN, COLOR_BRIGHTGREEN, COLOR_WHITE);
	init_pair(__LUIC__SYSGRN, COLOR_BRIGHTGREEN, COLOR_BLUE);
	init_pair(__LUIC__BCKGRN, COLOR_BRIGHTGREEN, COLOR_BLACK);
	init_pair(__LUIC__WNDCLR, COLOR_BRIGHTBLUE,  COLOR_WHITE);
	init_pair(__LUIC__WNDDEF, COLOR_BLACK,       COLOR_WHITE);
	init_pair(__LUIC__WBACLR, COLOR_BRIGHTWHITE, COLOR_WHITE);
	init_pair(__LUIC__WBBCLR, COLOR_GREY,        COLOR_WHITE);
	init_pair(__LUIC__BARCLR, COLOR_CYAN,        COLOR_WHITE);
	init_pair(__LUIC__COMMON, COLOR_BLACK,       COLOR_WHITE);

	init_pair(__LUIC__TXTCLR,  COLOR_WHITE, COLOR_BLACK);
	init_pair(__LUIC__TXTNFCS, COLOR_GREY,  COLOR_BLACK);
};

i16 LUIC::IOHandle::GetChild(Component* p_Child) {
	for (ui16 i = 0; i < (ui16)Children.size(); ++ i)
		if (Children[i] == p_Child)
			return i;

	return -1;
};

void LUIC::IOHandle::RemoveChild(ui16 p_idx) {
	Children.erase(Children.begin() + p_idx);
};

void LUIC::IOHandle::PushBackChild(Component* p_Child) {
	RemoveChild(GetChild(p_Child));
	AddChild(p_Child);
};

bool LUIC::IOHandle::__hasshdw() {
	return Shadow;
};

bool LUIC::IOHandle::__gchldfcsed() {
	return ChildFocused;
};

void LUIC::IOHandle::__schldfcsed(bool p_fcsed) {
	ChildFocused = p_fcsed;
};
