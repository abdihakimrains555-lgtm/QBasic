#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "statement.h"
#include <QString>
#include <QPair>
class tokenizer
{
public:
    tokenizer();

    static QPair<int,QPair<StatementType,QString>> convert(const QString& input);
};

#endif // TOKENIZER_H
