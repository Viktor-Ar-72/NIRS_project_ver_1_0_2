#include "loginwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;

    // Создание иконки приложения
    //ui->centralwidget->setWindowIcon()
    //QIcon Exe_Icon = new QIcon("Exe_Icon_1.png");
    //ui->centralwidget->setWindowIcon(QIcon("Exe_Icon_1.png"));
    //ui->centralwidget->setWindowTitle("Main EXE Window");
    //ui->layoutWidget->setWindowTitle("Hello");
    //QWidget UW;
    w.setWindowIcon(QIcon("Exe_Icon_1.png"));
    //w.setWindowTitle("Login access to databases");
    w.setWindowTitle("Учётные данные пользователя");

    w.show();
    return a.exec();
}
