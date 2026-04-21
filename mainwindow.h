#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "program.h"
#include "parser.h"
#include <QPair>
#include <QEventLoop>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_returnPressed();

    void on_input();
    void on_update_output(QString& str,bool clean);
    void on_update_trees(QString& str);
    void on_update_code(QString& str);
    void on_quit();
    void on_help();

    void on_btnClearCode_clicked();
    void on_btnRunCode_clicked();
    void on_btnLoadCode_clicked();

    void on_clearOutputButton_clicked();

private:
    Ui::MainWindow *ui;
    program* prog;
    QEventLoop* inputLoop;
    bool input_flag;
    const QString helpPath = "../../help.txt";
};
#endif // MAINWINDOW_H
