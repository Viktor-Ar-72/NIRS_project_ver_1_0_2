#include "dialog_sql_insert.h"
#include "ui_dialog_sql_insert.h"
// Библиотеки для генератора на основе Вихря Мерсенна
#include <stdlib.h>
#include <cstdlib>
#include <random>
#include <QMessageBox>
// Подключение окна MainWindow для передачи кое-каких параметров
#include <sql_window_main.h>

// Для работы с присланными таблицами
// Список таблиц
QStringList Insert_BD_Tables_List_Asked;
// Массив с типами полей каждой таблицы по номеру
QString **Insert_Matrix_Tables_FieldTypes;
QString **Insert_Matrix_Tables_FieldNames;
// Тип генератора
short GenType;
// Тип БД
int BD_Type_Insert;

Dialog_SQL_Insert::Dialog_SQL_Insert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SQL_Insert)
{
    ui->setupUi(this);
    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);

    //  Инициализация lineEdit_tableNow
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

// Глобальные переменные для работы с подключением к базе данных
QString Insert_Transfer_DB_Adress, Insert_Transfer_DB_Name, Insert_Transfer_DB_User, Insert_Transfer_DB_Password;
int Insert_Transfer_DB_Port;

// Глобальные переменные для формы вставки
int Insert_Table_Index;
int Insert_Fields_Number = 0, Insert_int_left = 0, Insert_int_right = 0, Insert_varchar_lenght, Insert_byteA_lenght, Insert_use_boolean;
double Insert_real_left, Insert_real_right;

