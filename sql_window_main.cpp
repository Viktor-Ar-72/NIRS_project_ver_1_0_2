#include "sql_window_main.h"
#include "ui_sql_window_main.h"
// Для работы с строками и файлами
#include <iostream>
#include <fstream>
using namespace std;

// Для работы с базами данных
QString Global_String_Querry;
int Current_Table_Number = 0;
//int Current_BD_Type = 0;


// Для передачи данных для подключения из базы данных
QString Transfer_DB_Adress, Transfer_DB_Name, Transfer_DB_User, Transfer_DB_Password; int Transfer_DB_Port;
// Список таблиц
QStringList BD_Tables_List_Asked;
// Тип текущей базы данных
int DATABASE_Type;
// Массив с типами полей каждой таблицы по номеру
//QString Matrix_Tables_Types = new QString [][];
//QString **Test_Mas;
QString **Matrix_Tables_FieldTypes;
QString **Matrix_Tables_FieldNames;


SQL_Window_Main::SQL_Window_Main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SQL_Window_Main)
{
    ui->setupUi(this);

    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);
    ui->tableView->setModel(QUERY_MODEL);
    //ui->tableView->close();

    // Инициализация окна Insert
    InsertWindow = new Dialog_SQL_Insert();
    // Подключение через кнопку в интерфейсе
    connect(InsertWindow, &Dialog_SQL_Insert::InWindow, this, &SQL_Window_Main::show);

    // Инициализация окна Delete
    DeleteWindow = new Dialog_SQL_Delete();
    // Подключение через кнопку в интерфейсе
    connect(DeleteWindow, &Dialog_SQL_Delete::DelWindow, this, &SQL_Window_Main::show);

    // Инициализация окна Update
    //UpdateWindow = new dialog_sql_update();
    UpdateWindow = new Dialog_SQL_Update();
    // Подключение через форму интерфейса
    //connect(UpdateWindow, &dialog_sql_update::UpWindow, this, &SQL_Window_Main::show);
    connect(UpdateWindow, &Dialog_SQL_Update::UpWindow, this, &SQL_Window_Main::show);

    // Подсказки при наведении
    ui->pushButton_2->setToolTip("Переключить таблицу влево");
    ui->pushButton_3->setToolTip("Переключить таблицу вправо");
    ui->pushButton_4->setToolTip("Вставка новых данных в выбранную таблицу");
    ui->pushButton_5->setToolTip("Удаление данных из выбранной таблицы");
    ui->pushButton_6->setToolTip("Выполнение пользовательского запроса к базе данных");
    ui->pushButton_7->setToolTip("Обновление данных в выбранной таблице");
}

SQL_Window_Main::~SQL_Window_Main()
{
    delete ui;
}

void SQL_Window_Main::on_pushButton_clicked()
{
    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);
    ui->tableView->setModel(QUERY_MODEL);
    qDebug() << "Закрытие основного окна!";

    // Возврат к первому окну
    this->close();
    emit firstWindow();
}

void SQL_Window_Main::changeIndex(int i)
{
    i++;
}

