#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDebug>
// Для чтения файла конфигурации
#include <iostream>
#include <fstream>
// Для работы с строками
#include <string>
#include <regex>
// Old: Переменные для авторизации
// QString User_login;
// QString User_Password;


// Переменная для типа БД
int DB_Login_Type;
// Переменная для адреса конфигурационного файла / файла БД
QString BD_File_Address = "";

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    // Инициализация окна SQL
    SQLWindow = new SQL_Window_Main();
    // Подключение через кнопку в интерфейсе
    connect(SQLWindow, &SQL_Window_Main::firstWindow, this, &LoginWindow::show);
    // Old: Настройка tabBar
    // ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),"PostgreSQL");
    // Инициализация окна файлового браузера
    FirstBrowser = new FirstFileBrowser();
    // Подключение через кнопку в интерфейсе
    connect(FirstBrowser, &FirstFileBrowser::fileWindow, this, &LoginWindow::show);

    // Настройка чек-бокса - чтобы пользователь не менял его состояние
    // Это не то, он наоборот делает 3 возможных состояния ui->checkBox->setTristate();
    //ui->checkBox->setEnabled(false);

    // Настройка lineEdit->adress_now
    ui->lineEdit_adress_now->setReadOnly(1);
    // Настройка цвета
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,Qt::lightGray);
    palette->setColor(QPalette::Text,Qt::darkGray);
    ui->lineEdit_adress_now->setPalette(*palette);
    ui->lineEdit_adress_now->setText("Файл не используется");
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_pushButton_clicked()
{
    // Old
    /*
    // Попытка подключения к базе данных, вариант для использования файла
    std::string File_Line;
    QString InformToAccess;
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
            DB.setUserName(User_login);
            // Считывание пароля пользователя
            DB.setPassword(User_Password);
}
*/

    // Подключение к базе данных PostgreSQL
    if (DB_Login_Type == 0)
    {
        DB_login = QSqlDatabase::addDatabase("QPSQL", "PQSL_Database_Connection");
        //if (BD_File_Address == "")
        // Если не поставлена галочка на запрос из файла
        if (!(ui->checkBox->isChecked()))
        {
            // Настройка адреса хоста
            if((ui->host_input_PSQL->text()).isEmpty())
            {
                DB_login.setHostName("127.0.0.1");
            }
            else
            {
                DB_login.setHostName(ui->host_input_PSQL->text());
            }
            // Настройка рабочего порта
            // DB.setPort(5432);
            if((ui->port_line_PSQL->text()).isEmpty())
            {
                DB_login.setPort(5432);
            }
            else
            {
                DB_login.setPort(ui->port_line_PSQL->text().toInt());
            }
            // Настройка имени базы данных
            // DB.setDatabaseName("NIRS_Test_Database");
            DB_login.setDatabaseName(ui->BD_name_PSQL->text());
            // Настройка имени пользователя
            // DB.setUserName("DatabaseUser");
            DB_login.setUserName(ui->login_line_PSQL->text());
            // Настройка пароля
            // DB.setPassword("adminpassword4");
            DB_login.setPassword(ui->password_input_PSQL->text());
            qDebug() << ui->host_input_PSQL->text() << ui->port_line_PSQL->text()
                     << ui->BD_name_PSQL->text() << ui->login_line_PSQL->text()
                     << ui->password_input_PSQL->text();
        }
        else
        {
            // Запрос из файла
            std::string File_Line;
            QString InformToAccess;
            // Открытие файла
            //std::ifstream PostgresFile("D://NIRS_Projects/PostgreSQL_NIRS_DB_Access.txt");
            std::string File_Address = BD_File_Address.toUtf8().constData();

            // Корректировка адреса файлов
            qDebug() << "Адрес до корректировки - " << File_Address.c_str();
            //std::string test = "abc def abc def";
            //test = std::regex_replace(test, std::regex("def"), "klm"); // replace 'def' -> 'klm'
            File_Address = std::regex_replace(File_Address, std::regex(":/"), "://");
            File_Address.pop_back(); // Убрать последний символ "/"
            qDebug() << "Адрес после корректировки - " << File_Address.c_str();

            std::ifstream PostgresFile(File_Address);
            if (PostgresFile.is_open())
            {
                    // Считывание адреса
                    PostgresFile >> File_Line;
                    File_Line.erase(0, 13);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setHostName(InformToAccess);

                    // Считывание порта
                    PostgresFile >> File_Line;
                    File_Line.erase(0, 13);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setPort(InformToAccess.toInt());

                    // Считывание имени БД
                    PostgresFile >> File_Line;
                    File_Line.erase(0, 17);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setDatabaseName(InformToAccess);

                    // Считывание имени пользователя
                    //DB.setUserName(User_login);
                    PostgresFile >> File_Line;
                    File_Line.erase(0, 18);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setUserName(InformToAccess);

                    // Считывание пароля пользователя
                    // DB.setPassword(User_Password);
                    PostgresFile >> File_Line;
                    File_Line.erase(0, 21);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setPassword(InformToAccess);

                    PostgresFile.close();
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Ошибка открытия файла конфигурации.\n Проверьте правильность выбранного файла");
            }
        }

        if(DB_login.open())
        {
            qDebug("PostgreSQL_Database_is_open");

            // qDebug() << "Временно закрываем связь";
            //DB_login.close();
            //DB_login.removeDatabase("PQSL_Database_Connection");
            SQL_Window_Main().get_DB_data_from_login(DB_login, DB_Login_Type);

            // Иконка и подпись для второго окна
            SQLWindow->setWindowIcon(QIcon("Exe_Icon_1.png"));
            //SQLWindow->setWindowTitle("Main page");
            SQLWindow->setWindowTitle("Основное окно");

            // Переход на второе окно
            SQLWindow->show();
            this->close();
        }
        else
        {
            // Чтобы лучше узнать, в чём ошибка
            qDebug() << DB_login.lastError();
            //QMessageBox::critical(this, "ERROR", "Ошибка подключения к базе данных PostgreSQL");
            QMessageBox::critical(this, "ERROR", "Ошибка подключения к базе данных PostgreSQL\n" + DB_login.lastError().databaseText() + "\n" + DB_login.lastError().driverText());
        }
    }
    if (DB_Login_Type == 1)
    {
        if (!(ui->checkBox->isChecked()))
        {
            DB_login = QSqlDatabase::addDatabase("QMYSQL", "QMySQL_Database_Connection");
            // Настройка адреса хоста
            if((ui->host_input_MySQL->text()).isEmpty())
            {
                DB_login.setHostName("127.0.0.1");
            }
            else
            {
                DB_login.setHostName(ui->host_input_MySQL->text());
            }
            // Настройка рабочего порта - не требуется для MySQL
            // Видимо, всё - таки требуется
            DB_login.setPort(3306);
            // Настройка имени базы данных
            DB_login.setDatabaseName(ui->BD_name_MySQL->text());
            // Настройка имени пользователя
            DB_login.setUserName(ui->login_line_MySQL->text());
            // Настройка пароля
            DB_login.setPassword(ui->password_input_MySQL->text());
            qDebug() << ui->host_input_MySQL->text() << ui->BD_name_MySQL->text()
                     << ui->login_line_MySQL->text() << ui->password_input_MySQL->text();
        }
        else
        {
            DB_login = QSqlDatabase::addDatabase("QMYSQL", "QMySQL_Database_Connection");
            // Запрос из файла
            std::string File_Line;
            QString InformToAccess;
            // Открытие файла
            std::string File_Address = BD_File_Address.toUtf8().constData();
            // Корректировка адреса файлов
            qDebug() << "Адрес до корректировки - " << File_Address.c_str();
            File_Address = std::regex_replace(File_Address, std::regex(":/"), "://");
            File_Address.pop_back(); // Убрать последний символ "/"
            qDebug() << "Адрес после корректировки - " << File_Address.c_str();
            std::ifstream MySQLFile(File_Address);
            if (MySQLFile.is_open())
            {
                    // Считывание адреса
                    MySQLFile >> File_Line;
                    File_Line.erase(0, 13);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setHostName(InformToAccess);

                    // Считывание порта
                    MySQLFile >> File_Line;
                    File_Line.erase(0, 13);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setPort(InformToAccess.toInt());

                    // Считывание имени БД
                    MySQLFile >> File_Line;
                    File_Line.erase(0, 17);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setDatabaseName(InformToAccess);

                    // Считывание имени пользователя
                    MySQLFile >> File_Line;
                    File_Line.erase(0, 18);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setUserName(InformToAccess);

                    // Считывание пароля пользователя
                    MySQLFile >> File_Line;
                    File_Line.erase(0, 21);
                    File_Line.erase(File_Line.find(">"), 1);
                    InformToAccess = QString::fromUtf8(File_Line.c_str());
                    qDebug() << InformToAccess;
                    DB_login.setPassword(InformToAccess);
                    MySQLFile.close();
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Ошибка открытия файла конфигурации.\n Проверьте правильность выбранного файла");
            }
        }
        if(DB_login.open())
        {
            qDebug("MySQL_Database_is_open");
            // Переход на второе окно
            SQLWindow->show();
            this->close();
        }
        else
        {
            // Чтобы лучше узнать, в чём ошибка
            qDebug() << DB_login.lastError();
            QMessageBox::critical(this, "ERROR", "Ошибка подключения к базе данных MySQL\n" + DB_login.lastError().databaseText() + "\n" + DB_login.lastError().driverText());
        }
    }
    if (DB_Login_Type == 2)
    {
        QString InformAccess;
        std::string line;
        DB_login = QSqlDatabase::addDatabase("QODBC");
        // Указание файла для открытия
        //std::ifstream AccessFile("D://NIRS_Projects/Access_access.txt");
        std::string File_Address = BD_File_Address.toUtf8().constData();
        // Корректировка адреса файлов
        qDebug() << "Адрес до корректировки - " << File_Address.c_str();
        //std::string test = "abc def abc def";
        //test = std::regex_replace(test, std::regex("def"), "klm"); // replace 'def' -> 'klm'
        File_Address = std::regex_replace(File_Address, std::regex(":/"), "://");
        File_Address.pop_back(); // Убрать последний символ "/"
        qDebug() << "Адрес после корректировки - " << File_Address.c_str();
        std::ifstream AccessFile(File_Address);
        if (AccessFile.is_open())
        {
            while(!AccessFile.eof())
            {
                AccessFile >> line;
                InformAccess = InformAccess.append(QString::fromUtf8(line.c_str()));
                InformAccess = InformAccess.append(" ");
            }
            qDebug() << InformAccess;
            InformAccess.resize(InformAccess.size() - 1);

        }
        else
        {
            qDebug() << "ERROR: File is not open";
            QMessageBox::critical(this, "ERROR", "Файл не был обнаружен или открыт.\n Проверьте местоположение и свойства файла");
        }
        qDebug() << InformAccess;
        //DB.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C://Users/VArutin/Documents/Database11_Test.accdb");
        DB_login.setDatabaseName(InformAccess);
        // Настройки для личного ПК
        //DB.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=D:/Work/BD_all/Database11.mdb");
        if(DB_login.open())
        {
            qDebug("Подключен Microsoft Access");
            SQL_Window_Main().get_DB_data_from_login(DB_login, DB_Login_Type);
            // Иконка и подпись для второго окна
            SQLWindow->setWindowIcon(QIcon("Exe_Icon_1.png"));
            SQLWindow->setWindowTitle("Основное окно");
            SQLWindow->show();
            this->close();
        }
        else
        {
            qDebug() << DB_login.lastError();
            QMessageBox::critical(this, "ERROR", "Ошибка при открытии базы данных Microsoft Access");
            QMessageBox::critical(this, "ERROR", DB_login.lastError().databaseText() + "\n" + DB_login.lastError().driverText());
        }
    }
}

