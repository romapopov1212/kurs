// осталось сделать удаление(проверка входных данных), поиск, вывод в файл, так же нужно добавить проверку входных данных
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyleFactory>
#include <QFileDialog>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), hashMap_(4)
{
    ui->setupUi(this);
    //  темный стиль
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);

    ////
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Код направления", "Группа", "Дисциплина", "Дата"});
    ////
    textEdit_ = ui->textEdit;  // Make sure textEdit is properly set in the UI file
    textTree_ = ui->TextTree;

    spin_ = ui->spinSize;
    spin_->setMinimum(2);

    textEditCode_ = ui->textCode;
    textEditNumber_ = ui->Insert_textNomer;
    textEditName_ = ui->Insert_textName;
    textEditDate_ = ui->Insert_textDate;
    insert_but_ = ui->insert_But;


    text_del_code_ = ui->TextDelCode;
    text_del_date_ = ui->TextDelDate;
    text_del_Name_ = ui->TextDelName;
    text_del_Number_ = ui->TextDelNomer;
    del_but_ = ui->del_But;

    write_ = ui->ExpotrButtom;
    textSearchFirstDate_ = ui->Search_FBut;
    textSearchSecondDate_ = ui->Search_SBut;
    textSearchCode_ = ui->Search_KodBut;
    search_but_ = ui->Search_Button;
    search_result_ = ui->Text_result_searching;

    connect(insert_but_, &QPushButton::clicked, this,&MainWindow:: readTextCode);
    connect(insert_but_, &QPushButton::clicked, this,&MainWindow:: readTextNumber);
    connect(insert_but_, &QPushButton::clicked, this,&MainWindow:: readTextName);
    connect(insert_but_, &QPushButton::clicked, this,&MainWindow:: readTextDate);



    connect(del_but_, &QPushButton::clicked, this, &MainWindow::readTextCodeForDel);
    connect(del_but_, &QPushButton::clicked, this, &MainWindow::readTextNumberForDel);
    connect(del_but_, &QPushButton::clicked, this, &MainWindow::readTextNameForDel);
    connect(del_but_, &QPushButton::clicked, this, &MainWindow::readTextDateForDel);

    connect(search_but_, &QPushButton::clicked, this, &MainWindow::readSDate);
    connect(search_but_, &QPushButton::clicked, this, &MainWindow::readFDate);
    connect(search_but_, &QPushButton::clicked, this, &MainWindow::readCode);
    connect(write_, &QPushButton::clicked, this, &MainWindow::saveFile);
}

MainWindow::~MainWindow() {delete ui;}



void MainWindow::FileWriting(Tree& tree, const std::string& filename, vector<D>& lines){
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string code_str, group_str, name_pred, date_str;
        if (!(iss >> code_str >> group_str >> name_pred >> date_str)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }

        if (code_str.size() != 8) {
            std::cerr << "Invalid code format: " << code_str << std::endl;
            continue;
        }

        Code code;
        code.setFirst(std::stoi(code_str.substr(0, 2)));
        code.setSecond(std::stoi(code_str.substr(3, 2)));
        code.setThird(std::stoi(code_str.substr(6, 2)));
        // if(code.getSecond()>3||code.getSecond()<0){

        // }
        Group group;
        group.setLetter(group_str.substr(0, 1));
        group.setNumbers(std::stoi(group_str.substr(1)));

        Date date;
        if (date_str.size() != 10) {
            std::cerr << "Invalid date format: " << date_str << std::endl;
            continue;
        }

        date.setDay(std::stoi(date_str.substr(0, 2)));
        date.setMonth(std::stoi(date_str.substr(3, 2)));
        date.setYear(std::stoi(date_str.substr(6, 4)));

        // qDebug()<<code.getFirst()<<" "<<code.getSecond();
        // qDebug()<<group.getLetter();
        tree.insert(code, group, name_pred, date);
        D d;
        d.setCode(code);
        d.setGroup(group);
        d.setDis(name_pred);
        d.setDate(date);
        lines.push_back(d);
        //std::cout << "Successfully read and inserted line: " << line << std::endl;
    }

    file.close();
}

