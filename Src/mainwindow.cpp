#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "smooth.h"
#include "fitscalibration.h"
#include "fitsmerger.h"
#include "fitsaligner.h"
#include "QDebug"
#include <QFileDialog>
#include <QString>
#include <QByteArray>
#include <bilateral.h>
#include <baryer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//进行对齐的相关操作

void MainWindow::on_Alignment_function_button_clicked()
{
    int currentIndex = ui->Alignment_widget->currentIndex();

    // 如果当前页面是页面0，切换到其他页面；否则，切换回页面0
    ui->Alignment_widget->setCurrentIndex(currentIndex == 0 ? 1 : 0);
}


void MainWindow::on_Alignment_button_clicked()
{

    if (Alignment_folderPath.isEmpty() || Alignment_targetFilePath.isEmpty() || Alignment_saveFolderPath.isEmpty()) {
        qDebug() << "Please select folder, target file, and save folder paths.";
        return;
    }
    QDir directory(Alignment_folderPath);
    QStringList fitsFiles = directory.entryList(QStringList() << "*.fits", QDir::Files);

    for (const QString& fitsFile : fitsFiles) {
        QString filePath = Alignment_folderPath + "/" + fitsFile;
        QByteArray utf8DatFoler = filePath.toUtf8();
        const char* charFolderPath = utf8DatFoler.constData();

        QString targetFilePath = Alignment_targetFilePath;
        QByteArray utf8TargetFoler = targetFilePath.toUtf8();
        const char* charTargetFilePath = utf8TargetFoler.constData();

        QString savePath = Alignment_saveFolderPath + "/Aligned_" + fitsFile;
        QByteArray utf8saveFoler = savePath.toUtf8();
        const char* charsavePath = utf8saveFoler.constData();

        processFITSFilesAffine(charFolderPath, charTargetFilePath, charsavePath);
    }

}

void MainWindow::on_Select_file_button_aligning_clicked()
{
    Alignment_folderPath = QFileDialog::getExistingDirectory(this, "选择包含对齐fits图文件夹", QDir::homePath());

    if (!Alignment_folderPath.isEmpty()) {
        // 用户选择了文件，可以进行相应的操作
        ui->Select_file_label_aligning->setText("文件路径: " + Alignment_folderPath);
        qDebug() << "Selected file path: " << Alignment_folderPath;
    }
}


void MainWindow::on_Object_file_button_aligning_clicked()
{
    Alignment_targetFilePath = QFileDialog::getOpenFileName(this, "选择对齐目标文件", QDir::homePath(), "FITS Files (*.fits)");
    if (!Alignment_targetFilePath.isEmpty()) {
        ui->Object_file_label_aligning->setText("目标文件路径: " + Alignment_targetFilePath);
        qDebug() << "Selected target file path: " << Alignment_targetFilePath;
    }
}

void MainWindow::on_Save_file_button_aligning_clicked()
{
    Alignment_saveFolderPath = QFileDialog::getExistingDirectory(this, "选择保存文件夹", QDir::homePath());
    if (!Alignment_saveFolderPath.isEmpty()) {
        ui->Save_file_label_aligning->setText("保存文件夹路径: " + Alignment_saveFolderPath);
        qDebug() << "Selected save folder path: " << Alignment_saveFolderPath;
    }
}


//叠加相关操作

void MainWindow::on_Superposition_function_button_clicked()
{
    int currentIndex = ui->Superposition_widget->currentIndex();

    // 如果当前页面是页面0，切换到其他页面；否则，切换回页面0
    ui->Superposition_widget->setCurrentIndex(currentIndex == 0 ? 1 : 0);
}

void MainWindow::on_Select_file_button_superposing_clicked()
{
    Superposition_folderPath = QFileDialog::getExistingDirectory(this, "选择包含叠加fits图文件夹", QDir::homePath());

    if (!Superposition_folderPath.isEmpty()) {
        // 用户选择了文件，可以进行相应的操作
        ui->Select_file_label_superposing->setText("文件路径: " + Superposition_folderPath);
        qDebug() << "Selected file path: " << Superposition_folderPath;
    }
}

