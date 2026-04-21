#ifndef PROGRAM_H
#define PROGRAM_H
#include "exp.h"
#include "statement.h"
#include "tokenizer.h"
#include "parser.h"
#include <QString>
#include <QMap>
#include <QMessageBox>
#include <QPair>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>

class program : public QObject
{
    Q_OBJECT
public:
    program();
    ~program();


    QPair<bool,QString> add_statement(QString& input);
    Expression* build(StatementType& type,QString& content);
    QString collect_code();

    int implement(int line);
    void implement(StatementType& type,QString& content);
    void update_use_cnt(Expression* root);
    void delete_line(int line);

    void clear();
    void run();
    void load();

    void print_trees();
    void print(Expression* root,int depth,int line);

signals:
    void input();
    void update_output(QString& str,bool clean);
    void update_trees(QString& str);
    void update_code(QString& str);
    void Clear();
    void Quit();
    void Help();

    //IF语句较为特殊，有左右两个语法树，存储时可以用一个Compound根节点，左右孩子分别连接到两棵语法树
    //dfs时分别遍历左右语法树即可
public:
    QMap<int,statement> statements;
    QMap<int,Expression*> trees;
    QMap<int,int> implement_cnt;
    QMap<int,QPair<int,int>> if_cnt;
    QMap<QString,int> use_cnt;

    EvaluationContext context;
    QString output;
    QString tree;
    int input_number;

    const QString dataPath = "../../save.txt";
};

#endif // PROGRAM_H
