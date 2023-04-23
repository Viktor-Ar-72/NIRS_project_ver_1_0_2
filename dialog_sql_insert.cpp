#include "dialog_sql_insert.h"
#include "ui_dialog_sql_insert.h"
// Библиотеки для генератора на основе Вихря Мерсенна
#include <stdlib.h>
#include <cstdlib>
#include <random>
#include <QMessageBox>
// Подключение окна MainWindow для передачи кое-каких параметров
#include <sql_window_main.h>
#include <QFile>

/*
// Предел генерирования случайного числа
//int Colvo_Unic = 100;
//int Colvo_Unic = 10;
// Глобальный массив для работы с уникальностью номеров
//int *variants_number = new int[Colvo_Unic];
*/
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

// Макрос дял определения размера массива (на всякий случай)
// Закомменчен, так как похоже, не требуется
//#define _countof(arr) (sizeof(arr) / sizeof((arr)[0]))

Dialog_SQL_Insert::Dialog_SQL_Insert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SQL_Insert)
{
    ui->setupUi(this);
    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);

    // Настройка списка таблиц - old
    /*
    QStringList test_list;
    qDebug() << "Tables list now - " << Insert_BD_Tables_List_Asked;
    for (int i = 0; i < Insert_BD_Tables_List_Asked.size(); ++i)
    {
        test_list << Insert_BD_Tables_List_Asked[i];
    }
    ui->comboBox_table_change->addItems(test_list);
    */
    /* Создание заглушки - old
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

    //  Инициализация lineEdit_tableNow
    ui->lineEdit_tableNow->setText("Тип генератора?");
    //ui->lineEdit_tableNow->setModified(0);
    ui->lineEdit_tableNow->setReadOnly(1);
    // Настройка цвета
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Base,Qt::lightGray);
    palette->setColor(QPalette::Text,Qt::darkGray);
    ui->lineEdit_tableNow->setPalette(*palette);

    /* Создание QTextEdit для справки - old
    //QLabel *KnowText = new QLabel();
    //KnowText->setReadOnly(true);
    //KnowText->setText("Используйте вкладку \"Вставка данных\" для генерации и вставки данных в выбранную таблицу базы данных. Обратите внимание, что вставка возможна лишь в случае, когда был выбран один из генераторов.                                                                 Введите целевые границы для типов вставляемых данных INT, REAL и BYTEA. Обратите внимание, что обе границы должны быть указаны, и не должны совпадать. Если хотите, чтобы генерируемые значения всегда были равны N, то установите в качестве границ N и N+1 (для типа данных INT) или N и N + 0.0001 (для типа данных REAL).                            В генерируемом типе данных VARCHAR всегда будут исключительно текстовые символы английского алфавита верхних и нижних регистров, а также пробел. Необходимо указать лишь длину генерируемой последовательности.               По умолчанию в запросах вставки значения типа BOOL будут равны TRUE. Вы можете изменить это, установив галочку в поле \"Иcпользовать генерацию\". Тогда будут генерироваться и значение FALSE, и значение TRUE.");
    //ui->scrollArea->setWidget(KnowText);
    //ui->scrollArea->setWidgetResizable(true);
    //ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    */
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
//QString student_id, full_name, student_group, variant;
//QString task_id, task_complication, task_text, task_variant;
//int Table_Index;

// Глобальные переменные для работы с подключением к базе данных
QString Insert_Transfer_DB_Adress, Insert_Transfer_DB_Name, Insert_Transfer_DB_User, Insert_Transfer_DB_Password;
int Insert_Transfer_DB_Port;

// Глобальные переменные для формы вставки
int Insert_Table_Index;
int Insert_Fields_Number = 0, Insert_int_left = 0, Insert_int_right = 0, Insert_varchar_lenght, Insert_byteA_lenght, Insert_use_boolean;
double Insert_real_left, Insert_real_right;


/* OLD Student_Table_Insert
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

}
*/


