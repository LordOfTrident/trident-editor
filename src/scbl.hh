#pragma once

#include <string>
#include <vector>
#include <cstdint>

#define SCBL_TYPE_INTEGER  0
#define SCBL_TYPE_FUNCTION 1
#define SCBL_CMD_END       2

#define SCBL_RUNTIME_OK 0
#define SCBL_PARSER_OK  0

#define SCBL_RUNTIME_ERROR 1
#define SCBL_PARSER_ERROR  1

#define SCBL_CONSTANT_1BYTE  1
#define SCBL_CONSTANT_2BYTES 2
#define SCBL_CONSTANT_4BYTES 4

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;

typedef uint8_t  ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;

namespace SCBL {
	typedef void (*callback)(std::vector <ui8>, void*);

	class ParameterHandler {
	public:
		ParameterHandler(std::vector <ui8> p_Parameters):
			Parameters(p_Parameters),
			Counter(0),
			OutOfParams(false)
		{};

		ui8 GetNextParam8() {
			if ((ui32)Parameters.size() <= Counter) {
				OutOfParams = true;

				return 0;
			};

			return Parameters[Counter ++];
		};

		ui16 GetNextParam16() {
			if ((ui32)Parameters.size() <= Counter) {
				OutOfParams = true;

				return 0;
			};

			std::vector <ui8> Bytes;

			for (ui8 i = 0; i < 2; ++ i) {
				if ((ui32)Parameters.size() <= Counter + i) {
					OutOfParams = true;

					return 0;
				};

				Bytes.push_back(Parameters[Counter + i]);
			};

			Counter += 2;

			return (ui16)((ui32)Bytes[0] << 8) | (ui32)Bytes[1];
		};

		ui32 GetNextParam32() {
			if ((ui32)Parameters.size() <= Counter) {
				OutOfParams = true;

				return 0;
			};

			std::vector <ui8> Bytes;

			for (ui8 i = 0; i < 4; ++ i) {
				if ((ui32)Parameters.size() <= Counter + i) {
					OutOfParams = true;

					return 0;
				};

				Bytes.push_back(Parameters[Counter + i]);
			};

			Counter += 4;

			return ((ui32)Bytes[0] << 24) | ((ui32)Bytes[1] << 16) | ((ui32)Bytes[2] << 8) | (ui32)Bytes[3];
		};

		ui32 GetNextParamInt() {
			if ((ui32)Parameters.size() <= Counter) {
				OutOfParams = true;

				return 0;
			};

			std::vector <ui8> Bytes;
			ui8 Size = 4;

			for (ui8 i = 0; i < 4; ++ i) {
				if ((ui32)Parameters.size() <= Counter + i) {
					Size = i;

					break;
				};

				Bytes.push_back(Parameters[Counter + i]);
			};

			if ((ui32)Bytes.size() == 0) {
				OutOfParams = true;

				return 0;
			};

			Counter += Size;

			switch (Size) {
				case 1: {
					return (ui32)Bytes[0];

					break;
				};

				case 2: {
					return (ui16)((ui32)Bytes[0] << 8) | (ui32)Bytes[1];

					break;
				};

				case 4: default: {
					return ((ui32)Bytes[0] << 24) | ((ui32)Bytes[1] << 16) | ((ui32)Bytes[2] << 8) | (ui32)Bytes[3];

					break;
				};
			};
		};

		std::string GetNextParamStr() {
			if ((ui32)Parameters.size() <= Counter) {
				OutOfParams = true;

				return 0;
			};

			std::string String = "";

			for (ui32 i = 0; i < (ui32)Parameters.size() and Parameters[Counter + i] != 0; ++ i)
				String += Parameters[Counter + i];

			Counter += String.length() + 1;

			return String;
		};

		bool IsOutOfParams() {
			return OutOfParams;
		};

		void ResetCounter() {
			Counter = 0;
		};

	private:
		std::vector <ui8> Parameters;
		ui32 Counter;
		bool OutOfParams;
	};

	class Token {
	public:
		Token(ui8 p_Type, ui8 p_Data):
			Type(p_Type),
			Data(p_Data)
		{};

		ui8 Type;
		ui8 Data;
	};

	class Function {
	public:
		Function(std::string p_Name, callback p_Callback):
			Name(p_Name),
			Callback(p_Callback)
		{};

		std::string Name;
		callback Callback;
	};

