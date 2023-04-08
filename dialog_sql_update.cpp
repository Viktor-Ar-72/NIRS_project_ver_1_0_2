#include "dialog_sql_update.h"
#include "ui_dialog_sql_update.h"
#include <sql_window_main.h>
#include <QCompleter>
#include <QSqlRecord>
#include <QFile>
// Для преобразования типа переменной
#include <type_traits>
#include <QVariant>

//dialog_sql_update::dialog_sql_update(QWidget *parent) :
Dialog_SQL_Update::Dialog_SQL_Update(QWidget *parent) :
    QDialog(parent),
    //ui(new Ui::dialog_sql_update)
    ui(new Ui::Dialog_SQL_Update)
{
    ui->setupUi(this);

    //  Инициализация lineEdit_tableNow
    ui->lineEdit_tableNow->setText("Тип генератора?");
    //ui->lineEdit_tableNow->setModified(0);
    ui->lineEdit_tableNow->setReadOnly(1);
    // Настройка цвета
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,Qt::lightGray);
    palette->setColor(QPalette::Text,Qt::darkGray);
    ui->lineEdit_tableNow->setPalette(*palette);

    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);
}

//dialog_sql_update::~dialog_sql_update()
Dialog_SQL_Update::~Dialog_SQL_Update()
{
    delete ui;
}
// Предел генерирования случайного числа
//int Colvo_Unic = 100;
int Up_Colvo_Unic = 10;
// Глобальный массив для работы с уникальностью номеров
int *up_variants_number = new int[Up_Colvo_Unic];

// Для настройки соединения
// Для работы с присланными таблицами
// Список таблиц
QStringList Update_BD_Tables_List_Asked;
// Массив с типами полей каждой таблицы по номеру
QString **Update_Matrix_Tables_FieldTypes;
QString **Update_Matrix_Tables_FieldNames;
// Тип генератора
short Update_GenType;

// Глобальные переменные для работы с подключением к базе данных
QString Update_Transfer_DB_Adress, Update_Transfer_DB_Name, Update_Transfer_DB_User, Update_Transfer_DB_Password;
int Update_Transfer_DB_Port;
// Глобальные переменные для формы обновления данных
int Update_Table_Index;

// Глобальные переменные для работы с формами
//QString up_student_id, up_full_name, up_student_group, up_student_variant;
//QString up_task_id, up_task_complication, up_task_text, up_task_variant;
//int Up_Table_Index;

int Update_Fields_Start, Update_Fields_Finish, Update_int_left = 0, Update_int_right = 0, Update_varchar_lenght = 0,
Update_byteA_lenght = 0, Update_use_boolean;
double Update_real_left, Update_real_right;

