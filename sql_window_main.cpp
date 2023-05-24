#include "sql_window_main.h"
#include "ui_sql_window_main.h"
// Для работы с строками и файлами
#include <iostream>
#include <fstream>
using namespace std;

// Для работы с базами данных
QString Global_String_Querry;
int Current_Table_Number = 0;
// Для передачи данных для подключения из базы данных
QString Transfer_DB_Adress, Transfer_DB_Name, Transfer_DB_User, Transfer_DB_Password; int Transfer_DB_Port;
// Список таблиц
QStringList BD_Tables_List_Asked;
// Тип текущей базы данных
int DATABASE_Type;
// Массив с типами полей каждой таблицы по номеру
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

    // Инициализация окна Insert
    InsertWindow = new Dialog_SQL_Insert();
    // Подключение через кнопку в интерфейсе
    connect(InsertWindow, &Dialog_SQL_Insert::InWindow, this, &SQL_Window_Main::show);

    // Инициализация окна Delete
    DeleteWindow = new Dialog_SQL_Delete();
    // Подключение через кнопку в интерфейсе
    connect(DeleteWindow, &Dialog_SQL_Delete::DelWindow, this, &SQL_Window_Main::show);

    // Инициализация окна Update
    UpdateWindow = new Dialog_SQL_Update();
    // Подключение через форму интерфейса
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



void SQL_Window_Main::on_comboBox_activated(int index)
{
    if (DATABASE_Type == 0)
    {
        // Строка для запроса из файлов
        std::string line;

        if (index == 0)
        {
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
                qDebug() << "База данных PostgreSQL точно открыта";

                // Это будет получение данных о таблицах открытой БД
                // Для начала - список таблиц
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
                Matrix_Tables_FieldNames = new QString* [BD_Tables_List_Asked.count()];
                Matrix_Tables_FieldTypes = new QString* [BD_Tables_List_Asked.count()];

                for (int i = 0; i < BD_Tables_List_Asked.size(); ++i)
                {
                    query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[i] + "\"";
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
                        // Вывод полученной информации
                        qDebug() << "Имя поля: " << Temp_Matrix_Tables_FieldNames[i][j] << "; Тип: " << Temp_Matrix_Tables_FieldTypes[i][j];
                        qDebug() << "Глобальное имя поля: " << Matrix_Tables_FieldNames[i][j] << "; Глобальный тип поля " <<Matrix_Tables_FieldTypes[i][j];
                    }
                    qDebug() << "";
                }

                // Очистка памяти от временного локального массива
                delete [] Temp_Matrix_Tables_FieldNames;
                delete [] Temp_Matrix_Tables_FieldTypes;

                TABLE_MODEL = new QSqlTableModel(this, DB);
                // Вывод с сортировкой по первому столбцу
                query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[0] + "\" ORDER BY" + '"' + Matrix_Tables_FieldNames[0][0] + '"';
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

            // Создание всплывающих подсказок
            ui->tableView->setToolTip("Таблица " + BD_Tables_List_Asked[Current_Table_Number - 1] + " подключённой базы данных");
        }
        if(index == 1)
        {

            qDebug() << "Проверка на открытие файла с запрошенными параметрами";
            DB = QSqlDatabase::addDatabase("QPSQL");
            DB.setHostName(Transfer_DB_Adress);
            DB.setPort(Transfer_DB_Port);
            DB.setDatabaseName(Transfer_DB_Name);
            DB.setUserName(Transfer_DB_User);
            DB.setPassword(Transfer_DB_Password);
            qDebug() << DB;
            if (DB.open())
            {
                qDebug() << "База данных PostgreSQL точно открыта";

                // Это будет получение данных о таблицах открытой БД
                // Для начала - список таблиц
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
                Matrix_Tables_FieldNames = new QString* [BD_Tables_List_Asked.count()];
                Matrix_Tables_FieldTypes = new QString* [BD_Tables_List_Asked.count()];

                for (int i = 0; i < BD_Tables_List_Asked.size(); ++i)
                {
                    query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[i] + "\"";
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
                        // Вывод информации о таблицах
                        qDebug() << "Имя поля: " << Temp_Matrix_Tables_FieldNames[i][j] << "; Тип: " << Temp_Matrix_Tables_FieldTypes[i][j];
                        qDebug() << "Глобальное имя поля: " << Matrix_Tables_FieldNames[i][j] << "; Глобальный тип поля " <<Matrix_Tables_FieldTypes[i][j];
                    }
                    qDebug() << "";
                }
                // Очистка памяти от временного локального массива
                delete [] Temp_Matrix_Tables_FieldNames;
                delete [] Temp_Matrix_Tables_FieldTypes;


                // Вывод списка таблиц базы данных PostgreSQL
                TABLE_MODEL = new QSqlTableModel(this, DB);
                query_text = "SELECT relid, relname AS Table_name, schemaname AS Schema_type, seq_tup_read AS Read_count, n_tup_ins AS Insert_count, n_tup_upd AS Update_count, n_tup_del AS Delete_count, n_live_tup, idx_scan, idx_tup_fetch FROM pg_stat_user_tables ORDER BY Table_name";

                qDebug() << query_text;
                QUERY_MODEL = new QSqlQueryModel();
                QUERY_MODEL->setQuery(query_text);
                ui->tableView->setModel(QUERY_MODEL);
                qDebug() << "Список доступных таблиц";
                // На случай, если не удалось получить подробную информацию о таблицах
                if (ui->tableView->model()->rowCount() == 0 || ui->tableView->model()->columnCount() == 0)
                {
                    QMessageBox::warning(this, "WARNING", "Не удалось получить подробную информацию о таблицах. Проверьте параметры подключённой базы данных.");
                }
            }
        }
    }

    if (DATABASE_Type == 2)
    {
        if (index == 0)
        {
            qDebug() << "Проверка на открытие файла с запрошенными параметрами";
            DB = QSqlDatabase::addDatabase("QODBC");
            DB.setDatabaseName(Transfer_DB_Name);
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
                        // BOOL == bool == "3"
                        // REAL == double == "6"
                        // BYTEA == ByteA == ("\f"== "12")
                        // Microsoft Access YES/NO == bool == "3"
                        qDebug() << "Текущее значение MA temp = ";
                        qDebug() << temp;
                        switch (temp)
                        {
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

                TABLE_MODEL = new QSqlTableModel(this, DB);
                // Вывод без сортировки по первому столбцу
                query_text = "SELECT * FROM " + BD_Tables_List_Asked[0] + " ORDER BY " + Matrix_Tables_FieldNames[0][0] + ";";
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
        if (index == 1)
        {
            qDebug() << "Проверка на открытие файла с запрошенными параметрами";
            DB = QSqlDatabase::addDatabase("QODBC");
            DB.setDatabaseName(Transfer_DB_Name);
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
                        // BOOL == bool == "3"
                        // REAL == double == "6"
                        // BYTEA == ByteA == ("\f"== "12")
                        switch (temp)
                        {
                        case (3):
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

                // Вывод списка таблиц базы данных Microsoft Access
                // Тут будет сильно зависеть от того, какие права у подключённого
                TABLE_MODEL = new QSqlTableModel(this, DB);
                query_text = "SELECT [Name] FROM MSysObjects WHERE [Type] = 1 AND [Name] NOT LIKE 'MSys*'";
                QUERY_MODEL = new QSqlQueryModel();
                QUERY_MODEL->setQuery(query_text);
                ui->tableView->setModel(QUERY_MODEL);
                // На случай, если не удалось получить подробную информацию о таблицах
                if (ui->tableView->model()->rowCount() == 0 || ui->tableView->model()->columnCount() == 0)
                {
                    QMessageBox::warning(this, "WARNING", "Недостаточно прав для получения подробной информации о базе данных.");
                }

            }
        }
    }
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
            Current_Table_Number -= 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;

            TABLE_MODEL = new QSqlTableModel(this, DB);
            // Вывод с сортировкой по первому столбцу
            QString query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[Current_Table_Number - 1] + "\" ORDER BY" + '"' + Matrix_Tables_FieldNames[Current_Table_Number - 1][0] + '"';
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);

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
}

