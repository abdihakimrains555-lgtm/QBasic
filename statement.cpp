#include "statement.h"


statement::statement(StatementType type,QString content)
    : type(type) , content(content){}

//静态函数工具，从枚举类型转换到字符串
QString statement::cast(const StatementType& type){
    switch(type){
        case RemStmt: return "REM";
        case LetStmt: return "LET";
        case PrintStmt: return "PRINT";
        case InputStmt: return "INPUT";
        case GotoStmt: return "GOTO";
        case IfStmt: return "IF";
        case EndStmt: return "END";
    }
    throw std::invalid_argument("invalid statement type");
}
