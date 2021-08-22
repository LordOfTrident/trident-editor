#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
	class TextBar: public Component {
	public:
		TextBar();
		TextBar(ui16 p_PosX, ui16 p_PosY, ui16 p_SizeX, flags p_Flags);

		virtual void Draw();
		virtual void Input(i16 p_Input, MEVENT* p_Event);

		void SetText(str p_Text);
		str GetText();

	private:
		str Text;
		bool Pressed;
		ui16 CurPos, ScrollX;
	};
};
