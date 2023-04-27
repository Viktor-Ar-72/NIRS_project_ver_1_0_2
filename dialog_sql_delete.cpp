#include "dialog_sql_delete.h"
#include "ui_dialog_sql_delete.h"
#include <sql_window_main.h>
#include <QMessageBox>
#include <QSqlRecord>
#include <QFile>
Dialog_SQL_Delete::Dialog_SQL_Delete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SQL_Delete)
{
    ui->setupUi(this);

    //  Инициализация lineEdit_tableNow
    ui->lineEdit_tableNow->setText("Диапазон?");
    ui->lineEdit_tableNow->setReadOnly(1);
    // Настройка цвета
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,Qt::lightGray);
    palette->setColor(QPalette::Text,Qt::darkGray);
    ui->lineEdit_tableNow->setPalette(*palette);

    // Инициализация lineEdit_isDEnable
    ui->lineEdit_isDEnable->setText("Заблокировано");
    ui->lineEdit_isDEnable->setReadOnly(1);
    // Настройка цвета
    ui->lineEdit_isDEnable->setPalette(*palette);

    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);
}
/* Команда для удаления PostgreSQL
 * DELETE FROM public."TestTable_1" WHERE "Column_Int_1" IN (14070);
 * DELETE FROM public."TestTable_2" WHERE "Column_Text_1" = 'hywVFsbCp';
 * DELETE FROM public."TestTable_1" WHERE "Column_Int_1" = 11
 * Microsoft Access
 * DELETE FROM Test_Table_1 WHERE Column_Int_1 = 15
 * DELETE FROM Test_Table_2 WHERE Column_Int = 6
*/
Dialog_SQL_Delete::~Dialog_SQL_Delete()
{
    delete ui;
}

// Для настройки соединения
// Для работы с присланными таблицами
// Список таблиц
QStringList Delete_BD_Tables_List_Asked;
// Массив с типами полей каждой таблицы по номеру
QString **Delete_Matrix_Tables_FieldTypes;
QString **Delete_Matrix_Tables_FieldNames;

// Глобальные переменные для работы с подключением к базе данных
QString Delete_Transfer_DB_Adress, Delete_Transfer_DB_Name, Delete_Transfer_DB_User, Delete_Transfer_DB_Password;
int Delete_Transfer_DB_Port;
// Глобальные переменные для формы обновления данных
int Delete_Table_Index;
// Переменные границ диапазона
int Delete_Fields_Start = NULL, Delete_Fields_Finish = NULL;
// Тип базы данных
int Delete_DB_Type;


void Dialog_SQL_Delete::changeIndex(int i)
{    i++;    }