// Old
/*
//void LoginWindow::on_login_line_editingFinished()
//{
    //QString username = ui->login_line->text();
    //User_login = ui->login_line->text();
    //qDebug() << "Login - " << User_login;
//}


//void LoginWindow::on_password_input_editingFinished()
//{
    //QString password = ui->password_input->text();
    //User_Password = ui->password_input->text();
    //qDebug() << "Password - " << User_Password;
//}
*/


void LoginWindow::on_pushButton_2_clicked()
{
    QCoreApplication::exit();
}


void LoginWindow::on_tabWidget_tabBarClicked(int index)
{
    if (index == 0)
    {
        DB_Login_Type = 0;
        qDebug() << "Current database type - PostgreSQL";
    }
    if (index == 1)
    {
        DB_Login_Type = 1;
        qDebug() << "Current database type - MySQL";
    }
    if (index == 2)
    {
        DB_Login_Type = 2;
        qDebug() << "Current database type - Microsoft Access";
    }
}

void LoginWindow::changeIndex(int i)
{
    i++;
}

void LoginWindow::get_PSQL_config_adress(QString configAdress)
{

    //QString test = A;
    //qDebug() << "Adress - " << test;
    BD_File_Address = configAdress;

    // Установка чек-бокса
    //ui->checkBox->stateChanged(true);
    //ui->checkBox->setChecked(1);
    //ui->checkBox->setCheckable(true);
    ui->checkBox->setEnabled(true);
    //ui->checkBox->setChecked(true);
    //ui->checkBox->checkState();
    //ui->checkBox->clicked(true);
    //ui->checkBox->
    //ui->checkBox->setEnabled(false);

}

