#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"

namespace LUIC {
	namespace Screen {
		class Pixel {
		public:
			Pixel();

			color Color;
			i32 Attribute;
			ch Char;
			cch* CChar;
		};

		extern vector <vector <Pixel>> Pixels;
		extern color LastColor;
		extern i32 LastAttribute;

		void Out(ui16 p_PosX, ui16 p_PosY, ch p_Char);
		void Out(ui16 p_PosX, ui16 p_PosY, cch* p_CChar);
		void Out(ui16 p_PosX, ui16 p_PosY, str p_Str);

		void OutWrap(ui16 p_PosX, ui16 p_PosY, ui16 p_MinPosX, ui16 p_MinPosY, ui16 p_MaxPosX, ui16 p_MaxPosY, ch p_Char);
		void OutWrap(ui16 p_PosX, ui16 p_PosY, ui16 p_MinPosX, ui16 p_MinPosY, ui16 p_MaxPosX, ui16 p_MaxPosY, cch* p_CChar);
		void OutWrap(ui16 p_PosX, ui16 p_PosY, ui16 p_MinPosX, ui16 p_MinPosY, ui16 p_MaxPosX, ui16 p_MaxPosY, str p_Str);

		void FillBox(ui16 p_MinPosX, ui16 p_MinPosY, ui16 p_MaxPosX, ui16 p_MaxPosY, ch p_Char);

		void Update();
	};
};