void Dialog_SQL_Update::on_pushButton_OK_clicked()
{
    /*
     * UPDATE public."TestTable_1"
SET "Column_Int_1" = 5, "Column_Int_2" = 10, "Column_Text" = 'Test Update text', "Column_Bool" = true
WHERE "Column_Int_1" = 0
     *
     */

    //if ((Update_Fields_Start >= 0) && (Update_Fields_Finish > 0) && ((Update_Fields_Finish - Update_Fields_Start) >= 0))
    if ((Update_Fields_Start >= 0) && (Update_Fields_Finish > 0) && ((Update_Fields_Finish - Update_Fields_Start + 1) > 0))
    {
        // Создаём N-ое количество массивов, для каждого типа столбца текущей таблицы
        // Для вывода матрицы пока что пользуюсь таким способом - при использовани count() или size() вылетает почему - то
        QString query_text;
        QSqlRecord Record_Test;
        QSqlQuery Query_Test;
        query_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[Update_Table_Index] + "\"";
        Query_Test.exec(query_text);
        Record_Test = Query_Test.record();
        //for (int R = 0; R < Insert_Matrix_Tables_FieldNames[Insert_Table_Index]->count() - 1; ++R)
        // Переменные для расчёта количества элементов
        int k_int = 0; int k_varchar = 0; int k_real = 0; int k_bytea = 0; int k_boolean = 0;
        char int_use = 'n'; char char_use = 'n'; char real_use = 'n'; char bytea_use = 'n';
        char bool_use = 'n';
        for (int K = 0; K < Record_Test.count(); ++K)
        {
            if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][K] == "int")
            {
                k_int += 1;
                int_use = 'y';
            }
            if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][K] == "QString")
            {
                k_varchar += 1;
                char_use = 'y';
            }
            if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][K] == "double")
            {
                k_real += 1;
                real_use = 'y';
            }
            if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][K] == "bool")
            {
                k_boolean += 1;
                bool_use = 'y';
            }
            if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][K] == "ByteA")
            {
                k_bytea += 1;
                bytea_use = 'y';
            }
        }
        int *Update_int_mass = new int[((Update_Fields_Finish - Update_Fields_Start + 1) * k_int)];
        QString* Update_varchar_mass = new QString[((Update_Fields_Finish - Update_Fields_Start + 1) * k_varchar)];
        double* Update_real_mass = new double[((Update_Fields_Finish - Update_Fields_Start + 1) * k_real)];
        bool* Update_bool_mass = new bool[((Update_Fields_Finish - Update_Fields_Start + 1) * k_boolean)];
        QString* Update_bytea_mass = new QString[((Update_Fields_Finish - Update_Fields_Start + 1) * k_bytea)];

        if (int_use == 'y')
        {
            if ((Update_int_left != Update_int_right) && (ui->lineEdit_int_left->text() != "") && (ui->lineEdit_int_right->text() != "") && (Update_int_left < Update_int_right))
            {
                if(Update_GenType == 1)
                {
                    Update_int_mass = Up_VihrMersenna_Gen_Int(Update_int_mass, ((Update_Fields_Finish - Update_Fields_Start + 1) * k_int), Update_int_left, Update_int_right);
                }
                else if (Update_GenType == 2)
                {
                    Update_int_mass = Up_MacLarenMarsalii_Gen_Int(Update_int_mass, (Update_Fields_Finish - Update_Fields_Start + 1) * k_int, Update_int_left, Update_int_right);
                }
                else
                {
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand int - " << Update_int_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_int); ++i)
                {
                    qDebug() << "Полученный элемент INTEGER " << i << " равен = " << Update_int_mass[i];

                }

            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Нарушение целевых границ генерируемой последовательности INT!\n Пожалуйста, переопределите!");
                return;
            }
        }

        // Заполнение генератора REAL
        if (real_use == 'y')
        {
            if ((Update_real_left != Update_real_right) && (ui->lineEdit_real_left->text() != "") && (ui->lineEdit_real_right->text() != "") && (Update_real_left < Update_real_right))
            {
                if(Update_GenType == 1)
                {
                    Update_real_mass = Up_VihrMersenna_Gen_Real(Update_real_mass, ((Update_Fields_Finish - Update_Fields_Start + 1)* k_real), Update_real_left, Update_real_right);
                }
                else if (Update_GenType == 2)
                {
                    Update_real_mass = Up_MacLarenMarsalii_Gen_Real(Update_real_mass, ((Update_Fields_Finish - Update_Fields_Start + 1) * k_real), Update_real_left, Update_real_right);
                }
                else
                {
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand real - " << Update_real_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_real); ++i)
                {
                    qDebug() << "Полученный REAL элемент " << i << " равен = " << Update_real_mass[i];
                }

            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Нарушение целевых границ генерируемой последовательности REAL!\n Пожалуйста, переопределите!");
                return;
            }
        }

        if (char_use == 'y')
        {
            // Заполнение генератора Varchar
            //if ((Update_int_left != Update_int_right) && (ui->lineEdit_int_left->text() != "") && (ui->lineEdit_int_right->text() != ""))
            if(Update_varchar_lenght != 0)
            {
                if(Update_GenType == 1)
                {
                    Update_varchar_mass = Up_VihrMersenna_Gen_Char(Update_varchar_mass, ((Update_Fields_Finish - Update_Fields_Start + 1) * k_varchar), Update_varchar_lenght, 0, 125);
                }
                else if (Update_GenType == 2)
                {
                    Update_varchar_mass = Up_MacLarenMarsalii_Gen_Char(Update_varchar_mass, ((Update_Fields_Finish - Update_Fields_Start + 1) * k_varchar), Update_varchar_lenght, 0, 125);
                }
                else
                {
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand varchar - " << Update_varchar_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_varchar); ++i)
                {
                    qDebug() << "Полученный элемент VARCHAR " << i << " равен = " << Update_varchar_mass[i];
                }
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не указаны размер последовательности VARCHAR!\n Пожалуйста, переопределите!");
                return;
            }
        }

        if (bool_use == 'y')
        {
            // Заполнение генератора BOOL
            if(Update_use_boolean != 0)
            {
                int* dop_intBool_mass = new int[((Update_Fields_Finish - Update_Fields_Start + 1) * k_boolean)];
                if(Update_GenType == 1)
                {
                    dop_intBool_mass = Up_VihrMersenna_Gen_Int(dop_intBool_mass, ((Update_Fields_Finish - Update_Fields_Start + 1) * k_boolean), 0, 1);
                }
                else if (Update_GenType == 2)
                {
                    dop_intBool_mass = Up_MacLarenMarsalii_Gen_Int(dop_intBool_mass, ((Update_Fields_Finish - Update_Fields_Start + 1) * k_boolean), 0, 1);
                }
                else
                {
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand intBool - " << dop_intBool_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_boolean); ++i)
                {
                    if (dop_intBool_mass[i] == 0)
                    {
                        Update_bool_mass[i] = false;
                    }
                    else
                    {
                        Update_bool_mass[i] = true;
                    }
                    qDebug() << "Полученный элемент BOOL " << i << " равен = " << Update_bool_mass[i];
                }
            }
            else
            {
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_boolean); ++i)
                {
                    Update_bool_mass[i] = true;
                }
            }
        }
        if (bytea_use == 'y')
        {
            //qDebug() << "Будет использоватьмся массив byteA";
            if(Update_byteA_lenght != 0)
            {
                if(Update_GenType == 1)
                {
                    Update_bytea_mass = Up_VihrMersenna_Gen_ByteA(Update_bytea_mass, ((Update_Fields_Finish - Update_Fields_Start + 1) * k_bytea), Update_byteA_lenght, 0, 90);
                }
                else if (Update_GenType == 2)
                {
                    Update_bytea_mass = Up_MacLarenMarsalii_Gen_ByteA(Update_bytea_mass, ((Update_Fields_Finish - Update_Fields_Start + 1) * k_bytea), Update_byteA_lenght, 0, 90);
                }
                else
                {
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand bytea - " << Update_bytea_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_bytea); ++i)
                {
                    qDebug() << "Полученный элемент BYTEA " << i << " равен = " << Update_bytea_mass[i];
                }
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не указаны размер последовательности BYTEA!\n Пожалуйста, переопределите!");
                return;
            }
        }


        QSqlQueryModel SQL_Querry_Model;
        QString query_U_borders;
        query_U_borders = "SELECT * FROM public.";
        query_U_borders += '"' + Update_BD_Tables_List_Asked[Update_Table_Index] + '"' + " ORDER BY" + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][0] + '"';
        qDebug() << "Запрос на поиск нужной строки - " << query_U_borders;
        SQL_Querry_Model.setQuery(query_U_borders);

        // Test 1
        //QString Test_string = "int";
        //std::conditional<Test_string == "int", int, double>::type k;
        // Test 2
        /*
        QString Test_string = "int";
        QVariant value;
        switch (qHash(Test_string)) {
            case qHash("int"):
                value = QVariant::fromValue<int>(0);
                break;
            case qHash("double"):
                value = QVariant::fromValue<double>(0.0);
                break;
            case qHash("QString"):
                value = QVariant::fromValue<QString>("");
                break;
            default:
                // неизвестный тип
                break;
        }
        */
        // Test 3
        /*
        enum Type { INT, DOUBLE, STRING };
        Type type = INT;
        switch(type) {
            case INT:
                int x = 0;
                break;
            case DOUBLE:
                double y = 0.0;
                break;
            case STRING:
                QString s = "";
                break;
            default:
                break;
        }
        */

        QString* Update_Where_Values = new QString[(Update_Fields_Finish - Update_Fields_Start + 1)];

        //for (int i = 0; i < SQL_Querry_Model.rowCount(); ++i)
        int m = 0;
        for (int i = 0; i < SQL_Querry_Model.rowCount(); ++i)
        {
            //int m = 0; //qDebug() << "Строка " << i;
            if (i >= Update_Fields_Start && i <= Update_Fields_Finish)
            {
                qDebug() << "Найдена " << m << " строка для обновления на " << i << " месте";
                //qDebug() << SQL_Querry_Model.record().value(Update_Matrix_Tables_FieldNames[Update_Table_Index][0]).toString();
                qDebug() << SQL_Querry_Model.record(i).value(Update_Matrix_Tables_FieldNames[Update_Table_Index][0]).toString();
                Update_Where_Values[m] = SQL_Querry_Model.record(i).value(Update_Matrix_Tables_FieldNames[Update_Table_Index][0]).toString();
                m += 1;
            }



            //int ID = SQL_Querry_Model.record(i).value("ID").toInt();
            //QString Data = SQL_Querry_Model.record(i).value("Data").toString();
            //qDebug() << ID << Data;
        }

        // Вставка значений в таблицу через Update
        // Testes
        //QString query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" (Column_Int_1, Column_Int_2, Column_Text, Column_Bool) VALUES ('0'::integer, '0'::integer, 'test_insert'::text, 'TRUE'::boolean)";
        // Такая реализаия работает. Надо придумать, как редактировать состав вставляемых данных
        //QString query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" VALUES (0, 0, 'Test_insert_1', TRUE)";
        QSqlRecord Update_Record;
        QSqlQuery Query_Update;
        QString query_vrem_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[Update_Table_Index] + "\"";
        Query_Update.exec(query_vrem_text);
        Update_Record = Query_Update.record();
        //int h = Record_Test.count();
        //qDebug() << "В данной таблице необходимо вставить " << h << " столбцов";
        QString query_Update_text;
        QString boolean_type_QString;
        //for (int V = 0; V < (Update_Fields_Finish - Update_Fields_Start); ++V)
        for (int V = 0; V < (Update_Fields_Finish - Update_Fields_Start + 1); ++V)
        {
            //query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" VALUES (";
            //query_insert_text = "";
            // Создание шаблона запроса. И снова спасибо ПЗЕ4:)
            //query_Update_text = "Insert INTO public.";
            // Test
            query_Update_text = "UPDATE public.";
            query_Update_text += '"';
            query_Update_text = query_Update_text + Update_BD_Tables_List_Asked[Update_Table_Index];
            query_Update_text.append('"');
            query_Update_text = query_Update_text + " SET ";

            // Создание дополнительных переменных для доп.циклов
            int start_index_int = V * k_int;
            //int end_index_int = start_index_int + k_int;
            int start_index_varchar = V * k_varchar;
            int start_index_real = V * k_real;
            int start_index_bool = V * k_boolean;
            int start_index_bytea = V * k_bytea;

            for (int A = 0; A < Update_Record.count(); ++A)
            {

                if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][A] == "int")
                {
                    //query_insert_text = query_insert_text + QString(Insert_int_mass[Insert_Fields_Number]) + ", ";
                    //query_insert_text = query_insert_text + "Нужно в столбец " + QString::number(A) + " добавить значение int, ";
                    //query_insert_text = query_insert_text + QString::number(Insert_int_mass[V]) + ", ";
                    query_Update_text = query_Update_text + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][A] + '"' + " = " + QString::number(Update_int_mass[start_index_int]) + ", ";
                    ++start_index_int;
                }
                else if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][A] == "QString")
                {
                    //query_insert_text = query_insert_text + Insert_varchar_mass[Insert_Fields_Number] + ", ";
                    //query_insert_text = query_insert_text + "Нужно в столбец " + QString::number(A) + " добавить значение QString, ";
                    //query_insert_text = query_insert_text + "'" + Insert_varchar_mass[V] + "', ";
                    query_Update_text = query_Update_text + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][A] + '"' + " = '" + Update_varchar_mass[start_index_varchar] + "', ";
                    ++start_index_varchar;
                }
                else if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][A] == "double")
                {
                    // Сохранение точности до определённого знака после запятой
                    query_Update_text = query_Update_text + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][A] + '"' + " = "  + QString::number(Update_real_mass[start_index_real], 'g', 7) + ", ";
                    ++start_index_real;
                }
                else if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][A] == "ByteA")
                {
                    //query_insert_text = query_insert_text + "'" + R"(\x)" + Insert_bytea_mass[start_index_bytea] + "', ";
                    query_Update_text = query_Update_text + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][A] + '"' + " = " + "'";
                    query_Update_text += QString::fromUtf8("\x5C\x78");
                    // int index = query_insert_text.lastIndexOf("\\");
                    // query_insert_text.replace(index, 1, "\x5C\x78");
                    // query_insert_text.remove(0, 1);
                    // query_insert_text.prepend("'");
                    query_Update_text = query_Update_text + Update_bytea_mass[start_index_bytea] + "', ";
                    ++start_index_bytea;
                    // Test byteA input check
                }
                else if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][A] == "bool")
                {
                    //query_insert_text = query_insert_text + Insert_bool_mass[Insert_Fields_Number] + ")";
                    //query_insert_text = query_insert_text + "Нужно в столбец " + QString::number(A) + " добавить значение bool, ";
                    //QString q = "";
                    boolean_type_QString = "";
                    //if (Insert_bool_mass[V] == true)
                    if (Update_bool_mass[start_index_bool] == true)
                    { boolean_type_QString = "TRUE"; }
                    else
                    { boolean_type_QString = "FALSE"; }
                    query_Update_text = query_Update_text + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][A] + '"' + " = " + boolean_type_QString + ", ";
                    ++start_index_bool;
                }
                else {
                    // На всякий случай заккоментил, чтобы не было лишнего Warning
                    //query_insert_text = query_insert_text;
                }
            }
            // Убираем последнюю запятую (спасибо ПЗЕ4 :) )
            //query_insert_text.remove((query_insert_text.length() - 1), 3);
            //query_insert_text = query_insert_text + ");";
            int position = query_Update_text.lastIndexOf(QChar(','));
            query_Update_text = query_Update_text.left(position);
            // Не требуется, так как у нас не Insert - запрос
            //query_Update_text = query_Update_text.append(");");
            // Дополнение запроса Update
            if ((Update_Matrix_Tables_FieldTypes[Update_Table_Index][0] == "int") || (Update_Matrix_Tables_FieldTypes[Update_Table_Index][0] == "bool") || (Update_Matrix_Tables_FieldTypes[Update_Table_Index][0] == "double"))
            {
                query_Update_text = query_Update_text + " WHERE " + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][0] + '"' + " = " + Update_Where_Values[V];
            }
            else
            {
                query_Update_text = query_Update_text + " WHERE " + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][0] + '"' + " = '" + Update_Where_Values[V] + "'";

            }
            qDebug() << "Запрос на вставку" << V << query_Update_text;
            QFile file("output_Update_Query_Test.txt");
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << query_Update_text; // записываем содержимое строки в файл
                file.close();
            }


            // Если временно закомменчено - значит, происходит тест на обновление данных
            //QUERY_MODEL->setQuery(query_Update_text);
            //qDebug() << "Запрос на вставку" << V << "выполнен.";
        }



        //query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" VALUES (" + QString(rand() % 255) + ", " + QString(rand() % 255) + ", 'Test_insert_1', TRUE)";
        // Сверка с генерированными запросами
        //query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" VALUES (0, 0, 'Test_insert_1', TRUE)";
        //qDebug() << "Запрос на вставку" << 9 << query_insert_text;

        query_Update_text = ""; // Заглушка
        QUERY_MODEL->setQuery(query_Update_text);
        // Закрытие окна вставки данных
        qDebug() << "Окно Update закрыто вместе с сохранением данных в таблицу";
        // Очистка массивов
        delete [] Update_int_mass;
        delete [] Update_varchar_mass;
        delete [] Update_real_mass;
        delete [] Update_bytea_mass;
        delete [] Update_bool_mass;
        // Возврат к основному окну
        this->close();
        emit UpWindow();
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Не указаны границы обновляемых записей!\n Пожалуйста, укажите!");
        return;
    }
    /* OLD_UPDATE
    //qDebug() << "44";
    if (Up_Table_Index == 0)
    {
        if(up_student_id == "")
        {
            // Сообщение - предупреждение пользователю
            QMessageBox::critical(this, "ERROR", "Не указан идентификатор студента. \n Изменения не будут сохранены.");
            // Возврат к основному окну
            this->close();
            emit UpWindow();
        }
        // UPDATE public."Students" SET full_name = 'Test_Update'::text, student_group = 'Test_Update'::text, variant = '8'::integer WHERE student_id = 9;
        else
        {
            if(up_full_name == "")
            {

                // Old tests
                //QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE student_id = "+up_student_id);
                //QUERY_MODEL->
                //up_full_name = QString::number(QUERY_MODEL->rowCount());

                QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE student_id = " + up_student_id);
                up_full_name = QUERY_MODEL->record(0).value("full_name").toString();
                qDebug() << "Текущее значение up_full_name = " << up_full_name;
            }
            if(up_student_group == "")
            {
                QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE student_id = " + up_student_id);
                up_student_group = QUERY_MODEL->record(0).value("student_group").toString();
                qDebug() << "Текущее значение up_student_group = " << up_student_group;
            }
            if(up_student_variant == "")
            {
                QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE student_id = " + up_student_id);
                up_student_variant = QUERY_MODEL->record(0).value("variant").toString();
                qDebug() << "Текущее значение up_student_variant = " << up_student_variant;
            }

            QUERY_MODEL->setQuery("UPDATE public.\"Students\" SET "
                                  "full_name = '" + up_full_name + "'::text, "
                                  "student_group = '" + up_student_group + "'::text, "
                                  "variant = '" + up_student_variant + "'::integer "
                                  "WHERE student_id = " + up_student_id +";");
            qDebug() << "Success Update Query for Students";
            // Очистка окон - но временно закомментил, чтобы пользователь в случае чего мог понять, с какими данными он работал раньше
            //ui->lineEdit_student_id->clear();
            //ui->lineEdit_task_id->clear();
            // Возврат к основному окну
            this->close();
            emit UpWindow();
        }
    }
    if (Up_Table_Index == 1)
    {
        if(up_task_id == "")
        {
            // Сообщение - предупреждение пользователю
            QMessageBox::critical(this, "ERROR", "Не указан идентификатор задания. \n Изменения не будут сохранены.");
            // Возврат к основному окну
            this->close();
            emit UpWindow();
        }
        else
        {
            if(up_task_complication == "")
            {
                QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE task_id = " + up_task_id);
                up_task_complication = QUERY_MODEL->record(0).value("complication").toString();
                qDebug() << "Текущее значение up_task_complication = " << up_task_complication;
            }
            if(up_task_text == "")
            {
                QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE task_id = " + up_task_id);
                up_task_text = QUERY_MODEL->record(0).value("task_text").toString();
                qDebug() << "Текущее значение up_task_text = " << up_task_text;
            }
            if(up_task_variant == "")
            {
                QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE task_id = " + up_task_id);
                up_task_variant = QUERY_MODEL->record(0).value("var_number").toString();
                qDebug() << "Текущее значение up_task_variant = " << up_task_variant;
            }

            QUERY_MODEL->setQuery("UPDATE public.\"Tasks\" SET "
                                  "complication = '" + up_task_complication + "'::integer, "
                                  "task_text = '" + up_task_text + "'::text, "
                                  "var_number = '" + up_task_variant + "'::integer "
                                  "WHERE task_id = " + up_task_id +";");
            qDebug() << "Success Update Query for Tasks";
            // Возврат к основному окну
            this->close();
            emit UpWindow();
        }
    }
    */
}


