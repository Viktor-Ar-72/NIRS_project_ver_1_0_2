#include "dialog_sql_insert.h"
#include "ui_dialog_sql_insert.h"
// Библиотеки для генератора на основе Вихря Мерсенна
#include <stdlib.h>
#include <cstdlib>
#include <random>
#include <QMessageBox>
// Предел генерирования случайного числа
//int Colvo_Unic = 100;
int Colvo_Unic = 10;
// Глобальный массив для работы с уникальностью номеров
int *variants_number = new int[Colvo_Unic];

Dialog_SQL_Insert::Dialog_SQL_Insert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SQL_Insert)
{
    ui->setupUi(this);
    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);
}

Dialog_SQL_Insert::~Dialog_SQL_Insert()
{
    delete ui;
}
/*
void Dialog_SQL_Insert::on_comboBox_activated(int index)
{
    if(index == 0)
    {
        qDebug() << "Вставка в таблицу Students";
    }
    else if (index == 1)
    {
        qDebug() << "Вставка в таблицу Tasks";
    }
}
*/

// Глобальные переменные для работы с формами
QString student_id, full_name, student_group, variant;
QString task_id, task_complication, task_text, task_variant;
int Table_Index;

void Dialog_SQL_Insert::on_lineEdit_editingFinished()
{
    // Построение массива на количество уникальных вариантов
    qDebug() << "Проверка вариантов на уникальность";
    QString query_text = "SELECT * from public.\"Tasks\"";
    QUERY_MODEL->setQuery(query_text);
    variants_number[0] = QUERY_MODEL->rowCount();
    qDebug() << "Varinat_number_0 = " << variants_number[0];
    for (int j = 1; j < Colvo_Unic; ++j)
    {
        variants_number[j] = 0;
    }
    for(int i = 1; i <= variants_number[0]; ++i)
    {
        query_text = "SELECT * FROM public.\"Tasks\" WHERE var_number = " + QString().number(i) + ";";
        //qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        variants_number[i] = QUERY_MODEL->rowCount();
        // qDebug() << "Для варианта " << i << " найдено " << variants_number[i] << " записей";
    }
    qDebug() << "Конец проверки количества уникальных вариантов и записей в них";

    student_id = ui->lineEdit->text();
    if ((student_id == "")||(student_id == "0"))
    {
        QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\"");
        int id_avto = QUERY_MODEL->rowCount() + 1;
        student_id = QString::number(id_avto);
    }
    else
    {
        QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE student_id = " + student_id + ";");
        int id_check = QUERY_MODEL->rowCount();
        if (id_check != 0)
        {
            QMessageBox::warning(this, "WARNING", "Введён неккоректный идентификатор студента.\n"
                                                 "Пожалуйста, переназначьте, или система сделает это автоматически");
            QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\"");
            int id_avto = QUERY_MODEL->rowCount() + 1;
            student_id = QString::number(id_avto);
        }
    }
    qDebug() << "Идентификатор студента - " << student_id;
}

void Dialog_SQL_Insert::on_lineEdit_2_editingFinished()
{
    full_name = ui->lineEdit_2->text();
    qDebug() << "ФИО студента - " << full_name;
}


void Dialog_SQL_Insert::on_lineEdit_3_editingFinished()
{
    student_group = ui->lineEdit_3->text();
    qDebug() << "Группа студента - " << student_group;
}