void SQL_Window_Main::on_comboBox_activated(int index)
{
    if (DATABASE_Type == 0)
    {
        // Строка для запроса из файлов
        std::string line;

        if (index == 0)
        {
                        /* Old realizations
            //qDebug() << "Тут пока пусто" << endl;
            //Current_BD_Type = 0;

            //DB.close();
            //qDebug() << "Подключение к Microsoft Access закрыто";
            //QMessageBox::warning(this, "Уведомление", "Подключение к базе данных Microsoft Access закрыто");
            //QMessageBox::information(this, "Уведомление", "Подключение к базе данных Microsoft Access закрыто");


        // Подключение драйвера БД
        DB = QSqlDatabase::addDatabase("QPSQL");
        // Вариант с запросом из файла
        line.erase();
        QString InformAccess;
        qDebug() << "Исследование файла PostgreSQL";
        InformAccess.clear();
        qDebug() << InformAccess << "  - она и должна быть пустой";
        std::ifstream PostgresFile("D://NIRS_Projects/PostgreSQL_access.txt");
        if (PostgresFile.is_open())
        {
                // Считывание адреса
                PostgresFile >> line;
                line.erase(0, 13);
                line.erase(line.find(">"), 1);
                InformAccess = QString::fromUtf8(line.c_str());
                qDebug() << InformAccess;
                DB.setHostName(InformAccess);

                // Считывание порта
                PostgresFile >> line;
                line.erase(0, 13);
                line.erase(line.find(">"), 1);
                InformAccess = QString::fromUtf8(line.c_str());
                qDebug() << InformAccess;
                DB.setPort(InformAccess.toInt());

                // Считывание имени БД
                PostgresFile >> line;
                line.erase(0, 17);
                line.erase(line.find(">"), 1);
                InformAccess = QString::fromUtf8(line.c_str());
                qDebug() << InformAccess;
                DB.setDatabaseName(InformAccess);

                // Считывание имени пользователя
                PostgresFile >> line;
                line.erase(0, 18);
                line.erase(line.find(">"), 1);
                InformAccess = QString::fromUtf8(line.c_str());
                qDebug() << InformAccess;
                DB.setUserName(InformAccess);

                // Считывание пароля пользователя
                PostgresFile >> line;
                line.erase(0,21);
                line.erase(line.find(">"), 1);
                InformAccess = QString::fromUtf8(line.c_str());
                qDebug() << InformAccess;
                DB.setPassword(InformAccess);

        }
        PostgresFile.close();

        if(DB.open())
        {
            qDebug("PostgreSQL_Database_is_open");
        }
        else
        {
            // Чтобы лучше узнать, в чём ошибка
            qDebug() << DB.lastError();
            QMessageBox::critical(this, "ERROR", "Ошибка подключения к базе данных PostgreSQL");
            QMessageBox::critical(this, "ERROR", DB.lastError().databaseText() + "\n" + DB.lastError().driverText());
        }
        */

            /* Заглушка - потому что непонятно, почему в LoginWindow БД открыта, а здесь нет
        qDebug() << "Заглушка";
        DB = QSqlDatabase::addDatabase("QPSQL");
        DB.setHostName("127.0.0.1"); DB.setPort(5432); DB.setDatabaseName("NIRS_Test_Database");
        DB.setUserName("DatabaseUser"); DB.setPassword("adminpassword4");
        */

            qDebug() << "Проверка на открытие файла с запрошенными параметрами";
            DB = QSqlDatabase::addDatabase("QPSQL");
            DB.setHostName(Transfer_DB_Adress);
            DB.setPort(Transfer_DB_Port);
            DB.setDatabaseName(Transfer_DB_Name);
            DB.setUserName(Transfer_DB_User);
            DB.setPassword(Transfer_DB_Password);
            // Это точно работает

            qDebug() << DB;
            if (DB.open())
            {
                /* Old database opening
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "SELECT * FROM public.\"Students\" ORDER BY student_id";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            Current_Table_Number = 0;
            */
                qDebug() << "База данных PostgreSQL точно открыта";

                // Это будет получение данных о таблицах открытой БД
                // Для начала - список таблиц
                // QStringList BD_Tables_List_Asked = DB.tables();
                BD_Tables_List_Asked = DB.tables();
                qDebug() << "Таблицы базы данных без сортировки - " << BD_Tables_List_Asked;
                // Сортировка имен таблиц в списке по возрастанию имен
                BD_Tables_List_Asked.sort();
                qDebug() << "Таблицы базы данных после сортировки - " << BD_Tables_List_Asked;
                // Получение данных о типах столбцов таблиц
                QString query_text;
                QSqlRecord Record_Test;
                QSqlQuery Query_Test;
                //QSqlField Test_Field;

                QString **Temp_Matrix_Tables_FieldTypes = new QString* [BD_Tables_List_Asked.count()];
                QString **Temp_Matrix_Tables_FieldNames = new QString* [BD_Tables_List_Asked.count()];
                //Test_Mas = new QString* [BD_Tables_List_Asked.count()];
                Matrix_Tables_FieldNames = new QString* [BD_Tables_List_Asked.count()];
                Matrix_Tables_FieldTypes = new QString* [BD_Tables_List_Asked.count()];

                for (int i = 0; i < BD_Tables_List_Asked.size(); ++i)
                {
                    query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[i] + "\"";
                    Query_Test.exec(query_text);
                    Record_Test = Query_Test.record();
                    qDebug() << "В таблице " << BD_Tables_List_Asked[i] << " количество столбцов = " << Record_Test.count();

                    //Matrix_Tables_FieldTypes[i] = new QString [Record_Test.count()];
                    //Matrix_Tables_FieldNames[i] = new QString [Record_Test.count()];
                    //Test_Mas[i] = new QString [Record_Test.count()];
                    Temp_Matrix_Tables_FieldTypes[i] = new QString [Record_Test.count()];
                    Temp_Matrix_Tables_FieldNames[i] = new QString [Record_Test.count()];

                    // Передача данных в глобальный массив
                    Matrix_Tables_FieldTypes[i] = new QString [Record_Test.count()];
                    Matrix_Tables_FieldNames[i] = new QString [Record_Test.count()];
                    Matrix_Tables_FieldNames[i] = Temp_Matrix_Tables_FieldNames[i];
                    Matrix_Tables_FieldTypes[i] = Temp_Matrix_Tables_FieldTypes[i];


                    qDebug() << "Имеются следующие столбцы:";
                    for(int j = 0; j < Record_Test.count(); ++j)
                    {
                        //qDebug() << "Имя поля: " << Record_Test.fieldName(j) << "; Тип: " << Record_Test.field(j).type();

                        //Matrix_Tables_FieldNames[i][j] = Record_Test.fieldName(j);
                        Temp_Matrix_Tables_FieldNames[i][j] = Record_Test.fieldName(j);
                        Matrix_Tables_FieldNames[i][j] = Record_Test.fieldName(j);

                        /* Old
                    //Test_Mas[i][j] = Record_Test.fieldName(j);
                    // Не очень пока - надо найти способ QVariant в QString
                    //Matrix_Tables_Types[i][j] = Record_Test.field(j).type(); QString::fromUtf8(line.c_str());
                    //string temp = to_string(Record_Test.field(j).type());
                    //Matrix_Tables_FieldTypes[i][j] = QString::fromUtf8(temp.c_str());
                    */

                        int temp = int(Record_Test.field(j).type());
                        // Теперь получается так:
                        // INT == int == "2"
                        // VARCHAR == TEXT == QString == "10"
                        // BOOLEAN == bool == "1"
                        // REAL == double == "6"
                        // BYTEA == ByteA == ("\f"== "12")
                        switch (temp)
                        {
                        case (1):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "bool";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (2):
                            //Matrix_Tables_FieldTypes[i][j] = "int";
                            Temp_Matrix_Tables_FieldTypes[i][j] = "int";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (10):
                            //Matrix_Tables_FieldTypes[i][j] = "QString";
                            Temp_Matrix_Tables_FieldTypes[i][j] = "QString";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (6):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "double";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (12):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "ByteA";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        default:
                            Temp_Matrix_Tables_FieldTypes[i][j] = "Unknown type. Code: " + QString(temp);
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        }
                        // Теперь вроде норм работает
                        //qDebug() << "Имя поля: " << Matrix_Tables_FieldNames[i][j] << "; Тип: " << Matrix_Tables_FieldTypes[i][j];
                        qDebug() << "Имя поля: " << Temp_Matrix_Tables_FieldNames[i][j] << "; Тип: " << Temp_Matrix_Tables_FieldTypes[i][j];
                        qDebug() << "Глобальное имя поля: " << Matrix_Tables_FieldNames[i][j] << "; Глобальный тип поля " <<Matrix_Tables_FieldTypes[i][j];
                    }
                    qDebug() << "";
                }

                // Очистка памяти от временного локального массива
                delete [] Temp_Matrix_Tables_FieldNames;
                delete [] Temp_Matrix_Tables_FieldTypes;

                /* Test, если закомменчено */
                TABLE_MODEL = new QSqlTableModel(this, DB);
                //QString query_text = "SELECT * FROM public.\"Students\" ORDER BY student_id";
                //query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[0] + "\"";
                // Вывод с сортировкой по первому столбцу
                query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[0] + "\" ORDER BY" + '"' + Matrix_Tables_FieldNames[0][0] + '"';
                QUERY_MODEL = new QSqlQueryModel();
                QUERY_MODEL->setQuery(query_text);
                ui->tableView->setModel(QUERY_MODEL);
                // Вывод на 1 позицию в списке таблиц (0 занята под пользовательские запросы)
                //Current_Table_Number = 0;
                Current_Table_Number = 1;
                // Тест - вывод индексов таблиц в списке
                int i = 0;
                while (i != BD_Tables_List_Asked.count())
                {
                    qDebug() << "По индексу " << i << " находится таблица " << BD_Tables_List_Asked[i];
                    i += 1;
                }
                qDebug() << "BD_List_Tables_Count == " << BD_Tables_List_Asked.count();
            }
            else
            {
                qDebug() << "ERROR!" << DB.lastError();
            }

            // Тест на создание всплывающих подсказок
            // ui->tableView->setToolTip("Всплывающая подсказка 1");
            // Работает, поэтому дальше будут основные подсказки
            //ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number] + " подключённой базы данных");
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number - 1] + " подключённой базы данных");
        }
        if(index == 1)
        {
            //QMessageBox::information(this, "Уведомление", "Функция временно недоступна");

            // Вывод списка таблиц базы данных PostgreSQL
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text;
            //Не работает query_text = "SHOW TABLES";
            // Вывод имён баз данных текущего сервера query_text = "SELECT datname FROM pg_database;";

            // Рабочий, выводит имена таблиц
            //query_text = "SELECT table_name FROM information_schema.tables WHERE table_schema NOT IN ('information_schema','pg_catalog');";

            // Тест на вывод более подробной информации о таблицах
            //query_text = "SELECT relname, relcrdate FROM pg_class WHERE relkind = 'r' AND relname NOT LIKE 'pg_%' AND relname NOT LIKE 'sql_%' ORDER BY relname;";
            query_text = "SELECT relid, relname AS Table_name, schemaname AS Schema_type, seq_tup_read AS Read_count, n_tup_ins AS Insert_count, n_tup_upd AS Update_count, n_tup_del AS Delete_count, n_live_tup, idx_scan, idx_tup_fetch FROM pg_stat_user_tables ORDER BY Table_name";

            qDebug() << query_text;
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            qDebug() << "Список доступных таблиц";
        }
    }

    if (DATABASE_Type == 2)
    {
        if (index == 0)
        {
            qDebug() << "Проверка на открытие файла с запрошенными параметрами";
            DB = QSqlDatabase::addDatabase("QODBC");
            //DB.setHostName(Transfer_DB_Adress);
            //DB.setPort(Transfer_DB_Port);
            DB.setDatabaseName(Transfer_DB_Name);
            //DB.setUserName(Transfer_DB_User);
            //DB.setPassword(Transfer_DB_Password);
            // Это точно работает
            qDebug() << DB;
            if (DB.open())
            {
                qDebug() << "База данных Access открыта прям точно";
                BD_Tables_List_Asked = DB.tables();
                qDebug() << "Таблицы базы данных без сортировки - " << BD_Tables_List_Asked;
                // Сортировка имен таблиц в списке по возрастанию имен
                BD_Tables_List_Asked.sort();
                qDebug() << "Таблицы базы данных после сортировки - " << BD_Tables_List_Asked;
                // Получение данных о типах столбцов таблиц
                QString query_text;
                QSqlRecord Record_Test;
                QSqlQuery Query_Test;

                QString **Temp_Matrix_Tables_FieldTypes = new QString* [BD_Tables_List_Asked.count()];
                QString **Temp_Matrix_Tables_FieldNames = new QString* [BD_Tables_List_Asked.count()];
                //Test_Mas = new QString* [BD_Tables_List_Asked.count()];
                Matrix_Tables_FieldNames = new QString* [BD_Tables_List_Asked.count()];
                Matrix_Tables_FieldTypes = new QString* [BD_Tables_List_Asked.count()];

                for (int i = 0; i < BD_Tables_List_Asked.size(); ++i)
                {
                    query_text = "SELECT * FROM " + BD_Tables_List_Asked[i] + ";";
                    Query_Test.exec(query_text);
                    Record_Test = Query_Test.record();
                    qDebug() << "В таблице " << BD_Tables_List_Asked[i] << " количество столбцов = " << Record_Test.count();

                    Temp_Matrix_Tables_FieldTypes[i] = new QString [Record_Test.count()];
                    Temp_Matrix_Tables_FieldNames[i] = new QString [Record_Test.count()];

                    // Передача данных в глобальный массив
                    Matrix_Tables_FieldTypes[i] = new QString [Record_Test.count()];
                    Matrix_Tables_FieldNames[i] = new QString [Record_Test.count()];
                    Matrix_Tables_FieldNames[i] = Temp_Matrix_Tables_FieldNames[i];
                    Matrix_Tables_FieldTypes[i] = Temp_Matrix_Tables_FieldTypes[i];
                    qDebug() << "Имеются следующие столбцы:";
                    for(int j = 0; j < Record_Test.count(); ++j)
                    {
                        Temp_Matrix_Tables_FieldNames[i][j] = Record_Test.fieldName(j);
                        Matrix_Tables_FieldNames[i][j] = Record_Test.fieldName(j);
                        int temp = int(Record_Test.field(j).type());
                        // Теперь получается так:
                        // INT == int == "2"
                        // VARCHAR == TEXT == QString == "10"
                        // BOOLEAN == bool == "1"
                        // REAL == double == "6"
                        // BYTEA == ByteA == ("\f"== "12")
                        // Microsoft Access YES/NO == bool == "3"
                        qDebug() << "Текущее значение MA temp = ";
                        qDebug() << temp;
                        switch (temp)
                        {
                        case (1):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "bool";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (2):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "int";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (10):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "QString";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (6):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "double";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (12):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "ByteA";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        case (3):
                            Temp_Matrix_Tables_FieldTypes[i][j] = "bool";
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        default:
                            Temp_Matrix_Tables_FieldTypes[i][j] = "Unknown type. Code: " + QString(temp);
                            Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                            break;
                        }
                        qDebug() << "Имя поля: " << Temp_Matrix_Tables_FieldNames[i][j] << "; Тип: " << Temp_Matrix_Tables_FieldTypes[i][j];
                        qDebug() << "Глобальное имя поля: " << Matrix_Tables_FieldNames[i][j] << "; Глобальный тип поля " <<Matrix_Tables_FieldTypes[i][j];
                    }
                    qDebug() << "";
                }
                // Очистка памяти от временного локального массива
                delete [] Temp_Matrix_Tables_FieldNames;
                delete [] Temp_Matrix_Tables_FieldTypes;

                /* Test, если закомменчено */
                TABLE_MODEL = new QSqlTableModel(this, DB);
                // Вывод без сортировки по первому столбцу
                query_text = "SELECT * FROM " + BD_Tables_List_Asked[0] + " ORDER BY " + Matrix_Tables_FieldNames[0][0] + ";";
                //qDebug() << "Test: " << query_text;
                QUERY_MODEL = new QSqlQueryModel();
                QUERY_MODEL->setQuery(query_text);
                ui->tableView->setModel(QUERY_MODEL);
                // Вывод на 1 позицию в списке таблиц (0 занята под пользовательские запросы)
                Current_Table_Number = 1;
                // Вывод индексов таблиц в списке
                int i = 0;
                while (i != BD_Tables_List_Asked.count())
                {
                    qDebug() << "По индексу " << i << " находится таблица " << BD_Tables_List_Asked[i];
                    i += 1;
                }
                qDebug() << "BD_List_Tables_Count == " << BD_Tables_List_Asked.count();
            }
            else
            {
                qDebug() << "ERROR!" << DB.lastError();
            }

            // Всплывающая подсказка
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number - 1] + " подключённой базы данных");
        }
    }
    if (index == 1)
    {
        qDebug() << "Проверка на открытие файла с запрошенными параметрами";
        DB = QSqlDatabase::addDatabase("QODBC");
        DB.setDatabaseName(Transfer_DB_Name);
        // Это точно работает
        qDebug() << DB;
        if (DB.open())
        {
            qDebug() << "База данных Access открыта прям точно";
            BD_Tables_List_Asked = DB.tables();
            qDebug() << "Таблицы базы данных без сортировки - " << BD_Tables_List_Asked;
            // Сортировка имен таблиц в списке по возрастанию имен
            BD_Tables_List_Asked.sort();
            qDebug() << "Таблицы базы данных после сортировки - " << BD_Tables_List_Asked;
            // Получение данных о типах столбцов таблиц
            QString query_text;
            QSqlRecord Record_Test;
            QSqlQuery Query_Test;
            //QSqlField Test_Field;

            QString **Temp_Matrix_Tables_FieldTypes = new QString* [BD_Tables_List_Asked.count()];
            QString **Temp_Matrix_Tables_FieldNames = new QString* [BD_Tables_List_Asked.count()];
            //Test_Mas = new QString* [BD_Tables_List_Asked.count()];
            Matrix_Tables_FieldNames = new QString* [BD_Tables_List_Asked.count()];
            Matrix_Tables_FieldTypes = new QString* [BD_Tables_List_Asked.count()];

            for (int i = 0; i < BD_Tables_List_Asked.size(); ++i)
            {
                query_text = "SELECT * FROM " + BD_Tables_List_Asked[i] + "";
                Query_Test.exec(query_text);
                Record_Test = Query_Test.record();
                qDebug() << "В таблице " << BD_Tables_List_Asked[i] << " количество столбцов = " << Record_Test.count();

                Temp_Matrix_Tables_FieldTypes[i] = new QString [Record_Test.count()];
                Temp_Matrix_Tables_FieldNames[i] = new QString [Record_Test.count()];

                // Передача данных в глобальный массив
                Matrix_Tables_FieldTypes[i] = new QString [Record_Test.count()];
                Matrix_Tables_FieldNames[i] = new QString [Record_Test.count()];
                Matrix_Tables_FieldNames[i] = Temp_Matrix_Tables_FieldNames[i];
                Matrix_Tables_FieldTypes[i] = Temp_Matrix_Tables_FieldTypes[i];
                qDebug() << "Имеются следующие столбцы:";
                for(int j = 0; j < Record_Test.count(); ++j)
                {
                    Temp_Matrix_Tables_FieldNames[i][j] = Record_Test.fieldName(j);
                    Matrix_Tables_FieldNames[i][j] = Record_Test.fieldName(j);
                    int temp = int(Record_Test.field(j).type());
                    // Теперь получается так:
                    // INT == int == "2"
                    // VARCHAR == TEXT == QString == "10"
                    // BOOLEAN == bool == "1"
                    // REAL == double == "6"
                    // BYTEA == ByteA == ("\f"== "12")
                    switch (temp)
                    {
                    case (1):
                        Temp_Matrix_Tables_FieldTypes[i][j] = "bool";
                        Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                        break;
                    case (2):
                        Temp_Matrix_Tables_FieldTypes[i][j] = "int";
                        Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                        break;
                    case (10):
                        Temp_Matrix_Tables_FieldTypes[i][j] = "QString";
                        Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                        break;
                    case (6):
                        Temp_Matrix_Tables_FieldTypes[i][j] = "double";
                        Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                        break;
                    case (12):
                        Temp_Matrix_Tables_FieldTypes[i][j] = "ByteA";
                        Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                        break;
                    default:
                        Temp_Matrix_Tables_FieldTypes[i][j] = "Unknown type. Code: " + QString(temp);
                        Matrix_Tables_FieldTypes[i][j] = Temp_Matrix_Tables_FieldTypes[i][j];
                        break;
                    }
                    qDebug() << "Имя поля: " << Temp_Matrix_Tables_FieldNames[i][j] << "; Тип: " << Temp_Matrix_Tables_FieldTypes[i][j];
                    qDebug() << "Глобальное имя поля: " << Matrix_Tables_FieldNames[i][j] << "; Глобальный тип поля " <<Matrix_Tables_FieldTypes[i][j];
                }
                qDebug() << "";
            }
            // Очистка памяти от временного локального массива
            delete [] Temp_Matrix_Tables_FieldNames;
            delete [] Temp_Matrix_Tables_FieldTypes;



        }
    }

    /* Реализация перенесена в другие части программы
     * Временно закрыто
    if (index == 1)
    {
        QString InformAccess;
        Current_BD_Type = 1;
        DB.close();
        qDebug() << "Подключение к PostgreSQL закрыто";
        QMessageBox::information(this, "Уведомление", "Подключение к базе данных PostgreSQL закрыто");
        DB = QSqlDatabase::addDatabase("QODBC");
        // Указание файла для открытия
        std::ifstream AccessFile("D://NIRS_Projects/Access_access.txt");
        if (AccessFile.is_open())
        {
            while(!AccessFile.eof())
            {
                AccessFile >> line;
                InformAccess = InformAccess.append(QString::fromUtf8(line.c_str()));
                InformAccess = InformAccess.append(" ");
            }
            qDebug() << InformAccess;
            InformAccess.resize(InformAccess.size() - 1);

        }
        else
        {
            qDebug() << "ERROR: File is not open";
            QMessageBox::critical(this, "ERROR", "Файл не был обнаружен или открыт.\n Проверьте местоположение и свойства файла");
        }
        qDebug() << InformAccess;
        //DB.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C://Users/VArutin/Documents/Database11_Test.accdb");
        DB.setDatabaseName(InformAccess);
        // Настройки для личного ПК
        //DB.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=D:/Work/BD_all/Database11.mdb");
        if(DB.open())
        {
            qDebug("Подключен Microsoft Access");
        }
        else
        {
            qDebug() << DB.lastError();
            QMessageBox::critical(this, "ERROR", "Ошибка при открытии базы данных Microsoft Access");
            QMessageBox::critical(this, "ERROR", DB.lastError().databaseText() + "\n" + DB.lastError().driverText());
        }
        ui->comboBox->setCurrentIndex(0);
        changeIndex(ui->comboBox->currentIndex());

        AccessFile.close();
    }
    */

}


