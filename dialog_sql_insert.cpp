#include "dialog_sql_insert.h"
#include "ui_dialog_sql_insert.h"
// Библиотеки для генератора на основе Вихря Мерсенна
#include <stdlib.h>
#include <cstdlib>
#include <random>
#include <QMessageBox>
// Подключение окна MainWindow для передачи кое-каких параметров
#include <sql_window_main.h>

// Предел генерирования случайного числа
//int Colvo_Unic = 100;
int Colvo_Unic = 10;
// Глобальный массив для работы с уникальностью номеров
int *variants_number = new int[Colvo_Unic];

// Для работы с присланными таблицами
// Список таблиц
QStringList Insert_BD_Tables_List_Asked;
// Массив с типами полей каждой таблицы по номеру
QString **Insert_Matrix_Tables_FieldTypes;
QString **Insert_Matrix_Tables_FieldNames;
// Тип генератора
short GenType;

// Макрос дял определения размера массива (на всякий случай)
#define _countof(arr) (sizeof(arr) / sizeof((arr)[0]))

Dialog_SQL_Insert::Dialog_SQL_Insert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SQL_Insert)
{
    ui->setupUi(this);
    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);

    // Настройка списка таблиц
    /*
    QStringList test_list;
    qDebug() << "Tables list now - " << Insert_BD_Tables_List_Asked;
    for (int i = 0; i < Insert_BD_Tables_List_Asked.size(); ++i)
    {
        test_list << Insert_BD_Tables_List_Asked[i];
    }
    ui->comboBox_table_change->addItems(test_list);
    */
    /* Создание заглушки
    QStringList list_plug;
    for (int j = 0; j < 3; ++j)
    {
        list_plug << "test_text";
    }

    //list_plug = SQL_Main_Window_Init->take_list_of_Tables(list_plug);
    //list_plug = SQL_Window_Main().take_list_of_Tables(list_plug);
    //list_plug =
    // Так работает, но теперь приложение перестало запускаться
    //list_plug = SQL_Window_Main().take_list_of_Tables(list_plug);

    //ui->comboBox_table_change->addItems(list_plug);
    // Это работает, но необходимо понять, как можно редактировать свойства вне конструктора
    //ui->comboBox_table_change->setEditable(1);
    //ui->comboBox_table_change->setCurrentIndex(1);
    //ui->comboBox_table_change->setItemText(1, "Hello_there!");
    //Insert_BD_Tables_List_Asked = DB.tables();
    //qDebug() << "Полученный список таблиц - " << Insert_BD_Tables_List_Asked;

    //connect(ui->comboBox_table_change);
    if(DB.open())
    {
        qDebug() << "Yes, DB is opened";
    }
    else
    {
        qDebug() << "DB not opened in constructor";
    }
    */

    // Test - инициализация lineEdit_tableNow
    ui->lineEdit_tableNow->setText("Тип генератора?");
    //ui->lineEdit_tableNow->setModified(0);
    ui->lineEdit_tableNow->setReadOnly(1);
    // Настройка цвета
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,Qt::lightGray);
    palette->setColor(QPalette::Text,Qt::darkGray);
    ui->lineEdit_tableNow->setPalette(*palette);

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

// Глобальные переменные для работы с подключением к базе данных
QString Insert_Transfer_DB_Adress, Insert_Transfer_DB_Name, Insert_Transfer_DB_User, Insert_Transfer_DB_Password;
int Insert_Transfer_DB_Port;