void Dialog_SQL_Insert::on_pushButton_clicked()
{
    /*
     * Insert INTO public."TestTable_1" VALUES (34523, 54434, 'Hello', TRUE);
     * INSERT INTO public."TestTable_2" VALUES ('Text_1', 'Text_2', '\xABCDEF', 1.78);
     * DELETE FROM public."TestTable_2" WHERE "Column_Text_1" = 'Text_1' AND "Column_Text_2" = 'Text_2' AND "Column_ByteA" = '\xABCDEF' AND "Column_Real" = 1.78;
     * MySQL
     * INSERT INTO `testdatabasemysql`.`test_table_1` (`Column_Int_1`, `Column_Int_2`, `Column_Text`, `Column_Bool`) VALUES ('1', '1', 'Hello, world!', '1');
     * DELETE FROM `testdatabasemysql`.`test_table_1` WHERE (`Column_Int_1` = '2');
     * Microsoft Access
     * INSERT INTO Table2 VALUES (11, 'Test_Text_Insert', 70);
     *
     **/
    /*
    // Тесты
    //int test_int = 0;
    if (Insert_Fields_Number > 0)
    {

        //int br = Insert_Fields_Number;
        //int[br] test_int;
        //for(int i = 0; i < Insert_Fields_Number; ++i)
        //{
        //    test_int_mass[i] = 0;
        //}
        */

    /* Old
    //if ((Insert_Fields_Number > 0) && (Insert_Fields_Number < 10001))
    //{
    end of old */
        // Создаём N-ое количество массивов, для каждого типа столбца текущей таблицы
        // Для вывода матрицы пока что пользуюсь таким способом - при использовани count() или size() вылетает почему - то
        QString query_text;
        QSqlRecord Record_Test;
        QSqlQuery Query_Test;
        query_text = "SELECT * FROM public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\"";
        Query_Test.exec(query_text);
        Record_Test = Query_Test.record();
        //for (int R = 0; R < Insert_Matrix_Tables_FieldNames[Insert_Table_Index]->count() - 1; ++R)
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
        //int *test_int_mass = new int[Insert_Fields_Number];
        int *Insert_int_mass = new int[(Insert_Fields_Number * k_int)];
        QString* Insert_varchar_mass = new QString[(Insert_Fields_Number * k_varchar)];
        double* Insert_real_mass = new double[(Insert_Fields_Number * k_real)];
        bool* Insert_bool_mass = new bool[(Insert_Fields_Number * k_boolean)];
        QString* Insert_bytea_mass = new QString[(Insert_Fields_Number * k_bytea)];

        /*
    //for (int R = 0; R < Record_Test.count(); ++R)
    //{

    //if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][R] == "int")
    //{
    */
        if (int_use == 'y')
        {
            if ((Insert_int_left != Insert_int_right) && (ui->lineEdit_int_left->text() != "") && (ui->lineEdit_int_right->text() != "") && (Insert_int_left < Insert_int_right))
            {
                //int *test_int_mass = new int[Insert_Fields_Number];
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
                    //Insert_int_mass = VihrMersenna_Gen_Int(Insert_int_mass, Insert_Fields_Number, Insert_int_left, Insert_int_right);
                    Insert_int_mass = VihrMersenna_Gen_Int(Insert_int_mass, (Insert_Fields_Number * k_int), Insert_int_left, Insert_int_right);
                }
                else if (GenType == 2)
                {
                    //Insert_int_mass = MacLarenMarsalii_Gen_Int(Insert_int_mass, Insert_Fields_Number, Insert_int_left, Insert_int_right);
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
                    /*
            // Test
            //char test_s;
            //test_int_mass[i] = 122;
            //qDebug() << "Символ кода - " << char(test_int_mass[i]);

            // Just for a test Debug message
            //if ((i % Insert_Fields_Number) == 1)
            //{
            //    qDebug() << "Сдвиг столбца";
            //}
            */
                }
                /* Type Real test
                    double* test_real_mass = new double[22];
                    //test_real_mass = VihrMersenna_Gen_Real(test_real_mass, 5, 4.7, 11.8);
                    //test_real_mass = MacLarenMarsalii_Gen_Real(test_real_mass, 5, 52.7, 64.8);
                    for (int j = 0; j < 5; ++j) {
                        qDebug() << "Полученный результат" << j << " теста Real = " << test_real_mass[j];
                    }
                    delete[] test_real_mass;
                    */

                /*
            // Type char test
            //QString* test_QString_mass = new QString[5];
            //test_QString_mass = VihrMersenna_Gen_Char(test_QString_mass, 5, 8, 0, 125);
            //test_QString_mass = MacLarenMarsalii_Gen_Char(test_QString_mass, 5, 10, 0, 125);
            //for (int j = 0; j < 5; ++j) {
            //    qDebug() << "Полученный результат" << j << " теста QString = " << test_QString_mass[j];
            //}
            //delete[] test_QString_mass;
            //delete[] Insert_int_mass;
            */

            }
            else
            {
                QMessageBox::critical(this, "ERROR", "Нарушение целевых границ генерируемой последовательности INT!\n Пожалуйста, переопределите!");
                return;
            }
        }

        /*
        //qDebug() << "Был обработан " << R << " столбец текущей таблицы, тип " <<Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][R];
        //}
        //}
        //else
        //{
        //QMessageBox::critical(this, "ERROR", "Введено неккоректное количество новых строк!\n Пожалуйста, введите число от 1 до 10000.");
        //}
        */

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
                    //if ((Insert_int_left != Insert_int_right) && (ui->lineEdit_int_left->text() != "") && (ui->lineEdit_int_right->text() != ""))
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
            //qDebug() << "Будет использоватьмся массив byteA";
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




        /*
        //else
        //{
        //    QMessageBox::critical(this, "ERROR", "Не указаны размер последовательности VARCHAR!\n Пожалуйста, переопределите!");
        //    return;
        //}
    //}

// Old
    //else
    //{
    //    QMessageBox::critical(this, "ERROR", "Введено неккоректное количество новых строк!\n Пожалуйста, введите число от 1 до 10000.");
    //}
// end of old
*/
    // Вставка значений в таблицу через Insert
    // Testes
    //QString query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" (Column_Int_1, Column_Int_2, Column_Text, Column_Bool) VALUES ('0'::integer, '0'::integer, 'test_insert'::text, 'TRUE'::boolean)";
    // Такая реализаия работает. Надо придумать, как редактировать состав вставляемых данных
    //QString query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" VALUES (0, 0, 'Test_insert_1', TRUE)";
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
    //int h = Record_Test.count();
    //qDebug() << "В данной таблице необходимо вставить " << h << " столбцов";
    QString query_insert_text;
    QString boolean_type_QString;
    for (int V = 0; V < Insert_Fields_Number; ++V)
    {
        //query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" VALUES (";
        //query_insert_text = "";
        if (BD_Type_Insert == 0)
        {
            // Создание шаблона запроса. И снова спасибо ПЗЕ4:)
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
        //int end_index_int = start_index_int + k_int;
        int start_index_varchar = V * k_varchar;
        int start_index_real = V * k_real;
        int start_index_bool = V * k_boolean;
        int start_index_bytea = V * k_bytea;

        for (int A = 0; A < Insert_Record.count(); ++A)
        {

            if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][A] == "int")
            {
                //query_insert_text = query_insert_text + QString(Insert_int_mass[Insert_Fields_Number]) + ", ";
                //query_insert_text = query_insert_text + "Нужно в столбец " + QString::number(A) + " добавить значение int, ";
                //query_insert_text = query_insert_text + QString::number(Insert_int_mass[V]) + ", ";
                query_insert_text = query_insert_text + QString::number(Insert_int_mass[start_index_int]) + ", ";
                ++start_index_int;
            }
            else if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][A] == "QString")
            {
                //query_insert_text = query_insert_text + Insert_varchar_mass[Insert_Fields_Number] + ", ";
                //query_insert_text = query_insert_text + "Нужно в столбец " + QString::number(A) + " добавить значение QString, ";
                //query_insert_text = query_insert_text + "'" + Insert_varchar_mass[V] + "', ";
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
                //query_insert_text = query_insert_text + "'" + R"(\x)" + Insert_bytea_mass[start_index_bytea] + "', ";
                query_insert_text = query_insert_text + "'";
                query_insert_text += QString::fromUtf8("\x5C\x78");
                // int index = query_insert_text.lastIndexOf("\\");
                // query_insert_text.replace(index, 1, "\x5C\x78");
                // query_insert_text.remove(0, 1);
                // query_insert_text.prepend("'");
                query_insert_text = query_insert_text + Insert_bytea_mass[start_index_bytea] + "', ";
                ++start_index_bytea;
                // Test byteA input check
                QFile file("output_BYTEA_Test.txt");
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    out << query_insert_text; // записываем содержимое строки в файл
                    file.close();
                }
            }
            else if(Insert_Matrix_Tables_FieldTypes[Insert_Table_Index][A] == "bool")
            {
                //query_insert_text = query_insert_text + Insert_bool_mass[Insert_Fields_Number] + ")";
                //query_insert_text = query_insert_text + "Нужно в столбец " + QString::number(A) + " добавить значение bool, ";
                //QString q = "";
                boolean_type_QString = "";
                //if (Insert_bool_mass[V] == true)
                if (Insert_bool_mass[start_index_bool] == true)
                { boolean_type_QString = "TRUE"; }
                else
                { boolean_type_QString = "FALSE"; }
                query_insert_text = query_insert_text + boolean_type_QString + ", ";
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
        int position = query_insert_text.lastIndexOf(QChar(','));
        query_insert_text = query_insert_text.left(position);
        query_insert_text = query_insert_text.append(");");
        qDebug() << "Запрос на вставку" << V << query_insert_text;
        // Если временно закомменчено - значит, происходит тест на вставку данных
        //QUERY_MODEL->setQuery(query_insert_text);
        //qDebug() << "Запрос на вставку" << V << "выполнен.";
    }

    /* Возможный вариант реализации просмотра нескольких значений массива на одну итерацию Insert от GPT-4
     *
    int n = 5; // количество итераций
    int k = 2; // количество чисел на каждой итерации
    int* array = new int[n * k]; // массив из N * k чисел

    for (int i = 0; i < n; i++) {
        int start_index = i * k;
        int end_index = start_index + k;
        int* iteration_numbers = new int[k];
        for (int j = start_index; j < end_index; j++) {
            iteration_numbers[j - start_index] = array[j];
        }
        cout << "Iteration " << i << ": ";
        for (int l = 0; l < k; l++) {
            cout << iteration_numbers[l] << " ";
        }
        cout << endl;
        delete[] iteration_numbers; // освобождение памяти после использования
    }

    delete[] array; // освобождение памяти после использования

    return 0;
}

     */

    //query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" VALUES (" + QString(rand() % 255) + ", " + QString(rand() % 255) + ", 'Test_insert_1', TRUE)";
    // Сверка с генерированными запросами
    //query_insert_text = "INSERT INTO public.\"" + Insert_BD_Tables_List_Asked[Insert_Table_Index] + "\" VALUES (0, 0, 'Test_insert_1', TRUE)";
    //qDebug() << "Запрос на вставку" << 9 << query_insert_text;

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
        //Table_Index = 0;
        //qDebug() << "Вставка в таблицу Students " << Table_Index;
        qDebug() << "Окно формы генерации";
    }
    else if (index == 1)
    {
        //Table_Index = 1;
        //qDebug() << "Вставка в таблицу Tasks " << Table_Index;
        qDebug() << "Окно справки по форме Insert";
    }
}