void Dialog_SQL_Insert::on_pushButton_clicked()
{
    // Создаём N-ое количество массивов, для каждого типа столбца текущей таблицы
    // Для вывода матрицы пока что пользуюсь таким способом - при использовани count() или size() вылетает почему - то
    QString query_text;
    QSqlRecord Record_Test;
    QSqlQuery Query_Test;
    if (BD_Type_Insert == 0)
    {
        query_text = "SELECT * FROM public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\"";
    }
    if (BD_Type_Insert == 2)
    {
        query_text = "SELECT * FROM " + Insert_BD_Tables_List_Asked[Insert_Table_Index] + ";";
    }
    Query_Test.exec(query_text);
    Record_Test = Query_Test.record();
    // Переменные для расчёта количества элементов
    int k_int = 0; int k_varchar = 0; int k_real = 0; int k_bytea = 0; int k_boolean = 0;
    char int_use = 'n'; char char_use = 'n'; char real_use = 'n'; char bytea_use = 'n';
    char bool_use = 'n';
    for (int K = 0; K < Record_Test.count(); ++K)
    {
        if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][K] == "int")
        {
            k_int += 1;
            int_use = 'y';
        }
        if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][K] == "QString")
        {
            k_varchar += 1;
            char_use = 'y';
        }
        if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][K] == "double")
        {
            k_real += 1;
            real_use = 'y';
        }
        if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][K] == "bool")
        {
            k_boolean += 1;
            bool_use = 'y';
        }
        if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][K] == "ByteA")
        {
            k_bytea += 1;
            bytea_use = 'y';
        }
    }
    int *Insert_int_mass = new int[(Insert_Fields_Number * k_int)];
    QString* Insert_varchar_mass = new QString[(Insert_Fields_Number * k_varchar)];
    double* Insert_real_mass = new double[(Insert_Fields_Number * k_real)];
    bool* Insert_bool_mass = new bool[(Insert_Fields_Number * k_boolean)];
    QString* Insert_bytea_mass = new QString[(Insert_Fields_Number * k_bytea)];

    if (int_use == 'y')
    {
        if ((Insert_int_left != Insert_int_right) && (ui->lineEdit_int_left->text() != "") && (ui->lineEdit_int_right->text() != "") && (Insert_int_left < Insert_int_right))
        {
            if(GenType == 1)
            {
                Insert_int_mass = VihrMersenna_Gen_Int(Insert_int_mass, (Insert_Fields_Number * k_int), Insert_int_left, Insert_int_right);
            }
            else if (GenType == 2)
            {
                Insert_int_mass = MacLarenMarsalii_Gen_Int(Insert_int_mass, Insert_Fields_Number * k_int, Insert_int_left, Insert_int_right);
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
            }
            qDebug() << "Полученный массив rand int - " << Insert_int_mass;
            for (int i = 0; i < (Insert_Fields_Number * k_int); ++i)
            {
                qDebug() << "Полученный элемент INTEGER " << i << " равен = " << Insert_int_mass[i];
            }
        }
        else
        {
            QMessageBox::critical(this, "ERROR", "Нарушение целевых границ генерируемой последовательности INT!\n Пожалуйста, переопределите!");
            return;
        }
    }

    // Заполнение генератора Real
    if (real_use == 'y')
    {
        if ((Insert_real_left != Insert_real_right) && (ui->lineEdit_real_left->text() != "") && (ui->lineEdit_real_right->text() != "") && (Insert_real_left < Insert_real_right))
        {
            if(GenType == 1)
            {
                Insert_real_mass = VihrMersenna_Gen_Real(Insert_real_mass, (Insert_Fields_Number * k_real), Insert_real_left, Insert_real_right);
            }
            else if (GenType == 2)
            {
                Insert_real_mass = MacLarenMarsalii_Gen_Real(Insert_real_mass, (Insert_Fields_Number * k_real), Insert_real_left, Insert_real_right);
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
            }
            qDebug() << "Полученный массив rand real - " << Insert_real_mass;
            for (int i = 0; i < (Insert_Fields_Number * k_real); ++i)
            {
                qDebug() << "Полученный REAL элемент " << i << " равен = " << Insert_real_mass[i];
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
        if(Insert_varchar_lenght > 0)
        {
            if(GenType == 1)
            {
                Insert_varchar_mass = VihrMersenna_Gen_Char(Insert_varchar_mass, (Insert_Fields_Number * k_varchar), Insert_varchar_lenght, 0, 125);
            }
            else if (GenType == 2)
            {
                Insert_varchar_mass = MacLarenMarsalii_Gen_Char(Insert_varchar_mass, (Insert_Fields_Number * k_varchar), Insert_varchar_lenght, 0, 125);
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
            }
            qDebug() << "Полученный массив rand varchar - " << Insert_varchar_mass;
            for (int i = 0; i < (Insert_Fields_Number * k_varchar); ++i)
            {
                qDebug() << "Полученный элемент VARCHAR " << i << " равен = " << Insert_varchar_mass[i];
            }
        }
        else
        {
            QMessageBox::critical(this, "ERROR", "Неправильно указан размер последовательности VARCHAR!\n Пожалуйста, переопределите!");
            return;
        }
    }

    if (bool_use == 'y')
    {
        // Заполнение генератора BOOL
        if(Insert_use_boolean != 0)
        {
            int* dop_intBool_mass = new int[(Insert_Fields_Number * k_boolean)];
            if(GenType == 1)
            {
                dop_intBool_mass = VihrMersenna_Gen_Int(dop_intBool_mass, (Insert_Fields_Number * k_boolean), 0, 1);
            }
            else if (GenType == 2)
            {
                dop_intBool_mass = MacLarenMarsalii_Gen_Int(dop_intBool_mass, (Insert_Fields_Number * k_boolean), 0, 1);
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
            }
            qDebug() << "Полученный массив rand intBool - " << dop_intBool_mass;
            for (int i = 0; i < (Insert_Fields_Number * k_boolean); ++i)
            {
                if (dop_intBool_mass[i] == 0)
                {
                    Insert_bool_mass[i] = false;
                }
                else
                {
                    Insert_bool_mass[i] = true;
                }
                qDebug() << "Полученный элемент BOOL " << i << " равен = " << Insert_bool_mass[i];
            }
        }
        else
        {
            for (int i = 0; i < (Insert_Fields_Number * k_boolean); ++i)
            {
                Insert_bool_mass[i] = true;
            }
        }
    }
    if (bytea_use == 'y')
    {
        if((Insert_byteA_lenght > 0) && ((Insert_byteA_lenght % 2) == 0))
        {
            if(GenType == 1)
            {
                Insert_bytea_mass = VihrMersenna_Gen_ByteA(Insert_bytea_mass, (Insert_Fields_Number * k_bytea), Insert_byteA_lenght, 0, 90);
            }
            else if (GenType == 2)
            {
                Insert_bytea_mass = MacLarenMarsalii_Gen_ByteA(Insert_bytea_mass, (Insert_Fields_Number * k_bytea), Insert_byteA_lenght, 0, 90);
            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Не выбран тип генератора!\n Пожалуйста, определите тип используемого генератора.");
            }
            qDebug() << "Полученный массив rand bytea - " << Insert_bytea_mass;
            for (int i = 0; i < (Insert_Fields_Number * k_bytea); ++i)
            {
                qDebug() << "Полученный элемент BYTEA " << i << " равен = " << Insert_bytea_mass[i];
            }
        }
        else
        {
            QMessageBox::critical(this, "ERROR", "Неправильно указан размер последовательности BYTEA!\n Пожалуйста, переопределите!");
            return;
        }
    }

    // Вставка значений в таблицу через Insert
    QSqlRecord Insert_Record;
    QSqlQuery Query_Insert;
    QString query_vrem_text;
    if (BD_Type_Insert == 0)
    {
        query_vrem_text = "SELECT * FROM public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\"";
    }
    if (BD_Type_Insert == 2)
    {
        query_vrem_text = "SELECT * FROM " + Insert_BD_Tables_List_Asked[Insert_Table_Index] + ";";
    }
    Query_Insert.exec(query_vrem_text);
    Insert_Record = Query_Insert.record();
    QString query_insert_text;
    QString boolean_type_QString;
    for (int V = 0; V < Insert_Fields_Number; ++V)
    {
        if (BD_Type_Insert == 0)
        {
            // Создание шаблона запроса.
            query_insert_text = "INSERT INTO public.";
            query_insert_text += '"';
            query_insert_text = query_insert_text + Insert_BD_Tables_List_Asked[Insert_Table_Index];
            query_insert_text.append('"');
            query_insert_text = query_insert_text + " VALUES (";
        }
        if (BD_Type_Insert == 2)
        {
            query_insert_text = "INSERT INTO ";
            query_insert_text = query_insert_text + Insert_BD_Tables_List_Asked[Insert_Table_Index];
            query_insert_text = query_insert_text + " VALUES (";
        }

        // Создание дополнительных переменных для доп.циклов
        int start_index_int = V * k_int;
        int start_index_varchar = V * k_varchar;
        int start_index_real = V * k_real;
        int start_index_bool = V * k_boolean;
        int start_index_bytea = V * k_bytea;

        for (int A = 0; A < Insert_Record.count(); ++A)
        {

            if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][A] == "int")
            {
                query_insert_text = query_insert_text + QString::number(Insert_int_mass[start_index_int]) + ", ";
                ++start_index_int;
            }
            else if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][A] == "QString")
            {
                query_insert_text = query_insert_text + "'" + Insert_varchar_mass[start_index_varchar] + "', ";
                ++start_index_varchar;
            }
            else if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][A] == "double")
            {
                // Сохранение точности до определённого знака после запятой
                query_insert_text = query_insert_text + QString::number(Insert_real_mass[start_index_real], 'g', 7) + ", ";
                ++start_index_real;
            }
            else if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][A] == "ByteA")
            {
                query_insert_text = query_insert_text + "'";
                query_insert_text += QString::fromUtf8("\x5C\x78");
                query_insert_text = query_insert_text + Insert_bytea_mass[start_index_bytea] + "', ";
                ++start_index_bytea;
            }
            else if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][A] == "bool")
            {
                boolean_type_QString = "";
                if (Insert_bool_mass[start_index_bool] == true)
                { boolean_type_QString = "TRUE"; }
                else
                { boolean_type_QString = "FALSE"; }
                query_insert_text = query_insert_text + boolean_type_QString + ", ";
                ++start_index_bool;
            }
            else {
            }
        }
        // Убираем последнюю запятую
        int position = query_insert_text.lastIndexOf(QChar(','));
        query_insert_text = query_insert_text.left(position);
        query_insert_text = query_insert_text.append(");");
        qDebug() << "Запрос на вставку" << V << query_insert_text;
        // Вставка данных
        QUERY_MODEL->setQuery(query_insert_text);
        qDebug() << "Запрос на вставку" << V << "выполнен.";
    }

    query_insert_text = ""; // Заглушка
    QUERY_MODEL->setQuery(query_insert_text);
    // Закрытие окна вставки данных
    qDebug() << "Окно Insert закрыто вместе с сохранением данных в таблицу";
    // Очистка массивов
    delete [] Insert_int_mass;
    delete [] Insert_varchar_mass;
    delete [] Insert_real_mass;
    delete [] Insert_bytea_mass;
    delete [] Insert_bool_mass;
    // Возврат к основному окну
    this->close();
    emit InWindow();
}

