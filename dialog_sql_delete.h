#ifndef DIALOG_SQL_DELETE_H
#define DIALOG_SQL_DELETE_H

#include <QDialog>
// Подключение модулей
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlTableModel>


namespace Ui {
class Dialog_SQL_Delete;
}

class Dialog_SQL_Delete : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SQL_Delete(QWidget *parent = nullptr);
    ~Dialog_SQL_Delete();

    // Функции для инициализации
    void get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types, int DB_Class);
    void get_DB_connection_from_MainWindow(QSqlDatabase DB_conn_data, int DB_Class);

signals:
    // Подключение окна DeleteWindow
    void DelWindow();

private slots:

    void on_pushButton_OK_clicked();

    void on_pushButton_Esc_clicked();

    void on_tabWidget_Delete_tabBarClicked(int index);

    void on_lineEdit_strok_start_editingFinished();

    void on_lineEdit_strok_end_editingFinished();

private:
    Ui::Dialog_SQL_Delete *ui;

    QSqlQueryModel *QUERY_MODEL;
    QSqlDatabase DB;
    QSqlQuery *QUERRY;
    QSqlTableModel *TABLE_MODEL;
};

#endif // DIALOG_SQL_DELETE_H