/* OLD Task_Insert_Functions
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


        //else
        //{
        //    variant = QString::number(rand_variant_task);
        //    t = 'Y';
        //}




//qDebug() << "Текущее значение количества записей - " << i;

        qDebug() << "Текущее значение варианта - " << task_variant;
        //}
    }
}

*/


// Новая функция для получения данных о таблицах
void Dialog_SQL_Insert::get_DB_Table_Info(QStringList DB_tables_list, QString **Matrix_Names, QString **Matrix_Types, int DB_Class)
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

    /*
    //QStringList new_table_list;
    //new_table_list = SQL_Window_Main().take_list_of_Tables(new_table_list);
    //qDebug() << "New string list is" << new_table_list;
    //ui->comboBox_table_change->setCurrentIndex(1);
    //ui->comboBox_table_change->setItemText(1, "Hello_there!");
    //ui->comboBox_table_change->lineEdit()->setPlaceholderText("Hello there new");

    // Хрена с два этот Combobox позволит себя редачить вне конструктора
    // Как вариант - запрашивать данные о БД прямо в конструкторе и дальше инициализировать
    //Dialog_SQL_Insert();
    */


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
        //Insert_Transfer_DB_Adress = DB.hostName();
        //qDebug() << "Insert: Полученный адрес БД в Qstring - " << Insert_Transfer_DB_Adress;
        //Insert_Transfer_DB_Port = DB.port();
        //qDebug() << "Insert: Полученный port БД в Qstring" << QString::number(Insert_Transfer_DB_Port);
        Insert_Transfer_DB_Name = DB.databaseName();
        qDebug() << "Insert: Полученное имя БД в QString" << Insert_Transfer_DB_Name;
        //Insert_Transfer_DB_User = DB.userName();
        //qDebug() << "Insert: Полученный логин пользователя в QString" << Insert_Transfer_DB_User;
        //Insert_Transfer_DB_Password = DB.password();
        //qDebug() << "Insert: Полученный пароль пользователя в QString" << Insert_Transfer_DB_Password;

        qDebug() << "Проверка на открытие файла с запрошенными параметрами";
        DB = QSqlDatabase::addDatabase("QODBC", "InsertWindowConnect");
        //DB.setHostName(Insert_Transfer_DB_Adress);
        //DB.setPort(Insert_Transfer_DB_Port);
        DB.setDatabaseName(Insert_Transfer_DB_Name);
        //DB.setUserName(Insert_Transfer_DB_User);
        //DB.setPassword(Insert_Transfer_DB_Password);
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
    Insert_real_left = ui->lineEdit_real_left->text().toDouble();
    qDebug() << "Левая граница типа Real для генератора - " << Insert_real_left;
}