void Dialog_SQL_Insert::on_pushButton_2_clicked()
{
    qDebug() << "Окно Insert закрыто без сохранения данных в таблицу";
    // Возврат к основному окну
    this->close();
    emit InWindow();
}

void Dialog_SQL_Insert::on_tabWidget_Students_tabBarClicked(int index)
{
    if(index == 0)
    {
        QMessageBox::warning(this, "WARNING", "Будьте внимательны, вводя параметры вставки!\nИзменения будут применены сразу!");
        qDebug() << "Окно формы генерации";
    }
    else if (index == 1)
    {
        qDebug() << "Окно справки по форме Insert";
    }
}

// Новая функция для получения данных о таблицах
void Dialog_SQL_Insert::get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types, int DB_Class)
{
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
        if (DB_Class == 0)
        {
            query_text = "SELECT * FROM public.\"" + Insert_BD_Tables_List_Asked[i] + "\"";
        }
        if (DB_Class == 2)
        {
            query_text = "SELECT * FROM " + Insert_BD_Tables_List_Asked[i] + ";";
        }
        Query_Test.exec(query_text);
        Record_Test = Query_Test.record();
        for(int j = 0; j < Record_Test.count(); ++j)
        {
            qDebug() << "Имя поля - " + Insert_Matrix_Tables_FieldNames[i][j] + "; Тип поля - " + Insert_Matrix_Tables_FieldTypes[i][j];
        }
    }

    // Передача индекса текущей таблицы для вставки
    Insert_BD_Tables_List_Asked.sort();
    int vrem_number = 0;
    Insert_Table_Index = SQL_Window_Main().take_list_of_Tables(vrem_number);
    qDebug() << "Согласно полученным данным, вставка будет вестись в таблицу с индексом" << Insert_Table_Index;
    qDebug() << "Это таблица" << Insert_BD_Tables_List_Asked[Insert_Table_Index];
}