void Dialog_SQL_Update::on_pushButton_Esc_clicked()
{

    // Возврат к основному окну
    this->close();
    emit UpWindow();
}


void Dialog_SQL_Update::on_tabWidget_currentChanged(int index)
{

    if(index == 0)
    {
        // Сообщение - предупреждение пользователю
        QMessageBox::warning(this, "WARNING", "Будьте осторожны, вводя новые данные. \n Изменения будут применены сразу.\n Вводите только те данные, которые хотите обновить.");
        qDebug() << "Страница обновления данных в выбранной таблице";
        //Up_Table_Index = 0;
        //qDebug() << "Обновление в таблице Students " << Up_Table_Index;
    }
    else if (index == 1)
    {
        //Up_Table_Index = 1;
       // qDebug() << "Обновление в таблице Tasks " << Up_Table_Index;
        qDebug() << "Окно справки";
    }
}

void Dialog_SQL_Update::changeIndex(int i)
{    i++;    }

void Dialog_SQL_Update::on_tabWidget_Update_currentChanged(int index)
{

    if(index == 0)
    {
        // Сообщение - предупреждение пользователю
        QMessageBox::warning(this, "WARNING", "Будьте осторожны, вводя новые данные. \n Изменения будут применены сразу.\n Вводите только те данные, которые хотите обновить.");

        //Up_Table_Index = 0;
        //qDebug() << "Обновление в таблице Students " << Up_Table_Index;
        qDebug() << "Окно обновления данных в выбранной таблице";
    }
    else if (index == 1)
    {
        //Up_Table_Index = 1;
        //qDebug() << "Обновление в таблице Tasks " << Up_Table_Index;
        qDebug() << "Окно справки в форме Update";
    }
}

