#include "dialog_sql_update.h"
#include "ui_dialog_sql_update.h"
#include <sql_window_main.h>
#include <QCompleter>
#include <QSqlRecord>
// Для преобразования типа переменной
#include <type_traits>
#include <QVariant>

Dialog_SQL_Update::Dialog_SQL_Update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SQL_Update)
{
    ui->setupUi(this);

    //  Инициализация lineEdit_tableNow
    ui->lineEdit_tableNow->setText("Тип генератора?");
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

Dialog_SQL_Update::~Dialog_SQL_Update()
{
    delete ui;
}

// Для настройки соединения
// Для работы с присланными таблицами
// Список таблиц
QStringList Update_BD_Tables_List_Asked;
// Массив с типами полей каждой таблицы по номеру
QString **Update_Matrix_Tables_FieldTypes;
QString **Update_Matrix_Tables_FieldNames;
// Тип генератора
short Update_GenType;
// Класс базы данных
int DB_Update_Type;

// Глобальные переменные для работы с подключением к базе данных
QString Update_Transfer_DB_Adress, Update_Transfer_DB_Name, Update_Transfer_DB_User, Update_Transfer_DB_Password;
int Update_Transfer_DB_Port;
// Глобальные переменные для формы обновления данных
int Update_Table_Index;

int Update_Fields_Start, Update_Fields_Finish, Update_int_left = 0, Update_int_right = 0, Update_varchar_lenght = 0,
Update_byteA_lenght = 0, Update_use_boolean;
double Update_real_left, Update_real_right;

void Dialog_SQL_Update::on_pushButton_OK_clicked()
{
    QString query_text;
    QSqlQueryModel Query_Test;
    if (DB_Update_Type == 0)
    {
        query_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[Update_Table_Index] + "\"";
    }
    if (DB_Update_Type == 2)
    {
        query_text = "SELECT * FROM " + Update_BD_Tables_List_Asked[Update_Table_Index] + ";";
    }
    Query_Test.setQuery(query_text);
    int table_size = Query_Test.rowCount();
    if ((Update_Fields_Start >= 0) && (Update_Fields_Finish >= 0) && ((Update_Fields_Finish - Update_Fields_Start + 1) > 0) && (Update_Fields_Finish < table_size))
    {
        // Создаём N-ое количество массивов, для каждого типа столбца текущей таблицы
        QString query_text;
        QSqlRecord Record_Test;
        QSqlQuery Query_Test;
        if (DB_Update_Type == 0)
        {
            query_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[Update_Table_Index] + "\"";
        }
        if (DB_Update_Type == 2)
        {
            query_text = "SELECT * FROM " + Update_BD_Tables_List_Asked[Update_Table_Index] + ";";
        }
        Query_Test.exec(query_text);
        Record_Test = Query_Test.record();
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
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\nПожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand int - " << Update_int_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_int); ++i)
                {
                    qDebug() << "Полученный элемент INTEGER " << i << " равен = " << Update_int_mass[i];

                }
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Нарушение целевых границ генерируемой последовательности INT!\nПожалуйста, переопределите!");
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
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\nПожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand real - " << Update_real_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_real); ++i)
                {
                    qDebug() << "Полученный REAL элемент " << i << " равен = " << Update_real_mass[i];
                }

            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Нарушение целевых границ генерируемой последовательности REAL!\nПожалуйста, переопределите!");
                return;
            }
        }

        if (char_use == 'y')
        {
            // Заполнение генератора Varchar
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
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\nПожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand varchar - " << Update_varchar_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_varchar); ++i)
                {
                    qDebug() << "Полученный элемент VARCHAR " << i << " равен = " << Update_varchar_mass[i];
                }
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не указаны размер последовательности VARCHAR!\nПожалуйста, переопределите!");
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
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\nПожалуйста, определите тип используемого генератора.");
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
                    QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\nПожалуйста, определите тип используемого генератора.");
                }
                qDebug() << "Полученный массив rand bytea - " << Update_bytea_mass;
                for (int i = 0; i < ((Update_Fields_Finish - Update_Fields_Start + 1) * k_bytea); ++i)
                {
                    qDebug() << "Полученный элемент BYTEA " << i << " равен = " << Update_bytea_mass[i];
                }
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не указаны размер последовательности BYTEA!\nПожалуйста, переопределите!");
                return;
            }
        }

        QSqlQueryModel SQL_Querry_Model;
        QString query_U_borders;
        if (DB_Update_Type == 0)
        {
        query_U_borders = "SELECT * FROM public.";
        query_U_borders += '"' + Update_BD_Tables_List_Asked[Update_Table_Index] + '"' + " ORDER BY" + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][0] + '"';
        }
        if (DB_Update_Type == 2)
        {
            query_U_borders = "SELECT * FROM " + Update_BD_Tables_List_Asked[Update_Table_Index] + " ORDER BY " + Update_Matrix_Tables_FieldNames[Update_Table_Index][0] + ";";
        }
        qDebug() << "Запрос на поиск нужной строки - " << query_U_borders;
        SQL_Querry_Model.setQuery(query_U_borders);

        QString* Update_Where_Values = new QString[(Update_Fields_Finish - Update_Fields_Start + 1)];

        int m = 0;
        for (int i = 0; i < SQL_Querry_Model.rowCount(); ++i)
        {
            if (i >= Update_Fields_Start && i <= Update_Fields_Finish)
            {
                qDebug() << "Найдена " << m << " строка для обновления на " << i << " месте";
                qDebug() << SQL_Querry_Model.record(i).value(Update_Matrix_Tables_FieldNames[Update_Table_Index][0]).toString();
                Update_Where_Values[m] = SQL_Querry_Model.record(i).value(Update_Matrix_Tables_FieldNames[Update_Table_Index][0]).toString();
                m += 1;
            }
        }

        // Вставка значений в таблицу через Update
        QSqlRecord Update_Record;
        QSqlQuery Query_Update;
        QString query_vrem_text;
        if (DB_Update_Type == 0)
        {
            query_vrem_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[Update_Table_Index] + "\"";
        }
        if (DB_Update_Type == 2)
        {
            query_vrem_text = "SELECT * FROM " + Update_BD_Tables_List_Asked[Update_Table_Index] + ";";
        }
        Query_Update.exec(query_vrem_text);
        Update_Record = Query_Update.record();
        QString query_Update_text;
        QString boolean_type_QString;
        for (int V = 0; V < (Update_Fields_Finish - Update_Fields_Start + 1); ++V)
        {
            if (DB_Update_Type == 0)
            {
            query_Update_text = "UPDATE public.";
            query_Update_text += '"';
            query_Update_text = query_Update_text + Update_BD_Tables_List_Asked[Update_Table_Index];
            query_Update_text.append('"');
            }
            if (DB_Update_Type == 2)
            {
                query_Update_text = "UPDATE " + Update_BD_Tables_List_Asked[Update_Table_Index];
            }
            query_Update_text = query_Update_text + " SET ";

            // Создание дополнительных переменных для доп.циклов
            int start_index_int = V * k_int;
            int start_index_varchar = V * k_varchar;
            int start_index_real = V * k_real;
            int start_index_bool = V * k_boolean;
            int start_index_bytea = V * k_bytea;

            for (int A = 0; A < Update_Record.count(); ++A)
            {
                if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][A] == "int")
                {
                    query_Update_text = query_Update_text + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][A] + '"' + " = " + QString::number(Update_int_mass[start_index_int]) + ", ";
                    ++start_index_int;
                }
                else if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][A] == "QString")
                {
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
                    query_Update_text = query_Update_text + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][A] + '"' + " = " + "'";
                    query_Update_text += QString::fromUtf8("\x5C\x78");
                    query_Update_text = query_Update_text + Update_bytea_mass[start_index_bytea] + "', ";
                    ++start_index_bytea;
                }
                else if(Update_Matrix_Tables_FieldTypes[Update_Table_Index][A] == "bool")
                {
                    boolean_type_QString = "";
                    if (Update_bool_mass[start_index_bool] == true)
                    { boolean_type_QString = "TRUE"; }
                    else
                    { boolean_type_QString = "FALSE"; }
                    query_Update_text = query_Update_text + '"' + Update_Matrix_Tables_FieldNames[Update_Table_Index][A] + '"' + " = " + boolean_type_QString + ", ";
                    ++start_index_bool;
                }
                else {
                }
            }
            // Убираем последнюю запятую
            int position = query_Update_text.lastIndexOf(QChar(','));
            query_Update_text = query_Update_text.left(position);
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

            // Обновление данных
            QUERY_MODEL->setQuery(query_Update_text);
            qDebug() << "Запрос на обновление" << V << "выполнен.";
        }

        query_Update_text = ""; // Заглушка
        QUERY_MODEL->setQuery(query_Update_text);
        // Закрытие окна обновления данных
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
        QMessageBox::critical(this, "ERROR", "Не указаны границы обновляемых записей!\nПожалуйста, укажите!");
        return;
    }
}