void Dialog_SQL_Delete::on_pushButton_OK_clicked()
{
    QString query_text;
    QSqlQueryModel Query_Test;
    if (Delete_DB_Type == 0)
    {
        query_text = "SELECT * FROM public.\"" + Delete_BD_Tables_List_Asked[Delete_Table_Index] + "\"";
    }
    if (Delete_DB_Type == 2)
    {
        query_text = "SELECT * FROM " + Delete_BD_Tables_List_Asked[Delete_Table_Index] + ";";
    }
    Query_Test.setQuery(query_text);
    int table_size = Query_Test.rowCount();
    if ((Delete_Fields_Start >= 0) && (Delete_Fields_Finish > 0) && ((Delete_Fields_Finish - Delete_Fields_Start + 1) > 0) && (Delete_Fields_Finish < table_size))
    {
        QSqlQueryModel SQL_Querry_Model;
        QString query_U_borders;
        if (Delete_DB_Type == 0)
        {
            query_U_borders = "SELECT * FROM public.";
            query_U_borders += '"' + Delete_BD_Tables_List_Asked[Delete_Table_Index] + '"' + " ORDER BY" + '"' + Delete_Matrix_Tables_FieldNames[Delete_Table_Index][0] + '"';
        }
        if (Delete_DB_Type == 2)
        {
            query_U_borders = "SELECT * FROM " + Delete_BD_Tables_List_Asked[Delete_Table_Index] + " ORDER BY " + Delete_Matrix_Tables_FieldNames[Delete_Table_Index][0] + ";";
        }
        qDebug() << "Запрос на поиск нужной строки - " << query_U_borders;
        SQL_Querry_Model.setQuery(query_U_borders);

        QString* Delete_Where_Values = new QString[(Delete_Fields_Finish - Delete_Fields_Start + 1)];

        int m = 0;
        for (int i = 0; i < SQL_Querry_Model.rowCount(); ++i)
        {
            if (i >= Delete_Fields_Start && i <= Delete_Fields_Finish)
            {
                qDebug() << "Найдена " << m << " строка для удаления на " << i << " месте";
                qDebug() << SQL_Querry_Model.record(i).value(Delete_Matrix_Tables_FieldNames[Delete_Table_Index][0]).toString();
                Delete_Where_Values[m] = SQL_Querry_Model.record(i).value(Delete_Matrix_Tables_FieldNames[Delete_Table_Index][0]).toString();
                m += 1;
            }
        }

        // Удаление значений в таблице через Delete
        QSqlRecord Delete_Record;
        QSqlQuery Query_Delete;
        QString query_vrem_text;
        if (Delete_DB_Type == 0)
        {
            query_vrem_text = "SELECT * FROM public.\"" + Delete_BD_Tables_List_Asked[Delete_Table_Index] + "\"";
        }
        if (Delete_DB_Type == 2)
        {
            query_vrem_text = "SELECT * FROM " + Delete_BD_Tables_List_Asked[Delete_Table_Index] + ";";
        }
        Query_Delete.exec(query_vrem_text);
        Delete_Record = Query_Delete.record();
        QString query_Delete_text;
        QString boolean_type_QString;
        for (int V = 0; V < (Delete_Fields_Finish - Delete_Fields_Start + 1); ++V)
        {
            if (Delete_DB_Type == 0)
            {
                query_Delete_text = "DELETE FROM public.";
                query_Delete_text += '"';
                query_Delete_text = query_Delete_text + Delete_BD_Tables_List_Asked[Delete_Table_Index];
                query_Delete_text.append('"');
            }
            if (Delete_DB_Type == 2)
            {
                query_Delete_text = "DELETE FROM " + Delete_BD_Tables_List_Asked[Delete_Table_Index];
            }

            //query_Delete_text = query_Delete_text + " SET ";
            // Убираем последнюю запятую (спасибо ПЗЕ4 :) )
            //query_insert_text.remove((query_insert_text.length() - 1), 3);
            //query_insert_text = query_insert_text + ");";
            //int position = query_Update_text.lastIndexOf(QChar(','));
            //query_Update_text = query_Update_text.left(position);
            // Не требуется, так как у нас не Insert - запрос
            //query_Update_text = query_Update_text.append(");");

            // Дополнение запроса Delete
            if ((Delete_Matrix_Tables_FieldTypes[Delete_Table_Index][0] == "int") || (Delete_Matrix_Tables_FieldTypes[Delete_Table_Index][0] == "bool") || (Delete_Matrix_Tables_FieldTypes[Delete_Table_Index][0] == "double"))
            {
                if (Delete_DB_Type == 0)
                {
                    query_Delete_text = query_Delete_text + " WHERE " + '"' + Delete_Matrix_Tables_FieldNames[Delete_Table_Index][0] + '"' + " = " + Delete_Where_Values[V];
                }
                if (Delete_DB_Type == 2)
                {
                    query_Delete_text = query_Delete_text + " WHERE " + Delete_Matrix_Tables_FieldNames[Delete_Table_Index][0] + " = " + Delete_Where_Values[V];
                }
            }
            else
            {
                if (Delete_DB_Type == 0)
                {
                    query_Delete_text = query_Delete_text + " WHERE " + '"' + Delete_Matrix_Tables_FieldNames[Delete_Table_Index][0] + '"' + " = '" + Delete_Where_Values[V] + "'";
                }
                if (Delete_DB_Type == 2)
                {
                    query_Delete_text = query_Delete_text + " WHERE " + Delete_Matrix_Tables_FieldNames[Delete_Table_Index][0] + " = '" + Delete_Where_Values[V] + "';";
                }
            }
            qDebug() << "Запрос на удаление" << V << query_Delete_text;
            QFile file("output_Delete_Query_Test.txt");
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << query_Delete_text; // записываем содержимое строки в файл
                file.close();
            }


            // Если временно закомменчено - значит, происходит тест на обновление данных
            QUERY_MODEL->setQuery(query_Delete_text);
            qDebug() << "Запрос на удаление" << V << "выполнен.";
        }


        query_Delete_text = ""; // Заглушка
        QUERY_MODEL->setQuery(query_Delete_text);
        // Закрытие окна вставки данных

        // Возврат к основному окну
        this->close();
        emit DelWindow();
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Не указаны границы обновляемых записей!\nПожалуйста, укажите!");
        return;
    }
    /* OLD DELETE
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
    */
}