// Новая функция для получения данных о таблицах
void Dialog_SQL_Update::get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types)
{
    // Для вывода матрицы пока что пользуюсь таким способом - при использовани count() или size() вылетает почему - то
    QString query_text;
    QSqlRecord Record_Test;
    QSqlQuery Query_Test;

    Update_BD_Tables_List_Asked = DB_tables_list;
    qDebug() << "Таблицы в БД, полученный список - " << Update_BD_Tables_List_Asked;
    Update_Matrix_Tables_FieldNames = Matrix_Names;
    Update_Matrix_Tables_FieldTypes = Matrix_Types;
    qDebug() << "Массив имён полей, полученные данные" << Update_Matrix_Tables_FieldNames;
    qDebug() << "Массив типов полей, полученные данные" << Update_Matrix_Tables_FieldTypes;
    for (int i = 0; i < Update_BD_Tables_List_Asked.size(); ++i)
    {
    //int i = 1;
        query_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[i] + "\"";
        Query_Test.exec(query_text);
        Record_Test = Query_Test.record();
        //for (int j = 0; j < Insert_Matrix_Tables_FieldNames[i]->count() - 1; ++j)
        for(int j = 0; j < Record_Test.count(); ++j)
        {

            qDebug() << "Имя поля - " + Update_Matrix_Tables_FieldNames[i][j] + "; Тип поля - " + Update_Matrix_Tables_FieldTypes[i][j];
        }
    }

    // Передача индекса текущей таблицы для вставки
    Update_BD_Tables_List_Asked.sort();
    int vrem_number = 0;
    Update_Table_Index = SQL_Window_Main().take_list_of_Tables(vrem_number);
    qDebug() << "Согласно полученным данным, обновление будет вестись в таблицу с индексом" << Update_Table_Index;
    qDebug() << "Это таблица" << Update_BD_Tables_List_Asked[Update_Table_Index];
    //QString text_for_lineEdit = Insert_BD_Tables_List_Asked[Insert_Table_Index];
    //ui->lineEdit_tableNow->setText(text_for_lineEdit);
}