void Dialog_SQL_Update::on_pushButton_Esc_clicked()
{
    // Возврат к основному окну
    this->close();
    emit UpWindow();
}

void Dialog_SQL_Update::on_tabWidget_Update_currentChanged(int index)
{

    if(index == 0)
    {
        // Сообщение - предупреждение пользователю
        QMessageBox::warning(this, "WARNING", "Будьте осторожны, вводя новые данные.\nИзменения будут применены сразу.\nВводите только те данные, которые хотите обновить.");
        qDebug() << "Окно обновления данных в выбранной таблице";
    }
    else if (index == 1)
    {
        qDebug() << "Окно справки в форме Update";
    }
}

// Новая функция для получения данных о таблицах
void Dialog_SQL_Update::get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types, int DB_Class)
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
        if (DB_Class == 0)
        {
            query_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[i] + "\"";
        }
        if (DB_Class == 2)
        {
            query_text = "SELECT * FROM " + Update_BD_Tables_List_Asked[i] + ";";
        }
        Query_Test.exec(query_text);
        Record_Test = Query_Test.record();
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
}

// Функция для подключения данных о подключении и установке соединения
void Dialog_SQL_Update::get_DB_connection_from_MainWindow(QSqlDatabase DB_conn_data, int DB_Class)
{
    DB_Update_Type = DB_Class;
    if (DB_Update_Type == 0)
    {
        DB = QSqlDatabase::cloneDatabase(DB_conn_data, "PostgreSQL_New_Connect_Update");
        qDebug() << "Update: Полученные данные про DB" << DB;
        Update_Transfer_DB_Adress = DB.hostName();
        qDebug() << "Update: Полученный адрес БД в Qstring - " << Update_Transfer_DB_Adress;
        Update_Transfer_DB_Port = DB.port();
        qDebug() << "Update: Полученный port БД в Qstring" << QString::number(Update_Transfer_DB_Port);
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
    if (DB_Update_Type == 2)
    {
        DB = QSqlDatabase::cloneDatabase(DB_conn_data, "MicrosoftAccess_New_Connect_Update");
        Update_Transfer_DB_Name = DB.databaseName();
        qDebug() << "Update: Полученное имя БД в QString" << Update_Transfer_DB_Name;
        DB = QSqlDatabase::addDatabase("QODBC", "UpdateWindowConnect");
        DB.setDatabaseName(Update_Transfer_DB_Name);
        if (DB.open())
        {
            qDebug() << "DB in Update is open: " << DB;
        }
        else
        {
            qDebug() << "Error in Update DB opening: " << DB.lastError();
        }
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
    QString query_text;
    QSqlQueryModel Query_Test;
    if (DB_Update_Type == 0)
    {
        query_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[Update_Table_Index] + "\"";
    }
    if (DB_Update_Type == 2)
    {
        query_text = "SELECT * FROM " + Update_BD_Tables_List_Asked[Update_Table_Index] + ";";
    }
    Query_Test.setQuery(query_text);
    int r = Query_Test.rowCount();
    Update_Fields_Start = ui->lineEdit_strok_start->text().toInt();
    if ((Update_Fields_Start > -1) && (Update_Fields_Start < r))
    {
        qDebug() << "Номер строки, с которой нужно обновить данные - " << Update_Fields_Start;
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Введен неправильный номер начальной строки!\nПожалуйста, переопределите.");
        ui->lineEdit_strok_start->clear();
        Update_Fields_Start = std::numeric_limits<int>::quiet_NaN();
    }
}


void Dialog_SQL_Update::on_lineEdit_strok_end_editingFinished()
{
    QString query_text;
    QSqlQueryModel Query_Test;
    if (DB_Update_Type == 0)
    {
        query_text = "SELECT * FROM public.\"" + Update_BD_Tables_List_Asked[Update_Table_Index] + "\"";
    }
    if (DB_Update_Type == 2)
    {
        query_text = "SELECT * FROM " + Update_BD_Tables_List_Asked[Update_Table_Index] + ";";
    }
    Query_Test.setQuery(query_text);
    int r = Query_Test.rowCount();
    Update_Fields_Finish = ui->lineEdit_strok_end->text().toInt();
    if ((Update_Fields_Finish >= Update_Fields_Start) && (Update_Fields_Finish < r))
    {
        qDebug() << "Номер строки, до которой нужно обновить данные - " << Update_Fields_Finish;
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Введен неправильный номер конечной строки!\nПожалуйста, переопределите.");
        ui->lineEdit_strok_end->clear();
        Update_Fields_Finish = std::numeric_limits<int>::quiet_NaN();
    }
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
    if ((Update_byteA_lenght % 2) == 0)
    {
        qDebug() << "Полученная длина последовательности Bytea / Blob - " << Update_byteA_lenght;
    }
    else
    {
        QMessageBox::critical(this, "ERROR", "Внимание: длина генерируемой последовательности ByteA должна быть чётной!");
        Update_byteA_lenght = std::numeric_limits<int>::quiet_NaN();
        ui->lineEdit_bytea_lenght->clear();
    }
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
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    for (int i = 0; i < rasmer; ++i)
    {
        getted_rand_mass[i] = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
    }

    engine.seed();
    return getted_rand_mass;
}

int* Dialog_SQL_Update::Up_MacLarenMarsalii_Gen_Int(int* getted_rand_mass, int rasmer, int gen_MM_left_edge, int gen_MM_right_edge)
{
    // Инициализация привязки устройства
    std::ranlux24 engine;
    std::random_device device;
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

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

    delete[] matrix_3;
    engine.seed();
    return getted_rand_mass;
}

double* Dialog_SQL_Update::Up_VihrMersenna_Gen_Real(double *getted_rand_mass, int rasmer, double gen_VM_left_edge, double gen_VM_right_edge)
{
    // Инициализация зерна генератора для Вихря Мерсенна
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

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
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

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
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

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
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

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
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

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
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

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

