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

signals:
    // Подключение окна InsertWindow
    void DelWindow();

private slots:
    //void on_tabWidget_Students_tabBarClicked(int index);

    void on_lineEdit_editingFinished();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    //void on_lineEdit_4_editingFinished();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_idTask_editingFinished();

    void on_taskComplication_editingFinished();

    void on_taskText_editingFinished();

    //void on_taskVariant_editingFinished();

    void on_tabWidget_Delete_tabBarClicked(int index);

private:
    Ui::Dialog_SQL_Delete *ui;

    void changeIndex(int i);
    QSqlQueryModel *QUERY_MODEL;
    QSqlDatabase DB;
    QSqlQuery *QUERRY;
    QSqlTableModel *TABLE_MODEL;
};

#endif // DIALOG_SQL_DELETE_H