// Функция для подключения данных о подключении и установке соединения
void Dialog_SQL_Update::get_DB_connection_from_MainWindow(QSqlDatabase DB_conn_data)
{
    DB = QSqlDatabase::cloneDatabase(DB_conn_data, "PostgreSQL_New_Connect_Update");
    qDebug() << "Update: Полученные данные про DB" << DB;
    Update_Transfer_DB_Adress = DB.hostName();
    qDebug() << "Update: Полученный адрес БД в Qstring - " << Update_Transfer_DB_Adress;
    Update_Transfer_DB_Port = DB.port();
    qDebug() << "update: Полученный port БД в Qstring" << QString::number(Update_Transfer_DB_Port);
    Update_Transfer_DB_Name = DB.databaseName();
    qDebug() << "Update: Полученное имя БД в QString" << Update_Transfer_DB_Name;
    Update_Transfer_DB_User = DB.userName();
    qDebug() << "Update: Полученный логин пользователя в QString" << Update_Transfer_DB_User;
    Update_Transfer_DB_Password = DB.password();
    qDebug() << "Update: Полученный пароль пользователя в QString" << Update_Transfer_DB_Password;

    qDebug() << "Проверка на открытие файла с запрошенными параметрами";
    DB = QSqlDatabase::addDatabase("QPSQL", "UpdateWindowConnect");
    DB.setHostName(Update_Transfer_DB_Adress);
    DB.setPort(Update_Transfer_DB_Port);
    DB.setDatabaseName(Update_Transfer_DB_Name);
    DB.setUserName(Update_Transfer_DB_User);
    DB.setPassword(Update_Transfer_DB_Password);
    if (DB.open())
    {
        qDebug() << "DB in Update is open: " << DB;
    }
    else
    {
        qDebug() << "Error in Update DB opening: " << DB.lastError();
    }
}

void Dialog_SQL_Update::on_comboBox_algoritms_activated(int index)
{
    QString text_for_lineEdit = Update_BD_Tables_List_Asked[Update_Table_Index];
    ui->lineEdit_tableNow->setText(text_for_lineEdit);
    if (index == 0)
    {
        Update_GenType = 0;
        qDebug() << "Тип генератора не выбран! Update_GenType = " << Update_GenType;
        ui->lineEdit_tableNow->setText("Тип генератора?");
    }
    if (index == 1)
    {
        Update_GenType = 1;
        qDebug() << "Тип генератора - Вихрь Мерсенна, Update_GenType = " << Update_GenType;
    }
    if (index == 2)
    {
        Update_GenType = 2;
        qDebug() << "Тип генератора - Макларена-Марсальи, GenType = " << Update_GenType;
    }
}

void Dialog_SQL_Update::on_lineEdit_strok_start_editingFinished()
{
    /*
    int vrem;
    vrem = ui->lineEdit_kolvo_new_strok->text().toInt();
    if ((vrem < 1) || (vrem > 300000))
    {
        QMessageBox::critical(this, "ERROR", "Введено неккоректное количество новых строк!\n Пожалуйста, введите число от 1 до 300 000.");
        Insert_Fields_Number = 0;
        ui->lineEdit_kolvo_new_strok->clear();
    }
    else
    {
        Insert_Fields_Number = vrem;
    }
    qDebug() << "Количество строк, которые хочет внести пользователь - " << Insert_Fields_Number;
    */
    Update_Fields_Start = ui->lineEdit_strok_start->text().toInt();
    qDebug() << "Номер строки, с которой нужно обновить данные - " << Update_Fields_Start;
}


void Dialog_SQL_Update::on_lineEdit_strok_end_editingFinished()
{
    Update_Fields_Finish = ui->lineEdit_strok_end->text().toInt();
    qDebug() << "Номер строки, до которой нужно обновить данные - " << Update_Fields_Finish;
}


