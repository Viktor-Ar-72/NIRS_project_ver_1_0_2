#ifndef DIALOG_SQL_INSERT_H
#define DIALOG_SQL_INSERT_H

#include <QDialog>
// Подключение модулей
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlRecord>

// Для генераторов
#include <random>
// Для строк
#include <QStringBuilder>
namespace Ui {
class Dialog_SQL_Insert;
}

class Dialog_SQL_Insert : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SQL_Insert(QWidget *parent = nullptr);
    ~Dialog_SQL_Insert();

    // Для получения данных о БД
    void get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types, int DB_Class);
    void get_DB_connection_from_MainWindow(QSqlDatabase DB_conn_data, int DB_class);
    // Функции реализации генераторов
    // Int + Bool
    int* VihrMersenna_Gen_Int(int* massiv_gen_numbers, int rasmer, int gen_VM_left_edge, int gen_VM_right_edge);
    int* MacLarenMarsalii_Gen_Int(int* massiv_gen_numbers, int rasmer, int gen_MM_left_edge, int gen_MM_right_edge);
    // Real
    double* VihrMersenna_Gen_Real(double* massiv_gen_numbers, int rasmer, double gen_VM_left_edge, double gen_VM_right_edge);
    double* MacLarenMarsalii_Gen_Real(double* massiv_gen_numbers, int rasmer, double gen_MM_left_edge, double gen_MM_right_edge);
    // Varchar
    QString* VihrMersenna_Gen_Char(QString* massiv_gen_numbers, int rasmer, int length, int gen_VM_left_edge, int gen_VM_right_edge);
    QString* MacLarenMarsalii_Gen_Char(QString* massiv_gen_numbers, int rasmer, int length,  int gen_MM_left_edge, int gen_MM_right_edge);
    // ByteA
    QString* VihrMersenna_Gen_ByteA(QString* massiv_gen_numbers, int rasmer, int length, int gen_VM_left_edge, int gen_VM_right_edge);
    QString* MacLarenMarsalii_Gen_ByteA(QString* massiv_gen_numbers, int rasmer, int length,  int gen_MM_left_edge, int gen_MM_right_edge);


signals:
    // Подключение окна InsertWindow
    void InWindow();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tabWidget_Students_tabBarClicked(int index);

    void on_comboBox_algoritms_activated(int index);

    void on_lineEdit_kolvo_new_strok_editingFinished();

    void on_lineEdit_int_left_editingFinished();

    void on_lineEdit_int_right_editingFinished();

    void on_lineEdit_varchar_lenght_editingFinished();

    void on_lineEdit_real_left_editingFinished();

    void on_lineEdit_real_right_editingFinished();

    void on_lineEdit_bytea_lenght_editingFinished();

    void on_checkBox_useBoolGen_clicked();

private:
    Ui::Dialog_SQL_Insert *ui;
    int N_ActiveRow;
    QSqlQueryModel *QUERY_MODEL;
    QSqlDatabase DB;
    QSqlQuery *QUERRY;
    QSqlTableModel *TABLE_MODEL;

};

#endif // DIALOG_SQL_INSERT_H
