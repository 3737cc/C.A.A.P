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
//对齐的相关变量
    QString Alignment_folderPath;    // 用于存储选择的文件夹路径
    QString Alignment_targetFilePath; // 用于存储选择的目标文件路径
    QString Alignment_saveFolderPath; // 用于存储选择的保存文件夹路径
//叠加的相关变量
    QString Superposition_folderPath;    // 用于存储选择的文件夹路径
    QString Superposition_saveFolderPath; // 用于存储选择的保存文件夹路径

//降噪相关变量
    QString Noise_reduction_targetFilePath;
    QString Noise_reduction_saveFolderPath;

//校准相关变量
    QString Calibration_folderPath;     //选择
    QString Flat_targetFilePath;
    QString Dark_targetFilePath;
    QString Bias_targetFilePath;
    QString Calibration_saveFolderPath; //保存

private slots:
    //对齐相关操作
    void on_Alignment_function_button_clicked();

    void on_Select_file_button_aligning_clicked();

    void on_Object_file_button_aligning_clicked();

    void on_Save_file_button_aligning_clicked();

    void on_Alignment_button_clicked();

    //叠加相关操作
    void on_Superposition_function_button_clicked();

    void on_Select_file_button_superposing_clicked();

    void on_Save_file_button_superposing_clicked();

    void on_Superposition_button_clicked();

    //降噪相关操作

    void on_Noise_reduction_function_button_2_clicked();

    void on_Select_file_button_noise_reduction_clicked();

    void on_Save_file_button_noise_reduction_clicked();

    void on_Noise_reduction_button_clicked();

    void on_Bilateral_reduction_button_clicked();


    //校准相关操作

    void on_Calibration_function_button_clicked();


    void on_Select_file_button_calibration_clicked();

    void on_Object_file_button_flat_clicked();

    void on_Object_file_button_dark_clicked();

    void on_Object_file_button_bias_clicked();

    void on_Save_file_button_calibration_clicked();

    void on_Flat_field_button_clicked();

    void on_Dark_field_button_clicked();

    void on_Bias_field_button_clicked();

    void on_Calibration_button_clicked();



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
