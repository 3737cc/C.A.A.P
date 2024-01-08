#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString folderPath;    // 用于存储选择的文件夹路径
    QString targetFilePath; // 用于存储选择的目标文件路径
    QString saveFolderPath; // 用于存储选择的保存文件夹路径

    void on_pushButton_toggled(bool checked);

    void on_pushButton_pressed();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_label_linkActivated(const QString &link);

    void on_label_3_linkActivated(const QString& link);



    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
