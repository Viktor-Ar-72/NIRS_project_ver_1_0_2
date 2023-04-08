#include "dialog_sql_delete.h"
#include "ui_dialog_sql_delete.h"
#include <QMessageBox>
#include <QSqlRecord>
Dialog_SQL_Delete::Dialog_SQL_Delete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SQL_Delete)
{
    ui->setupUi(this);
    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);
}
/* Команда для удаления PostgreSQL
 * DELETE FROM public."TestTable_1" WHERE "Column_Int_1" IN (14070);
*/
Dialog_SQL_Delete::~Dialog_SQL_Delete()
{
    delete ui;
}

// Глобальные переменные для работы с формами
QString del_student_id, del_full_name, del_student_group, del_variant;
QString del_task_id, del_task_complication, del_task_text, del_task_variant;
int Del_Table_Index;

//void Dialog_SQL_Delete::on_tabWidget_Students_tabBarClicked(int index)
//{
//}

void Dialog_SQL_Delete::changeIndex(int i)
{    i++;    }


void Dialog_SQL_Delete::on_pushButton_clicked()
{

    if (Del_Table_Index == 0)
    {
        //QString query_text = "INSERT INTO public.\"Students\" (student_id, full_name, student_group, variant) VALUES ('" + student_id + "'::integer, '" + full_name + "'::text, '" + student_group + "'::text, '" + variant + "'::integer) returning student_id;";
        QString query_text = "DELETE FROM public.\"Students\" WHERE student_id IN (" + del_student_id + ");";
        qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        qDebug() << "Success Delete query";
    }
    if (Del_Table_Index == 1)
    {
        //QString query_text = "INSERT INTO public.\"Students\" (student_id, full_name, student_group, variant) VALUES ('" + student_id + "'::integer, '" + full_name + "'::text, '" + student_group + "'::text, '" + variant + "'::integer) returning student_id;";
        //QString query_text = "INSERT INTO public.\"Tasks\" (task_id, var_number, complication, task_text) VALUES ('" + task_id + "'::integer, '" + task_variant + "'::integer, '" + task_complication + "'::integer, '" + task_text + "'::text)returning task_id;";
        //QString query_text = "DELETE FROM public.\"Tasks\" WHERE task_id IN (" + del_task_id + ");";
        QString query_text = "DELETE FROM public.\"Tasks\" WHERE task_id IN (" + del_task_id + ");";
        QUERY_MODEL->setQuery(query_text);
    }

    // Возврат к основному окну
    // Временно закомменчено, чтобы не вылетала
    // Раскомменчена - вылеты были не из - за этого
    this->close();
    emit DelWindow();
}


void Dialog_SQL_Delete::on_pushButton_2_clicked()
{
    del_student_id = ""; del_student_group = ""; del_full_name = "";
    del_task_id = ""; del_task_text = ""; del_task_complication = "";
    del_task_variant = "";
    // Возврат к основному окну
    this->close();
    emit DelWindow();
}

void Dialog_SQL_Delete::on_tabWidget_Delete_tabBarClicked(int index)
{
    if(index == 0)
    {
        Del_Table_Index = 0;
        qDebug() << "Удаление записи в таблице Students " << Del_Table_Index;
    }
    else if (index == 1)
    {
        Del_Table_Index = 1;
        qDebug() << "Удаление записи в таблице Tasks " << Del_Table_Index;
    }
}

