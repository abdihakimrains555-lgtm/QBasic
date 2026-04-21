#include "parser.h"


//静态map，表示每个操作符的优先级
QMap<QString,int> parser::precedence = {
    {"**", 4},
    {"MOD", 3},
    {"*", 3},
    {"/", 3},
    {"+", 2},
    {"-", 2},
    {"=", 1}
};

//静态数组，表示变量名中不应该出现的字符
QList<QString> parser::forbid = {
    "REM","LET","PRINT","INPUT","IF","GOTO","END","RUN","LOAD","LIST","CLEAR","HELP","QUIT"
};


parser::parser() {

}

//静态函数，用于判断一个字符串是不是数字
bool parser::is_number(QString& str){
    if(!str.size()) return false;
    bool ok;
    str.toInt(&ok);
    return ok;
}

//静态函数，用于判断变量名是否合法
bool parser::check(QString& str){
    if(!str.size()) return false;
    if(str[0] >= '0' && str[0] <= '9') return false;
    for(auto& tar : parser::forbid){
        if(str == tar) return false;
    }
    //变量名中也不能有括号、空格，否则容易引起歧义
    if(str.contains('(') || str.contains(')') || str.contains(' ')) return false;
    return true;
}

//抛出异常前释放内存，避免内存泄露
void parser::error(QList<Expression*>& Operands){
    for(auto& ptr : Operands){
        delete ptr;
        ptr = nullptr;
    }
    throw std::invalid_argument("无效表达式！");
}

//静态成员函数，核心工具，用于从字符串中提取表达式，若非法会抛出异常
Expression* parser::build(QString content){
    QString temp;
    //预处理，确保括号前后有空格
    for(auto c : content){
        if(c == '(' || c == ')'){
            temp += ' ';
            temp += c;
            temp += ' ';
        }else{
            temp += c;
        }
    }
    //按空格分隔字符，开双栈构建表达式
    QStringList list = temp.split(" ",Qt::SkipEmptyParts);
    QList<QString> Operators;
    QList<Expression*> Operands;
    //弹出两个操作数进行计算，返回是否成功操作
    auto calc = [&]()->bool{
        //至少存在两个操作数，操作符不能是括号
        if(Operands.size() < 2 || !Operators.size()) return false;
        if(Operators.back() == '(' || Operators.back() == ')') return false;
        int n = Operands.size();
        Expression* expr = new CompoundExp(Operators.back(),Operands[n - 2],Operands[n - 1]);
        Operators.pop_back();
        Operands.pop_back();
        Operands.pop_back();
        Operands.push_back(expr);
        return true;
    };
    for(auto& str : list){
        if(is_number(str)){
            //是常数
            int value = str.toInt();
            if(value < 0 && Operators.size() && Operators.back() != '(' && Operators.back() != '='){
                //根据要求，负数必须加括号，除非是第一个数
                error(Operands);
            }
            Expression* expr = new ConstantExp(value);
            Operands.push_back(expr);
        }else if(!precedence.contains(str) && str != "(" && str != ")"){
            //是变量
            if(!check(str)){
                //变量名非法
                error(Operands);
            }
            Expression* expr = new IdentifierExp(str);
            Operands.push_back(expr);
        }else if(str == '('){
            //左括号直接入栈
            Operators.push_back(str);
        }else if(str == ')'){
            //右括号，一直操作到栈顶为左括号
            while(Operators.size() && Operators.back() != '('){
                if(!calc()){
                    error(Operands);
                }
            }
            if(!Operators.size() || Operators.back() != '('){
                error(Operands);
            }
            //弹出左括号
            Operators.pop_back();
        }else{
            //是运算符，一直操作直到优先级大于栈顶（乘方是大于等于，因为右结合），或者栈顶为左括号
            while(Operators.size() && Operators.back() != '(' && precedence[str] <= precedence[Operators.back()]){
                if(str == "**" && Operators.back() == "**"){
                    //幂运算右结合
                    break;
                }
                if(!calc()){
                    error(Operands);
                }
            }
            //操作符入栈
            Operators.push_back(str);
        }
    }
    //操作剩余的操作符
    while(Operators.size()){
        if(!calc()){
            error(Operands);
        }
    }
    //如果最后剩余不止一个Expression，说明缺操作符
    if(Operands.size() != 1){
        throw std::invalid_argument("无效表达式！");
    }
    return Operands.back();
}