void Dialog_SQL_Insert::on_lineEdit_real_right_editingFinished()
{
    Insert_real_right = ui->lineEdit_real_right->text().toDouble();
    qDebug() << "Правая граница типа Real для генератора - " << Insert_real_right;
}

/* Old ByteA, removed
void Dialog_SQL_Insert::on_lineEdit_bytea_left_editingFinished()
{
    Insert_byteA_left = ui->lineEdit_bytea_left->text().toInt();
    qDebug() << "Левая граница типа ByteA для генератора - " << Insert_byteA_left;

}
*/


void Dialog_SQL_Insert::on_lineEdit_bytea_lenght_editingFinished()
{
    Insert_byteA_lenght = ui->lineEdit_bytea_lenght->text().toInt();
    qDebug() << "Полученная длина последовательности Bytea / Blob - " << Insert_byteA_lenght;
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
    //std::mt19937_64 engine;
    //std::random_device device;
    //engine.seed(device());
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
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
    //std::ranlux24 engine;
    //std::random_device device;
    //engine.seed(device());
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
    return getted_rand_mass;
    delete[] matrix_3;
    engine.seed();
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
        //getted_rand_mass[i] = gen_VM_left_edge + double(engine()) % (gen_VM_right_edge + 1 - gen_VM_left_edge);
        // Работающий, но с огрехами
        //getted_rand_mass[i] = gen_VM_left_edge + engine() * (gen_VM_right_edge + 1 - gen_VM_left_edge);
        // Вариант от ПЗЕ4, работает хорошо))
        double rand_num = double(engine()) / engine.max(); // Случайное число от 0 до 1
        getted_rand_mass[i] = gen_VM_left_edge + range * rand_num;
    }
    return getted_rand_mass;
    // Вроде как должно сменить зерно генерации, чтобы не было повторяющихся значений
    engine.seed();
}

