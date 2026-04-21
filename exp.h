#ifndef EXP_H
#define EXP_H
#include <QString>
#include <QMap>
#include <QDebug>
#include <cmath>

class exp
{
public:
    exp();
};

class EvaluationContext;

/*
 * Type: ExpressionType
 * --------------------
 * This enumerated type is used to differentiate the three different
 * expression types: CONSTANT, IDENTIFIER, and COMPOUND.
 */

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

/*
 * Class: Expression
 * -----------------
 * This class is used to represent a node in an expression tree.
 * Expression itself is an abstract class.  Every Expression object
 * is therefore created using one of the three concrete subclasses:
 * ConstantExp, IdentifierExp, or CompoundExp.
 */

class Expression {

public:

    Expression();
    virtual ~Expression();
    virtual int eval(EvaluationContext & context) = 0;
    virtual QString toString() = 0;
    virtual ExpressionType type() = 0;

    /* Getter methods for convenience */

    virtual int getConstantValue();
    virtual QString getIdentifierName();
    virtual QString getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();

    virtual bool compare(EvaluationContext & context);

};

/*
 * Class: ConstantExp
 * ------------------
 * This subclass represents a constant integer expression.
 */

class ConstantExp: public Expression {

public:

    ConstantExp(int val);

    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual int getConstantValue();

private:

    int value;

};

/*
 * Class: IdentifierExp
 * --------------------
 * This subclass represents a expression corresponding to a variable.
 */

class IdentifierExp: public Expression {

public:

    IdentifierExp(QString name);

    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual QString getIdentifierName();

private:

    QString name;

};

/*
 * Class: CompoundExp
 * ------------------
 * This subclass represents a compound expression.
 */

class CompoundExp: public Expression {

public:

    CompoundExp(QString op, Expression *lhs, Expression *rhs);
    virtual ~CompoundExp();

    virtual int eval(EvaluationContext & context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual bool compare(EvaluationContext & context);

    virtual QString getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();

private:

    QString op;
    Expression *lhs, *rhs;

};

/*
 * Class: EvaluationContext
 * ------------------------
 * This class encapsulates the information that the evaluator needs to
 * know in order to evaluate an expression.
 */

class EvaluationContext {

public:

    void setValue(QString var, int value);
    int getValue(QString var);
    bool isDefined(QString var);
    void clear();

private:

    QMap<QString,int> symbolTable;

};


#endif // EXP_H
