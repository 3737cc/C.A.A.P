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
    long naxes[2];

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
        Mat image = loadFitsImage(charFolderPath, naxes);

        QByteArray utfTargetFoler = Alignment_targetFilePath.toUtf8();
        const char* charTargetFilePath = utfTargetFoler.constData();
        Mat targetImage = loadFitsImage(charTargetFilePath, naxes);

        vector<KeyPoint> keypoints1, keypoints2;
        Mat descriptors1, descriptors2;
        detectAndComputeORB(image, keypoints1, descriptors1);
        detectAndComputeORB(targetImage, keypoints2, descriptors2);
        Mat H = matchAndComputeHomography(descriptors1, descriptors2, keypoints1, keypoints2);
        Mat alignedImage = warpImage(image, H, targetImage);

        QString FFolderPath = Alignment_saveFolderPath + "/Aligned_" + fitsFile;
        QByteArray utf8saveFoler = FFolderPath.toUtf8();
        const char* charsavePath = utf8saveFoler.constData();
        saveFitsImage(charsavePath, alignedImage, naxes);
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
    Noise_reduction_targetFilePath = QFileDialog::getOpenFileName(this, "选择需降噪文件", QDir::homePath(), "FITS Files (*.fits)");
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


void MainWindow::on_Noise_reduction_button_clicked()
{
    Smooth smooth;
    QByteArray utf8objectFile = Noise_reduction_targetFilePath.toUtf8();
    const char* objectFile = utf8objectFile.constData();

    QString FFolderPath = Noise_reduction_saveFolderPath + "/Noise_reduction.fits" ;
    QByteArray utf8saveLocation = FFolderPath.toUtf8();
    const char* saveLocation = utf8saveLocation.constData();
    smooth.processFitsFile(objectFile,saveLocation);
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
    Calibration_folderPath = QFileDialog::getOpenFileName(this, "选择需校准文件", QDir::homePath(), "FITS Files (*.fits)");
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
    fitscaLibration.flatCalibration(Calibration_path, Flat_path, Save_path);
}

//暗场校准
void MainWindow::on_Dark_field_button_clicked()
{
    FitscaLibration fitscaLibration;
    const std::string Calibration_path = Calibration_folderPath.toStdString();
    const std::string Dark_path = Dark_targetFilePath.toStdString();
    const std::string Save_path = Calibration_saveFolderPath.toStdString();
    fitscaLibration.darkCalibration(Calibration_path, Dark_path, Save_path);
}

//偏置场校准
void MainWindow::on_Bias_field_button_clicked()
{
    FitscaLibration fitscaLibration;
    const std::string Calibration_path = Calibration_folderPath.toStdString();
    const std::string Bias_path = Bias_targetFilePath.toStdString();
    const std::string Save_path = Calibration_saveFolderPath.toStdString();
    fitscaLibration.biasCalibration(Calibration_path, Bias_path, Save_path);
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

