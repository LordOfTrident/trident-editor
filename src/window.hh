#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "screen.hh"

namespace LUIC {
	class Window {
	public:
		Window();
		Window(i16 p_PosX, i16 p_PosY, ui16 p_SizeX, ui16 p_SizeY);

		void SetPos(i16 p_PosX, i16 p_PosY);
		ui16 GetPosX();
		ui16 GetPosY();

		void SetSize(ui16 p_SizeX, ui16 p_SizeY);
		ui16 GetSizeX();
		ui16 GetSizeY();

		void SetBorder(ui16 p_PosXA, ui16 p_PosYA, ui16 p_PosXB, ui16 p_PosYB, color p_ColorA, color p_ColorB);
		void SetDBBorder(ui16 p_PosXA, ui16 p_PosYA, ui16 p_PosXB, ui16 p_PosYB, color p_ColorA, color p_ColorB);

		void OutAt(ui16 p_PosX, ui16 p_PosY, str p_txt);
		void OutAt(ui16 p_PosX, ui16 p_PosY, str p_txt, color p_Color);
		void OutAt(ui16 p_PosX, ui16 p_PosY, cch* p_ch);
		void OutAt(ui16 p_PosX, ui16 p_PosY, cch* p_ch, color p_Color);

		void SetBackgroundColor(color p_Color);
		void SetColor(color p_Color);
		void SetAttribute(i32 p_Attribute, bool p_On);

		void DrawShadow(color p_Color);
		void Clear();

		void __swsz(ui16 p_SizeX, ui16 p_SizeY);
		void __swps(ui16 p_PosX, ui16 p_PosY);

	private:
		ui16 PosX, PosY, SizeX, SizeY;
	};
};