void Dialog_SQL_Insert::on_lineEdit_4_editingFinished()
{
    // Построение массива на количество уникальных вариантов
    qDebug() << "Проверка вариантов на уникальность";
    QString query_text = "SELECT * from public.\"Tasks\"";
    QUERY_MODEL->setQuery(query_text);
    variants_number[0] = QUERY_MODEL->rowCount();
    qDebug() << "Varinat_number_0 = " << variants_number[0];
    for (int j = 1; j < Colvo_Unic; ++j)
    {
        variants_number[j] = 0;
    }
    for(int i = 1; i <= variants_number[0]; ++i)
    {
        query_text = "SELECT * FROM public.\"Tasks\" WHERE var_number = " + QString().number(i) + ";";
        //qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        variants_number[i] = QUERY_MODEL->rowCount();
        //qDebug() << "Для варианта " << i << " найдено " << variants_number[i] << " записей";
    }
    qDebug() << "Конец проверки количества уникальных вариантов и записей к нему в массиве";

    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(device());
    variant = ui->lineEdit_4->text();
    qDebug() << "Номер введённого варианта - " << variant;
    if (variant == "0")
    {
        int rand_variant;
        char t = 'N';
        while(t == 'N')
        {
            rand_variant = engine() % Colvo_Unic;
            if (!((variants_number[rand_variant] == 0) || (rand_variant == 0)))
            {
                variant = QString::number(rand_variant);
                t = 'Y';
            }
        }
        qDebug() << "Номер случайного варианта - " << variant;
    }
    else
    {
        QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE var_number = " + variant + ";");
        int id_check = QUERY_MODEL->rowCount();
        if (id_check == 0)
        {
            QMessageBox::warning(this, "WARNING", "Введён пустой вариант.\n"
                                                 "Пожалуйста, переназначьте, или система сделает это автоматически");
            int rand_variant;
            char t = 'N';
            while(t == 'N')
            {
                rand_variant = engine() % Colvo_Unic;
                if (!((variants_number[rand_variant] == 0) || (rand_variant == 0)))
                {
                    variant = QString::number(rand_variant);
                    t = 'Y';
                }
            }
            qDebug() << "Номер переназначенного случайного варианта - " << variant;
        }
    }

    // Old_realization
    /*
    //std::mt19937_64 engine;
    //std::random_device device;
    //engine.seed(device());
    variant = ui->lineEdit_4->text();
    qDebug() << "Номер введённого варианта - " << variant;
    if (variant == "0")
    {
        int rand_variant;
        char t = 'N';
        while(t == 'N')
        {
            rand_variant = engine() % Colvo_Unic;
            if ((variants_number[rand_variant] == 0) || (rand_variant == 0))
            {
                t = 'N';
                qDebug() << "Empty variant number choose";
            }
            else
            {
                variant = QString::number(rand_variant);
                t = 'Y';
            }
        }
        qDebug() << "Номер случайного варианта - " << variant;

    }
    */
}


void Dialog_SQL_Insert::on_pushButton_clicked()
{
    if (Table_Index == 0)
    {
        QString query_text = "INSERT INTO public.\"Students\" (student_id, full_name, student_group, variant) VALUES ('" + student_id + "'::integer, '" + full_name + "'::text, '" + student_group + "'::text, '" + variant + "'::integer) returning student_id;";
        QUERY_MODEL->setQuery(query_text);
    }
    if (Table_Index == 1)
    {
        //QString query_text = "INSERT INTO public.\"Students\" (student_id, full_name, student_group, variant) VALUES ('" + student_id + "'::integer, '" + full_name + "'::text, '" + student_group + "'::text, '" + variant + "'::integer) returning student_id;";
        QString query_text = "INSERT INTO public.\"Tasks\" (task_id, var_number, complication, task_text) VALUES ('" + task_id + "'::integer, '" + task_variant + "'::integer, '" + task_complication + "'::integer, '" + task_text + "'::text)returning task_id;";
        QUERY_MODEL->setQuery(query_text);
    }

    // Возврат к основному окну
    // Временно закомменчено, чтобы не вылетала
    // Раскомменчена - вылеты были не из - за этого
    this->close();
    emit InWindow();
}

void Dialog_SQL_Insert::on_pushButton_2_clicked()
{
    // Возврат к основному окну
    this->close();
    emit InWindow();
}

void Dialog_SQL_Insert::changeIndex(int i)
{    i++;    }



void Dialog_SQL_Insert::on_tabWidget_Students_tabBarClicked(int index)
{
    if(index == 0)
    {
        Table_Index = 0;
        qDebug() << "Вставка в таблицу Students " << Table_Index;
    }
    else if (index == 1)
    {
        Table_Index = 1;
        qDebug() << "Вставка в таблицу Tasks " << Table_Index;
    }
}