void Dialog_SQL_Update::on_lineEdit_int_left_editingFinished()
{
    Update_int_left = ui->lineEdit_int_left->text().toInt();
    qDebug() << " Левая граница для генератора INT - " << Update_int_left;
}


void Dialog_SQL_Update::on_lineEdit_int_right_editingFinished()
{
    Update_int_right = ui->lineEdit_int_right->text().toInt();
    qDebug() << " Правая граница для генератора INT - " << Update_int_right;
}


void Dialog_SQL_Update::on_lineEdit_varchar_lenght_editingFinished()
{
    Update_varchar_lenght = ui->lineEdit_varchar_lenght->text().toInt();
    qDebug() << "Полученная длина последовательности VARCHAR - " << Update_varchar_lenght;
}


void Dialog_SQL_Update::on_lineEdit_real_left_editingFinished()
{
    Update_real_left = ui->lineEdit_real_left->text().toDouble();
    qDebug() << "Левая граница типа REAL для генератора - " << Update_real_left;
}


void Dialog_SQL_Update::on_lineEdit_real_right_editingFinished()
{
    Update_real_right = ui->lineEdit_real_right->text().toDouble();
    qDebug() << "Правая граница типа REAL для генератора - " << Update_real_right;
}


void Dialog_SQL_Update::on_lineEdit_bytea_lenght_editingFinished()
{
    Update_byteA_lenght = ui->lineEdit_bytea_lenght->text().toInt();
    qDebug() << "Полученная длина последовательности Bytea / Blob - " << Update_byteA_lenght;
}


void Dialog_SQL_Update::on_checkBox_useBoolGen_clicked()
{
    if (ui->checkBox_useBoolGen->isChecked())
    {
        Update_use_boolean = 1;
        qDebug() << "Будет использоваться генератор булевых значений";
    }
    else
    {
        Update_use_boolean = 0;
        qDebug() << "Не будет использоваться генератор булевых значений";
    }
}


int* Dialog_SQL_Update::Up_VihrMersenna_Gen_Int(int* getted_rand_mass, int rasmer, int gen_VM_left_edge, int gen_VM_right_edge)
{
    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(device());
    for (int i = 0; i < rasmer; ++i)
    {
        getted_rand_mass[i] = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
    }
    return getted_rand_mass;
    engine.seed();
}

int* Dialog_SQL_Update::Up_MacLarenMarsalii_Gen_Int(int* getted_rand_mass, int rasmer, int gen_MM_left_edge, int gen_MM_right_edge)
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

double* Dialog_SQL_Update::Up_VihrMersenna_Gen_Real(double *getted_rand_mass, int rasmer, double gen_VM_left_edge, double gen_VM_right_edge)
{
    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(device());;
    // Расчёт длины диапазона
    double range = gen_VM_right_edge - gen_VM_left_edge;
    for (int i = 0; i < rasmer; ++i)
    {
        double rand_num = double(engine()) / engine.max(); // Случайное число от 0 до 1
        getted_rand_mass[i] = gen_VM_left_edge + range * rand_num;
    }
    return getted_rand_mass;
    engine.seed();
}

double* Dialog_SQL_Update::Up_MacLarenMarsalii_Gen_Real(double *getted_rand_mass, int rasmer, double gen_MM_left_edge, double gen_MM_right_edge)
{
    // Инициализация привязки устройства
    std::ranlux24 engine;
    std::random_device device;
    engine.seed(device());
    // Генерация первых 2-х случайных чисел
    double u1 = engine() / double(engine.max());
    double u2 = engine() / double(engine.max());
    for (int i = 0; i < rasmer; ++i)
    {
        double u1_prev = u1;
        u1 = u2;
        u2 = engine() / double(engine.max());
        double z1 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        double z2 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
        double rand_num = gen_MM_left_edge + ((z1 + z2) / 2.0) * (gen_MM_right_edge - gen_MM_left_edge);
        if (rand_num < gen_MM_left_edge || rand_num > gen_MM_right_edge) {
            do {
                u1_prev = u1;
                u1 = u2;
                u2 = engine() / double(engine.max());
                z1 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
                z2 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
                rand_num = gen_MM_left_edge + ((z1 + z2) / 2.0) * (gen_MM_right_edge - gen_MM_left_edge);
            } while (rand_num < gen_MM_left_edge || rand_num > gen_MM_right_edge);
        }
        getted_rand_mass[i] = rand_num;
    }
    engine.seed();
    return getted_rand_mass;
}

QString* Dialog_SQL_Update::Up_VihrMersenna_Gen_Char(QString *massiv_gen_numbers, int rasmer, int length, int gen_VM_left_edge, int gen_VM_right_edge)
{
    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed();
    int t = 0; bool r = 0; QString s;
    for (int i = 0; i < rasmer; ++i)
    {
        s = "";
        for (int j = 0; j < length; ++j)
        {
            r = 0;
            while(r == 0)
            {
                t = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
                if ((t == 32) || ((t > 64) && (t < 91)) || ((t > 96) && (t < 123)))
                {
                    s = s + QString(t);
                    r = 1;
                }
                else
                {
                    r = 0;
                }
            }
        }
        massiv_gen_numbers[i] = s;
    }
    engine.seed();
    return massiv_gen_numbers;
}

QString* Dialog_SQL_Update::Up_MacLarenMarsalii_Gen_Char(QString *massiv_gen_numbers, int rasmer, int length, int gen_MM_left_edge, int gen_MM_right_edge)
{
    // Инициализация привязки устройства
    std::ranlux24 engine;
    std::random_device device;
    engine.seed(device());
    // Инициализаций матрицы генератора
    int *matrix_3 = new int[1000];
    int init; bool r;
    for (int i = 0; i < 1000; ++i)
    {
        r = 0;
        while(r == 0)
        {
            init = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
            if ((init == 32) || ((init > 64) && (init < 91)) || ((init > 96) && (init < 123)))
            {
                matrix_3[i] = init;
                r = 1;
            }
            else
            {
                r = 0;
            }
        }
    }
    int P_3_1; int P_3_2, P_3_3; QString word;
    for (int i = 0; i < rasmer; ++i)
    {
        word = "";
        for (int j = 0; j < length; ++j)
        {

            r = 0;
            while (r == 0)
            {
                P_3_1 = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
                P_3_2 = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
                if ((P_3_1 == 32) || ((P_3_1 > 64) && (P_3_1 < 91)) || ((P_3_1 > 96) && (P_3_1 < 123)))
                {
                    P_3_3 = (P_3_1 * P_3_2) % 1000;
                    word = word + QString(matrix_3[P_3_3]);
                    matrix_3[P_3_3] = P_3_1;
                    r = 1;
                }
                else
                {
                    r = 0;
                }
            }
        }
        massiv_gen_numbers[i] = word;
    }
    delete[] matrix_3;
    engine.seed();
    return massiv_gen_numbers;
}

