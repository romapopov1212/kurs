#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QFileDialog>
#include "tree.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QTableWidget>
#include<string>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QTextEdit>
#include"showTreeAndHt.h"
#include "showtree.h"
#include <QSpinBox>
#include <cstdlib>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void FileWriting(Tree& tree, const std::string& filename, std::vector<D>& line); // метод считывания из файла
    void ToCode(QString str, int forWhat);
    void ToGroup(QString str, int forWhat);
    void ToDate(QString str, int forWhat);
    void ToCode(QString str);
    void ToGroup(QString str);
    void ToDate(QString str);
    void searchRange(Date& d1, Date& d2, Code& c, QTextEdit*s);
    void writeToFile(QString &filename);
    void saveFile();
private slots:

    void on_readFile_clicked();

    void on_print_Button_clicked();
    void on_print_Button_2_clicked();


   void on_spinSize_valueChanged(int arg1);

   void on_insert_But_clicked();
   void readTextCode();
   void readTextNumber();
   void readTextName();
   void readTextDate();


   void readTextCodeForDel();
   void readTextNumberForDel();
   void readTextNameForDel();
   void readTextDateForDel();
   void on_del_But_clicked();

   void readFDate();
   void readSDate();
   void readCode();
   void on_Search_Button_clicked();



   private:
    Ui::MainWindow *ui;
    Tree tree_;
    HashMap hashMap_;
    QTextEdit* textEdit_;
    QTextEdit* textTree_;
    QSpinBox* spin_;
    QTextEdit *textEditCode_, *text_del_code_, *textSearchFirstDate_;
    QTextEdit *textEditNumber_, *text_del_Number_, *textSearchSecondDate_;
    QTextEdit *textEditName_, *text_del_Name_, *textSearchCode_;
    QTextEdit *textEditDate_, *text_del_date_;
    QTextEdit *search_result_;
    QPushButton * insert_but_ , *del_but_, *search_but_, *write_;
    QString text_Code_, text_Number_, text_Date_, textDelCode_, textDelNumber_, textDelDate_, text_search_date_first_, text_search_date_second_, text_search_code_;
    std::string text_Name_, textDelName_;
    Code code_, code1_, code2_;
    Group group_, group1_, group2_;
    Date date_, date1_, date2_, date3_;
};
#endif // MAINWINDOW_H
