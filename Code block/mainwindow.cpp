#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "filehandler.h"
#include "smooth.h"
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




void MainWindow::on_pushButton_clicked()
{
    long naxes[2];

    if (folderPath.isEmpty() || targetFilePath.isEmpty() || saveFolderPath.isEmpty()) {
        qDebug() << "Please select folder, target file, and save folder paths.";
        return;
    }
    QDir directory(folderPath);
    QStringList fitsFiles = directory.entryList(QStringList() << "*.fits", QDir::Files);

    for (const QString& fitsFile : fitsFiles) {
        QString filePath = folderPath + "/" + fitsFile;
        QByteArray utf8DatFoler = filePath.toUtf8();
        const char* charFolderPath = utf8DatFoler.constData();
        Mat image = loadFitsImage(charFolderPath, naxes);

        QByteArray utfTargetFoler = targetFilePath.toUtf8();
        const char* charTargetFilePath = utfTargetFoler.constData();
        Mat targetImage = loadFitsImage(charTargetFilePath, naxes);

        vector<KeyPoint> keypoints1, keypoints2;
        Mat descriptors1, descriptors2;
        detectAndComputeORB(image, keypoints1, descriptors1);
        detectAndComputeORB(targetImage, keypoints2, descriptors2);
        Mat H = matchAndComputeHomography(descriptors1, descriptors2, keypoints1, keypoints2);
        Mat alignedImage = warpImage(image, H, targetImage);

        QString FFolderPath = saveFolderPath + "/Aligned_" + fitsFile;
        QByteArray utf8saveFoler = FFolderPath.toUtf8();
        const char* charsavePath = utf8saveFoler.constData();
        saveFitsImage(charsavePath, alignedImage, naxes);
    }
}


void MainWindow::on_pushButton_5_clicked()
{

    folderPath = QFileDialog::getExistingDirectory(this, "选择文件夹", QDir::homePath());

    if (!folderPath.isEmpty()) {
        // 用户选择了文件，可以进行相应的操作
        ui->label->setText("文件路径: " + folderPath);
        qDebug() << "Selected file path: " << folderPath;
    }
}


void MainWindow::on_pushButton_6_clicked()
{
    saveFolderPath = QFileDialog::getExistingDirectory(this, "选择保存文件夹", QDir::homePath());
    if (!saveFolderPath.isEmpty()) {
        ui->label_2->setText("保存文件夹路径: " + saveFolderPath);
        qDebug() << "Selected save folder path: " << saveFolderPath;
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    targetFilePath = QFileDialog::getOpenFileName(this, "选择目标文件", QDir::homePath(), "FITS Files (*.fits)");
    if (!targetFilePath.isEmpty()) {
        ui->label_3->setText("目标文件路径: " + targetFilePath);
        qDebug() << "Selected target file path: " << targetFilePath;
    }
}



void MainWindow::on_label_linkActivated(const QString &link)
{

}





void MainWindow::on_label_3_linkActivated(const QString &link)
{

}







void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