/* OLD DELETE
void Dialog_SQL_Delete::on_lineEdit_editingFinished()
{
    del_student_id = ui->lineEdit->text();
    QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE student_id = " + del_student_id);
    //up_full_name = QUERY_MODEL->record(0).value("full_name").toString();
    if (((QUERY_MODEL->rowCount()) == 0) && (del_student_id != ""))
    {
        QMessageBox::warning(this, "WARNING", "Введён несуществующий идентификатор студента.\n"
                                              "Пожалуйста, переопределите");
        del_student_id = "";
        ui->lineEdit->clear();
    }
}


void Dialog_SQL_Delete::on_lineEdit_2_editingFinished()
{
    del_full_name = ui->lineEdit_2->text();
    QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE full_name = '" + del_full_name + "';");
    //up_full_name = QUERY_MODEL->record(0).value("full_name").toString();
    if (((QUERY_MODEL->rowCount()) == 0) && (del_full_name != ""))
    {
        QMessageBox::warning(this, "WARNING", "Введено несуществующее ФИО студента.\n"
                                              "Пожалуйста, переопределите данные");
        del_student_id = "";
        del_full_name = "";
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
    else if ((QUERY_MODEL->rowCount()) > 1)
    {
        QMessageBox::warning(this, "WARNING", "Введено не являющееся уникальным ФИО студента.\n"
                                              "Пожалуйста, переопределите данные");
        del_student_id = "";
        del_full_name = "";
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
    else
    {
        QUERY_MODEL->setQuery("SELECT student_id FROM public.\"Students\" WHERE full_name = '" + del_full_name + "';");
        del_student_id = QUERY_MODEL->record(0).value("student_id").toString();
        qDebug() << del_student_id;
    }

}


void Dialog_SQL_Delete::on_lineEdit_3_editingFinished()
{
    del_student_group = ui->lineEdit_3->text();
    QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE student_group = '" + del_student_group + "';");
    //up_full_name = QUERY_MODEL->record(0).value("full_name").toString();
    if (((QUERY_MODEL->rowCount()) == 0) && (del_student_group != ""))
    {
        QMessageBox::warning(this, "WARNING", "Введена несуществующая группа студента.\n"
                                              "Пожалуйста, переопределите данные");
        del_student_id = "";
        del_student_group = "";
        ui->lineEdit->clear();
        ui->lineEdit_3->clear();
    }
    else if ((QUERY_MODEL->rowCount()) > 1)
    {
        // QMessageBox::warning(this, "WARNING", "Введена не являющаяся уникальным группа студента.\n"
                                              //"Пожалуйста, переопределите данные");

        QMessageBox::warning(this, "WARNING", "Введён не являющийся уникальным текст задания.\n"
                                              "Пожалуйста, переопределите данные");
        del_student_id = "";
        del_student_group = "";
        ui->lineEdit->clear();
        ui->lineEdit_3->clear();
    }
    else
    {
        del_student_id = QUERY_MODEL->record(0).value("student_id").toString();
    }
}


//void Dialog_SQL_Delete::on_lineEdit_4_editingFinished()
//{
//    variant = ui->lineEdit_4->text();
//}
*/
/* OLD DELETE
void Dialog_SQL_Delete::on_idTask_editingFinished()
{
    del_task_id = ui->idTask->text();
    QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE task_id = " + del_task_id);
    if (((QUERY_MODEL->rowCount()) == 0) && (del_task_id != ""))
    {
        QMessageBox::warning(this, "WARNING", "Введён несуществующий идентификатор задания.\n"
                                              "Пожалуйста, переопределите");
        del_task_id = "";
        ui->idTask->clear();
    }
}


void Dialog_SQL_Delete::on_taskComplication_editingFinished()
{
    del_task_complication = ui->taskComplication->text();
    QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE complication = " + del_task_complication + ";");
    if (((QUERY_MODEL->rowCount()) == 0) && (del_task_complication != ""))
    {
        QMessageBox::warning(this, "WARNING", "Введена несуществующая сложность задания.\n"
                                              "Пожалуйста, переопределите данные");
        del_task_id = "";
        del_task_complication = "";
        ui->taskComplication->clear();
        ui->idTask->clear();
    }
    else if ((QUERY_MODEL->rowCount()) > 1)
    {
        QMessageBox::warning(this, "WARNING", "Введена не являющаяся уникальным сложность задания.\n"
                                              "Пожалуйста, переопределите данные");
        del_task_id = "";
        del_task_complication = "";
        ui->taskComplication->clear();
        ui->idTask->clear();
    }
    else
    {
        QUERY_MODEL->setQuery("SELECT task_id FROM public.\"Tasks\" WHERE complication = " + del_task_complication + ";");
        del_task_id = QUERY_MODEL->record(0).value("task_id").toString();
        qDebug() << "Найденный идентификатор задания" << del_task_id;
    }
}


void Dialog_SQL_Delete::on_taskText_editingFinished()
{
    del_task_text = ui->taskText->text();
    QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE task_text = '" + del_task_text + "';");
    if (((QUERY_MODEL->rowCount()) == 0) && (del_task_text != ""))
    {
        QMessageBox::warning(this, "WARNING", "Введена несуществующий текст задания.\n"
                                              "Пожалуйста, переопределите данные");
        del_task_id = "";
        del_task_text = "";
        ui->taskText->clear();
        ui->idTask->clear();
    }
    else if ((QUERY_MODEL->rowCount()) > 1)
    {
        QMessageBox::warning(this, "WARNING", "Введен не являющийся уникальным текст задания.\n"
                                              "Пожалуйста, переопределите данные");
        del_task_id = "";
        del_task_text = "";
        ui->taskText->clear();
        ui->idTask->clear();
    }
    else
    {
        QUERY_MODEL->setQuery("SELECT task_id FROM public.\"Tasks\" WHERE task_text = '" + del_task_text + "';");
        del_task_id = QUERY_MODEL->record(0).value("task_id").toString();
        qDebug() << "Найденный идентификатор задания" << del_task_id;
    }
}

*/