void Dialog_SQL_Insert::on_idTask_editingFinished()
{
    // Построение массива на количество уникальных вариантов
    qDebug() << "Проверка вариантов на уникальность";
    QString query_text = "SELECT * from public.\"Tasks\"";
    QUERY_MODEL->setQuery(query_text);
    variants_number[0] = QUERY_MODEL->rowCount();
    qDebug() << "Varinat_number_0 = " << variants_number[0];
    for (int j = 1; j < Colvo_Unic; ++j)
    {
        variants_number[j] = 0;
    }
    for(int i = 1; i <= variants_number[0]; ++i)
    {
        query_text = "SELECT * FROM public.\"Tasks\" WHERE var_number = " + QString().number(i) + ";";
        //qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        variants_number[i] = QUERY_MODEL->rowCount();
        //qDebug() << "Для варианта " << i << " найдено " << variants_number[i] << " записей";
    }
    qDebug() << "Конец проверки количества уникальных вариантов и записей к нему в массиве";

    task_id = ui->idTask->text();
    if ((task_id == "")||(task_id == "0"))
    {
        QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\"");
        int id_avto = QUERY_MODEL->rowCount() + 1;
        task_id = QString::number(id_avto);
    }
    else
    {
        QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE task_id = " + task_id + ";");
        int id_check = QUERY_MODEL->rowCount();
        if (id_check != 0)
        {
            QMessageBox::warning(this, "WARNING", "Введён неккоректный идентификатор задания.\n"
                                                 "Пожалуйста, переназначьте, или система сделает это автоматически");
            QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\"");
            int id_avto = QUERY_MODEL->rowCount() + 1;
            task_id = QString::number(id_avto);
        }
    }
    qDebug() << "Идентификатор задания - " << task_id;
}


void Dialog_SQL_Insert::on_taskComplication_editingFinished()
{
    task_complication = ui->taskComplication->text();
    qDebug() << "Сложность задания - " << task_complication;
}


void Dialog_SQL_Insert::on_taskText_editingFinished()
{
    task_text = ui->taskText->text();
    qDebug() << "Текст задания - " << task_text;
}


void Dialog_SQL_Insert::on_taskVariant_editingFinished()
{
    // Построение массива на количество уникальных вариантов
    qDebug() << "Проверка вариантов на уникальность";
    QString query_text = "SELECT * from public.\"Tasks\"";
    QUERY_MODEL->setQuery(query_text);
    variants_number[0] = QUERY_MODEL->rowCount();
    qDebug() << "Varinat_number_0 = " << variants_number[0];
    for (int j = 1; j < Colvo_Unic; ++j)
    {
        variants_number[j] = 0;
    }
    for(int i = 1; i <= variants_number[0]; ++i)
    {
        query_text = "SELECT * FROM public.\"Tasks\" WHERE var_number = " + QString().number(i) + ";";
        //qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        variants_number[i] = QUERY_MODEL->rowCount();
        //qDebug() << "Для варианта " << i << " найдено " << variants_number[i] << " записей";
    }
    qDebug() << "Конец проверки количества уникальных вариантов и записей к нему в массиве";

    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(device());

    task_variant = ui->taskVariant->text();
    qDebug() << "Введёный номер варианта для задания - " << task_variant;
    if(task_variant == "0")
    {
        char t = 'N';
        int rand_variant_task;
        while(t == 'N')
        {
            rand_variant_task = engine() % Colvo_Unic;
            if ((variants_number[rand_variant_task] > 2) || (rand_variant_task == 0))
            {
                t = 'N';
                qDebug() << "Full variant number choose";
            }
            else if(variants_number[rand_variant_task] <= 2)
            {
                variants_number[rand_variant_task] += 1;
                task_variant = QString::number(rand_variant_task);
                t = 'Y';
            }
        }

        /*
        //else
        //{
        //    variant = QString::number(rand_variant_task);
        //    t = 'Y';
        //}
        */

        /*
        //if (Table_Index == 1)
        //{\
        */
        /*
        QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\"");

        //int k = QUERY_MODEL->rowCount();
        //QSqlTableModel SQL_Table_Model;
        //SQL_Table_Model.setTable("public.\"Tasks\"");
        //SQL_Table_Model.select();
        */
        /*
        //for (int i = 0; i < QUERY_MODEL->rowCount(); ++i)
        //for (int i = 0; i < SQL_Table_Model.rowCount(); ++i)
        //{
        //        qDebug() << "Номер текущей записи - " << i;
        //        variant = QString::number(i);
        // Создать Query_model_2, вызывать на проверку условия, сколько раз
                // был уже использован данный номер варианта - через проверку одномерного массива
                // Если меньше 3 - ставить его как номер варианта
                // Иначе - создавать другой номер
                */
        /*
               //int ID = SQL_Table_Model.record(i).value("ID").toInt();
               //QString DATA = SQL_Table_Model.record(i).value("DATA").toString();
               //qDebug() << ID << DATA;
               */
        //}

        /*
        //QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\"");
        //int i = QUERY_MODEL->rowCount() + 1;
        //qDebug() << "Текущее значение количества записей - " << i;
        */
        qDebug() << "Текущее значение варианта - " << task_variant;
        //}
    }
}
