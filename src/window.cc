#include "window.hh"

LUIC::Window::Window() {};

LUIC::Window::Window(i16 p_PosX, i16 p_PosY, ui16 p_SizeX, ui16 p_SizeY):
    PosX(p_PosX < 0? 0 : p_PosX),
    PosY(p_PosY < 0? 0 : p_PosY),
    SizeX(p_SizeX),
    SizeY(p_SizeY)
{
    if (SizeX > getmaxx(stdscr)) SizeX = getmaxx(stdscr);
    if (SizeY > getmaxy(stdscr)) SizeY = getmaxy(stdscr);

    if (PosX > getmaxx(stdscr)) PosX = getmaxx(stdscr);
    if (PosY > getmaxy(stdscr)) PosY = getmaxy(stdscr);
};

void LUIC::Window::SetPos(i16 p_PosX, i16 p_PosY) {
    if (p_PosX > getmaxx(stdscr)) p_PosX = getmaxx(stdscr);
    if (p_PosY > getmaxy(stdscr)) p_PosY = getmaxy(stdscr);

    if (p_PosX < 0) p_PosX = 0;
    if (p_PosY < 0) p_PosY = 0;

    PosX = p_PosX;
    PosY = p_PosY;
};

ui16 LUIC::Window::GetPosX() {
    return PosX;
}

ui16 LUIC::Window::GetPosY() {
    return PosY;
};

void  LUIC::Window::SetSize(ui16 p_SizeX, ui16 p_SizeY) {
    SizeX = p_SizeX;
    SizeY = p_SizeY;

    if (SizeX > getmaxx(stdscr)) SizeX = getmaxx(stdscr);
    if (SizeY > getmaxy(stdscr)) SizeY = getmaxy(stdscr);
};

ui16 LUIC::Window::GetSizeX() {
    return SizeX;
};

ui16 LUIC::Window::GetSizeY() {
    return SizeY;
};

void LUIC::Window::SetBorder(ui16 p_PosXA, ui16 p_PosYA, ui16 p_PosXB, ui16 p_PosYB, color p_ColorA, color p_ColorB) {
    color prevclr = Screen::LastColor;

    Screen::LastAttribute = A_ALTCHARSET;
    Screen::LastColor = p_ColorA;

    Screen::OutWrap(p_PosXA, p_PosYA, PosX, PosY, SizeX, SizeY, ACS_ULCORNER);
    Screen::OutWrap(p_PosXA, p_PosYB, PosX, PosY, SizeX, SizeY, ACS_LLCORNER);

    for (ui16 i = p_PosXA + 1; i < p_PosXB; ++ i)
        Screen::OutWrap(i, p_PosYA, PosX, PosY, SizeX, SizeY, ACS_HLINE);

    for (ui16 i = p_PosYA + 1; i < p_PosYB; ++ i)
        Screen::OutWrap(p_PosXA, i, PosX, PosY, SizeX, SizeY, ACS_VLINE);

    Screen::LastColor = p_ColorB;

    Screen::OutWrap(p_PosXB, p_PosYA, PosX, PosY, SizeX, SizeY, ACS_URCORNER);
    Screen::OutWrap(p_PosXB, p_PosYB, PosX, PosY, SizeX, SizeY, ACS_LRCORNER);

    for (ui16 i = p_PosXA + 1; i < p_PosXB; ++ i)
        Screen::OutWrap(i, p_PosYB, PosX, PosY, SizeX, SizeY, ACS_HLINE);

    for (ui16 i = p_PosYA + 1; i < p_PosYB; ++ i)
        Screen::OutWrap(p_PosXB, i, PosX, PosY, SizeX, SizeY, ACS_VLINE);

    Screen::LastAttribute = 0;
    Screen::LastColor = prevclr;
};

void LUIC::Window::SetDBBorder(ui16 p_PosXA, ui16 p_PosYA, ui16 p_PosXB, ui16 p_PosYB, color p_ColorA, color p_ColorB) {
    color prevclr = Screen::LastColor;

    Screen::LastAttribute = A_ALTCHARSET;
    Screen::LastColor = p_ColorA;

    Screen::OutWrap(p_PosXA, p_PosYA, PosX, PosY, SizeX, SizeY, WACS_D_ULCORNER);
    Screen::OutWrap(p_PosXA, p_PosYB, PosX, PosY, SizeX, SizeY, WACS_D_LLCORNER);

    for (ui16 i = p_PosXA + 1; i < p_PosXB; ++ i)
        Screen::OutWrap(i, p_PosYA, PosX, PosY, SizeX, SizeY, WACS_D_HLINE);

    for (ui16 i = p_PosYA + 1; i < p_PosYB; ++ i)
        Screen::OutWrap(p_PosXA, i, PosX, PosY, SizeX, SizeY, WACS_D_VLINE);

    Screen::LastColor = p_ColorB;

    Screen::OutWrap(p_PosXB, p_PosYA, PosX, PosY, SizeX, SizeY, WACS_D_URCORNER);
    Screen::OutWrap(p_PosXB, p_PosYB, PosX, PosY, SizeX, SizeY, WACS_D_LRCORNER);

    for (ui16 i = p_PosXA + 1; i < p_PosXB; ++ i)
        Screen::OutWrap(i, p_PosYB, PosX, PosY, SizeX, SizeY, WACS_D_HLINE);

    for (ui16 i = p_PosYA + 1; i < p_PosYB; ++ i)
        Screen::OutWrap(p_PosXB, i, PosX, PosY, SizeX, SizeY, WACS_D_VLINE);

    Screen::LastAttribute = 0;
    Screen::LastColor = prevclr;
};

