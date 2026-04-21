#include "program.h"

program::program() {}

program::~program() {
    clear();
}

//判断字符串是否是行号
bool is_line(QString& str){
    bool ok;
    int res = str.toInt(&ok);
    if(!ok) return false;
    if(res < 0 || res > 1000000) return false;
    return true;
}

//判断字符串是否是数字
bool is_number(QString& str){
    bool ok;
    int res = str.toInt(&ok);
    return ok;
}

//清理数据，释放内存，避免内存泄露
void program::clear(){
    qDeleteAll(trees);
    statements.clear();
    trees.clear();
    implement_cnt.clear();
    if_cnt.clear();
    use_cnt.clear();
    context.clear();
    output = "";
}

//添加一条语句
QPair<bool,QString> program::add_statement(QString& input){
    auto res = tokenizer::convert(input);
    int line = res.first;
    StatementType type = res.second.first;
    QString content = res.second.second;
    bool update = false;

    //非立即执行的指令，需要构建语法树（从而检查表达式合法性），并存储
    //立即执行的指令直接implement，行号传入-1，同时不能存储
    if(line != -1){
        if(statements.contains(line)){
            delete_line(line);
            //throw std::invalid_argument("行号已存在！");
        }
        statements[line] = statement(type,content);
        implement_cnt[line] = 0;
        if(type == IfStmt){
            if_cnt[line] = {0,0};
        }
        Expression* expr = build(type,content);
        if(expr != nullptr) trees[line] = expr;
        //添加成功，需要更新代码段，update为true
        update = true;
    }else{
        implement(type,content);
        return {false,""};
    }

    if(!update) return {false,""};
    QString code = collect_code();
    return {true,code};
}

//判定是否需要建树，是则返回建树后的根节点
Expression* program::build(StatementType& type,QString& content){
    if(type == LetStmt){
        if(content.indexOf("=") == -1){
            throw std::invalid_argument("无效表达式！");
        }
        Expression* expr = parser::build(content);
        return expr;
    }
    if(type == PrintStmt){
        Expression* expr = parser::build(content);
        return expr;
    }
    if(type == IfStmt){
        //if语句较为特殊，需要构建左右两棵语法树，连接到一个根上，这个根的运算符是关系运算符
        int n = content.size();
        int l = 0,m = 0,r = 0;
        QString target = ">=<";
        for(auto& tar : target){
            m = content.indexOf(tar,l);
            if(m != -1) break;
        }
        r = content.indexOf("THEN",l);
        //非法：找不到关系运算符、找不到THEN、THEN后面没有行号
        if(m == -1 || r == -1 || m > r || r + 5 >= n){
            throw std::invalid_argument("无效表达式！");
        }
        QString last = content.mid(r + 5,n - (r + 5));
        //非法：THEN后面不是有效行号
        if(!is_line(last)){
            throw std::invalid_argument("非法行号！");
        }
        QString exp1 = content.mid(l,m - l);
        QString exp2 = content.mid(m + 1,r - (m + 1));
        Expression* left = parser::build(exp1);
        Expression* right = parser::build(exp2);
        Expression* res = new CompoundExp(content[m],left,right);

        return res;
    }
    if(type == GotoStmt){
        if(!is_line(content)){
            throw std::invalid_argument("非法行号！");
        }
        return nullptr;
    }
    return nullptr;
}

//遍历map，按行号升序构造当前代码段，以供ui展示
QString program::collect_code(){
    QString code = "";
    for(auto it = statements.begin();it != statements.end();it++){
        code += QString::number(it.key());
        code += " ";
        code += statement::cast(it.value().type);
        code += " ";
        code += it.value().content;
        code += "\n";
    }
    return code;
}

