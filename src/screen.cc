#include "screen.hh"

vector <vector <LUIC::Screen::Pixel>> LUIC::Screen::Pixels = {};
color LUIC::Screen::LastColor = 0;
i32 LUIC::Screen::LastAttribute = 0;

LUIC::Screen::Pixel::Pixel() {};

void LUIC::Screen::Out(ui16 p_PosX, ui16 p_PosY, ch p_Char) {
	if (p_PosX >= getmaxx(stdscr) or p_PosY >= getmaxy(stdscr))
		return;

	if (p_Char < 32)
		p_Char = 32;

	Screen::Pixels[p_PosY][p_PosX].Char = p_Char;
	Screen::Pixels[p_PosY][p_PosX].CChar = NULL;
	Screen::Pixels[p_PosY][p_PosX].Color = Screen::LastColor;
	Screen::Pixels[p_PosY][p_PosX].Attribute = Screen::LastAttribute;
};

void LUIC::Screen::Out(ui16 p_PosX, ui16 p_PosY, cch* p_CChar) {
	if (p_PosX >= getmaxx(stdscr) or p_PosY >= getmaxy(stdscr))
		return;

	Screen::Pixels[p_PosY][p_PosX].CChar = p_CChar;
	Screen::Pixels[p_PosY][p_PosX].Color = Screen::LastColor;
	Screen::Pixels[p_PosY][p_PosX].Attribute = Screen::LastAttribute;
};

void LUIC::Screen::Out(ui16 p_PosX, ui16 p_PosY, str p_Str) {
	for (ui16 i = 0; i < p_Str.length(); ++ i) {
		if (p_PosX > getmaxx(stdscr))
			return;

		if (p_Str[i] == 10) {
			++ p_PosY;

			p_PosX = -1;
		};

		if (p_PosY >= getmaxy(stdscr))
			return;

		if (not (p_Str[i] == 10)) {
			Screen::Pixels[p_PosY][p_PosX].Char = p_Str[i];
			Screen::Pixels[p_PosY][p_PosX].CChar = NULL;
			Screen::Pixels[p_PosY][p_PosX].Color = Screen::LastColor;
			Screen::Pixels[p_PosY][p_PosX].Attribute = Screen::LastAttribute;
		};

		++ p_PosX;
	};
};

void LUIC::Screen::OutWrap(ui16 p_PosX, ui16 p_PosY, ui16 p_MinPosX, ui16 p_MinPosY, ui16 p_MaxPosX, ui16 p_MaxPosY, ch p_Char) {
	if (p_PosX + p_MinPosX >= getmaxx(stdscr) or p_PosY + p_MinPosY >= getmaxy(stdscr))
		return;

	Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].Char = p_Char;
	Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].CChar = NULL;
	Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].Color = Screen::LastColor;
	Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].Attribute = Screen::LastAttribute;
};

void LUIC::Screen::OutWrap(ui16 p_PosX, ui16 p_PosY, ui16 p_MinPosX, ui16 p_MinPosY, ui16 p_MaxPosX, ui16 p_MaxPosY, cch* p_CChar) {
	if (p_PosX + p_MinPosX >= getmaxx(stdscr) or p_PosY + p_MinPosY >= getmaxy(stdscr))
		return;

	Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].CChar = p_CChar;
	Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].Color = Screen::LastColor;
	Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].Attribute = Screen::LastAttribute;
};

void LUIC::Screen::OutWrap(ui16 p_PosX, ui16 p_PosY, ui16 p_MinPosX, ui16 p_MinPosY, ui16 p_MaxPosX, ui16 p_MaxPosY, str p_Str) {
	for (ui16 i = 0; i < p_Str.length(); ++ i) {
		if (p_PosX > p_MaxPosX)
			return;

		if (p_PosY > p_MaxPosY)
			return;

		if (p_Str[i] == 10) {
			++ p_PosY;

			p_PosX = -1;
		};

		if (p_PosY + p_MinPosY >= getmaxy(stdscr))
			return;

		if (not (p_PosX + p_MinPosX >= getmaxx(stdscr)) and not (p_Str[i] == 10)) {
			Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].Char = p_Str[i];
			Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].CChar = NULL;
			Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].Color = Screen::LastColor;
			Screen::Pixels[p_PosY + p_MinPosY][p_PosX + p_MinPosX].Attribute = Screen::LastAttribute;
		};

		++ p_PosX;
	};
};

void LUIC::Screen::Update() {
	move(0, 0);

	i32 prevattr = 0;

	for (ui16 i = 0; i < (ui16)Screen::Pixels.size(); ++ i)
		for (ui16 j = 0; j < (ui16)Screen::Pixels[i].size(); ++ j) {
			if (prevattr != Screen::Pixels[i][j].Attribute) {
				attroff(prevattr);

				prevattr = Screen::Pixels[i][j].Attribute;
				attron(prevattr);
			};

			attron(COLOR_PAIR(Screen::Pixels[i][j].Color));

			if (Screen::Pixels[i][j].Char < 32)
				Screen::Pixels[i][j].Char = 32;

			if (Screen::Pixels[i][j].CChar == NULL)
				addch(Screen::Pixels[i][j].Char);
			else
				add_wch(Screen::Pixels[i][j].CChar);
		};

	attroff(prevattr);

	refresh();
};

void LUIC::Screen::FillBox(ui16 p_MinPosX, ui16 p_MinPosY, ui16 p_MaxPosX, ui16 p_MaxPosY, ch p_Char) {
	for (ui16 i = p_MinPosY; i < p_MaxPosY + p_MinPosY; ++ i)
		for (ui16 j = p_MinPosX; j < p_MaxPosX + p_MinPosX; ++ j)
			Out(j, i, p_Char);
};