void MainWindow::on_readFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Все файлы (*.*);;Текстовые файлы (*.txt)");
    if (fileName.isEmpty())
        return;


    std::vector<D>lines;
    FileWriting(tree_, fileName.toStdString(), lines);

    // Clear the table before populating
    ui->tableWidget->setRowCount(0);

    // Populate the table with data from the tree
    int row = 0;
    for (auto entry:lines) {  // Assuming tree is iterable
        ui->tableWidget->insertRow(row);


        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(entry.getCode().toString())));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(entry.getGroup().ToString())));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(entry.getDiscipline())));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(entry.getDate().toString())));




    }
}


void MainWindow::on_print_Button_clicked()
{
    textEdit_->clear();

    TreeVisualizer visualizer(&tree_, textEdit_);
    visualizer.print();
}




void MainWindow::on_print_Button_2_clicked()
{
    textTree_->clear();
    TreeVisualizerS vis(&tree_, textTree_);
    vis.print();

}


void MainWindow::on_spinSize_valueChanged(int arg1) {tree_.setSizeTable(arg1);}

void MainWindow::ToCode(QString str, int forWhat)
{
    qDebug() << "Input string:" << str;
    // Split the QString by '.' to extract the three parts
    QStringList parts = str.split('.');
    if (parts.size() != 3) {
        qDebug()<<"invalid";
        return;
    }
    UInt first = parts[0].toUInt();
    UInt second = parts[1].toUInt();
    UInt third = parts[2].toUInt();

    if(forWhat == 1){ // for insert
        code_.setFirst(first);
        code_.setSecond(second);
        code_.setThird(third);
    }
    else if(forWhat == 2){ // for remove
        code1_.setFirst(first);
        code1_.setSecond(second);
        code1_.setThird(third);
    }else if (forWhat == 3){ // for search
        code2_.setFirst(first);
        code2_.setSecond(second);
        code2_.setThird(third);
    }


}

void MainWindow::ToGroup(QString str, int forWhat)
{
    // Предполагаем формат строки str как "A123", где A - это буква, а 123 - числовая часть

    // Извлечение буквенной части (предполагаем, что это первый символ строки)
    Str letter = str.left(1).toStdString(); // Извлекаем первый символ как строку

    // Извлечение числовой части (предполагаем, что она идет после буквы)
    Str numbersStr = str.mid(1).toStdString(); // Извлекаем подстроку, начиная с индекса 1

    // Преобразование числовой строки в беззнаковое целое число (UInt)
    UInt numbers = static_cast<UInt>(std::stoi(numbersStr));
    if(forWhat == 1){ // for insert
        group_.setLetter(letter);
        group_.setNumbers(numbers);
    }
    else if(forWhat == 2){ // for remove
        group1_.setLetter(letter);
        group1_.setNumbers(numbers);
    }else if (forWhat == 3){ // for search
        group2_.setLetter(letter);
        group2_.setNumbers(numbers);
    }

}

void MainWindow::ToDate(QString str, int forWhat)
{
    // Предполагаем формат строки str как "01.02.2023", где день, месяц и год разделены точками

    // Разделение строки по точкам для извлечения трех частей
    QStringList parts = str.split('.');

    // Убедимся, что у нас есть ровно три части
    if (parts.size() != 3) {
        // Обработка ошибки, выброс исключения или возврат объекта по умолчанию
        // Для простоты возвращаем пустой объект Date
        ///
    }

    // Преобразование строковых частей в беззнаковые целые числа (UInt)
    UInt day = parts[0].toUInt();
    UInt month = parts[1].toUInt();
    UInt year = parts[2].toUInt();

    if(forWhat == 1){ // for insert
        date_.setDay(day);
        date_.setMonth(month);
        date_.setYear(year);
    }
    else if(forWhat == 2){ // for remove
        date1_.setDay(day);
        date1_.setMonth(month);
        date1_.setYear(year);
    }else if (forWhat == 3){ // for search
        date2_.setDay(day);
        date2_.setMonth(month);
        date2_.setYear(year);
    }else if (forWhat == 4){ // for search2
        date3_.setDay(day);
        date3_.setMonth(month);
        date3_.setYear(year);
    }



}