void SQL_Window_Main::on_tableView_clicked(const QModelIndex &index)
{
    N_ActiveRow = index.row();
}


void SQL_Window_Main::on_pushButton_2_clicked()
{
    // PostgreSQL
    if (DATABASE_Type == 0)
    {
        // Сортировка таблиц в списке на всякий случай
        BD_Tables_List_Asked.sort();

        // Если кто-то пытается перейти с окна пользовательского запроса к таблице влево
        if (Current_Table_Number == 0)
        {
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[Current_Table_Number] + "\"";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            // Всплывающая подсказка
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number] + " подключённой базы данных");
            Current_Table_Number = 1;
        }

        // Реализация свободного переключения таблиц
        if(Current_Table_Number == 1)
        {
            qDebug() << "Левая граница возможных таблиц!";
            Current_Table_Number = 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;
        }
        else
        {
            // Test, success
            Current_Table_Number -= 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;

            TABLE_MODEL = new QSqlTableModel(this, DB);
            //QString query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[Current_Table_Number - 1] + "\"";
            // Вывод с сортировкой по первому столбцу
            QString query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[Current_Table_Number - 1] + "\" ORDER BY" + '"' + Matrix_Tables_FieldNames[Current_Table_Number - 1][0] + '"';
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            // Old
            //Current_Table_Number -= 1;
            //qDebug() << "Текущий индекс - " << Current_Table_Number;

            // Всплывающая подсказка
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number - 1] + " подключённой базы данных");
            // Чтобы окно осталось
            return;
        }
    }

    // Microsoft Access
    if (DATABASE_Type == 2)
    {
        // Сортировка таблиц в списке на всякий случай
        BD_Tables_List_Asked.sort();

        // Если кто-то пытается перейти с окна пользовательского запроса к таблице влево
        if (Current_Table_Number == 0)
        {
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "SELECT * FROM " + BD_Tables_List_Asked[Current_Table_Number] + ";";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            // Всплывающая подсказка
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number] + " подключённой базы данных");
            Current_Table_Number = 1;
        }

        // Реализация свободного переключения таблиц
        if(Current_Table_Number == 1)
        {
            qDebug() << "Левая граница возможных таблиц!";
            Current_Table_Number = 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;
        }
        else
        {
            // Test, success
            Current_Table_Number -= 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;

            TABLE_MODEL = new QSqlTableModel(this, DB);
            // Вывод с сортировкой по первому столбцу
            QString query_text = "SELECT * FROM " + BD_Tables_List_Asked[Current_Table_Number - 1] + " ORDER BY " + Matrix_Tables_FieldNames[Current_Table_Number - 1][0] + ";";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            // Всплывающая подсказка
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number - 1] + " подключённой базы данных");
            // Чтобы окно осталось
            return;
        }

    }

        /* Старая реализация переключения таблиц
        if (Current_Table_Number == 3)
        {
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "SELECT DISTINCT full_name, variant, task_text FROM public.\"Students\" JOIN public.\"Tasks\" ON variant = var_number ORDER BY full_name";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            Current_Table_Number = 2;
            return;
        }
        if(Current_Table_Number == 2)
        {
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "SELECT * FROM public.\"Tasks\" ORDER BY task_id";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            Current_Table_Number = 1;
            // Чтобы окно осталось - иначе сразу активизируется следующее
            return;
        }
        if(Current_Table_Number == 1)
        {
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "SELECT * FROM public.\"Students\" ORDER BY student_id";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            Current_Table_Number = 0;
            // test
            return;
        }
        if(Current_Table_Number == 0)
        {

            //TABLE_MODEL = new QSqlTableModel(this, DB);
            //QString query_text = "";
            //QUERY_MODEL = new QSqlQueryModel();
            //QUERY_MODEL->setQuery(query_text);
            //ui->tableView->setModel(QUERY_MODEL);

            qDebug() << "Just stay there!";
}
*/
        /* Just for test
            QString query_text;
            QSqlRecord Record_Test;
            QSqlQuery Query_Test;
            for (int i = 0; i < BD_Tables_List_Asked.size(); ++i)
            {
                qDebug() << "Данные из " << i << " таблицы";
                query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[i] + "\"";
                Query_Test.exec(query_text);
                Record_Test = Query_Test.record();
                for (int j = 0; j < Record_Test.count(); ++j)
                {
                    qDebug() << "Данные из массива Test_Mas" << Test_Mas[i][j];
                }
            }*/

}
void SQL_Window_Main::on_pushButton_3_clicked()
{
    // PostgreSQL
    if (DATABASE_Type == 0)
    {
        /* Старое переключение таблиц
        if(Current_Table_Number == 0)
        {
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "SELECT * FROM public.\"Tasks\" ORDER BY task_id";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            Current_Table_Number = 1;
            // Чтобы окно осталось
            return;
        }
        if(Current_Table_Number == 1)
        {
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "SELECT DISTINCT full_name, variant, task_text FROM public.\"Students\" JOIN public.\"Tasks\" ON variant = var_number ORDER BY full_name";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            Current_Table_Number = 2;
            return;

            //TABLE_MODEL = new QSqlTableModel(this, DB);
            //QString query_text = "";
            //QUERY_MODEL = new QSqlQueryModel();
            //QUERY_MODEL->setQuery(query_text);
            //ui->tableView->setModel(QUERY_MODEL);

            //qDebug() << "Just stay there!";
        }
        if(Current_Table_Number == 2)
        {
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = Global_String_Querry;
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            Current_Table_Number = 3;

        }
        */

        // Сортировка таблиц в списке на всякий случай
        BD_Tables_List_Asked.sort();

        // Реализация свободного переключения таблиц
        //if(Current_Table_Number == (BD_Tables_List_Asked.count() - 1))
        if(Current_Table_Number == (BD_Tables_List_Asked.count() - 1) + 1)
        {
            qDebug() << "Правая граница возможных таблиц!";
            //Current_Table_Number -= 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;
        }
        else
        {
            // Test, success
            Current_Table_Number += 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;

            TABLE_MODEL = new QSqlTableModel(this, DB);
            //QString query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[Current_Table_Number - 1] + "\"";
            // Вывод с сортировкой по певому столбцу
            QString query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[Current_Table_Number - 1] + "\" ORDER BY" + '"' + Matrix_Tables_FieldNames[Current_Table_Number - 1][0] + '"';
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            // Old
            //Current_Table_Number += 1;
            //qDebug() << "Текущий индекс - " << Current_Table_Number;

            // Всплывающая подсказка
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number - 1] + " подключённой базы данных");
            // Чтобы окно осталось
            return;
        }
    }
    // Microsoft Access
    if (DATABASE_Type == 2)
    {
        // Сортировка таблиц в списке на всякий случай
        BD_Tables_List_Asked.sort();
        // Реализация свободного переключения таблиц
        if(Current_Table_Number == (BD_Tables_List_Asked.count() - 1) + 1)
        {
            qDebug() << "Правая граница возможных таблиц!";
            qDebug() << "Текущий индекс - " << Current_Table_Number;
        }
        else
        {
            // Test, success
            Current_Table_Number += 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;
            TABLE_MODEL = new QSqlTableModel(this, DB);
            // Вывод с сортировкой по певому столбцу
            QString query_text = "SELECT * FROM " + BD_Tables_List_Asked[Current_Table_Number - 1] + " ORDER BY " + Matrix_Tables_FieldNames[Current_Table_Number - 1][0] + ";";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            // Всплывающая подсказка
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number - 1] + " подключённой базы данных");
            // Чтобы окно осталось
            return;
        }

    }
}


