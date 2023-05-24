#include "loginwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;

    // Создание иконки приложения
    w.setWindowIcon(QIcon("Exe_Icon_1.png"));
    w.setWindowTitle("Учётные данные пользователя");

    w.show();
    return a.exec();
}
