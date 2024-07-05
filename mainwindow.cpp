#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QRegularExpressionMatchIterator>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Функция для удаления знаков препинания из строки
QString removePunctuation(const QString& input)
{
    QString result;
    for (QChar c : input) {
        if (c.isLetter() || c == ' ' || c == '\n') {
            result += c;
        }
    }
    return result;
}


void MainWindow::on_pushButton_clicked()
{
    QString str=ui->textEdit->toPlainText();
    if(str=="")
    {
        QMessageBox::critical(this,"Error","Empty result!");
        QCoreApplication::exit(1);
    }
    QVector<QString>temp;
    QVector<QString>new_text;
    QTextStream in(&str);
    while (!in.atEnd())//пока не конец файла
    {
        QString line=in.readLine();//читаются строки из файлв
        QString cline = removePunctuation(line); // Удаляем знаки препинания и разбиваем строку на слова
        QRegularExpression rgx("\\w+");
        QRegularExpressionMatchIterator it= rgx.globalMatch(cline);//globalMatch() - возвращает итератор по всем совпадениям регулярного выражения в строке cline
        while (it.hasNext()) //hasNext() используется для проверки наличия следующего элемента в итераторе
        {
            QRegularExpressionMatch word1 = it.next(); // вызываем it.next(), чтобы получить первое совпадение и сохранить его в объект QRegularExpressionMatch
            it.next(); //для перехода к следуюшему элементу
            if(it.hasNext())
            {
                QRegularExpressionMatch word2 = it.next();
                if (word1.capturedLength()==word2.capturedLength())
                {
                    temp.append(word1.captured());//captured()  -  для извлечения захваченного текста совпадения
                    new_text.append(word2.captured());
                }
            }
        }
    }
    for (const auto& s : temp)
    {
        new_text.prepend(s);
    }
    std::sort(new_text.begin(), new_text.end(), [](const QString& a, const QString& b)
         {
             return a.length() > b.length();
         });

    ui->label->setText("Стоящие подряд слова с одинаковым количеством букв");
    ui->textEdit->clear();
    for(int i=0;i<new_text.size();i++)
        ui->textEdit->append(new_text.at(i));
    ui->pushButton->setText("Закрыть");
    connect(ui->pushButton, &QPushButton::clicked, this, &QWidget::close);

}