void SQL_Window_Main::on_pushButton_4_clicked()
{
    if(Current_Table_Number == 0)
    {
        // Значит, мы сейчас на таблице пользовательского запроса
        QMessageBox::warning(this, "Warning", "Вы находитесь на таблице пользовательского запроса. /n Вставка будет осуществляться в первую таблицу базы данных.");
        TABLE_MODEL = new QSqlTableModel(this, DB);
        QString query_text;
        if (DATABASE_Type == 0)
        {
            query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[0] + "\"";
        }
        if (DATABASE_Type == 2)
        {
            query_text = "SELECT * FROM " + BD_Tables_List_Asked[0] + ";";
        }
        QUERY_MODEL = new QSqlQueryModel();
        QUERY_MODEL->setQuery(query_text);
        ui->tableView->setModel(QUERY_MODEL);
        Current_Table_Number = 1;
    }

    qDebug("Переход на окно вставки данных");
    // Передача показаний о БД
    Dialog_SQL_Insert().get_DB_connection_from_MainWindow(DB, DATABASE_Type);
    Dialog_SQL_Insert().get_DB_Table_Info(BD_Tables_List_Asked, Matrix_Tables_FieldNames, Matrix_Tables_FieldTypes, DATABASE_Type);
    // Переход на второе окно
    // Иконка и подпись для окна Update
    InsertWindow->setWindowIcon(QIcon("Exe_Icon_1.png"));
    InsertWindow->setWindowTitle("Вставка данных");
    InsertWindow->show();

    // Закомментено, чтобы не закрывалось основное окно
    // А потом раскомментено, чтобы не вылетала программа
    // Но она все равно вылетает, хотя данные честно вставляет
    // роде перестала вылетать, так что снова закомменченно
    //this->close();
/* Old testes
    // Думаю, необходимо будет перевести всё это в отдельный файл - форму
    //TABLE_MODEL->insertRow(TABLE_MODEL->rowCount());
    // Тут более - менее рабочая реализация
    QString student_id, full_name, student_group, variant;
    // Необходимо заполнить данные для запроса
    // Для students_id - посчитать все вхождения и добавить новый номер
    QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\"");
    int i = QUERY_MODEL->rowCount() + 1;
    qDebug() << i;
    // Testes data
    full_name = "Test";
    student_group = "Test";
    //student_id = "5";
    //variant = "5";
    student_id = QString::number(i);
    variant = QString::number(i);
    // Добавление записи
    QString query_text = "INSERT INTO public.\"Students\" (student_id, full_name, student_group, variant) VALUES ('" + student_id + "'::integer, '" + full_name + "'::text, '" + student_group + "'::text, '" + variant + "'::integer) returning student_id;";
    QUERY_MODEL->setQuery(query_text);
    ui->tableView->setModel(QUERY_MODEL);
*/
}