QString* Dialog_SQL_Update::Up_VihrMersenna_Gen_ByteA(QString *massiv_gen_numbers, int rasmer, int length, int gen_VM_left_edge, int gen_VM_right_edge)
{
    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed();
    int t = 0; bool r = 0; QString s;
    for (int i = 0; i < rasmer; ++i)
    {
        s = "";
        for (int j = 0; j < length; ++j)
        {
            r = 0;
            while(r == 0)
            {
                t = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
                if (((t > 47) && (t < 58)) || ((t > 64) && (t < 71)))
                {
                    s = s + QString(t);
                    r = 1;
                }
                else
                {
                    r = 0;
                }
            }
        }
        massiv_gen_numbers[i] = s;
    }
    engine.seed();
    return massiv_gen_numbers;
}

QString* Dialog_SQL_Update::Up_MacLarenMarsalii_Gen_ByteA(QString *massiv_gen_numbers, int rasmer, int length, int gen_MM_left_edge, int gen_MM_right_edge)
{
    // Инициализация привязки устройства
    std::ranlux24 engine;
    std::random_device device;
    engine.seed(device());
    // Инициализаций матрицы генератора
    int *matrix_3 = new int[1000];
    int init; bool r;
    for (int i = 0; i < 1000; ++i)
    {
        r = 0;
        while(r == 0)
        {
            init = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
            if (((init > 47) && (init < 58)) || ((init > 64) && (init < 71)))
            {
                matrix_3[i] = init;
                r = 1;
            }
            else
            {
                r = 0;
            }
            //}
        }
    }
    int P_3_1; int P_3_2, P_3_3; QString word;
    for (int i = 0; i < rasmer; ++i)
    {
        word = "";
        for (int j = 0; j < length; ++j)
        {

            r = 0;
            while (r == 0)
            {
                P_3_1 = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
                P_3_2 = gen_MM_left_edge + engine() % (gen_MM_right_edge + 1 - gen_MM_left_edge);
                if (((P_3_1 > 47) && (P_3_1 < 58)) || ((P_3_1 > 64) && (P_3_1 < 71)))
                {
                    P_3_3 = (P_3_1 * P_3_2) % 1000;
                    word = word + QString(matrix_3[P_3_3]);
                    matrix_3[P_3_3] = P_3_1;
                    r = 1;
                }
                else
                {
                    r = 0;
                }
            }
        }
        massiv_gen_numbers[i] = word;
    }
    delete[] matrix_3;
    engine.seed();
    return massiv_gen_numbers;
}