//执行代码并返回下一次的行号，如果未指定行号返回-1
int program::implement(int line){
    StatementType& type = statements[line].type;
    QString& content = statements[line].content;
    Expression* expr = trees[line];
    implement_cnt[line] += 1;
    if(type == RemStmt){
        return -1;
    }
    if(type == LetStmt){
        expr->eval(context);
        //赋值语句需要统计右侧表达式上每个变量使用次数
        update_use_cnt(expr->getRHS());
        return -1;
    }
    if(type == PrintStmt){
        int res = expr->eval(context);
        //print过程中，语法树上每个变量都使用了一次
        update_use_cnt(expr);
        //计入输出内容
        QString s = QString::number(res);
        output += s;
        output += "\n";
        return -1;
    }
    if(type == InputStmt){
        if(!parser::check(content)){
            throw std::invalid_argument("无效变量名！");
        }
        //发出信号，通知MainWindow处理输入逻辑，等待用户输入
        emit input();
        context.setValue(content,input_number);
        return -1;
    }
    if(type == GotoStmt){
        int next_line = content.toInt();
        if(!statements.count(next_line)){
            throw std::invalid_argument("goto跳转不存在的行号！");
        }
        return next_line;
    }
    if(type == IfStmt){
        int l = content.indexOf("THEN",0);
        l += 5;
        QString temp = content.mid(l,content.size() - l);
        int next_line = temp.toInt();
        if(!statements.count(next_line)){
            throw std::invalid_argument("if目标行号不存在！");
        }
        //if语句中，左右两个语法树上每个变量都使用了一次
        update_use_cnt(expr->getLHS());
        update_use_cnt(expr->getRHS());
        bool ok = expr->compare(context);
        if(ok){
            //跳转
            if_cnt[line].first += 1;
            return next_line;
        }else{
            //不跳转
            if_cnt[line].second += 1;
            return -1;
        }
    }
    if(type == EndStmt){
        return -1;
    }
    throw std::invalid_argument("未知指令！");
}

//递归遍历语法树，计数变量使用次数
void program::update_use_cnt(Expression* root){
    if(root->type() == IDENTIFIER){
        //是变量
        QString name = root->toString();
        use_cnt[name] += 1;
    }
    if(root->type() == COMPOUND){
        if(root->getLHS() != nullptr){
            update_use_cnt(root->getLHS());
        }
        if(root->getRHS() != nullptr){
            update_use_cnt(root->getRHS());
        }
    }
    return;
}

//无行号立即执行的指令
void program::implement(StatementType& type,QString& content){
    Expression* expr = build(type,content);
    if(type == LetStmt){
        expr->eval(context);
        return;
    }
    if(type == InputStmt){
        if(!parser::check(content)){
            throw std::invalid_argument("无效变量名！");
        }
        emit input();
        context.setValue(content,input_number);
        return;
    }
    if(type == PrintStmt){
        //立即执行的PRINT需要发送信号让主窗口更新显示，且参数false表示无需清空历史输出
        int res = expr->eval(context);
        QString s = QString::number(res);
        emit update_output(s,false);
        return;
    }
    if(type == RUN){
        run();
        return;
    }
    if(type == LOAD){
        //先清理数据，再读取
        emit Clear();
        load();
        return;
    }
    if(type == LIST){
        return;
    }
    if(type == CLEAR){
        emit Clear();
        return;
    }
    if(type == HELP){
        emit Help();
        return;
    }
    if(type == QUIT){
        emit Quit();
        return;
    }
    if(type == DELETE){
        if(!is_line(content)){
            throw std::invalid_argument("无效行号");
        }
        int line = content.toInt();
        if(!statements.contains(line)){
            throw std::invalid_argument("行号不存在");
        }
        delete_line(line);
        //重新显示代码段
        QString code = collect_code();
        emit update_code(code);
        return;
    }

    throw std::invalid_argument("未知指令！");
}

void program::delete_line(int line){
    //按行号删除指令
    statements.remove(line);
    if(trees.contains(line)){
        delete trees[line];
        trees.remove(line);
    }
    return;
}