void MainWindow::searchRange(Date &d1, Date &d2, Code &c, QTextEdit* s)
{
    s->clear();
    QString line;
    int count;
    QString codeF;
    QString codeS;
    QString codeT;
    std::vector<Node*> res;
    vector<Leaf*> n = tree_.searchByCodeAndDateRange(c, d1, d2, res, count);

    for(auto it: res){

        if(it->getCode().getFirst()<10){
            codeF = "0"+ QString::number(it->getCode().getFirst());
        }
        else codeF = QString::number(it->getCode().getFirst());
        if(it->getCode().getSecond()<10) {
            codeS = "0"+ QString::number(it->getCode().getSecond());
        }
        else codeS = QString::number(it->getCode().getSecond());
        if(it->getCode().getThird() < 10){
            codeT = "0"+ QString::number(it->getCode().getThird());
        }
        else codeT = QString::number(it->getCode().getThird());

        line+= codeF
                +"." + codeS
                +"."+ codeT
                + " "+ QString::fromStdString(it->getGroup().getLetter())
                + QString::number( it->getGroup().getNumbers())
                + " " + QString::fromStdString(it->getDiscipline());


        s->append(line);
        line = "";
    }

    line = "";
    s->append(line);
    s->append(line);
    line += "count steps: " + QString::number(count);
    s->append(line);
}




void MainWindow::readTextCode() {text_Code_ = textEditCode_->toPlainText();}
void MainWindow::readTextNumber() {text_Number_ = textEditNumber_->toPlainText();}

void MainWindow::readTextName() {text_Name_ = textEditName_->toPlainText().toStdString();}
void MainWindow::readTextDate() {text_Date_ = textEditDate_->toPlainText();}


void MainWindow::readTextCodeForDel() {textDelCode_ = text_del_code_->toPlainText();}
void MainWindow::readTextNumberForDel() {textDelNumber_ = text_del_Number_->toPlainText();}
void MainWindow::readTextNameForDel() {textDelName_ = text_del_Name_->toPlainText().toStdString();}
void MainWindow::readTextDateForDel() {textDelDate_ = text_del_date_->toPlainText();}



void MainWindow::on_insert_But_clicked()
{
    readTextCode();
    readTextNumber();
    readTextName();
    readTextDate();


    if (!text_Code_.isEmpty() && !text_Number_.isEmpty() && !text_Name_.empty() && !text_Date_.isEmpty()) {
        ToCode(text_Code_, 1);
        ToGroup(text_Number_, 1);
        ToDate(text_Date_, 1);
        if (code_.getFirst() <0 || code_.getFirst() > 99 ||
            code_.getSecond() < 0 || code_.getSecond() > 99 ||
            code_.getThird() < 0 || code_.getThird() > 99) {
            QMessageBox::critical(this, "Error", "Invalid code format!");
            return;
        }
        bool isG = group_.isValid();
        // Validate Group format
        if (isG == false) {
            QMessageBox::critical(this, "Error", "Invalid group format!");
            return;
        }
        bool isD = date_.is_Date();
        // Validate Date format
        if (isD == false) {
            QMessageBox::critical(this, "Error", "Invalid date format!");
            return;
        }

        tree_.insert(code_, group_, text_Name_, date_);
    } else {
        QMessageBox::critical(this,"Error" ,"One or more input fields are empty!");
    }


}

