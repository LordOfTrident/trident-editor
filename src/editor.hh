#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
    class Editor: public Component {
    public:
        Editor();
        Editor(ui16 p_posx, ui16 p_posy, ui16 p_SizeX, ui16 p_SizeY, flags p_flags);

        virtual void Draw();
        virtual void Input(i16 p_in, MEVENT* p_evt);

        void SetText(str p_Text);
        str GetText();

        void SetFT(str p_FT);
        str GetFT();

        void SetModif(bool p_Modif);
        bool GetModif();

        void SetTitle(str p_ttl);
        str GetTitle();

        void SetTabSize(ui8 p_tabsize);
        ui8 GetTabSize();

        str __shptxtbuf(str p_Text, ui32 p_X, ui32 p_Y, ui32 p_SizeX, ui32 p_SizeY);

    private:
        str Text, FT, Title;

        bool Pressed, Modif, CurRender;

        ui32 CurPos, CurX, CurY, ScrollX, ScrollY, CurOff, HigherLinePos;
        ui8 TabSize;
    };
};
