#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

namespace LUIC {
	class SubOption {
	public:
		SubOption();
		SubOption(str p_Name, str p_Keybind);

		str Name, Keybind;
	};

	class Option {
	public:
		Option();
		Option(str p_Name, vector <SubOption> p_SubOptions);

		str Name;
		vector <SubOption> SubOptions;
	};

	class TopBar: public Component {
	public:
		TopBar();
		TopBar(str p_Title, vector <Option> p_Options);

		virtual void Draw();
		virtual void Input(i16 p_Input, MEVENT* p_Event);

		void SetTitle(str p_Title);

		i8 GetChoice();
		i8 GetSubChoice();

	private:
		str Title;
		bool Pressed;
		Window OWnd;

		i16 Over, SubOver, Choice, SubChoice;

		vector <Option> Options;

		ch WalkerSprites[4];
		i16 Walker;
		i8 WalkerState;
	};
};
