#include "exp.h"

exp::exp() {}
Expression::Expression(){}
Expression::~Expression(){}
int Expression::getConstantValue(){return 0;}
QString Expression::getIdentifierName(){return "";}
QString Expression::getOperator(){return "";}
Expression* Expression::getLHS(){return nullptr;}
Expression* Expression::getRHS(){return nullptr;}
bool Expression::compare(EvaluationContext & context){return true;}

ConstantExp::ConstantExp(int val){
    value = val;
}

int ConstantExp::eval(EvaluationContext & context) {
    return value;
}

QString ConstantExp::toString(){
    return QString::number(value);
}

ExpressionType ConstantExp::type(){
    return CONSTANT;
}

int ConstantExp::getConstantValue(){
    return value;
}




IdentifierExp::IdentifierExp(QString name){
    this->name = name;
}

int IdentifierExp::eval(EvaluationContext & context) {
    if (!context.isDefined(name)){
        std::string error = name.toStdString();
        error += " is undefined";
        throw std::invalid_argument(error);
    }
    return context.getValue(name);
}

QString IdentifierExp::toString(){
    return name;
}

ExpressionType IdentifierExp::type(){
    return IDENTIFIER;
}

QString IdentifierExp::getIdentifierName(){
    return name;
}





CompoundExp::CompoundExp(QString op, Expression *lhs, Expression *rhs){
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

CompoundExp::~CompoundExp(){
    if(lhs != nullptr){
        delete lhs;
        lhs = nullptr;
    }
    if(rhs != nullptr){
        delete rhs;
        rhs = nullptr;
    }
}

//递归计算，返回结果
int CompoundExp::eval(EvaluationContext & context) {
    int right = rhs->eval(context);
    //赋值运算，更新context
    if (op == "=") {
        context.setValue(lhs->getIdentifierName(), right);
        return right;
    }
    int left = lhs->eval(context);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        //除法分母不能为0
        if (right == 0){
            throw std::invalid_argument("Division by 0");
        }
        return left / right;
    }
    if(op == "MOD"){
        //取模模数不能为0
        if (right == 0){
            throw std::invalid_argument("Division by 0");
        }
        int rem = left % right;
        if(rem != 0){
            //确保结果与模数同号
            bool rem_pos = (rem > 0);
            bool right_pos = (right > 0);
            if(rem_pos != right_pos){
                rem += right;
            }
        }
        return rem;
    }
    if(op == "**"){
        //乘方不能出现0的负数次幂
        if(left == 0 && right < 0){
            throw std::invalid_argument("The negative power of zero");
        }
        int res = static_cast<int>(std::pow(left,right));
        return res;
    }
    throw std::invalid_argument("Illegal operator in expression");
    return 0;
}

//专用于if语句的函数，语法树根节点的op表示一个关系运算符，返回这个命题是否成立
bool CompoundExp::compare(EvaluationContext & context){
    int left = lhs->eval(context);
    int right = rhs->eval(context);
    if(op == "="){
        return left == right;
    }else if(op == "<"){
        return left < right;
    }else if(op == ">"){
        return left > right;
    }
    throw std::invalid_argument("Invalid compare operator");
}

QString CompoundExp::toString(){
    return lhs->toString() + op + rhs->toString();
}

ExpressionType CompoundExp::type(){
    return COMPOUND;
}

QString CompoundExp::getOperator(){
    return op;
}

Expression* CompoundExp::getLHS(){
    return lhs;
}

Expression* CompoundExp::getRHS(){
    return rhs;
}



void EvaluationContext::setValue(QString var, int value){
    symbolTable[var] = value;
}

int EvaluationContext::getValue(QString var){
    if (!isDefined(var)){
        std::string error = var.toStdString();
        error += " is undefined";
        throw std::invalid_argument(error);
    }
    return symbolTable[var];
}

bool EvaluationContext::isDefined(QString var){
    return symbolTable.contains(var);
}

void EvaluationContext::clear(){
    symbolTable.clear();
}
