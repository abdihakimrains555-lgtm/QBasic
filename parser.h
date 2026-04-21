#ifndef PARSER_H
#define PARSER_H

#include "exp.h"
#include <QList>
#include <QString>
#include <QMap>
class parser
{
public:
    parser();
    static Expression* build(QString content);
    static bool is_number(QString& str);
    static bool check(QString& str);
    static void error(QList<Expression*>& Operands);
    static QMap<QString,int> precedence;
    static QList<QString> forbid;

};

#endif // PARSER_H
