#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
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

private slots:
    void on_pushButton_clicked();

    void on_login_line_editingFinished();

    void on_password_input_editingFinished();

    void on_pushButton_2_clicked();

private:
    Ui::LoginWindow *ui;
    // Инициализация второго окна
    SQL_Window_Main *SQLWindow;
    // Инициализация объекта базы данных
    QSqlDatabase DB;
};
#endif // LOGINWINDOW_H