/* OLD UPDATE_Students_Tasks
void dialog_sql_update::on_lineEdit_student_id_editingFinished()
{
    // Построение массива на количество уникальных вариантов
    qDebug() << "Проверка вариантов на уникальность";
    QString query_text = "SELECT * from public.\"Tasks\"";
    QUERY_MODEL->setQuery(query_text);
    up_variants_number[0] = QUERY_MODEL->rowCount();
    qDebug() << "Varinat_number_0 = " << up_variants_number[0];
    for (int j = 1; j < Up_Colvo_Unic; ++j)
    {
        up_variants_number[j] = 0;
    }
    for(int i = 1; i <= up_variants_number[0]; ++i)
    {
        query_text = "SELECT * FROM public.\"Tasks\" WHERE var_number = " + QString().number(i) + ";";
        //qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        up_variants_number[i] = QUERY_MODEL->rowCount();
        // qDebug() << "Для варианта " << i << " найдено " << variants_number[i] << " записей";
    }
    qDebug() << "Конец проверки количества уникальных вариантов и записей в них";


    //QSqlQueryModel *model = new QSqlQueryModel(this);
    //model->setQuery("SELECT fieldWithNeedData FROM table", sqlBase); //Выбираем список фамилий из базы
    QUERY_MODEL->setQuery("SELECT student_id FROM public.\"Students\";");
    QLineEdit *lineEdit = new QLineEdit(this);
    QCompleter *completer = new QCompleter(QUERY_MODEL, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion); //вариант с выпадающим списком.
    lineEdit->setCompleter(completer);

    //QLineEdit *lineEdit = new QLineEdit(on_lineEdit_student_id_editingFinished());


    up_student_id = ui->lineEdit_student_id->text();
    if ((up_student_id == "") || (up_student_id == NULL))
    {
        up_student_id = "";
    }
    else
    {
        // Проверка на то, что указанный идентификатор существует
        query_text = "SELECT * FROM public.\"Students\" WHERE student_id = " + up_student_id + ";";
        QUERY_MODEL->setQuery(query_text);
        if((QUERY_MODEL->rowCount()) != 1)
        {
            QMessageBox::warning(this, "WARNING", "Введён несуществующий идентификатор студента.\n"
                                                  "Пожалуйста, переопределите");
            up_student_id = "";
            ui->lineEdit_student_id->clear();
        }
    }
    qDebug() << "Идентификатор студента - " << up_student_id;
}

void dialog_sql_update::on_lineEdit_full_name_editingFinished()
{
    up_full_name = ui->lineEdit_full_name->text();
    qDebug() << "Имя студента - " << up_full_name;
}


void dialog_sql_update::on_lineEdit_student_group_editingFinished()
{
    up_student_group = ui->lineEdit_student_group->text();
    qDebug() << "Группа студента - " << up_student_group;
}

void dialog_sql_update::on_lineEdit_student_var_editingFinished()
{
    // Построение массива на количество уникальных вариантов
    qDebug() << "Проверка вариантов на уникальность";
    QString query_text = "SELECT * from public.\"Tasks\"";
    QUERY_MODEL->setQuery(query_text);
    up_variants_number[0] = QUERY_MODEL->rowCount();
    qDebug() << "Varinat_number_0 = " << up_variants_number[0];
    for (int j = 1; j < Up_Colvo_Unic; ++j)
    {
        up_variants_number[j] = 0;
    }
    for(int i = 1; i <= up_variants_number[0]; ++i)
    {
        query_text = "SELECT * FROM public.\"Tasks\" WHERE var_number = " + QString().number(i) + ";";
        //qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        up_variants_number[i] = QUERY_MODEL->rowCount();
        //qDebug() << "Для варианта " << i << " найдено " << variants_number[i] << " записей";
    }
    qDebug() << "Конец проверки количества уникальных вариантов и записей к нему в массиве";

    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(device());

    up_student_variant = ui->lineEdit_student_var->text();
    qDebug() << "Номер введённого варианта - " << up_student_variant;
    if ((up_student_variant == "") || (up_student_variant == NULL))
    {
        up_student_variant = "";
    }
    else if (up_student_variant == "0")
    {
        int rand_variant;
        char t = 'N';
        while(t == 'N')
        {
            rand_variant = engine() % Up_Colvo_Unic;
            if (!((up_variants_number[rand_variant] == 0) || (rand_variant == 0)))
            {
                up_student_variant = QString::number(rand_variant);
                t = 'Y';
            }
        }
        qDebug() << "Номер случайного варианта - " << up_student_variant;
    }
    else
    {
        QUERY_MODEL->setQuery("SELECT * FROM public.\"Tasks\" WHERE var_number = " + up_student_variant + ";");
        int id_check = QUERY_MODEL->rowCount();
        if (id_check == 0)
        {
            QMessageBox::warning(this, "WARNING", "Введён пустой вариант.\n"
                                                 "Пожалуйста, переназначьте, или система сделает это автоматически");
            int rand_variant;
            char t = 'N';
            while(t == 'N')
            {
                rand_variant = engine() % Up_Colvo_Unic;
                if (!((up_variants_number[rand_variant] == 0) || (rand_variant == 0)))
                {
                    up_student_variant = QString::number(rand_variant);
                    t = 'Y';
                }
            }
            qDebug() << "Номер переназначенного случайного варианта - " << up_student_variant;
        }
    }
    qDebug() << "Вариант студента - " << up_student_variant;
}

void dialog_sql_update::on_lineEdit_task_id_editingFinished()
{
    // Построение массива на количество уникальных вариантов
    qDebug() << "Проверка вариантов на уникальность";
    QString query_text = "SELECT * from public.\"Tasks\"";
    QUERY_MODEL->setQuery(query_text);
    up_variants_number[0] = QUERY_MODEL->rowCount();
    qDebug() << "Varinat_number_0 = " << up_variants_number[0];
    for (int j = 1; j < Up_Colvo_Unic; ++j)
    {
        up_variants_number[j] = 0;
    }
    for(int i = 1; i <= up_variants_number[0]; ++i)
    {
        query_text = "SELECT * FROM public.\"Tasks\" WHERE var_number = " + QString().number(i) + ";";
        //qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        up_variants_number[i] = QUERY_MODEL->rowCount();
        //qDebug() << "Для варианта " << i << " найдено " << variants_number[i] << " записей";
    }
    qDebug() << "Конец проверки количества уникальных вариантов и записей к нему в массиве";

    up_task_id = ui->lineEdit_task_id->text();
    if ((up_task_id == "") || (up_task_id == NULL))
    {
        up_task_id = "";
    }
    else
    {
        // Проверка на то, что указанный идентификатор существует
        query_text = "SELECT * FROM public.\"Tasks\" WHERE task_id = " + up_task_id + ";";
        QUERY_MODEL->setQuery(query_text);
        if((QUERY_MODEL->rowCount()) != 1)
        {
            QMessageBox::warning(this, "WARNING", "Введён несуществующий идентификатор задания.\n"
                                                  "Пожалуйста, переопределите");
            up_task_id = "";
            ui->lineEdit_task_id->clear();
        }
    }
    qDebug() << "Идентификатор задания - " << up_task_id;
}


void dialog_sql_update::on_lineEdit_task_complication_editingFinished()
{
    up_task_complication = ui->lineEdit_task_complication->text();
    qDebug() << "Сложность задания - " << up_task_complication;
}


void dialog_sql_update::on_lineEdit_task_Text_editingFinished()
{
    up_task_text = ui->lineEdit_task_Text->text();
    qDebug() << "Текст задания - " << up_task_text;
}


void dialog_sql_update::on_lineEdit_task_var_editingFinished()
{
    // Построение массива на количество уникальных вариантов
    qDebug() << "Проверка вариантов на уникальность";
    QString query_text = "SELECT * from public.\"Tasks\"";
    QUERY_MODEL->setQuery(query_text);
    up_variants_number[0] = QUERY_MODEL->rowCount();
    qDebug() << "Varinat_number_0 = " << up_variants_number[0];
    for (int j = 1; j < Up_Colvo_Unic; ++j)
    {
        up_variants_number[j] = 0;
    }
    for(int i = 1; i <= up_variants_number[0]; ++i)
    {
        query_text = "SELECT * FROM public.\"Tasks\" WHERE var_number = " + QString().number(i) + ";";
        //qDebug() << query_text;
        QUERY_MODEL->setQuery(query_text);
        up_variants_number[i] = QUERY_MODEL->rowCount();
        //qDebug() << "Для варианта " << i << " найдено " << variants_number[i] << " записей";
    }
    qDebug() << "Конец проверки количества уникальных вариантов и записей к нему в массиве";

    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(device());

    up_task_variant = ui->lineEdit_task_var->text();
    qDebug() << "Введёный номер варианта для задания - " << up_task_variant;
    if ((up_task_variant == "") || (up_task_variant == NULL))
    {
        up_task_variant = "";
    }
    if(up_task_variant == "0")
    {
        char t = 'N';
        int rand_variant_task;
        while(t == 'N')
        {
            rand_variant_task = engine() % Up_Colvo_Unic;
            if ((up_variants_number[rand_variant_task] > 2) || (rand_variant_task == 0))
            {
                t = 'N';
                qDebug() << "Full variant number choose";
            }
            else if(up_variants_number[rand_variant_task] <= 2)
            {
                up_variants_number[rand_variant_task] += 1;
                up_task_variant = QString::number(rand_variant_task);
                t = 'Y';
            }
        }
    }
    qDebug() << "Текущее значение варианта для задания - " << up_task_variant;
}

*/
