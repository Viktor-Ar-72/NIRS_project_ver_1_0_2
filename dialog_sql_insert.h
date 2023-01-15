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

namespace Ui {
class Dialog_SQL_Insert;
}

class Dialog_SQL_Insert : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SQL_Insert(QWidget *parent = nullptr);
    ~Dialog_SQL_Insert();

signals:
    // Подключение окна InsertWindow
    void InWindow();

private slots:
    //void on_comboBox_activated(int index);

    void on_lineEdit_editingFinished();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    void on_lineEdit_4_editingFinished();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tabWidget_Students_tabBarClicked(int index);

    void on_idTask_editingFinished();

    void on_taskComplication_editingFinished();

    void on_taskText_editingFinished();

    void on_taskVariant_editingFinished();

private:
    Ui::Dialog_SQL_Insert *ui;
    int N_ActiveRow;
    void changeIndex(int i);
    QSqlQueryModel *QUERY_MODEL;
    QSqlDatabase DB;
    QSqlQuery *QUERRY;
    QSqlTableModel *TABLE_MODEL;
};

#endif // DIALOG_SQL_INSERT_H
