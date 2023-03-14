#ifndef FIRSTFILEBROWSER_H
#define FIRSTFILEBROWSER_H

#include <QDialog>
// Для иконок файлов, оглавления и информации о файлах
#include <QIcon>
#include <QDir>
#include <QFileInfoList>
#include <QListWidgetItem>

namespace Ui {
class FirstFileBrowser;
}

class FirstFileBrowser : public QDialog
{
    Q_OBJECT

// Инициализация для окна авторизации
signals:
    void fileWindow();

public:
    explicit FirstFileBrowser(QWidget *parent = nullptr);
    ~FirstFileBrowser();

    //Показать детали в текущем каталоге в QListWidget
    void showFileInfoList(QFileInfoList pInfoList);
    //Получить значок в соответствии с характером файла
    //iType = 1: папка, iType = 2: файл
    QIcon *getItemPropertyIcon(int iType);

public slots:
    //Показать папки и файлы в текущем каталоге
    void showCurrentDirFiles();
    //Отображение файлов в папке в списке двойным щелчком
    void showNextDirFiles(QListWidgetItem *item);

    // Для одиночного клика по файлу
    void copyAdressReturn(QListWidgetItem *item);

private slots:
    void on_OK_button_clicked();

    void on_return_button_clicked();

private:
    Ui::FirstFileBrowser *ui;
};

#endif // FIRSTFILEBROWSER_H