void LoginWindow::on_pushButton_PSQL_clicked()
{
    qDebug() << "File browser for PSQL config file opened";
    // Настройка интерфейса файлового браузера
    FirstBrowser->setWindowIcon(QIcon("Exe_Icon_1.png"));
    FirstBrowser->setWindowTitle("Файловый браузер");
    FirstBrowser->show();
}

void LoginWindow::transfer_DB_data(QSqlDatabase DB_Data)
{
    DB_Data = DB_login;
    qDebug() << "Переданные данные - " << DB_Data;
}


void LoginWindow::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked())
    {
        qDebug() << "Был вызван файл конфигурации";
        ui->lineEdit_adress_now->setText(BD_File_Address);
    }
    else
    {
        qDebug() << "Пользователь отказался от использования файла конфигурации";
        ui->lineEdit_adress_now->setText("Файл не используется");
    }

}


void LoginWindow::on_pushButton_MySQL_clicked()
{
    qDebug() << "File browser for MySQL config file opened";
    // Настройка интерфейса файлового браузера
    FirstBrowser->setWindowIcon(QIcon("Exe_Icon_1.png"));
    FirstBrowser->setWindowTitle("Файловый браузер");
    FirstBrowser->show();
}


void LoginWindow::on_pushButton_ODBC_clicked()
{
    qDebug() << "File browser for Microsoft Access config file opened";
    // Настройка интерфейса файлового браузера
    FirstBrowser->setWindowIcon(QIcon("Exe_Icon_1.png"));
    FirstBrowser->setWindowTitle("Файловый браузер");
    FirstBrowser->show();

    /* Полный процесс подключения
    QString InformAccess;
    std::string line;
    QSqlDatabase DB = QSqlDatabase::addDatabase("QODBC");
    // Указание файла для открытия
    std::ifstream AccessFile("D://NIRS_Projects/Access_access.txt");
    if (AccessFile.is_open())
    {
        while(!AccessFile.eof())
        {
            AccessFile >> line;
            InformAccess = InformAccess.append(QString::fromUtf8(line.c_str()));
            InformAccess = InformAccess.append(" ");
        }
        qDebug() << InformAccess;
        InformAccess.resize(InformAccess.size() - 1);

    }
    else
    {
        qDebug() << "ERROR: File is not open";
        QMessageBox::critical(this, "ERROR", "Файл не был обнаружен или открыт.\n Проверьте местоположение и свойства файла");
    }
    qDebug() << InformAccess;
    //DB.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C://Users/VArutin/Documents/Database11_Test.accdb");
    DB.setDatabaseName(InformAccess);
    // Настройки для личного ПК
    //DB.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=D:/Work/BD_all/Database11.mdb");
    if(DB.open())
    {
        qDebug("Подключен Microsoft Access");
        SQLWindow->show();
        this->close();
    }
    else
    {
        qDebug() << DB.lastError();
        QMessageBox::critical(this, "ERROR", "Ошибка при открытии базы данных Microsoft Access");
        QMessageBox::critical(this, "ERROR", DB.lastError().databaseText() + "\n" + DB.lastError().driverText());
    }
    */
}

