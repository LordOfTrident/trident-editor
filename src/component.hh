#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "classes.hh"
#include "iohandle.hh"

namespace LUIC {
	class Component {
	public:
		Component();

		virtual void Draw() = 0;
		virtual void Input(i16 p_Input, MEVENT* p_Event) = 0;

		void SetParent(Component* p_Parent);
		void SetIOHandle(IOHandle* p_IOH);

		Component* GetParent();
		IOHandle* GetIOHandle();

		void AddChild(Component* p_Child);
		i16 GetChild(Component* p_Child);
		void RemoveChild(ui16 p_Idx);
		void PushBackChild(Component* p_Child);

		void SetFocus(bool p_Focus);
		bool IsInFocus();

		void ToTop();

		void SetVisible(bool p_Visible);
		bool GetVisible();

		void SetPos(i16 p_PosX, i16 p_PosY);
		ui16 GetPosX();
		ui16 GetPosY();

		void SetSize(i16 p_SizeX, i16 p_SizeY);
		ui16 GetSizeX();
		ui16 GetSizeY();

		void SetColorscheme(colors p_ColorScheme);
		colors GetColorscheme();

		str GetType();

		void SetFlags(flags p_Flags);
		flags GetFlags();

		bool IsLastInFocus();

		void SetInset(bool p_On);
		bool GetInset();

		void __sttlps(ui16 p_PosX, ui16 p_PosY);
		ui16 __gttlpsx();
		ui16 __gttlpsy();

		void __fcsprnts();
		void __fxps();

		Window* __gwndacs();

	protected:
		str Type;

		vector <Component*> Children;

		IOHandle* IOH;
		Component* Parent;
		Window Wnd;

		ui16 PosX, PosY;
		colors ColorScheme;
		bool InFocus, Visible, LastInFocus, Inset;
		flags Flags;
	};
};