void MainWindow::on_Save_file_button_superposing_clicked()
{
    Superposition_saveFolderPath = QFileDialog::getExistingDirectory(this, "选择保存文件夹", QDir::homePath());
    if (!Superposition_saveFolderPath.isEmpty()) {
        ui->Save_file_label_superposing->setText("保存文件夹路径: " + Superposition_saveFolderPath);
        qDebug() << "Selected save folder path: " << Superposition_saveFolderPath;
    }
}

void MainWindow::on_Superposition_button_clicked()
{
    FitsMerger fitsMerger;

    QDir dir(Superposition_folderPath);

    // 获取文件夹下所有的文件
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);

    // 过滤出 fits 文件
    QStringList fitsFiles;
    for (const QFileInfo& fileInfo : fileInfoList) {
        if (fileInfo.suffix().toLower() == "fits") {
            fitsFiles.append(fileInfo.absoluteFilePath());
        }
    }

    // 将 QStringList 转换为 std::vector<std::string>
    std::vector<std::string> inputFiles;
    for (const QString& filePath : fitsFiles) {
        inputFiles.push_back(filePath.toStdString());
    }

    std::string saveLocation = Superposition_saveFolderPath.toStdString();

    fitsMerger.processFitsFiles(inputFiles, saveLocation);
}

//降噪相关操作

void MainWindow::on_Noise_reduction_function_button_2_clicked()
{
    int currentIndex = ui->Noise_reduction_widget->currentIndex();

    // 如果当前页面是页面0，切换到其他页面；否则，切换回页面0
    ui->Noise_reduction_widget->setCurrentIndex(currentIndex == 0 ? 1 : 0);
}


void MainWindow::on_Select_file_button_noise_reduction_clicked()
{
    Noise_reduction_targetFilePath = QFileDialog::getExistingDirectory(this, "选择需降噪文件", QDir::homePath());
    if (!Noise_reduction_targetFilePath.isEmpty()) {
        ui->Select_file_label_noise_reduction->setText("目标文件路径: " + Noise_reduction_targetFilePath);
        qDebug() << "Selected target file path: " << Noise_reduction_targetFilePath;
    }
}


void MainWindow::on_Save_file_button_noise_reduction_clicked()
{
    Noise_reduction_saveFolderPath = QFileDialog::getExistingDirectory(this, "选择保存文件夹", QDir::homePath());
    if (!Noise_reduction_saveFolderPath.isEmpty()) {
        ui->Save_file_label_noise_reduction->setText("保存文件夹路径: " + Noise_reduction_saveFolderPath);
        qDebug() << "Selected save folder path: " << Noise_reduction_saveFolderPath;
    }
}

//高斯降噪相关操作
void MainWindow::on_Noise_reduction_button_clicked()
{
    Smooth smooth;

    if (Noise_reduction_saveFolderPath.isEmpty() || Noise_reduction_targetFilePath.isEmpty()) {
        qDebug() << "Please target file, and save folder paths";
        return;
    }
    QDir directory(Noise_reduction_targetFilePath);
    QStringList fitsFiles = directory.entryList(QStringList() << "*.fits", QDir::Files);

    for (const QString& fitsFile : fitsFiles) {
        QString filepath = Noise_reduction_targetFilePath + "/" + fitsFile;
        QByteArray utf8objectFile = filepath.toUtf8();
        const char* objectFile = utf8objectFile.constData();

        QString FFolderPath = Noise_reduction_saveFolderPath + "/Noise_reduction_"+ fitsFile;
        QByteArray utf8saveLocation = FFolderPath.toUtf8();
        const char* saveLocation = utf8saveLocation.constData();

        smooth.processFitsFile(objectFile, saveLocation);

    }
}