// Функция для подключения данных о подключении и установке соединения
void Dialog_SQL_Insert::get_DB_connection_from_MainWindow(QSqlDatabase DB_conn_data, int DB_class)
{
    BD_Type_Insert = DB_class;
    if (DB_class == 0)
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
    if (DB_class == 2)
    {
        DB = QSqlDatabase::cloneDatabase(DB_conn_data, "MicrosoftAccess_New_Connect");
        qDebug() << "Insert: Полученные данные про DB" << DB;
        Insert_Transfer_DB_Name = DB.databaseName();
        qDebug() << "Insert: Полученное имя БД в QString" << Insert_Transfer_DB_Name;

        qDebug() << "Проверка на открытие файла с запрошенными параметрами";
        DB = QSqlDatabase::addDatabase("QODBC", "InsertWindowConnect");
        DB.setDatabaseName(Insert_Transfer_DB_Name);
        if (DB.open())
        {
            qDebug() << "DB in Insert is open: " << DB;
        }
        else
        {
            qDebug() << "Error in Insert DB opening: " << DB.lastError();
        }
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
}

void Dialog_SQL_Insert::on_lineEdit_varchar_lenght_editingFinished()
{
    Insert_varchar_lenght = ui->lineEdit_varchar_lenght->text().toInt();
    qDebug() << "Полученная длина последовательности VARCHAR - " << Insert_varchar_lenght;
}

void Dialog_SQL_Insert::on_lineEdit_real_left_editingFinished()
{
    Insert_real_left = ui->lineEdit_real_left->text().toDouble();
    qDebug() << "Левая граница типа Real для генератора - " << Insert_real_left;
}

void Dialog_SQL_Insert::on_lineEdit_real_right_editingFinished()
{
    Insert_real_right = ui->lineEdit_real_right->text().toDouble();
    qDebug() << "Правая граница типа Real для генератора - " << Insert_real_right;
}

void Dialog_SQL_Insert::on_lineEdit_bytea_lenght_editingFinished()
{
    Insert_byteA_lenght = ui->lineEdit_bytea_lenght->text().toInt();
    qDebug() << "Полученная длина последовательности Bytea / Blob - " << Insert_byteA_lenght;
}

int* Dialog_SQL_Insert::VihrMersenna_Gen_Int(int* getted_rand_mass, int rasmer, int gen_VM_left_edge, int gen_VM_right_edge)
{
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    for (int i = 0; i < rasmer; ++i)
    {
        getted_rand_mass[i] = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
    }

    // Смена зерна генерации, чтобы не было повторяющихся значений
    engine.seed();
    return getted_rand_mass;
}

int* Dialog_SQL_Insert::MacLarenMarsalii_Gen_Int(int* getted_rand_mass, int rasmer, int gen_MM_left_edge, int gen_MM_right_edge)
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

double* Dialog_SQL_Insert::VihrMersenna_Gen_Real(double *getted_rand_mass, int rasmer, double gen_VM_left_edge, double gen_VM_right_edge)
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

    engine.seed();
    return getted_rand_mass;
}

