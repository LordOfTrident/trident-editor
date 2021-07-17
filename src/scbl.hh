#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

#define SCBL_TYPE_INTEGER 0
#define SCBL_TYPE_FUNCTION 1
#define SCBL_CMD_END 2

#define SCBL_RUNTIME_OK 0
#define SCBL_PARSER_OK 0
#define SCBL_RUNTIME_ERROR 1
#define SCBL_PARSER_ERROR 1

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;

typedef uint8_t  ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;

namespace SCBL {
    typedef void (*callback)(std::vector <ui32>);
    
    class Token {
    public: Token(ui8 p_Type, ui32 p_Data);

        ui8 Type;
        ui32 Data;
    };

    class Function {
    public: Function(std::string p_Name, callback p_Callback);

        std::string Name;
        callback Callback;
    };

    class Constant {
    public: Constant(std::string p_Name, ui32 p_Value);

        std::string Name;
        ui32 Value;
    };

    class SCBL_Interpreter {
    public: SCBL_Interpreter();

        i8 Parse(std::string p_Code);
        i8 Run();
        void AddFunction(Function p_func);
        void AddConstant(Constant p_const);
        std::string GetErrorMsg();

    private:
        std::string ErrorMsg;

        std::vector <Token> Tokens;
        std::vector <Function> Functions;
        std::vector <Constant> Constants;
    };
};