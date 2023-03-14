#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
// Подключение окна файлового браузера
#include <firstfilebrowser.h>
// Подключение второго окна
#include <sql_window_main.h>
// Подключение значений для логина и пароля
#include <QSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    // Получение значений адреса из файлового браузера
    void get_PSQL_config_adress(QString A);
    // Передача данных о БД в SQL_Main_Window
    void transfer_DB_data(QSqlDatabase DB_Data);

private slots:
    void on_pushButton_clicked();

    //void on_login_line_editingFinished();

    //void on_password_input_editingFinished();

    void on_pushButton_2_clicked();

    void on_tabWidget_tabBarClicked(int index);

    // Открытие файлового браузера для файла конфигурации PSQL
    void on_pushButton_PSQL_clicked();

    // Вызов чек-бокса для демонстрации, из файла ли будут взяты данные подключения
    void on_checkBox_stateChanged(int arg1);

private:
    Ui::LoginWindow *ui;
    // Инициализация окна файлового браузера
    FirstFileBrowser *FirstBrowser;
    // Инициализация второго окна
    SQL_Window_Main *SQLWindow;
    // Инициализация объекта базы данных
    QSqlDatabase DB_login;
    // Функция для tabBar
    void changeIndex(int index);
};
#endif // LOGINWINDOW_H
