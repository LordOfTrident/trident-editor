#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
	class Frame: public Component {
	public:
		Frame();
		Frame(str p_Title, ui16 p_PosX, ui16 p_PosY, ui16 p_SizeX, ui16 p_SizeY, flags p_Flags);

		virtual void Draw();
		virtual void Input(i16 p_Input, MEVENT* p_Event);

		void SetTitle(str p_Title);
		str GetTitle();

	private:
		str Title;
		bool Pressed, Close, Drag, Resize;

		ui16 OffsetX, OffsetY;
	};
};