double* Dialog_SQL_Insert::MacLarenMarsalii_Gen_Real(double *getted_rand_mass, int rasmer, double gen_MM_left_edge, double gen_MM_right_edge)
{
    /*
    // Инициализация привязки устройства
    std::ranlux24 engine;
    std::random_device device;
    engine.seed(device());
    // Инициализаций матрицы генератора
    double *matrix_3 = new double[1000];
    for (int i = 0; i < 1000; ++i)
    {
        matrix_3[i] = gen_MM_left_edge + engine() * (gen_MM_right_edge + 1 - gen_MM_left_edge);
    }
    double P_3_1; int P_3_2, P_3_3;
    //P_3_3 = NULL;
    for (int i = 0; i < rasmer; ++i)
    {
        P_3_1 = gen_MM_left_edge + engine() * (gen_MM_right_edge + 1 - gen_MM_left_edge);
        P_3_2 = engine() % 1000;
        P_3_3 = engine() % 1000;
        P_3_3 = (P_3_2 * P_3_3) % 1000;
        //P_3_2 = gen_MM_left_edge + engine() * (gen_MM_right_edge + 1 - gen_MM_left_edge);
        //P_3_3 = (P_3_1 * P_3_2) % 1000;
        //P_3_3 = (P_3_1 * P_3_2) % 1000 * 1.0;
        //P_3_3 = ((P_3_1 * P_3_2 * 1.0) - (P_3_1 * P_3_2 / 1000.0));
        getted_rand_mass[i] = matrix_3[P_3_3];
        matrix_3[P_3_3] = P_3_1;
    }
    return getted_rand_mass;
    delete[] matrix_3;
    engine.seed();
    */
    // Вариант от ПЗЕ4
    // Инициализация привязки устройства
    //std::ranlux24 engine;
    //std::random_device device;
    //engine.seed(device());
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
    //std::mt19937_64 engine;
    //std::random_device device;
    //engine.seed();
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
        //massiv_gen_numbers[i] = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
    }
    // Вроде как должно сменить зерно генерации, чтобы не было повторяющихся значений
    engine.seed();
    return massiv_gen_numbers;
}

