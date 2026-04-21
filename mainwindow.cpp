#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    prog = new program();
    inputLoop = new QEventLoop();
    input_flag = false;
    connect(ui->cmdLineEdit,&QLineEdit::returnPressed,this,&MainWindow::on_returnPressed);
    connect(prog,&program::input,this,&MainWindow::on_input);
    connect(prog,&program::update_output,this,&MainWindow::on_update_output);
    connect(prog,&program::update_trees,this,&MainWindow::on_update_trees);
    connect(prog,&program::update_code,this,&MainWindow::on_update_code);
    connect(prog,&program::Clear,this,&MainWindow::on_btnClearCode_clicked);
    connect(prog,&program::Quit,this,&MainWindow::on_quit);
    connect(prog,&program::Help,this,&MainWindow::on_help);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete prog;
    delete inputLoop;
}

//按下回车后，处理命令行输入
void MainWindow::on_returnPressed()
{
    try{
        QString cmd = ui->cmdLineEdit->text();

        if(input_flag){
            //当前命令框针对INPUT执行
            if(cmd.size() < 2){
                ui->cmdLineEdit->setText("? ");
                throw std::invalid_argument("非法格式！");
            }
            QString input = cmd.mid(2,cmd.size() - 2);
            if(!parser::is_number(input)){
                //非法，重新打印？，弹窗提示
                ui->cmdLineEdit->setText("? ");
                throw std::invalid_argument("非法数字！");
            }
            //将输入的数字传给program
            prog->input_number = input.toInt();
            input_flag = false;
            ui->cmdLineEdit->clear();
            //合法后终止阻塞
            ui->btnClearCode->setEnabled(true);
            ui->btnLoadCode->setEnabled(true);
            ui->btnRunCode->setEnabled(true);
            ui->clearOutputButton->setEnabled(true);
            inputLoop->quit();
        }else{
            ui->cmdLineEdit->clear();
            if(cmd.size()){
                //添加语句，获取新的代码段，进行展示
                auto res = prog->add_statement(cmd);
                if(res.first){
                    ui->CodeDisplay->setText(res.second);
                }
            }
        }
    }catch(const std::invalid_argument& e){
        // 异常输入：弹出错误弹窗
        QMessageBox::critical(
            this,
            "错误!",
            QString(e.what()),
            QMessageBox::Ok
            );
        ui->cmdLineEdit->setFocus();
    }
}

void MainWindow::on_input(){
    //进入INPUT模式
    input_flag = true;
    ui->btnClearCode->setEnabled(false);
    ui->btnLoadCode->setEnabled(false);
    ui->btnRunCode->setEnabled(false);
    ui->clearOutputButton->setEnabled(false);
    ui->cmdLineEdit->setText("? ");
    if(inputLoop->isRunning()){
        inputLoop->quit();
    }
    //阻塞，直到输入合法数字
    inputLoop->exec();
}

//接收program的信号，更新程序输出结果，clean表示是否清除历史输出
void MainWindow::on_update_output(QString& str,bool clean){
    QString res = "";
    if(!clean){
        res += ui->textBrowser->toPlainText();
    }
    //处理字符串结尾的换行逻辑，确保行与行直接紧密连接
    bool new_line = false;
    while(res.size() && res.back() == '\n'){
        res.remove(res.size() - 1,1);
        new_line = true;
    }
    if(new_line) res += "\n";
    res += str + "\n";
    ui->textBrowser->setText(res);
}

//接收program的信号，更新语法树
void MainWindow::on_update_trees(QString& str){
    ui->treeDisplay->setText(str);
}

//接收program的信号，更新代码段
void MainWindow::on_update_code(QString& str){
    ui->CodeDisplay->setText(str);
}

//接收program的信号，清理数据，退出程序
void MainWindow::on_quit(){
    on_btnClearCode_clicked();
    QApplication::exit(0);
}

//接收program的信号，显示帮助
void MainWindow::on_help(){
    QString help = "";
    QFile file(helpPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::invalid_argument("文件打开失败：" + file.errorString().toStdString());
        return;
    }

    QTextStream in(&file);
    QString line;
    // 逐行读取（自动处理中文，保留换行符逻辑）
    while (!(line = in.readLine()).isNull()) {
        help += line + "\n";
    }

    QMessageBox::information(
        this,
        "Help",
        help,
        QMessageBox::Close
        );
    ui->cmdLineEdit->setFocus();
}

//按下clear按钮，清理数据
void MainWindow::on_btnClearCode_clicked()
{
    ui->CodeDisplay->clear();
    ui->treeDisplay->clear();
    ui->textBrowser->clear();
    prog->clear();
}

//按下run按钮，让program运行程序
void MainWindow::on_btnRunCode_clicked()
{
    try{
        prog->run();
    }catch(const std::invalid_argument& e){
        QMessageBox::critical(
            this,
            "错误!",
            QString(e.what()),
            QMessageBox::Ok
            );
        ui->cmdLineEdit->setFocus();
    }
}

//按下load按钮，让program读取文件中的代码
void MainWindow::on_btnLoadCode_clicked()
{
    QString path = QFileDialog::getOpenFileName();
    QFile file(path);
    try{
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::invalid_argument("文件打开失败");
            return;
        }
    }catch(const std::invalid_argument& e){
        QMessageBox::critical(
            this,
            "错误!",
            QString(e.what()),
            QMessageBox::Ok
            );

        ui->cmdLineEdit->setFocus();
        return;
    }
    on_btnClearCode_clicked();
    char buffer[1024]; // 缓冲区
    while (qint64 len = file.readLine(buffer, sizeof(buffer))) {
        QString line = QString::fromUtf8(buffer, len).trimmed();
        if(!line.size()) break;
        try{
            prog->add_statement(line);
        }catch(const std::invalid_argument& e){
            QMessageBox::critical(
                this,
                "错误!",
                QString(e.what()),
                QMessageBox::Ok
                );

            ui->cmdLineEdit->setFocus();
        }
    }
    file.close();
    QString code = prog->collect_code();
    on_update_code(code);
}

//额外：按下程序输出下的clear按钮，清空输出
void MainWindow::on_clearOutputButton_clicked()
{
    ui->textBrowser->clear();
}