void SQL_Window_Main::on_pushButton_5_clicked()
{
    if(Current_Table_Number == 0)
    {
        // Значит, мы сейчас на таблице пользовательского запроса
        QMessageBox::warning(this, "Warning", "Вы находитесь на таблице пользовательского запроса. /n Удаление данных будет осуществляться из первой таблицы базы данных.");
        TABLE_MODEL = new QSqlTableModel(this, DB);
        QString query_text;
        if (DATABASE_Type == 0)
        {
            query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[0] + "\"";
        }
        if (DATABASE_Type == 2)
        {
            query_text = "SELECT * FROM " + BD_Tables_List_Asked[0] + ";";
        }
        QUERY_MODEL = new QSqlQueryModel();
        QUERY_MODEL->setQuery(query_text);
        ui->tableView->setModel(QUERY_MODEL);
        Current_Table_Number = 1;
    }

    // Переход на окно удаления данных
    qDebug() << "Переход на окно удаления данных";
    Dialog_SQL_Delete().get_DB_connection_from_MainWindow(DB, DATABASE_Type);
    Dialog_SQL_Delete().get_DB_Table_Info(BD_Tables_List_Asked, Matrix_Tables_FieldNames, Matrix_Tables_FieldTypes, DATABASE_Type);
    // Иконка и подпись для окна Delete
    DeleteWindow->setWindowIcon(QIcon("Exe_Icon_1.png"));
    DeleteWindow->setWindowTitle("Удаление данных");
    DeleteWindow->show();
}