void MainWindow::on_del_But_clicked()
{
    readTextCodeForDel();
    readTextDateForDel();
    readTextNameForDel();
    readTextNumberForDel();
    qDebug()<<textDelCode_<<" "<<textDelNumber_<<" "<<textDelName_<<" "<<textDelDate_;
    if (!textDelCode_.isEmpty() && !textDelNumber_.isEmpty() && !textDelName_.empty() && !textDelDate_.isEmpty()) {
        ToCode(textDelCode_, 2);
        ToGroup(textDelNumber_, 2);
        ToDate(textDelDate_, 2);

        if (code1_.getFirst() <0 || code1_.getFirst() > 99 ||
            code1_.getSecond() < 0 || code1_.getSecond() > 99 ||
            code1_.getThird() < 0 || code1_.getThird() > 99) {
            QMessageBox::critical(this, "Error", "Invalid code format!");
            return;
        }
        // Str n = " ";
        // if(textDelName_.){
        //     QMessageBox::critical(this, "Error", "if you have two and more words, please use '_'");
        // }
        bool isG =group1_ .isValid();
        // Validate Group format
        if (isG == false) {
            QMessageBox::critical(this, "Error", "Invalid group format!");
            return;
        }
        bool isD = date1_.is_Date();
        // Validate Date format
        if (isD == false) {
            QMessageBox::critical(this, "Error", "Invalid date format!");
            return;
        }
        qDebug()<<QString::fromStdString(textDelName_);
        qDebug()<<QString::number(code1_.getFirst());
        qDebug()<< QString::number(date1_.getDay());
        qDebug()<< QString::fromStdString(group1_.getLetter());
        qDebug()<< QString::number(group1_.getNumbers());
        bool isDel = tree_.remove(code1_, group1_, textDelName_, date1_);
        if(isDel == true){
            QMessageBox msgBox;
            msgBox.setWindowTitle("Success");
            msgBox.setText("Успешно!");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        }
        else{
            QMessageBox::critical(this,"Error" ,"В дереве нет такого элемента");
        }
    } else {
        QMessageBox::critical(this,"Error" ,"One or more input fields are empty!");

    }
}

void MainWindow::readFDate()
{
    text_search_date_first_ = textSearchFirstDate_->toPlainText();
}

void MainWindow::readSDate()
{
    text_search_date_second_ = textSearchSecondDate_->toPlainText();
}

void MainWindow::readCode()
{
    text_search_code_ = textSearchCode_->toPlainText();
}


void MainWindow::on_Search_Button_clicked()
{
    readFDate();
    readSDate();
    readCode();
    if (!text_search_date_first_.isEmpty() && !text_search_date_second_.isEmpty() && !text_search_code_.isEmpty()) {
        ToDate(text_search_date_first_, 3);
        ToDate(text_search_date_second_, 4);
        ToCode(text_search_code_, 3);
        if (code2_.getFirst() <0 || code2_.getFirst() > 99 ||
            code2_.getSecond() < 0 || code2_.getSecond() > 99 ||
            code2_.getThird() < 0 || code2_.getThird() > 99) {
            QMessageBox::critical(this, "Error", "Invalid code format!");
            return;
        }

        bool isD1 = date2_.is_Date();
        bool isD2 = date3_.is_Date();
        // Validate Date format
        if (isD1 == false || isD2 == false) {
            QMessageBox::critical(this, "Error", "Invalid date format!");
            return;
        }

        else
        searchRange(date2_, date3_ ,code2_ ,search_result_ );
    } else {
        QMessageBox::critical(this,"Error" ,"One or more input fields are empty!");
    }
}



void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("Текстовые файлы (*.txt);;Все файлы (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи");
        return;
    }

    QTextStream out(&file);

    // Сохраняем данные из tableWidget в файл
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QStringList rowData;
        for (int column = 0; column < ui->tableWidget->columnCount(); ++column) {
            QTableWidgetItem *item = ui->tableWidget->item(row, column);
            if (item) {
                rowData << item->text();
            } else {
                rowData << ""; // Если ячейка пустая
            }
        }
        out << rowData.join(" ") << "\n";
    }

    file.close();
    ui->statusbar->showMessage(tr("Файл успешно сохранен"), 5000);
}