QString* Dialog_SQL_Insert::MacLarenMarsalii_Gen_Char(QString *massiv_gen_numbers, int rasmer, int length, int gen_MM_left_edge, int gen_MM_right_edge)
{
    // Инициализация привязки устройства
    //std::ranlux24 engine;
    //std::random_device device;
    //engine.seed(device());
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
            // Testes
            //int k = matrix_3[j];
            //word = word + QString(k);
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
    //std::mt19937_64 engine;
    //std::random_device device;
    //engine.seed();
    // Инициализация привязки устройства
    std::mt19937_64 engine;
    std::random_device device;
    engine.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    int t = 0; bool r = 0; QString s;
    for (int i = 0; i < rasmer; ++i)
    {
        //s = "\\x";
        //s = QString::fromLatin1("\\x");
        s = "";
        for (int j = 0; j < length; ++j)
        {
            r = 0;
            while(r == 0)
            {
                t = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
                // Старое условие, не треубется
                //if ((j == 0) && (t == 48))
                //{ r = 0;}
                //else
                //{
                    if (((t > 47) && (t < 58)) || ((t > 64) && (t < 71)))
                    {
                        s = s + QString(t);
                        r = 1;
                    }
                    else
                    {
                        r = 0;
                    }
                //}
            }
        }
        massiv_gen_numbers[i] = s;
        //massiv_gen_numbers[i] = gen_VM_left_edge + engine() % (gen_VM_right_edge + 1 - gen_VM_left_edge);
    }
    // Вроде как должно сменить зерно генерации, чтобы не было повторяющихся значений
    engine.seed();
    return massiv_gen_numbers;
}

QString* Dialog_SQL_Insert::MacLarenMarsalii_Gen_ByteA(QString *massiv_gen_numbers, int rasmer, int length, int gen_MM_left_edge, int gen_MM_right_edge)
{
    // Инициализация привязки устройства
    //std::ranlux24 engine;
    //std::random_device device;
    //engine.seed(device());

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
            //if ((i == 0) && (init == 48))
            //{ r = 0;}
            //else {
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
        //word = "\\x";
        //word = QString::fromLatin1("\\x");
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
                    //if ((P_3_3 == 48) && (j == 0))
                    //{r = 0;}
                    //else
                    //{
                        word = word + QString(matrix_3[P_3_3]);
                        matrix_3[P_3_3] = P_3_1;
                        r = 1;
                    //}
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