//双边滤波器降噪相关操作
void MainWindow::on_Bilateral_reduction_button_clicked()
{
    Bilateral bilateral;

    if (Noise_reduction_saveFolderPath.isEmpty() || Noise_reduction_targetFilePath.isEmpty()) {
        qDebug() << "Please target file, and save folder paths";
        return;
    }
    QDir directory(Noise_reduction_targetFilePath);
    QStringList fitsFiles = directory.entryList(QStringList() << "*.fits", QDir::Files);

    for (const QString& fitsFile : fitsFiles) {
        QString filepath = Noise_reduction_targetFilePath + "/" + fitsFile;
        QByteArray utf8objectFile = filepath.toUtf8();
        const char* objectFile = utf8objectFile.constData();

        QString FFolderPath = Noise_reduction_saveFolderPath + "/Noise_reduction_"+ fitsFile;
        QByteArray utf8saveLocation = FFolderPath.toUtf8();
        const char* saveLocation = utf8saveLocation.constData();

        bilateral.processFitsFile(objectFile, saveLocation);

    }
}

//校准相关操作

void MainWindow::on_Calibration_function_button_clicked()
{
    int currentIndex = ui->Calibration_widget->currentIndex();

    // 如果当前页面是页面0，切换到其他页面；否则，切换回页面0
    ui->Calibration_widget->setCurrentIndex(currentIndex == 0 ? 1 : 0);
}




void MainWindow::on_Select_file_button_calibration_clicked()
{
    Calibration_folderPath = QFileDialog::getExistingDirectory(this, "选择需校准文件", QDir::homePath());
    if (!Calibration_folderPath.isEmpty()) {
        ui->Select_file_label_calibration->setText("校准文件路径: " + Calibration_folderPath);
        qDebug() << "Selected target file path: " << Calibration_folderPath;
    }
}


void MainWindow::on_Object_file_button_flat_clicked()
{
    Flat_targetFilePath = QFileDialog::getOpenFileName(this, "选择平场文件", QDir::homePath(), "FITS Files (*.fits)");
    if (!Flat_targetFilePath.isEmpty()) {
        ui->Select_file_label_flat->setText("平场文件路径: " + Flat_targetFilePath);
        qDebug() << "Selected target file path: " << Flat_targetFilePath;
    }
}


void MainWindow::on_Object_file_button_dark_clicked()
{
    Dark_targetFilePath = QFileDialog::getOpenFileName(this, "选择暗场文件", QDir::homePath(), "FITS Files (*.fits)");
    if (!Dark_targetFilePath.isEmpty()) {
        ui->Object_file_label_dark->setText("暗场文件路径: " + Dark_targetFilePath);
        qDebug() << "Selected target file path: " << Dark_targetFilePath;
    }
}


void MainWindow::on_Object_file_button_bias_clicked()
{
    Bias_targetFilePath = QFileDialog::getOpenFileName(this, "选择偏置场文件", QDir::homePath(), "FITS Files (*.fits)");
    if (!Bias_targetFilePath.isEmpty()) {
        ui->Object_file_label_bias->setText("偏置场文件路径: " + Bias_targetFilePath);
        qDebug() << "Selected target file path: " << Bias_targetFilePath;
    }
}


void MainWindow::on_Save_file_button_calibration_clicked()
{

    Calibration_saveFolderPath = QFileDialog::getExistingDirectory(this, "选择保存文件夹", QDir::homePath());
    if (!Calibration_saveFolderPath.isEmpty()) {
        ui->Save_file_label_calibration->setText("保存文件路径: " + Calibration_saveFolderPath);
        qDebug() << "Selected save folder path: " << Calibration_saveFolderPath;
    }
}


