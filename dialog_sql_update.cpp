#include "dialog_sql_update.h"
#include "ui_dialog_sql_update.h"

#include <QCompleter>
#include <QSqlRecord>

dialog_sql_update::dialog_sql_update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_sql_update)
{
    ui->setupUi(this);

    TABLE_MODEL = new QSqlTableModel(this, DB);
    QString query_text = "";
    QUERY_MODEL = new QSqlQueryModel();
    QUERY_MODEL->setQuery(query_text);
}

dialog_sql_update::~dialog_sql_update()
{
    delete ui;
}
// Предел генерирования случайного числа
//int Colvo_Unic = 100;
int Up_Colvo_Unic = 10;
// Глобальный массив для работы с уникальностью номеров
int *up_variants_number = new int[Up_Colvo_Unic];

// Глобальные переменные для работы с формами
QString up_student_id, up_full_name, up_student_group, up_student_variant;
QString up_task_id, up_task_complication, up_task_text, up_task_variant;
int Up_Table_Index;

void dialog_sql_update::on_pushButton_OK_clicked()
{
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
                /*
                // Old tests
                //QUERY_MODEL->setQuery("SELECT * FROM public.\"Students\" WHERE student_id = "+up_student_id);
                //QUERY_MODEL->
                //up_full_name = QString::number(QUERY_MODEL->rowCount());
                */
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
}


void dialog_sql_update::on_pushButton_Esc_clicked()
{

    // Возврат к основному окну
    this->close();
    emit UpWindow();
}


void dialog_sql_update::on_tabWidget_currentChanged(int index)
{
    // Сообщение - предупреждение пользователю
    QMessageBox::warning(this, "WARNING", "Будьте осторожны, вводя новые данные. \n Изменения будут применены сразу.\n Вводите только те данные, которые хотите обновить.");

    if(index == 0)
    {
        Up_Table_Index = 0;
        qDebug() << "Обновление в таблице Students " << Up_Table_Index;
    }
    else if (index == 1)
    {
        Up_Table_Index = 1;
        qDebug() << "Обновление в таблице Tasks " << Up_Table_Index;
    }
}

void dialog_sql_update::changeIndex(int i)
{    i++;    }


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

    /*
    //QSqlQueryModel *model = new QSqlQueryModel(this);
    //model->setQuery("SELECT fieldWithNeedData FROM table", sqlBase); //Выбираем список фамилий из базы
    QUERY_MODEL->setQuery("SELECT student_id FROM public.\"Students\";");
    QLineEdit *lineEdit = new QLineEdit(this);
    QCompleter *completer = new QCompleter(QUERY_MODEL, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion); //вариант с выпадающим списком.
    lineEdit->setCompleter(completer);
    */
    //QLineEdit *lineEdit = new QLineEdit(on_lineEdit_student_id_editingFinished());

    /*
     * //заполнение combo
ui->combobox->addItem(TON_KG);

//условия
if(ui->combobox->currentText()==TON_KG
     */

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

void dialog_sql_update::on_tabWidget_Update_currentChanged(int index)
{
    // Сообщение - предупреждение пользователю
    QMessageBox::warning(this, "WARNING", "Будьте осторожны, вводя новые данные. \n Изменения будут применены сразу.\n Вводите только те данные, которые хотите обновить.");

    if(index == 0)
    {
        Up_Table_Index = 0;
        qDebug() << "Обновление в таблице Students " << Up_Table_Index;
    }
    else if (index == 1)
    {
        Up_Table_Index = 1;
        qDebug() << "Обновление в таблице Tasks " << Up_Table_Index;
    }
}