	class Constant {
	public:
		Constant(std::string p_Name, ui32 p_Value, ui8 p_Size):
			Name(p_Name),
			Size(p_Size)
		{
			switch (p_Size) {
				case 1: {
					Bytes = {
						(ui8)p_Value
					};

					break;
				};
				case 2: {
					Bytes = {
						(ui8)((p_Value & 0xFF00) >> 8),
						(ui8)(p_Value & 0x00FF)
					};

					break;
				};
				case 4: default: {
					Bytes = {
						(ui8)((p_Value & 0xFF000000) >> 24),
						(ui8)((p_Value & 0x00FF0000) >> 16),
						(ui8)((p_Value & 0x0000FF00) >> 8),
						(ui8)(p_Value & 0x000000FF)
					};

					break;
				};
			};
		};

		std::string Name;
		ui8 Size;
		std::vector <ui8> Bytes;
	};

	class SCBL_Interpreter {
	public:
		SCBL_Interpreter() {};

		i8 Parse(std::string p_Code) {
			ErrorMsg = "";
			Tokens = {};

			if (p_Code == "")
				return SCBL_PARSER_OK;

			std::string token;
			for (i32 i = 0; i < (i32)p_Code.length(); ++ i) {
				token = "";

				while (p_Code[i] <= 32 and p_Code[i] >= 0 and i < (i32)p_Code.length())
					++ i;

				if ((p_Code[i] <= 32 and p_Code[i] >= 0) or i >= (i32)p_Code.length())
					continue;
				else if (p_Code[i] >= '0' and p_Code[i] <= '9') {
					while (p_Code[i] >= '0' and p_Code[i] <= '9') {
						token += p_Code[i];

						++ i;
					};

					ui8 Size = 4;

					if (p_Code[i] == ':') {
						++ i;

						if (not (p_Code[i] == '1' or p_Code[i] == '2' or p_Code[i] == '4')) {
							ErrorMsg = (std::string) "Unexpected bytes count \"" + p_Code[i] + "\"";

							return SCBL_PARSER_ERROR;
						};

						Size = p_Code[i] - 48;

						++ i;
					};

					if (not (p_Code[i] <= 32 and p_Code[i] >= 0) and p_Code[i] != ';' and p_Code[i] != '{') {
						ErrorMsg = (std::string) "Unexpected symbol \"" + p_Code[i] + "\" in number";

						return SCBL_PARSER_ERROR;
					};

					switch (Size) {
						case 1: {
							Tokens.push_back(Token(SCBL_TYPE_INTEGER, (ui8)std::stoi(token)));

							break;
						};

						case 2: {
							ui16 Integer = std::stoi(token);

							Tokens.push_back(Token(SCBL_TYPE_INTEGER, (ui8)((Integer & 0xFF00) >> 8)));
							Tokens.push_back(Token(SCBL_TYPE_INTEGER, (ui8)(Integer & 0x00FF)));

							break;
						};

						case 4: default: {
							ui32 Integer = std::stoi(token);

							Tokens.push_back(Token(SCBL_TYPE_INTEGER, (ui8)((Integer & 0xFF000000) >> 24)));
							Tokens.push_back(Token(SCBL_TYPE_INTEGER, (ui8)((Integer & 0x00FF0000) >> 16)));
							Tokens.push_back(Token(SCBL_TYPE_INTEGER, (ui8)((Integer & 0x0000FF00) >> 8)));
							Tokens.push_back(Token(SCBL_TYPE_INTEGER, (ui8)(Integer & 0x000000FF)));

							break;
						};
					};

					-- i;
				} else if (p_Code[i] == '"') {
					++ i;

					while (p_Code[i] != '"' and not (i > (i32)p_Code.length())) {
						Tokens.push_back(Token(SCBL_TYPE_INTEGER, p_Code[i]));

						++ i;
					};

					if (p_Code[i] != '"') {
						ErrorMsg = "String not closed";

						return SCBL_PARSER_ERROR;
 					};

					++ i;

					if (p_Code[i] != '~')
						Tokens.push_back(Token(SCBL_TYPE_INTEGER, 0));
 					else
						++ i;

					if (not (p_Code[i] <= 32 and p_Code[i] >= 0) and p_Code[i] != ';' and p_Code[i] != '{') {
						ErrorMsg = "Unexpected symbol after string end";

						return SCBL_PARSER_ERROR;
					};

					-- i;
				} else if (p_Code[i] == '{') {
					++ i;

					while (p_Code[i] != '}' and not (i > (i32)p_Code.length()))
						++ i;

					if (p_Code[i] != '}') {
						ErrorMsg = "Comment not closed";

						return SCBL_PARSER_ERROR;
					};
				} else if (p_Code[i] == '#') {
					++ i;

					while (p_Code[i] != 10 and not (i > (i32)p_Code.length()))
						++ i;
				} else if (p_Code[i] == ';')
					Tokens.push_back(Token(SCBL_CMD_END, 0));
				else {
					while (not (p_Code[i] <= 32 and p_Code[i] >= 0) and p_Code[i] != ';' and not (i > (i32)p_Code.length())) {
						token += p_Code[i];

						++ i;
					};

					bool found = false;
					ui32 j;

					for (j = 0; j < Functions.size(); ++ j)
						if (Functions[j].Name == token) {
							found = true;

							break;
						};

					if (found) {
						Tokens.push_back(Token(SCBL_TYPE_FUNCTION, (ui8)((j & 0xFF000000) >> 24)));
						Tokens.push_back(Token(SCBL_TYPE_FUNCTION, (ui8)((j & 0x00FF0000) >> 16)));
						Tokens.push_back(Token(SCBL_TYPE_FUNCTION, (ui8)((j & 0x0000FF00) >> 8)));
						Tokens.push_back(Token(SCBL_TYPE_FUNCTION, (ui8)(j & 0x000000FF)));
					} else {
						found = false;

						for (j = 0; j < Constants.size(); ++ j)
							if (Constants[j].Name == token) {
								found = true;

								break;
							};

						if (found) {
							switch (Constants[j].Size) {
								case 1: {
									Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[j].Bytes[0]));

									break;
								};

								case 2: {
									Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[j].Bytes[0]));
									Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[j].Bytes[1]));

									break;
								};

