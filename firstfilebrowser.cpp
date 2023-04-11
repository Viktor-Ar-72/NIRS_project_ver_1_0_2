#include "firstfilebrowser.h"
#include "ui_firstfilebrowser.h"
#include <QDebug>
#include <loginwindow.h>

// Глобальная переменная, хранящая адрес каталога/файла
QString Object_Adress;

FirstFileBrowser::FirstFileBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirstFileBrowser)
{
    ui->setupUi(this);

    // Подключение элементов GUI
    // Показать текущий список файлов
    //connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(showCurrentDirFiles()));
    // Update: если использовать returnPressed, то вылетает окно браузера, и приходится включать заново, чтобы увидеть результат.
    // Так нормально выводит, когда жмешь за пределы линии, но при нажатии на Enter все равно есть такой баг
    connect(ui->lineEdit, SIGNAL(editingFinished()), this, SLOT(showCurrentDirFiles()));
    // Показать новый список файлов после кликов
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showNextDirFiles(QListWidgetItem*)));
    // Этот вроде нормально работает. Можно ещё попробовать папку с точкой убрать, видел где-то гайд.

    // Ещё неплохо бы настроить, чтобы сразу был активен на диск D
    // Вот так работает)
    ui->lineEdit->setText("D:/");
    ui->lineEdit->editingFinished();
    // Можно попробовать вывести нужный файл с конфигурацией по однократному нажатию
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(copyAdressReturn(QListWidgetItem*)));

}

void FirstFileBrowser::copyAdressReturn(QListWidgetItem *item)
{
    // Можно получить имя файла, дважды кликнув по нему
    QString file_name = item->text();
    // Запись имени в глобальную переменную
    Object_Adress = ui->lineEdit->text() + "/" + item->text() + "/";
    qDebug() << "Global address (one click) = " + Object_Adress;
}

FirstFileBrowser::~FirstFileBrowser()
{
    delete ui;
}

void FirstFileBrowser::on_OK_button_clicked()
{
    LoginWindow().get_PSQL_config_adress(Object_Adress);
    this->close();
}

void FirstFileBrowser::showCurrentDirFiles()
{
    // Общее: Выводит введённый в lineEdit каталог
    // Получает текущий введённый адрес каталога
    QDir currentDir(ui->lineEdit->text());
    // Переменная для отображения
    QStringList files_list;
    files_list << "*";
    // Функция entryInfoList возвращает информацию о списке файлов, имеет три параметра
    // 1) Фильтры на имена: * - любые
    // 2) Фильтры на тип: AllEntries - любой (диски, папки, файлы)
    // 3) Сортировка: DirsFirst - сначала папки
    QFileInfoList Files_Info_List = currentDir.entryInfoList(files_list, QDir::AllEntries, QDir::DirsFirst);
    // Отображение в QListWidget через вызов отдельной функции
    this->showFileInfoList(Files_Info_List);
}

void FirstFileBrowser::showNextDirFiles(QListWidgetItem *item)
{
    //QListWidgetItem zaglushka = *item;
    // Общее: Получает новый каталог по клику
    // Можно получить имя файла, дважды кликнув по нему
    QString file_name = item->text();
    // Запись имени в глобальную переменную -- перенесено в отдельную функцию
    //Object_Adress = ui->lineEdit->text() + "/" + item->text() + "/";
    //qDebug() << "Global address (double-click) = " + Object_Adress;
    // Создать новую переменную каталога
    QDir new_Dir;
    // Установить новый путь
    new_Dir.setPath(ui->lineEdit->text());
    // Сбросить старый путь к файлу
    new_Dir.cd(file_name);
    // Обновить текущий путь в lineEdit
    ui->lineEdit->setText(new_Dir.absolutePath());
    // Обновить список файлов в QListWidget
    showCurrentDirFiles();
}

void FirstFileBrowser::showFileInfoList(QFileInfoList pInfoList)
{
    // Общее: Вывод информации о файлах в QWidgetList
    // Очистка QWidgetList
    ui->listWidget->clear();
    // Вывод в цикле всех объектов в папке
    for(int i = 0; i < pInfoList.size(); ++i)
    {
        // Переменные для инфы об объектах в каталоге
        QFileInfo tmpInfo = pInfoList.at(i);
        QString object_name = tmpInfo.fileName();

        // Чтобы убрать папку "."
        if (object_name == ".")
        {
            continue;
        }
        // Попытка, чтобы не выводил файлы определённого типа
        if (object_name.contains(".exe") || object_name.contains(".jpg") || object_name.contains(".docx")
                || object_name.contains(".pdf") || object_name.contains(".doc") || object_name.contains(".xlsx")
                || object_name.contains(".zip") || object_name.contains(".png") || object_name.contains(".pptx")
                || object_name.contains(".m4a") || (object_name.contains(".rar")))
        {
            continue;
        }

        /*
        // Чтобы пользователю было проще
        QString dop = "";
        if (object_name == "..")
        {
            //object_name = "Перейти в в папку верхнего уровня";
            dop = ".Перейти в папку верхнего уровня...";
        }
        // Но это не очень работает - при замене имени перестают работать папки верхнего уровня
        */

        // Переменная для данных выводимого объекта
        QListWidgetItem *tmpItem = new QListWidgetItem(object_name);
        // Установка иконки в зависимости от типа
        if(tmpInfo.isDir())
        {
            tmpItem->setIcon(*getItemPropertyIcon(2));
        }
        else
        {
            tmpItem->setIcon(*getItemPropertyIcon(1));
        }
        // Собственно, сам вывод
        ui->listWidget->addItem(tmpItem);
    }
}

QIcon * FirstFileBrowser::getItemPropertyIcon(int object_type)
{
    // Запрос на путь к папке приложения
    QDir exe_dir;
    QString exe_path = exe_dir.currentPath();
    // Выбор иконки в зависимости от типа файла
    switch (object_type)
    {
    case 1:
        return new QIcon(exe_path + "/Config.png");
        break;
    case 2:
        return new QIcon(exe_path + "/Dir.png");
        break;
    }
    // Дефолтный return
    return NULL;
}

void FirstFileBrowser::on_return_button_clicked()
{
    this->close();
}

