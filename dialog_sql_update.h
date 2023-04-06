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
//#include <sql_window_main.h>
namespace Ui {
//class dialog_sql_update;
class Dialog_SQL_Update;
}

#include <QSqlError>

using namespace std;
// Библиотеки для генератора на основе Вихря Мерсенна
#include <stdlib.h>
#include <cstdlib>
#include <random>

//class dialog_sql_update : public QDialog
class Dialog_SQL_Update : public QDialog
{
    Q_OBJECT

public:
    //explicit dialog_sql_update(QWidget *parent = nullptr);
    explicit Dialog_SQL_Update(QWidget *parent = nullptr);
    //~dialog_sql_update();
    ~Dialog_SQL_Update();

    // Функции для инициализации
    void get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types);
    void get_DB_connection_from_MainWindow(QSqlDatabase DB_conn_data);

    // Функции реализации генераторов
    // Int + Bool
    int* Up_VihrMersenna_Gen_Int(int* massiv_gen_numbers, int rasmer, int gen_VM_left_edge, int gen_VM_right_edge);
    int* Up_MacLarenMarsalii_Gen_Int(int* massiv_gen_numbers, int rasmer, int gen_MM_left_edge, int gen_MM_right_edge);
    // Real
    double* Up_VihrMersenna_Gen_Real(double* massiv_gen_numbers, int rasmer, double gen_VM_left_edge, double gen_VM_right_edge);
    double* Up_MacLarenMarsalii_Gen_Real(double* massiv_gen_numbers, int rasmer, double gen_MM_left_edge, double gen_MM_right_edge);
    // Varchar
    QString* Up_VihrMersenna_Gen_Char(QString* massiv_gen_numbers, int rasmer, int length, int gen_VM_left_edge, int gen_VM_right_edge);
    QString* Up_MacLarenMarsalii_Gen_Char(QString* massiv_gen_numbers, int rasmer, int length,  int gen_MM_left_edge, int gen_MM_right_edge);
    // ByteA / Blob
    QString* Up_VihrMersenna_Gen_ByteA(QString* massiv_gen_numbers, int rasmer, int length, int gen_VM_left_edge, int gen_VM_right_edge);
    QString* Up_MacLarenMarsalii_Gen_ByteA(QString* massiv_gen_numbers, int rasmer, int length,  int gen_MM_left_edge, int gen_MM_right_edge);


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


    void on_comboBox_algoritms_activated(int index);

    void on_lineEdit_strok_start_editingFinished();

    void on_lineEdit_strok_end_editingFinished();

    void on_lineEdit_int_left_editingFinished();

    void on_lineEdit_int_right_editingFinished();

    void on_lineEdit_varchar_lenght_editingFinished();

    void on_lineEdit_real_left_editingFinished();

    void on_lineEdit_real_right_editingFinished();

    void on_lineEdit_bytea_lenght_editingFinished();

    void on_checkBox_useBoolGen_clicked();

private:
    //Ui::dialog_sql_update *ui;
    Ui::Dialog_SQL_Update *ui;

    // Переменные для работы с БД
    QSqlDatabase DB;
    QSqlQuery *QUERRY;
    QSqlTableModel *TABLE_MODEL;
    QSqlQueryModel *QUERY_MODEL;

    int N_ActiveRow;
    void changeIndex(int i);

};

#endif // DIALOG_SQL_UPDATE_H