// Глобальные переменные для формы вставки
int Insert_Table_Index;
int Insert_Fields_Number = 0, Insert_int_left = 0, Insert_int_right = 0, Insert_varchar_lenght, Insert_real_left, Insert_real_right, Insert_byteA_left, Insert_byteA_right, Insert_use_boolean;

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
            //rand_variant = engine() % Colvo_Unic;
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
                //rand_variant = engine() % Colvo_Unic;
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
    // Тесты
    //int test_int = 0;
    if (Insert_Fields_Number > 0)
    {
        /*
        //int br = Insert_Fields_Number;
        //int[br] test_int;
        //for(int i = 0; i < Insert_Fields_Number; ++i)
        //{
        //    test_int_mass[i] = 0;
        //}
        */
        if (Insert_int_left != Insert_int_right)
        {
            int *test_int_mass = new int[Insert_Fields_Number];
            /* old Testes
            //for (int i = 0; i < Insert_Fields_Number; ++i)
            //{
                //test_int = i;
                //test_int = VihrMersenna_Gen(Insert_int_left, Insert_int_right, engine);
                //test_int = engine() % 255;
                //test_int_mass[i] = test_int;
                //test_int_mass[i] = engine() % Insert_int_right - Insert_int_left;
                //Gen_result = gen_VM_left_edge + engine() % (gen_VM_right_edge - gen_VM_left_edge);
            //    test_int_mass[i] = Insert_int_left + engine() % (Insert_int_right - Insert_int_left);
            //    qDebug() << "Полученное значение = " << test_int_mass[i];
            //}
            */
            if(GenType == 1)
            {
                test_int_mass = VihrMersenna_Gen_Int(test_int_mass, Insert_Fields_Number, Insert_int_left, Insert_int_right);
            }
            else if (GenType == 2)
            {
                test_int_mass = MacLarenMarsalii_Gen_Int(test_int_mass, Insert_Fields_Number, Insert_int_left, Insert_int_right);
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
            }
            qDebug() << "Полученный массив rand int - " << test_int_mass;
            for (int i = 0; i < Insert_Fields_Number; ++i) {
                qDebug() << "Полученный элемент " << i << " равен = " << test_int_mass[i];
                // Test
                //char test_s;
                //test_int_mass[i] = 122;
                qDebug() << "Символ кода - " << char(test_int_mass[i]);
            }
            delete[] test_int_mass;
        }
        else
        {
            QMessageBox::critical(this, "ERROR", "Одинаковые целевые границы генератора!\n Пожалуйста, переопределите!");
        }
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Введено неккоректное количество новых строк!\n Пожалуйста, введите число от 1 до 10000.");
    }



    /* Старая реализация
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

    qDebug() << "Окно Insert закрыто вместе с сохранением данных в таблицу";
    // Возврат к основному окну
    // Временно закомменчено, чтобы не вылетала
    // Раскомменчена - вылеты были не из - за этого
    this->close();
    emit InWindow();
    */
}

void Dialog_SQL_Insert::on_pushButton_2_clicked()
{
    qDebug() << "Окно Insert закрыто без сохранения данных в таблицу";
    // Возврат к основному окну
    this->close();
    emit InWindow();
}

