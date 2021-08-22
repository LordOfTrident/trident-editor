#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "classes.hh"
#include "component.hh"
#include "screen.hh"

#define __LUIC__SHDWCLR 247

#define __LUIC__WNDCLR 250
#define __LUIC__WNDDEF 249
#define __LUIC__WNDGRN 245
#define __LUIC__SYSGRN 207
#define __LUIC__BCKGRN 208
#define __LUIC__WBACLR 240
#define __LUIC__WBBCLR 230

#define __LUIC__HGLTCLR 251
#define __LUIC__BTNCLR  252
#define __LUIC__BTNPCLR 246
#define __LUIC__BPRSCLR 253
#define __LUIC__BARCLR  254
#define __LUIC__HGHLGHT 241
#define __LUIC__ABCKCLR 202
#define __LUIC__BCKCLR  255
#define __LUIC__SYSCLR  222
#define __LUIC__COMMON  200
#define __LUIC__TXTCLR  233
#define __LUIC__TXTNFCS 232

#define LUIC_TYPE_FRAME   "frame"
#define LUIC_TYPE_LABEL   "label"
#define LUIC_TYPE_BUTTON  "button"
#define LUIC_TYPE_TOPBAR  "topbar"
#define LUIC_TYPE_TEXTBAR "textbar"
#define LUIC_TYPE_EDITOR  "editor"

namespace LUIC {
	class IOHandle {
	public:
		IOHandle();
		IOHandle(str p_Title, flags p_Flags);

		~IOHandle();

		void Draw();
		i16 Input();

		void AddColor(ui8 p_ID, color p_Background, color p_Foreground);
		void AddChild(Component* p_child);
		i16 GetChild(Component* p_child);
		void RemoveChild(ui16 p_Idx);
		void PushBackChild(Component* p_child);

		ui16 GetWindowSizeX();
		ui16 GetWindowSizeY();

		void InitTheme(bool p_Shadow);

		void SetFlags(flags p_Flags);
		void SetTitle(str p_Title);
		void SetBody(str p_Body);

		word GetTick();

		bool __hasshdw();

		bool __gchldfcsed();
		void __schldfcsed(bool p_Focused);

	private:
		Component* WindowTopBar;

		vector <Component*> Children;

		ui16 WindowSizeX, WindowSizeY;

		bool Shadow, ChildFocused;

		ui16 XPrevious, YPrevious;

		str Title, Body;
		flags Flags;

		word Tick;
	};
};