void Dialog_SQL_Delete::on_pushButton_Esc_clicked()
{
    // Возврат к основному окну
    this->close();
    emit DelWindow();

    /* OLD DELETE
    del_student_id = ""; del_student_group = ""; del_full_name = "";
    del_task_id = ""; del_task_text = ""; del_task_complication = "";
    del_task_variant = "";
    */
}

void Dialog_SQL_Delete::on_tabWidget_Delete_tabBarClicked(int index)
{
    if(index == 0)
    {
        //Del_Table_Index = 0;
        //qDebug() << "Удаление записи в таблице Students " << Del_Table_Index;
        qDebug() << "Основное окно формы DELETE";
    }
    else if (index == 1)
    {
        //Del_Table_Index = 1;
        //qDebug() << "Удаление записи в таблице Tasks " << Del_Table_Index;
        qDebug() << "Окно справки формы DELETE";
    }
}



void Dialog_SQL_Delete::on_lineEdit_strok_start_editingFinished()
{
    QString query_text;
    QSqlQueryModel Query_Test;
    if (Delete_DB_Type == 0)
    {
        query_text = "SELECT * FROM public.\"" + Delete_BD_Tables_List_Asked[Delete_Table_Index] + "\"";
    }
    if (Delete_DB_Type == 2)
    {
        query_text = "SELECT * FROM " + Delete_BD_Tables_List_Asked[Delete_Table_Index] + ";";
    }
    Query_Test.setQuery(query_text);
    int w = Query_Test.rowCount();
    Delete_Fields_Start = ui->lineEdit_strok_start->text().toInt();
    if ((Delete_Fields_Start > -1) && (Delete_Fields_Start < w))
    {
        qDebug() << "Номер строки, с которой нужно удалить данные - " << Delete_Fields_Start;
        if (Delete_Fields_Finish != NULL)
        {
            ui->lineEdit_tableNow->setText(Delete_BD_Tables_List_Asked[Delete_Table_Index]);
            ui->lineEdit_isDEnable->setText("Готов к исполнению");
        }
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Введен неправильный номер начальной строки!\nПожалуйста, переопределите.");
        ui->lineEdit_strok_start->clear();
        ui->lineEdit_tableNow->setText("Диапазон?");
        ui->lineEdit_isDEnable->setText("Заблокировано");
        Delete_Fields_Start = NULL;
    }
}


void Dialog_SQL_Delete::on_lineEdit_strok_end_editingFinished()
{
    QString query_text;
    QSqlQueryModel Query_Test;
    if (Delete_DB_Type == 0)
    {
        query_text = "SELECT * FROM public.\"" + Delete_BD_Tables_List_Asked[Delete_Table_Index] + "\"";
    }
    if (Delete_DB_Type == 2)
    {
        query_text = "SELECT * FROM " + Delete_BD_Tables_List_Asked[Delete_Table_Index] + ";";
    }
    Query_Test.setQuery(query_text);
    int w = Query_Test.rowCount();
    Delete_Fields_Finish = ui->lineEdit_strok_end->text().toInt();
    if ((Delete_Fields_Finish >= Delete_Fields_Start) && (Delete_Fields_Finish < w))
    {
        qDebug() << "Номер строки, до которой нужно удалить данные - " << Delete_Fields_Finish;
        if (Delete_Fields_Start != NULL)
        {
            ui->lineEdit_tableNow->setText(Delete_BD_Tables_List_Asked[Delete_Table_Index]);
            ui->lineEdit_isDEnable->setText("Готово к исполнению");
        }
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Введен неправильный номер конечной строки!\nПожалуйста, переопределите.");
        ui->lineEdit_strok_end->clear();
        ui->lineEdit_tableNow->setText("Диапазон?");
        ui->lineEdit_isDEnable->setText("Заблокировано");
        Delete_Fields_Finish = NULL;
    }
}