void SQL_Window_Main::on_pushButton_6_clicked()
{
    if(Global_String_Querry == "")
    {
        QMessageBox::warning(this, "WARNING", "Введён пустой запрос к базе данных.");
        //QMessageBox::critical(this, "ERROR", "Ошибка при выполнении запроса к базе данных. \n Проверьте правильность запроса!");
    }
    else
    {
        QSqlQuery Dop_Query;
        Dop_Query.exec(Global_String_Querry);
        if (Dop_Query.exec(Global_String_Querry))
        {
        //if (Запрос не выполнен)
        //{}
        //else {
        // Выполнение пользовательского запроса
        TABLE_MODEL = new QSqlTableModel(this, DB);
        QUERY_MODEL = new QSqlQueryModel();
        QUERY_MODEL->setQuery(Global_String_Querry);
        ui->tableView->setModel(QUERY_MODEL);
        //Current_Table_Number = 3;
        Current_Table_Number = 0;
        /* Old testes
        //if(TABLE_MODEL->lastError().isValid())
        if(ui->tableView->selectionModel())
        {
            qDebug() << TABLE_MODEL->lastError();
            QMessageBox::critical(this, "ERROR", "Ошибка выполнения пользовательского вопроса\n" + TABLE_MODEL->lastError().databaseText() + "\n" + TABLE_MODEL->lastError().driverText());

        }
        */
        }
        else
        {
            qDebug() << "Ошибка в выполнении запроса - " << Dop_Query.lastError();
            QMessageBox::critical(this, "ERROR", "Ошибка выполнения пользовательского вопроса\n" + Dop_Query.lastError().databaseText()); //+ "\n" + TABLE_MODEL->lastError().driverText());
            QUERY_MODEL->setQuery("");
            ui->tableView->setModel(QUERY_MODEL);
            //Current_Table_Number = 3;
            Current_Table_Number = 1;
        }
    }
}


