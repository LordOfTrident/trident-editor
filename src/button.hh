#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
	class Button: public Component {
	public:
		Button();
		Button(str p_Text, ui16 p_PosX, ui16 p_PosY, ui16 p_SizeX, ui16 p_SizeY, flags p_Flags);

		virtual void Draw();
		virtual void Input(i16 p_Input, MEVENT* p_Event);

		void SetText(str p_Text);
		bool IsClicked();

	private:
		str Text;
		bool Pressed, Click;
	};
};