void program::run(){
    //每次执行前，先清理上次执行的数据
    implement_cnt.clear();
    use_cnt.clear();
    for(auto& it : if_cnt){
        it.first = 0;
        it.second = 0;
    }
    tree = "";
    output = "";
    //遍历map，执行逻辑并获取下一个行号
    for(auto it = statements.begin();it != statements.end();){
        int next_line = implement(it.key());
        if(it.value().type == EndStmt) break;
        if(next_line != -1){
            it = statements.lowerBound(next_line);
        }else{
            it++;
        }
    }
    //发送信号，通知MainWindow显示程序输出、语法树
    emit update_output(output,true);
    //打印语法树，存储到tree中
    print_trees();
    emit update_trees(tree);
}

//打印语法树、语句执行次数、变量使用次数，存储到字符串tree中
void program::print_trees(){
    QString tab = "    ";
    for(auto it = statements.begin();it != statements.end();it++){
        QString line = QString::number(it.key());
        QString cnt = QString::number(implement_cnt[it.key()]);
        StatementType type = it.value().type;
        QString content = it.value().content;
        Expression* expr = trees[it.key()];
        if(type == RemStmt){
            tree += line + " REM " + cnt + "\n";
            tree += tab + content + "\n";
        }
        if(type == LetStmt){
            tree += line + " LET ";
            print(expr,0,it.key());
            tree += "\n";
        }
        if(type == PrintStmt){
            tree += line + " PRINT " + cnt;
            print(expr,1,it.key());
            tree += "\n";
        }
        if(type == InputStmt){
            QString cnt1 = QString::number(use_cnt[content]);
            tree += line + " INPUT " + cnt + "\n";
            tree += tab + content + " " + cnt1 + "\n";
        }
        if(type == GotoStmt){
            tree += line + " GOTO " + cnt + "\n";
            tree += tab + content + "\n";
        }
        if(type == IfStmt){
            int l = content.indexOf("THEN",0) + 5;
            QString nline = content.mid(l,content.size() - l);
            QString cnt1 = QString::number(if_cnt[it.key()].first);
            QString cnt2 = QString::number(if_cnt[it.key()].second);
            tree += line + " IF THEN " + cnt1 + " " + cnt2;
            print(expr->getLHS(),1,it.key());
            tree += "\n" + tab + expr->getOperator();
            print(expr->getRHS(),1,it.key());
            tree += "\n" + tab + nline + "\n";
        }
        if(type == EndStmt){
            tree += line + " END " + cnt + "\n";
            //break;
        }

    }
}

//递归打印某条指令的语法树，深度对应空格数量
void program::print(Expression* root,int depth,int line){
    if(depth > 0){
        //注意根节点和语句是同行的
        tree += "\n";
    }
    QString tab = QString(4 * depth,' ');
    QString str;
    if(root->type() == COMPOUND){
        str = root->getOperator();
    }else{
        str = root->toString();
    }
    tree += tab + str;
    if(depth == 0){
        tree += " " + QString::number(implement_cnt[line]);
    }
    if(root->type() == COMPOUND){
        if(root->getLHS() != nullptr){
            print(root->getLHS(),depth + 1,line);
            if(depth == 0 && statements[line].type == LetStmt){
                //LET语句需要打印左值的使用次数
                QString name = root->getLHS()->toString();
                QString cnt = QString::number(use_cnt[name]);
                tree += " " + cnt;
            }
        }
        if(root->getRHS() != nullptr){
            print(root->getRHS(),depth + 1,line);
        }
    }
}

//读取文件中代码
void program::load(){
    QString path = QFileDialog::getOpenFileName();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::invalid_argument("文件打开失败");
        return;
    }
    char buffer[1024]; // 缓冲区
    while (qint64 len = file.readLine(buffer, sizeof(buffer))) {
        QString line = QString::fromUtf8(buffer, len).trimmed();
        if(!line.size()) break;
        add_statement(line);
    }
    file.close();
    QString code = collect_code();
    emit update_code(code);
}