void LUIC::Window::OutAt(ui16 p_PosX, ui16 p_PosY, cch* p_ch) {
    Screen::OutWrap(p_PosX, p_PosY, PosX, PosY, SizeX, SizeY, p_ch);
};

void LUIC::Window::OutAt(ui16 p_PosX, ui16 p_PosY, cch* p_ch, color p_Color) {
    color prevclr = Screen::LastColor;

    Screen::LastColor = p_Color;
    Screen::OutWrap(p_PosX, p_PosY, PosX, PosY, SizeX, SizeY, p_ch);

    Screen::LastColor = prevclr;
};

void LUIC::Window::OutAt(ui16 p_PosX, ui16 p_PosY, str p_txt, color p_Color) {
    color prevclr = Screen::LastColor;

    Screen::LastColor = p_Color;
    Screen::OutWrap(p_PosX, p_PosY, PosX, PosY, SizeX, SizeY, p_txt);

    Screen::LastColor = prevclr;
};

void LUIC::Window::OutAt(ui16 p_PosX, ui16 p_PosY, str p_txt) {
    Screen::OutWrap(p_PosX, p_PosY, PosX, PosY, SizeX, SizeY, p_txt);
};

void LUIC::Window::SetBackgroundColor(color p_Color) {
    Screen::LastColor = p_Color;
    Screen::FillBox(PosX, PosY, SizeX, SizeY, ' ');
};

void LUIC::Window::SetColor(color p_Color) {
    Screen::LastColor = p_Color;
};

void LUIC::Window::SetAttribute(i32 p_Attribute, bool p_On) {
    Screen::LastAttribute = p_On? p_Attribute : 0;
};

void LUIC::Window::DrawShadow(color p_Color) {
    if (SizeY > 1) {
        color prevclr = Screen::LastColor;
        Screen::LastColor = p_Color;

        ui16 minPosX = PosX + SizeX;
        ui16 maxPosX = 2;

        ui16 minPosY = PosY + 1;
        ui16 maxPosY = SizeY;

        for (ui16 i = minPosY; i < maxPosY + minPosY; ++ i)
            for (ui16 j = minPosX; j < maxPosX + minPosX; ++ j) {
                if (j >= getmaxx(stdscr)) continue;
                if (i >= getmaxy(stdscr)) continue;

                Screen::Pixels[i][j].Color = p_Color;
            };

        Screen::LastColor = prevclr;
    };

    if (SizeX > 1) {
        color prevclr = Screen::LastColor;

        Screen::LastColor = p_Color;

        ui16 minPosX = PosX + 2;
        ui16 maxPosX = SizeX;

        ui16 minPosY = PosY + SizeY;
        ui16 maxPosY = 1;

        for (ui16 i = minPosY; i < maxPosY + minPosY; ++ i)
            for (ui16 j = minPosX; j < maxPosX + minPosX; ++ j) {
                if (j >= getmaxx(stdscr)) continue;
                if (i >= getmaxy(stdscr)) continue;

                Screen::Pixels[i][j].Color = p_Color;
            };

        Screen::LastColor = prevclr;
    };
};

void LUIC::Window::Clear() {
    Screen::FillBox(PosX, PosY, SizeX, SizeY, ' ');
};

void LUIC::Window::__swsz(ui16 p_SizeX, ui16 p_SizeY) {
    if (p_SizeX > getmaxx(stdscr)) p_SizeX = getmaxx(stdscr);
    if (p_SizeY > getmaxy(stdscr)) p_SizeY = getmaxy(stdscr);
};

void LUIC::Window::__swps(ui16 p_PosX, ui16 p_PosY) {
    if (p_PosX > getmaxx(stdscr)) p_PosX = getmaxx(stdscr);
    if (p_PosY > getmaxy(stdscr)) p_PosY = getmaxy(stdscr);

    if (p_PosX < 0) p_PosX = 0;
    if (p_PosY < 0) p_PosY = 0;
};
