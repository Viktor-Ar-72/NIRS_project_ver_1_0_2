#ifndef DIALOG_SQL_UPDATE_H
#define DIALOG_SQL_UPDATE_H

#include <QDialog>
// Подключение модулей
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>
namespace Ui {
class dialog_sql_update;
}

using namespace std;
// Библиотеки для генератора на основе Вихря Мерсенна
#include <stdlib.h>
#include <cstdlib>
#include <random>

class dialog_sql_update : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_sql_update(QWidget *parent = nullptr);
    ~dialog_sql_update();

// Инициализация для второго окна
signals:
    void UpWindow();

private slots:

    void on_pushButton_OK_clicked();

    void on_pushButton_Esc_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_tabWidget_Update_currentChanged(int index);

    /*
    void on_lineEdit_student_id_editingFinished();

    void on_lineEdit_student_var_editingFinished();

    void on_lineEdit_full_name_editingFinished();

    void on_lineEdit_student_group_editingFinished();

    void on_lineEdit_task_id_editingFinished();

    void on_lineEdit_task_complication_editingFinished();

    void on_lineEdit_task_Text_editingFinished();

    void on_lineEdit_task_var_editingFinished();
*/


private:
    Ui::dialog_sql_update *ui;

    // Переменные для работы с БД
    QSqlDatabase DB;
    QSqlQuery *QUERRY;
    QSqlTableModel *TABLE_MODEL;
    QSqlQueryModel *QUERY_MODEL;

    int N_ActiveRow;
    void changeIndex(int i);

};

#endif // DIALOG_SQL_UPDATE_H