double* Dialog_SQL_Insert::MacLarenMarsalii_Gen_Real(double *getted_rand_mass, int rasmer, double gen_MM_left_edge, double gen_MM_right_edge)
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

        // Преобразование Box-Muller
        double z1 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        double z2 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

        // Масштабирование и сдвиг
        double rand_num = gen_MM_left_edge + ((z1 + z2) / 2.0) * (gen_MM_right_edge - gen_MM_left_edge);

        // Проверка, входит ли число в заданный диапазон
        if (rand_num < gen_MM_left_edge || rand_num > gen_MM_right_edge) {
            // Перегенерирование числа, пока оно не попадет в заданный диапазон
            do {
                u1_prev = u1;
                u1 = u2;
                u2 = engine() / double(engine.max());
                z1 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
                z2 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
                rand_num = gen_MM_left_edge + ((z1 + z2) / 2.0) * (gen_MM_right_edge - gen_MM_left_edge);
            } while (rand_num < gen_MM_left_edge || rand_num > gen_MM_right_edge);
        }

        // Сохранение в массив
        getted_rand_mass[i] = rand_num;
    }

    // Очистка ресурсов и возврат массива
    engine.seed();
    return getted_rand_mass;
}

QString* Dialog_SQL_Insert::VihrMersenna_Gen_Char(QString *massiv_gen_numbers, int rasmer, int length, int gen_VM_left_edge, int gen_VM_right_edge)
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

QString* Dialog_SQL_Insert::MacLarenMarsalii_Gen_Char(QString *massiv_gen_numbers, int rasmer, int length, int gen_MM_left_edge, int gen_MM_right_edge)
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

void Dialog_SQL_Insert::on_checkBox_useBoolGen_clicked()
{
    if (ui->checkBox_useBoolGen->isChecked())
    {
        Insert_use_boolean = 1;
        qDebug() << "Будет использоваться генератор булевых значений";
    }
    else
    {
        Insert_use_boolean = 0;
        qDebug() << "Не будет использоваться генератор булевых значений";
    }
}

QString* Dialog_SQL_Insert::VihrMersenna_Gen_ByteA(QString *massiv_gen_numbers, int rasmer, int length, int gen_VM_left_edge, int gen_VM_right_edge)
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
    // Смена зерна генерации, чтобы не было повторяющихся значений
    engine.seed();
    return massiv_gen_numbers;
}

QString* Dialog_SQL_Insert::MacLarenMarsalii_Gen_ByteA(QString *massiv_gen_numbers, int rasmer, int length, int gen_MM_left_edge, int gen_MM_right_edge)
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