void SQL_Window_Main::on_lineEdit_editingFinished()
{
    // Ввод пользовательского запроса
    Global_String_Querry = ui->lineEdit->text();
    qDebug() << "Текущее значение запроса - " << Global_String_Querry;
}


void SQL_Window_Main::on_pushButton_7_clicked()
{

    if(Current_Table_Number == 0)
    {
        // Значит, мы сейчас на таблице пользовательского запроса
        QMessageBox::warning(this, "Warning", "Вы находитесь на таблице пользовательского запроса. /n Обновление данных будет осуществляться в первой таблице базы данных.");
        TABLE_MODEL = new QSqlTableModel(this, DB);
        QString query_text;
        if (DATABASE_Type == 0)
        {
            query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[0] + "\"";
        }
        if (DATABASE_Type == 2)
        {
            query_text = "SELECT * FROM " + BD_Tables_List_Asked[0] + ";";
        }
        QUERY_MODEL = new QSqlQueryModel();
        QUERY_MODEL->setQuery(query_text);
        ui->tableView->setModel(QUERY_MODEL);
        Current_Table_Number = 1;
    }

    // Передача данных о таблице
    //dialog_sql_update().get_DB_connection_from_MainWindow(DB);
    //dialog_sql_update().get_DB_Table_Info(BD_Tables_List_Asked, Matrix_Tables_FieldNames, Matrix_Tables_FieldTypes);
    Dialog_SQL_Update().get_DB_connection_from_MainWindow(DB, DATABASE_Type);
    Dialog_SQL_Update().get_DB_Table_Info(BD_Tables_List_Asked, Matrix_Tables_FieldNames, Matrix_Tables_FieldTypes, DATABASE_Type);

    // Переход на окно обновления данных
    qDebug() << "Переход на окно обновления данных";
    // Иконка и подпись для окна Update
    UpdateWindow->setWindowIcon(QIcon("Exe_Icon_1.png"));
    UpdateWindow->setWindowTitle("Обновление данных");
    UpdateWindow->show();
}