								case 4: default: {
									Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[j].Bytes[0]));
									Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[j].Bytes[1]));
									Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[j].Bytes[2]));
									Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[j].Bytes[3]));

									break;
								};
							};
						} else {
							ErrorMsg = "Identifier \"" + token + "\" not found";

							return SCBL_PARSER_ERROR;
						};
					};

					-- i;
				};
			};

			return SCBL_PARSER_OK;
		};

		i8 Run()  {
			for (ui32 i = 0; i < (ui32)Tokens.size(); ++ i) {
				if (Tokens[i].Type != SCBL_TYPE_FUNCTION) {
					ErrorMsg = "[" + std::to_string((int)i) + "] Expected function, got (" + (Tokens[i].Type == SCBL_CMD_END? "CMD_END, " : "INT, ") + ".Data:" + std::to_string(Tokens[i].Data) + ")";

					return SCBL_RUNTIME_ERROR;
				};

				if (i + 4 > (ui32)Tokens.size()) {
					ErrorMsg = "Expected 4 function tokens in a row, got less";

					return SCBL_RUNTIME_ERROR;
				};

				std::vector <ui8> Bytes;

				for (ui32 j = 0; j < 4; ++ j) {
					if (Tokens[i + j].Type != SCBL_TYPE_FUNCTION) {
						ErrorMsg = "Expected 4 function tokens in a row, got less";

						return SCBL_RUNTIME_ERROR;
					};

					Bytes.push_back(Tokens[i + j].Data);
				};

				i += 4;

				ui32 FuncIdx = ((ui32)Bytes[0] << 24) | ((ui32)Bytes[1] << 16) | ((ui32)Bytes[2] << 8) | (ui32)Bytes[3];
				std::vector <ui8> Params;

				while (i < (ui32)Tokens.size() and Tokens[i].Type != SCBL_CMD_END) {
					if (Tokens[i].Type != SCBL_TYPE_INTEGER) {
						ErrorMsg = "[" + std::to_string(i) + "] Expected integer, got (" + (Tokens[i].Type == SCBL_CMD_END? "CMD_END, " : "FUNC, ") + ".Data:" + std::to_string(Tokens[i].Data) + ")";

						return SCBL_RUNTIME_ERROR;
					};

					Params.push_back(Tokens[i].Data);

					++ i;
				};

				Functions[FuncIdx].Callback(Params, UserData);
			};

		    return SCBL_RUNTIME_OK;
		};

		void SetUserData(void* p_userdata) {
			UserData = p_userdata;
		};

		void AddFunction(Function p_func) {
			Functions.push_back(p_func);
		};

		void SetFunction(Function p_func) {
			for (ui32 i = 0; i < (ui32)Functions.size(); ++ i) {
				if (Functions[i].Name == p_func.Name) {
					Functions[i] = p_func;

					return;
				};
			};

			Functions.push_back(p_func);
		};

		void AddConstant(Constant p_const) {
			Constants.push_back(p_const);
		};

		void SetConstant(Constant p_const) {
			for (ui32 i = 0; i < (ui32)Constants.size(); ++ i) {
				if (Constants[i].Name == p_const.Name) {
					Constants[i] = p_const;

					return;
				};
			};

			Constants.push_back(p_const);
		};

		std::string GetErrorMsg() {
			return ErrorMsg;
		};

	private:
		std::string ErrorMsg;

		std::vector <Token> Tokens;
		std::vector <Function> Functions;
		std::vector <Constant> Constants;

		void* UserData;
	};
};
