#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDebug>
// Для чтения файла конфигурации
#include <iostream>
#include <fstream>

QString User_login;
QString User_Password;

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    // Инициализация окна SQL
    SQLWindow = new SQL_Window_Main();
    // Подключение через кнопку в интерфейсе
    connect(SQLWindow, &SQL_Window_Main::firstWindow, this, &LoginWindow::show);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_pushButton_clicked()
{
    std::string File_Line;
    QString InformToAccess;
    // Попытка подключения к базе данных, вариант для использования файла
    DB = QSqlDatabase::addDatabase("QPSQL");
    std::ifstream PostgresFile("D://NIRS_Projects/PostgreSQL_NIRS_DB_Access.txt");
    if (PostgresFile.is_open())
    {
            // Считывание адреса
            PostgresFile >> File_Line;
            File_Line.erase(0, 13);
            File_Line.erase(File_Line.find(">"), 1);
            InformToAccess = QString::fromUtf8(File_Line.c_str());
            qDebug() << InformToAccess;
            DB.setHostName(InformToAccess);

            // Считывание порта
            PostgresFile >> File_Line;
            File_Line.erase(0, 13);
            File_Line.erase(File_Line.find(">"), 1);
            InformToAccess = QString::fromUtf8(File_Line.c_str());
            qDebug() << InformToAccess;
            DB.setPort(InformToAccess.toInt());

            // Считывание имени БД
            PostgresFile >> File_Line;
            File_Line.erase(0, 17);
            File_Line.erase(File_Line.find(">"), 1);
            InformToAccess = QString::fromUtf8(File_Line.c_str());
            qDebug() << InformToAccess;
            DB.setDatabaseName(InformToAccess);

            // Считывание имени пользователя
            /*
            PostgresFile >> File_Line;
            File_Line.erase(0, 18);
            File_Line.erase(File_Line.find(">"), 1);
            InformToAccess = QString::fromUtf8(File_Line.c_str());
            qDebug() << InformToAccess;
            DB.setUserName(InformToAccess);
            */
            DB.setUserName(User_login);

            // Считывание пароля пользователя
            /*
            PostgresFile >> File_Line;
            File_Line.erase(0,21);
            File_Line.erase(File_Line.find(">"), 1);
            InformToAccess = QString::fromUtf8(File_Line.c_str());
            qDebug() << InformToAccess;
            DB.setPassword(InformToAccess);
            */
            DB.setPassword(User_Password);
}
    if(DB.open())
    {
        qDebug("PostgreSQL_Database_is_open");
        // Переход на второе окно
        SQLWindow->show();
        this->close();
    }
    else
    {
        // Чтобы лучше узнать, в чём ошибка
        qDebug() << DB.lastError();
        //QMessageBox::critical(this, "ERROR", "Ошибка подключения к базе данных PostgreSQL");
        QMessageBox::critical(this, "ERROR", "Ошибка подключения к PostgreSQL\n" + DB.lastError().databaseText() + "\n" + DB.lastError().driverText());
    }
}


void LoginWindow::on_login_line_editingFinished()
{
    //QString username = ui->login_line->text();
    User_login = ui->login_line->text();
    qDebug() << "Login - " << User_login;
}


void LoginWindow::on_password_input_editingFinished()
{
    //QString password = ui->password_input->text();
    User_Password = ui->password_input->text();
    qDebug() << "Password - " << User_Password;
}


void LoginWindow::on_pushButton_2_clicked()
{
    QCoreApplication::exit();
}