void SQL_Window_Main::on_pushButton_3_clicked()
{
    // PostgreSQL
    if (DATABASE_Type == 0)
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
            Current_Table_Number += 1;
            qDebug() << "Текущий индекс - " << Current_Table_Number;

            TABLE_MODEL = new QSqlTableModel(this, DB);
            // Вывод с сортировкой по певому столбцу
            QString query_text = "SELECT * FROM public.\"" + BD_Tables_List_Asked[Current_Table_Number - 1] + "\" ORDER BY" + '"' + Matrix_Tables_FieldNames[Current_Table_Number - 1][0] + '"';
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);

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
        // Значит, пользоваель сейчас на таблице пользовательского запроса
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
    }
    else
    {
        QSqlQuery Dop_Query;
        Dop_Query.exec(Global_String_Querry);
        if (Dop_Query.exec(Global_String_Querry))
        {
            // Выполнение пользовательского запроса
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(Global_String_Querry);
            ui->tableView->setModel(QUERY_MODEL);
            Current_Table_Number = 0;
        }
        else
        {
            qDebug() << "Ошибка в выполнении запроса - " << Dop_Query.lastError();
            QMessageBox::critical(this, "ERROR", "Ошибка выполнения пользовательского вопроса\n" + Dop_Query.lastError().databaseText()); //+ "\n" + TABLE_MODEL->lastError().driverText());
            QUERY_MODEL->setQuery("");
            ui->tableView->setModel(QUERY_MODEL);
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
        Transfer_DB_Name = DB.databaseName();
        qDebug() << "Полученное имя БД в QString" << Transfer_DB_Name;
    }
}

// Передача данных о списке таблиц
int SQL_Window_Main::take_list_of_Tables(int Current_table_number_for_insert)
{
    Current_table_number_for_insert = Current_Table_Number - 1;
    qDebug() << Current_table_number_for_insert << " - В данный момент в Insert / Update передан индекс таблицы " << BD_Tables_List_Asked[Current_table_number_for_insert];
    return Current_table_number_for_insert;
}

