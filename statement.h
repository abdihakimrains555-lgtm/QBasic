#ifndef STATEMENT_H
#define STATEMENT_H
#include "exp.h"
#include <QString>

enum StatementType{
    RemStmt , LetStmt , PrintStmt , InputStmt , GotoStmt , IfStmt , EndStmt ,
    RUN , LOAD , LIST , CLEAR , HELP , QUIT , DELETE
};

class statement
{
public:
    statement(){}
    statement(StatementType type,QString content = "");
    static QString cast(const StatementType& type);

    StatementType type;
    QString content;
};

#endif // STATEMENT_H