// Получение данных о БД для подключения
void SQL_Window_Main::get_DB_data_from_login(QSqlDatabase DB_Data, int DataBase_Type)
{
    //DB = DB_Data;
    DATABASE_Type = DataBase_Type;
    if (DATABASE_Type == 0)
    {
        DB = QSqlDatabase::cloneDatabase(DB_Data, "PostgreSQL_New_Connect");
        qDebug() << "Полученные данные про DB" << DB;
        Transfer_DB_Adress = DB.hostName();
        qDebug() << "Полученный адрес БД в Qstring - " << Transfer_DB_Adress;
        Transfer_DB_Port = DB.port();
        qDebug() << "Полученный port БД в Qstring" << QString::number(Transfer_DB_Port);
        Transfer_DB_Name = DB.databaseName();
        qDebug() << "Полученное имя БД в QString" << Transfer_DB_Name;
        Transfer_DB_User = DB.userName();
        qDebug() << "Полученный логин пользователя в QString" << Transfer_DB_User;
        Transfer_DB_Password = DB.password();
        qDebug() << "Полученный пароль пользователя в QString" << Transfer_DB_Password;
    }
    else if (DATABASE_Type == 2)
    {
        DB = QSqlDatabase::cloneDatabase(DB_Data, "MicrosoftAccess_New_Connect");
        qDebug() << "Полученные данные про DB" << DB;
        //Transfer_DB_Adress = DB.hostName();
        //qDebug() << "Полученный адрес БД в Qstring - " << Transfer_DB_Adress;
        //Transfer_DB_Port = DB.port();
        //qDebug() << "Полученный port БД в Qstring" << QString::number(Transfer_DB_Port);
        Transfer_DB_Name = DB.databaseName();
        qDebug() << "Полученное имя БД в QString" << Transfer_DB_Name;
        //Transfer_DB_User = DB.userName();
        //qDebug() << "Полученный логин пользователя в QString" << Transfer_DB_User;
        //Transfer_DB_Password = DB.password();
        //qDebug() << "Полученный пароль пользователя в QString" << Transfer_DB_Password;
    }
}

// Передача данных о списке таблиц
int SQL_Window_Main::take_list_of_Tables(int Current_table_number_for_insert)
{
    //List_of_Tables << "Hello" << "there!";
    //List_of_Tables = BD_Tables_List_Asked;
    //qDebug() << "List of tables = " << List_of_Tables;
    //return List_of_Tables;
    //Current_table_number_for_insert = Current_Table_Number;
    // Пришлось ставить -1, так как иначе будет ошибка утечки памяти и программа крашнется
    Current_table_number_for_insert = Current_Table_Number - 1;
    qDebug() << Current_table_number_for_insert << " - В данный момент в Insert / Update передан индекс таблицы " << BD_Tables_List_Asked[Current_table_number_for_insert];
    return Current_table_number_for_insert;
}


void rpz_code_function()
{

    QSqlDatabase DatabaseMain = QSqlDatabase::addDatabase("QPSQL", "DatabaseConnect");
    DatabaseMain.setHostName("127.0.0.1");
    DatabaseMain.setPort(5432);
    DatabaseMain.setDatabaseName("Test_Database");
    DatabaseMain.setUserName("User");
    DatabaseMain.setPassword("Password");
    DatabaseMain.open();
    if (!(DatabaseMain.isOpen()))
    {
        qDebug() << DatabaseMain.lastError();
    }

    QSqlQuery Querry_Main;
    Querry_Main.exec("SELECT * FROM public.\"TestTable1\"");
    if (!(Querry_Main.exec()))
    {
        qDebug() << Querry_Main.lastError();
    }

    //QSqlDatabase PostgreSQLDatabase = QSqlDatabase::database();
    //if (PostgreSQLDatabase.driver()->hasFeature(QSqlDriver::QDoubleValidator()))
    //{
    //    qDebug() << "Опция поддерживается";
    //} else {
    //    qDebug() << "Опция не поддерживается";
    //}

    QSqlQuery Querry_New;
    Querry_New.prepare("INSERT INTO public.\"TestTable1\" (\"ID\", \"TEST_2\") "
                    "VALUES (:id::smallint, :year::integer) returning \"ID\";");
    Querry_New.bindValue(":id", '7');
    Querry_New.bindValue(":data", '2023');
    Querry_New.exec();

    QSqlQuery Querry_Update;
    Querry_Update.prepare("UPDATE public.\"TestTable1\" "
                          "SET \"TEXT\" = :data::text "
                          "WHERE \"ID\" = 7;");
    Querry_Update.bindValue(":data", 'Update_Querry_Text');
    Querry_Update.exec();

    QSqlQuery Querry_Delete;
    Querry_Delete.prepare("DELETE FROM public.\"TestTable1\" WHERE \"ID\" IN (:id);");
    Querry_Delete.bindValue(":id", 8);
    Querry_Delete.exec();

    QSqlQueryModel SQL_Querry_Model;
    SQL_Querry_Model.setQuery("SELECT * FROM public.\"TestTable1\"");
    for (int i = 0; i < SQL_Querry_Model.rowCount(); ++i)
    {
        int ID = SQL_Querry_Model.record(i).value("ID").toInt();
        QString Data = SQL_Querry_Model.record(i).value("Data").toString();
        qDebug() << ID << Data;
    }

    QSqlTableModel SQL_Table_Model;
    SQL_Table_Model.setTable("public.\"TestTable1\"");
    SQL_Table_Model.setFilter("\"ID\" > 4");
    SQL_Table_Model.select();
    for (int i = 0; i < SQL_Table_Model.rowCount(); ++i)
    {
        int id = SQL_Table_Model.record(i).value("ID").toInt();
        QString data = SQL_Table_Model.record(i).value("Data").toString();
        qDebug() << id << data;
    }

    QSqlRelationalTableModel SQL_Relation_Model;
    SQL_Relation_Model.setTable("public.\"TestTable1\"");
    QSqlRecord SomeRecord;
    int RecordNumber = 4;
    QString data = "New Data";
    SomeRecord.setValue("Data", data);
    SQL_Relation_Model.setRecord(RecordNumber, SomeRecord);
    int NumberDeleteRows = 2;
    SQL_Relation_Model.removeRows(RecordNumber - 1, NumberDeleteRows);
    SQL_Relation_Model.submitAll();

    SQL_Relation_Model.setTable("public.\"TestTable1\"");
    SQL_Relation_Model.setRelation(4, QSqlRelation("TestTable2", "ID", "Data"));

    QAbstractItemModel* ViewModel = new QSqlRelationalTableModel;
    QTableView *view = new QTableView;
    view->setModel(ViewModel);
    view->show();
}
