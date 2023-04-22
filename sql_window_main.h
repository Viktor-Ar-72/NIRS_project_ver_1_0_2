#ifndef SQL_WINDOW_MAIN_H
#define SQL_WINDOW_MAIN_H

#include <QMainWindow>
// Подключение окна для вставки значений
#include <dialog_sql_insert.h>
// Подключение окна для удаления значений
#include <dialog_sql_delete.h>
// Подключение окна для обновления значений
#include <dialog_sql_update.h>
// Для работы c базами данных
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSql>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QMessageBox>
// Для работы с записями и полями таблиц
#include <QSqlRecord>
#include <QSqlField>
// Для работы с оформлением
#include <QIcon>
#include <QWidget>

namespace Ui {
class SQL_Window_Main;
}

class SQL_Window_Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit SQL_Window_Main(QWidget *parent = nullptr);
    ~SQL_Window_Main();

    // Для получения информации о БД из окна логина
    //void get_DB_data_from_login(QSqlDatabase DB_Data);
    // Почему - то даже при успешном переносе не хочет открываться QSqlDatabase(invalid)
    // Теперь работает
    void get_DB_data_from_login(QSqlDatabase DB_Data, int DB_Type);

    // Передача данных о текущей таблице в insert
    int take_list_of_Tables(int Current_table_number_for_insert);

// Инициализация для второго окна
signals:
    void firstWindow();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_activated(int index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_lineEdit_editingFinished();

    void on_pushButton_7_clicked();


private:
    Ui::SQL_Window_Main *ui;
    // Переменные для работы с БД
    QSqlDatabase DB;
    QSqlQuery *QUERRY;
    QSqlTableModel *TABLE_MODEL;
    QSqlQueryModel *QUERY_MODEL;

    int N_ActiveRow;
    void changeIndex(int i);

    // Инициализация окна для вставки значений
    // Временно закомменчено - ошибка сборки
    Dialog_SQL_Insert *InsertWindow;
    // Инициализация окна для удаления значений
    Dialog_SQL_Delete *DeleteWindow;
    // Инициализация окна для обновления значений
    //dialog_sql_update *UpdateWindow;
    //dialog_sql_update *UpdateWindow;
    Dialog_SQL_Update *UpdateWindow;

};

#endif // SQL_WINDOW_MAIN_H
