#ifndef FITS_CALIBRATION_H
#define FITS_CALIBRATION_H

#include <fitsio.h>          // CFITSIO 库
#include <opencv2/opencv.hpp> // OpenCV 库
#include <filesystem>        // 文件系统操作
#include <string>
#include <vector>

using namespace cv;
using namespace std;

class FitscaLibration {
private:
    int originalBitpixType;
    int bitpixType;

public:
    FitscaLibration();
    ~FitscaLibration();

    // 新增：处理文件夹中所有FITS文件的函数
    vector<string> getFitsFilesInDirectory(const string& directoryPath);

    // 修改：支持文件夹路径的校准函数
    void flatCalibrations(const string& originalFileDirectory, const string& flatFieldPath, const string& saveLocation);
    void darkCalibrations(const string& originalFileDirectory, const string& darkFieldPath, const string& saveLocation);
    void biasCalibrations(const string& originalFileDirectory, const string& biasFieldPath, const string& saveLocation);

    // 现有的单文件处理函数
    void flatCalibration(const string& originalFilePath, const string& flatFieldPath, const string& saveLocation);
    void darkCalibration(const string& originalFilePath, const string& darkFieldPath, const string& saveLocation);
    void biasCalibration(const string& originalFilePath, const string& biasFieldPath, const string& saveLocation);

    // 综合校准函数
    void calibration(const string& originalFilePath, const string& flatFieldPath, const string& darkFieldPath, const string& biasFieldPath, const string& saveLocation);

private:
    // 内部使用的辅助函数
    Mat readFitsImage(const string& filePath);
    void saveFitsImage(const string& saveLocation, const string& outputFile, const Mat& image);
    Mat flatCalibrate(const Mat& originalImage, const Mat& calibrationField);
    Mat darkCalibrate(const Mat& originalImage, const Mat& calibrationField);
    Mat biasCalibrate(const Mat& originalImage, const Mat& calibrationField);
    Mat calibrate(const Mat& flatImage, const Mat& darkImage, const Mat& biasImage, const Mat& calibrationField);
};

#endif // FITS_CALIBRATION_H