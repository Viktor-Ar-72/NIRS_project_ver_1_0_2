#include "sql_window_main.h"
#include "ui_sql_window_main.h"
// Для работы с строками и файлами
#include <iostream>
#include <fstream>

using namespace std;
// Для работы с базами данных
QString Global_String_Querry;
int Current_Table_Number = 0;
int Current_BD_Type = 0;

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
    UpdateWindow = new dialog_sql_update();
    // Подключение через форму интерфейса
    connect(UpdateWindow, &dialog_sql_update::UpWindow, this, &SQL_Window_Main::show);
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
    qDebug() << "Just stay there!";

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
    // Строка для запроса из файлов
    std::string line;

    if (index == 0)
    {
        //qDebug() << "Тут пока пусто" << endl;
        Current_BD_Type = 0;

        //DB.close();
        //qDebug() << "Подключение к Microsoft Access закрыто";
        //QMessageBox::warning(this, "Уведомление", "Подключение к базе данных Microsoft Access закрыто");
        //QMessageBox::information(this, "Уведомление", "Подключение к базе данных Microsoft Access закрыто");

        /*
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

        TABLE_MODEL = new QSqlTableModel(this, DB);
        QString query_text = "SELECT * FROM public.\"Students\" ORDER BY student_id";
        QUERY_MODEL = new QSqlQueryModel();
        QUERY_MODEL->setQuery(query_text);
        ui->tableView->setModel(QUERY_MODEL);
        Current_Table_Number = 0;

    }
    if(index == 1)
    {
        QMessageBox::information(this, "Уведомление", "Функция временно недоступна");
    }

    /* Временно закрыто
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
    if (Current_BD_Type == 0)
    {
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
        }
        if(Current_Table_Number == 0)
        {
            /*
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            */
            qDebug() << "Just stay there!";
        }

    }
}


void SQL_Window_Main::on_pushButton_3_clicked()
{
    if (Current_BD_Type == 0)
    {
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
            /*
            TABLE_MODEL = new QSqlTableModel(this, DB);
            QString query_text = "";
            QUERY_MODEL = new QSqlQueryModel();
            QUERY_MODEL->setQuery(query_text);
            ui->tableView->setModel(QUERY_MODEL);
            */
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
        if(Current_Table_Number == 3)
        {
            qDebug() << "Just stay there!";
        }
    }
}


void SQL_Window_Main::on_pushButton_4_clicked()
{
    qDebug("Переход на окно вставки данных");
    // Переход на второе окно
    InsertWindow->show();
    // Закомментено, чтобы не закрывалось основное окно
    // А потом раскомментено, чтобы не вылетала программа
    // Но она все равно вылетает, хотя данные честно вставляет
    // роде перестала вылетать, так что снова закомменченно
    //this->close();

/*
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
    // Переход на окно удаления данных
    qDebug() << "Переход на окно удаления данных";
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
        Current_Table_Number = 3;
        /*
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
            Current_Table_Number = 3;
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
    // Переход на окно обновления данных
    qDebug() << "Переход на окно обновления данных";
    UpdateWindow->show();
}
