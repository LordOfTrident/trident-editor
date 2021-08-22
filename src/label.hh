#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
	class Label: public Component {
	public:
		Label();
		Label(str p_Text, ui16 p_PosX, ui16 p_PosY, flags p_Flags);

		virtual void Draw();
		virtual void Input(i16 p_Input, MEVENT* p_Event);

		void SetText(str p_Text);

	private:
		str Text;
	};
};