//平场校准
void MainWindow::on_Flat_field_button_clicked()
{
    FitscaLibration fitscaLibration;
    const std::string Calibration_path = Calibration_folderPath.toStdString();
    const std::string Flat_path = Flat_targetFilePath.toStdString();
    const std::string Save_path = Calibration_saveFolderPath.toStdString();
    fitscaLibration.flatCalibrations(Calibration_path, Flat_path, Save_path);
}

//暗场校准
void MainWindow::on_Dark_field_button_clicked()
{
    FitscaLibration fitscaLibration;
    const std::string Calibration_path = Calibration_folderPath.toStdString();
    const std::string Dark_path = Dark_targetFilePath.toStdString();
    const std::string Save_path = Calibration_saveFolderPath.toStdString();
    fitscaLibration.darkCalibrations(Calibration_path, Dark_path, Save_path);
}

//偏置场校准
void MainWindow::on_Bias_field_button_clicked()
{
    FitscaLibration fitscaLibration;
    const std::string Calibration_path = Calibration_folderPath.toStdString();
    const std::string Bias_path = Bias_targetFilePath.toStdString();
    const std::string Save_path = Calibration_saveFolderPath.toStdString();
    fitscaLibration.biasCalibrations(Calibration_path, Bias_path, Save_path);
}

//一键校准
void MainWindow::on_Calibration_button_clicked()
{
    FitscaLibration fitscaLibration;
    const std::string Calibration_path = Calibration_folderPath.toStdString();
    const std::string Flat_path = Flat_targetFilePath.toStdString();
    const std::string Dark_path = Dark_targetFilePath.toStdString();
    const std::string Bias_path = Bias_targetFilePath.toStdString();
    const std::string Save_path = Calibration_saveFolderPath.toStdString();
    fitscaLibration.calibration(Calibration_path, Flat_path, Dark_path, Bias_path, Save_path);
}




//解拜尔
void MainWindow::on_Baryer_function_button_2_clicked()
{
    int currentIndex = ui->Baryer_widget->currentIndex();

    // 如果当前页面是页面0，切换到其他页面；否则，切换回页面0
    ui->Baryer_widget->setCurrentIndex(currentIndex == 0 ? 1 : 0);
}

void MainWindow::on_Select_file_button_baryer_clicked()
{
    Baryer_folderPath = QFileDialog::getExistingDirectory(this, "选择文件夹", QDir::homePath());
    if (!Baryer_folderPath.isEmpty()) {
        // 用户选择了文件，可以进行相应的操作
        ui->Select_file_label_baryer->setText("文件夹路径: " + Baryer_folderPath);
        qDebug() << "Selected file path: " << Baryer_folderPath;
    }
}


void MainWindow::on_Save_file_button_baryer_clicked()
{
    Baryer_saveFolderPath = QFileDialog::getExistingDirectory(this, "选择保存文件夹", QDir::homePath());
    if (!Baryer_saveFolderPath.isEmpty()) {
        ui->Save_file_label_baryer->setText("保存文件夹路径: " + Baryer_saveFolderPath);
        qDebug() << "Selected save folder path: " << Baryer_saveFolderPath;
    }
}


void MainWindow::on_Superposition_baryer_clicked()
{
    Baryer baryer;

    if (Baryer_folderPath.isEmpty() || Baryer_saveFolderPath.isEmpty()) {
        qDebug() << "Please target file, and save folder paths";
        return;
    }
    QDir directory(Baryer_folderPath);
    QStringList fitsFiles = directory.entryList(QStringList() << "*.fits", QDir::Files);

    for (const QString& fitsFile : fitsFiles) {
        QString filepath = Baryer_folderPath + "/" + fitsFile;
        QByteArray utf8objectFile = filepath.toUtf8();
        const char* objectFile = utf8objectFile.constData();

        QString FFolderPath = Baryer_saveFolderPath + "/Baryer" + fitsFile;
        QByteArray utf8saveLocation = FFolderPath.toUtf8();
        const char* saveLocation = utf8saveLocation.constData();

        baryer.saveRGBToFITS(objectFile, saveLocation);

    }
}