void Dialog_SQL_Insert::changeIndex(int i)
{
    i++;

    /*
    // Just a test
    //ui->comboBox_table_change->setCurrentIndex(1);
    //ui->comboBox_table_change->setItemText(1, "Hello_there!");
    */
}



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
    //std::mt19937_64 engine;
    //std::random_device device;
    //engine.seed(device());

    task_variant = ui->taskVariant->text();
    qDebug() << "Введёный номер варианта для задания - " << task_variant;
    if(task_variant == "0")
    {
        char t = 'N';
        int rand_variant_task;
        while(t == 'N')
        {
            //rand_variant_task = engine() % Colvo_Unic;
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


// Новая функция для получения данных о таблицах
void Dialog_SQL_Insert::get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types)
{
    // Для вывода матрицы пока что пользуюсь таким способом - при использовани count() или size() вылетает почему - то
    QString query_text;
    QSqlRecord Record_Test;
    QSqlQuery Query_Test;

    Insert_BD_Tables_List_Asked = DB_tables_list;
    qDebug() << "Таблицы в БД, полученный список - " << Insert_BD_Tables_List_Asked;
    Insert_Matrix_Tables_FieldNames = Matrix_Names;
    Insert_Matrix_Tables_FieldTypes = Matrix_Types;
    qDebug() << "Массив имён полей, полученные данные" << Insert_Matrix_Tables_FieldNames;
    qDebug() << "Массив типов полей, полученные данные" << Insert_Matrix_Tables_FieldTypes;
    for (int i = 0; i < Insert_BD_Tables_List_Asked.size(); ++i)
    {
    //int i = 1;
        query_text = "SELECT * FROM public.\"" + Insert_BD_Tables_List_Asked[i] + "\"";
        Query_Test.exec(query_text);
        Record_Test = Query_Test.record();
        //for (int j = 0; j < Insert_Matrix_Tables_FieldNames[i]->count() - 1; ++j)
        for(int j = 0; j < Record_Test.count(); ++j)
        {

            qDebug() << "Имя поля - " + Insert_Matrix_Tables_FieldNames[i][j] + "; Тип поля - " + Insert_Matrix_Tables_FieldTypes[i][j];
        }
    }
/*
    for (int i = 0; i < Insert_BD_Tables_List_Asked.size(); ++i)
    {
        for (int j = 0; j < Insert_Matrix_Tables_FieldTypes[i]->size(); ++j)
        {
            qDebug() << QString(i) + "." + QString(j) + " == " + Insert_Matrix_Tables_FieldTypes[i][j];
        }
    }
*/

    //Настройка combobox_table_change - old, перенесено в конструктор
    /*
    for (int i = 0; i < Insert_BD_Tables_List_Asked.size(); ++i)
    {
        //ui->comboBox_table_change->setDuplicatesEnabled(1);
        //QVariant justTest;
        //justTest.setValue(7);
        //ui->comboBox_table_change->InsertAtBottom(Insert_BD_Tables_List_Asked[i]);
        //ui->comboBox_table_change->setInsertPolicy(QComboBox::InsertAtBottom);
        //ui->comboBox_table_change->addItem("test_1", justTest);
        //ui->comboBox_table_change->insertItem(i+1, "test " + QString(i));
        //ui->comboBox_table_change->setItemText(0, "Hello");
        //ui->comboBox_table_change->setCurrentIndex(i + 1);
        //ui->comboBox_table_change->setCurrentText(Insert_BD_Tables_List_Asked[i]);

        ui->comboBox_table_change->setCurrentIndex(i+1);
        ui->comboBox_table_change->setCurrentText("Test_New_2");
        QStringList new_test_list;
        for (int i = 0; i < Insert_BD_Tables_List_Asked.size(); ++i)
        {
            new_test_list << Insert_BD_Tables_List_Asked[i];
        }
        ui->comboBox_table_change->addItems(new_test_list);
        qDebug() << "Установка в comboBox таблицы " << Insert_BD_Tables_List_Asked[i];
    }

    QStringList test_list;
    test_list << "test_1" << "Test_2";
    ui->comboBox_table_change->addItems(test_list);
    */

    //QStringList new_table_list;
    //new_table_list = SQL_Window_Main().take_list_of_Tables(new_table_list);
    //qDebug() << "New string list is" << new_table_list;
    //ui->comboBox_table_change->setCurrentIndex(1);
    //ui->comboBox_table_change->setItemText(1, "Hello_there!");
    //ui->comboBox_table_change->lineEdit()->setPlaceholderText("Hello there new");

    // Хрена с два этот Combobox позволит себя редачить вне конструктора
    // Как вариант - запрашивать данные о БД прямо в конструкторе и дальше инициализировать
    //Dialog_SQL_Insert();

    /*
    if (DB.open())
    {
    QSqlQueryModel *special_model = new QSqlQueryModel();
    QSqlQuery *special_query = new QSqlQuery();
    //special_query->prepare("SHOW TABLES");
    special_query->prepare("SELECT * FROM public.\"Students\"");
    special_query->exec();
    special_model->setQuery(*special_query);
    ui->comboBox_table_change->setModel(special_model);
    }
    else
    {
        qDebug() << "BD not opened";
    }
    */

    // Передача индекса текущей таблицы для вставки
    Insert_BD_Tables_List_Asked.sort();
    int vrem_number = 0;
    Insert_Table_Index = SQL_Window_Main().take_list_of_Tables(vrem_number);
    qDebug() << "Согласно полученным данным, вставка будет вестись в таблицу с индексом" << Insert_Table_Index;
    qDebug() << "Это таблица" << Insert_BD_Tables_List_Asked[Insert_Table_Index];
    //QString text_for_lineEdit = Insert_BD_Tables_List_Asked[Insert_Table_Index];
    //ui->lineEdit_tableNow->setText(text_for_lineEdit);
}

// Функция для подключения данных о подключении и установке соединения
void Dialog_SQL_Insert::get_DB_connection_from_MainWindow(QSqlDatabase DB_conn_data)
{
    DB = QSqlDatabase::cloneDatabase(DB_conn_data, "PostgreSQL_New_Connect");
    qDebug() << "Insert: Полученные данные про DB" << DB;
    Insert_Transfer_DB_Adress = DB.hostName();
    qDebug() << "Insert: Полученный адрес БД в Qstring - " << Insert_Transfer_DB_Adress;
    Insert_Transfer_DB_Port = DB.port();
    qDebug() << "Insert: Полученный port БД в Qstring" << QString::number(Insert_Transfer_DB_Port);
    Insert_Transfer_DB_Name = DB.databaseName();
    qDebug() << "Insert: Полученное имя БД в QString" << Insert_Transfer_DB_Name;
    Insert_Transfer_DB_User = DB.userName();
    qDebug() << "Insert: Полученный логин пользователя в QString" << Insert_Transfer_DB_User;
    Insert_Transfer_DB_Password = DB.password();
    qDebug() << "Insert: Полученный пароль пользователя в QString" << Insert_Transfer_DB_Password;

    qDebug() << "Проверка на открытие файла с запрошенными параметрами";
    DB = QSqlDatabase::addDatabase("QPSQL", "InsertWindowConnect");
    DB.setHostName(Insert_Transfer_DB_Adress);
    DB.setPort(Insert_Transfer_DB_Port);
    DB.setDatabaseName(Insert_Transfer_DB_Name);
    DB.setUserName(Insert_Transfer_DB_User);
    DB.setPassword(Insert_Transfer_DB_Password);
    if (DB.open())
    {
        qDebug() << "DB in Insert is open: " << DB;
    }
    else
    {
        qDebug() << "Error in Insert DB opening: " << DB.lastError();
    }
}

void Dialog_SQL_Insert::on_comboBox_algoritms_activated(int index)
{
    QString text_for_lineEdit = Insert_BD_Tables_List_Asked[Insert_Table_Index];
    ui->lineEdit_tableNow->setText(text_for_lineEdit);
    if (index == 0)
    {
        GenType = 0;
        qDebug() << "Тип генератора не выбран! GenType = " << GenType;
        ui->lineEdit_tableNow->setText("Тип генератора?");
    }
    if (index == 1)
    {
        GenType = 1;
        qDebug() << "Тип генератора - Вихрь Мерсенна, GenType = " << GenType;
    }
    if (index == 2)
    {
        GenType = 2;
        qDebug() << "Тип генератора - Макларена-Марсальи, GenType = " << GenType;
    }
}


void Dialog_SQL_Insert::on_lineEdit_kolvo_new_strok_editingFinished()
{
    Insert_Fields_Number = ui->lineEdit_kolvo_new_strok->text().toInt();
    qDebug() << "Количество строк, которые хочет внести пользователь - " << Insert_Fields_Number;
}

void Dialog_SQL_Insert::on_lineEdit_int_left_editingFinished()
{
    Insert_int_left = ui->lineEdit_int_left->text().toInt();
    qDebug() << "Левая граница типа Int для генератора - " << Insert_int_left;
}

void Dialog_SQL_Insert::on_lineEdit_int_right_editingFinished()
{
    Insert_int_right = ui->lineEdit_int_right->text().toInt();
    qDebug() << "Правая граница типа Int для генератора - " << Insert_int_right;

    //Just a test
    //ui->lineEdit_tableNow->setText("Hello");
}

void Dialog_SQL_Insert::on_lineEdit_varchar_lenght_editingFinished()
{
    Insert_varchar_lenght = ui->lineEdit_varchar_lenght->text().toInt();
    qDebug() << "Полученная длина последовательности VARCHAR - " << Insert_varchar_lenght;
}


void Dialog_SQL_Insert::on_lineEdit_real_left_editingFinished()
{
    Insert_real_left = ui->lineEdit_real_left->text().toInt();
    qDebug() << "Левая граница типа Real для генератора - " << Insert_real_left;
}


void Dialog_SQL_Insert::on_lineEdit_real_right_editingFinished()
{
    Insert_real_right = ui->lineEdit_real_right->text().toInt();
    qDebug() << "Правая граница типа Real для генератора - " << Insert_real_right;
}


void Dialog_SQL_Insert::on_lineEdit_bytea_left_editingFinished()
{
    Insert_byteA_left = ui->lineEdit_bytea_left->text().toInt();
    qDebug() << "Левая граница типа ByteA для генератора - " << Insert_byteA_left;

}


void Dialog_SQL_Insert::on_lineEdit_bytea_right_editingFinished()
{
    Insert_byteA_right = ui->lineEdit_bytea_right->text().toInt();
    qDebug() << "Правая граница типа ByteA для генератора - " << Insert_byteA_right;

}

int* Dialog_SQL_Insert::VihrMersenna_Gen_Int(int* getted_rand_mass, int rasmer, int gen_VM_left_edge, int gen_VM_right_edge)
{
    //int Gen_result = 0;
    //Gen_result = gen_VM_left_edge + engine() % (gen_VM_right_edge - gen_VM_left_edge);
    //Gen_result = Gen_engine() % gen_VM_right_edge - gen_VM_left_edge;
    //Gen_result = Gen_engine();
    //return Gen_result;

    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(device());
    //int N = sizeof(getted_rand_mass)/sizeof(int);
    //int N = sizeof(getted_rand_mass) / sizeof(*getted_rand_mass);
    //int N = _countof(getted_rand_mass);
    //int N = rasmer;
    for (int i = 0; i < rasmer; ++i)
    {
        //getted_rand_mass[i] = i;
        //qDebug() << "Элемент " + QString(i);
        //getted_rand_mass[i] = gen_VM_left_edge + engine() % (gen_VM_right_edge - gen_VM_left_edge);
        getted_rand_mass[i] = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
    }
    return getted_rand_mass;
    // Вроде как должно сменить зерно генерации, чтобы не было повторяющихся значений
    engine.seed();
}

int* Dialog_SQL_Insert::MacLarenMarsalii_Gen_Int(int* getted_rand_mass, int rasmer, int gen_MM_left_edge, int gen_MM_right_edge)
{
    // Инициализация привязки устройства
    std::ranlux24 engine;
    std::random_device device;
    engine.seed(device());
    // Инициализаций матрицы генератора
    int *matrix_3 = new int[1000];
    for (int i = 0; i < 1000; ++i)
    {
        matrix_3[i] = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
    }
    int P_3_1; int P_3_2, P_3_3;
    for (int i = 0; i < rasmer; ++i)
    {
        P_3_1 = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
        P_3_2 = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
        P_3_3 = (P_3_1 * P_3_2) % 1000;
        getted_rand_mass[i] = matrix_3[P_3_3];
        matrix_3[P_3_3] = P_3_1;
    }
    return getted_rand_mass;
    delete[] matrix_3;
    engine.seed();
}