// Новая функция для получения данных о таблицах
void Dialog_SQL_Delete::get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types, int DB_Class)
{
    Delete_DB_Type = DB_Class;
    // Для вывода матрицы пока что пользуюсь таким способом - при использовани count() или size() вылетает почему - то
    QString query_text;
    QSqlRecord Record_Test;
    QSqlQuery Query_Test;

    Delete_BD_Tables_List_Asked = DB_tables_list;
    qDebug() << "Таблицы в БД, полученный список - " << Delete_BD_Tables_List_Asked;
    Delete_Matrix_Tables_FieldNames = Matrix_Names;
    Delete_Matrix_Tables_FieldTypes = Matrix_Types;
    qDebug() << "Массив имён полей, полученные данные" << Delete_Matrix_Tables_FieldNames;
    qDebug() << "Массив типов полей, полученные данные" << Delete_Matrix_Tables_FieldTypes;
    for (int i = 0; i < Delete_BD_Tables_List_Asked.size(); ++i)
    {
        //int i = 1;
        if (DB_Class == 0)
        {
            query_text = "SELECT * FROM public.\"" + Delete_BD_Tables_List_Asked[i] + "\"";
        }
        if (DB_Class == 2)
        {
            query_text = "SELECT * FROM " + Delete_BD_Tables_List_Asked[i] + ";";
        }
        Query_Test.exec(query_text);
        Record_Test = Query_Test.record();
        for(int j = 0; j < Record_Test.count(); ++j)
        {

            qDebug() << "Имя поля - " + Delete_Matrix_Tables_FieldNames[i][j] + "; Тип поля - " + Delete_Matrix_Tables_FieldTypes[i][j];
        }
    }

    // Передача индекса текущей таблицы для удаления
    Delete_BD_Tables_List_Asked.sort();
    int vrem_number = 0;
    Delete_Table_Index = SQL_Window_Main().take_list_of_Tables(vrem_number);
    qDebug() << "Согласно полученным данным, удаление будет вестись в таблицу с индексом" << Delete_Table_Index;
    qDebug() << "Это таблица" << Delete_BD_Tables_List_Asked[Delete_Table_Index];
}

// Функция для подключения данных о подключении и установке соединения
void Dialog_SQL_Delete::get_DB_connection_from_MainWindow(QSqlDatabase DB_conn_data, int DB_Class)
{
    Delete_DB_Type = DB_Class;
    if (DB_Class == 0)
    {
        DB = QSqlDatabase::cloneDatabase(DB_conn_data, "PostgreSQL_New_Connect_Delete");
        qDebug() << "Delete: Полученные данные про DB" << DB;
        Delete_Transfer_DB_Adress = DB.hostName();
        qDebug() << "Delete: Полученный адрес БД в Qstring - " << Delete_Transfer_DB_Adress;
        Delete_Transfer_DB_Port = DB.port();
        qDebug() << "Delete: Полученный port БД в Qstring" << QString::number(Delete_Transfer_DB_Port);
        Delete_Transfer_DB_Name = DB.databaseName();
        qDebug() << "Delete: Полученное имя БД в QString" << Delete_Transfer_DB_Name;
        Delete_Transfer_DB_User = DB.userName();
        qDebug() << "Delete: Полученный логин пользователя в QString" << Delete_Transfer_DB_User;
        Delete_Transfer_DB_Password = DB.password();
        qDebug() << "Delete: Полученный пароль пользователя в QString" << Delete_Transfer_DB_Password;

        qDebug() << "Проверка на открытие соединения с запрошенными параметрами";
        DB = QSqlDatabase::addDatabase("QPSQL", "DeleteWindowConnect");
        DB.setHostName(Delete_Transfer_DB_Adress);
        DB.setPort(Delete_Transfer_DB_Port);
        DB.setDatabaseName(Delete_Transfer_DB_Name);
        DB.setUserName(Delete_Transfer_DB_User);
        DB.setPassword(Delete_Transfer_DB_Password);
    }
    if (DB_Class == 2)
    {
        DB = QSqlDatabase::cloneDatabase(DB_conn_data, "MicrosoftAccess_New_Connect_Delete");
        qDebug() << "Delete: Полученные данные про DB" << DB;
        Delete_Transfer_DB_Name = DB.databaseName();
        qDebug() << "Delete: Полученное имя БД в QString" << Delete_Transfer_DB_Name;

        qDebug() << "Проверка на открытие соединения с запрошенными параметрами";
        DB = QSqlDatabase::addDatabase("QODBC", "DeleteWindowConnect");
        DB.setDatabaseName(Delete_Transfer_DB_Name);
    }
    if (DB.open())
    {
        qDebug() << "DB in Delete is open: " << DB;
    }
    else
    {
        qDebug() << "Error in Delete DB opening: " << DB.lastError();
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
/* OLD DELETE
// Глобальные переменные для работы с формами
QString del_student_id, del_full_name, del_student_group, del_variant;
QString del_task_id, del_task_complication, del_task_text, del_task_variant;
int Del_Table_Index;

//void Dialog_SQL_Delete::on_tabWidget_Students_tabBarClicked(int index)
//{
//}
*/


