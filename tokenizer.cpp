#include "tokenizer.h"

tokenizer::tokenizer() {}

//从原始字符串中提取语句行号、类型、内容
QPair<int,QPair<StatementType,QString>> tokenizer::convert(const QString& input){
    int line;
    StatementType type;
    QString content = "";

    int n = input.size();
    int l = 0;
    int r = input.indexOf(' ',l);
    if(r == -1) r = n;
    QString str = input.mid(l,r - l);
    l = r + 1;

    //尝试提取行号
    bool ok;
    line = str.toInt(&ok);
    if(ok){
        if(line < 0 || line > 1000000){
            throw std::invalid_argument("行号超出范围！");
        }
    }else{
        //未输入行号，指针置零
        line = -1;
        l = 0,r = 0;
    }
    if(l >= n) throw std::invalid_argument("输入格式错误！");

    //尝试提取语句类型
    r = input.indexOf(' ',l);
    if(r == -1) r = n;
    str = input.mid(l,r - l);
    l = r + 1;


    if(line == -1){
        //下面的语句不能输入行号
        if(str == "RUN"){
            type = RUN;
            return {line,{type,content}};
        }
        else if(str == "LOAD"){
            type = LOAD;
            return {line,{type,content}};
        }
        else if(str == "LIST"){
            type = LIST;
            return {line,{type,content}};
        }
        else if(str == "CLEAR"){
            type = CLEAR;
            return {line,{type,content}};
        }
        else if(str == "HELP"){
            type = HELP;
            return {line,{type,content}};
        }
        else if(str == "QUIT"){
            type = QUIT;
            return {line,{type,content}};
        }
    }

    if(str == "END"){
        type = EndStmt;
        return {line,{type,content}};
    }

    //下面的语句一定有content
    if(l >= n) throw std::invalid_argument("输入格式错误！");
    content = input.mid(l,n - l);

    if(str == "REM"){
        type = RemStmt;
        return {line,{type,content}};
    }
    else if(str == "LET"){
        type = LetStmt;
        return {line,{type,content}};
    }
    else if(str == "PRINT"){
        type = PrintStmt;
        return {line,{type,content}};
    }
    else if(str == "INPUT"){
        type = InputStmt;
        return {line,{type,content}};
    }
    else if(str == "GOTO"){
        type = GotoStmt;
        return {line,{type,content}};
    }
    else if(str == "IF"){
        type = IfStmt;
        return {line,{type,content}};
    }
    else if(str == "DELETE"){
        type = DELETE;
        return {line,{type,content}};
    }

    throw std::invalid_argument("未知语句类型！");
}

