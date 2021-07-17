#include "./scbl.hh"

SCBL::Token::Token(ui8 p_Type, ui32 p_Data):
	Type(p_Type),
	Data(p_Data) 
{};

SCBL::Function::Function(std::string p_Name, callback p_Callback):
	Name(p_Name),
	Callback(p_Callback) 
{};

SCBL::Constant::Constant(std::string p_Name, ui32 p_Value):
	Name(p_Name),
	Value(p_Value) 
{};

SCBL::SCBL_Interpreter::SCBL_Interpreter() {};

std::string SCBL::SCBL_Interpreter::GetErrorMsg() {
    return ErrorMsg;
};

void SCBL::SCBL_Interpreter::AddFunction(Function p_func) {
    Functions.push_back(p_func);
};

void SCBL::SCBL_Interpreter::AddConstant(Constant p_const) {
    Constants.push_back(p_const);
};

i8 SCBL::SCBL_Interpreter::Parse(std::string p_Code) {
    ErrorMsg = "";
    Tokens = {};

    if (p_Code == "") return SCBL_PARSER_OK;

    std::string token;
    for (i32 i = 0; i < (i32)p_Code.length(); ++ i) {
        token = "";

        while (p_Code[i] <= 32 && p_Code[i] >= 0) ++ i;

        if ((p_Code[i] <= 32 && p_Code[i] >= 0) || i > (i32)p_Code.length()) 
        	continue;
        else if (p_Code[i] >= '0' && p_Code[i] <= '9') {
        	while (p_Code[i] >= '0' && p_Code[i] <= '9') {
                token += p_Code[i];

                ++ i;
            };
            
            if (!(p_Code[i] <= 32 && p_Code[i] >= 0) && p_Code[i] != ';') {
                ErrorMsg = (std::string) "Unexpected symbol \"" + p_Code[i] + "\" in number";

                return SCBL_PARSER_ERROR;
            };

            Tokens.push_back(Token(SCBL_TYPE_INTEGER, std::stoi(token)));

            -- i;
        } else if (p_Code[i] == ';') 
        	Tokens.push_back(Token(SCBL_CMD_END, 0));
        else {
            while (!(p_Code[i] <= 32 && p_Code[i] >= 0) && p_Code[i] != ';' && !(i > (i32)p_Code.length())) {
                token += p_Code[i];
                
                ++ i;
            };

            auto FuncIdx = std::find_if(Functions.begin(), Functions.end(), [token] (const SCBL::Function& p_func) {
                return p_func.Name == token;
            });

            if (FuncIdx != Functions.end()) 
            	Tokens.push_back(Token(SCBL_TYPE_FUNCTION, std::distance(Functions.begin(), FuncIdx)));
            else {
                auto ConstIdx = std::find_if(Constants.begin(), Constants.end(), [token] (const SCBL::Constant& p_const) {
                    return p_const.Name == token;
                });

                if (ConstIdx != Constants.end()) 
                	Tokens.push_back(Token(SCBL_TYPE_INTEGER, Constants[std::distance(Constants.begin(), ConstIdx)].Value));
                else {
                    ErrorMsg = "Identifier \"" + token + "\" not found";

                    return SCBL_PARSER_ERROR;
                };
            };

            -- i;
        };
    };

    return SCBL_PARSER_OK;
};

i8 SCBL::SCBL_Interpreter::Run() {
    for (i32 i = 0; i < (i32)Tokens.size(); ++ i) {
        if (Tokens[i].Type != SCBL_TYPE_FUNCTION) {
            ErrorMsg = "[" + std::to_string((int)i) + "] Expected function, got (" + (Tokens[i].Type == SCBL_CMD_END? "CMD_END, " : "INT, ") + ".Data:" + std::to_string(Tokens[i].Data) + ")";

            return SCBL_RUNTIME_ERROR;
        };

        ui32 FuncIdx = Tokens[i].Data;
        std::vector <ui32> Params;

        ++ i;

        while (i < (i32)Tokens.size() && Tokens[i].Type != SCBL_CMD_END) {
            if (Tokens[i].Type != SCBL_TYPE_INTEGER) {
                ErrorMsg = "[" + std::to_string(i) + "] Expected integer, got (" + (Tokens[i].Type == SCBL_CMD_END? "CMD_END, " : "FUNC, ") + ".Data:" + std::to_string(Tokens[i].Data) + ")";

                return SCBL_RUNTIME_ERROR;
            };

            Params.push_back(Tokens[i].Data);

            ++ i;
        };

        Functions[FuncIdx].Callback(Params);
    };

    return SCBL_RUNTIME_OK;
};