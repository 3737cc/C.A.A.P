#ifndef FITS_CALIBRATION_H
#define FITS_CALIBRATION_H

#include <fitsio.h>          // CFITSIO ��
#include <opencv2/opencv.hpp> // OpenCV ��
#include <filesystem>        // �ļ�ϵͳ����
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

    // �����������ļ���������FITS�ļ��ĺ���
    vector<string> getFitsFilesInDirectory(const string& directoryPath);

    // �޸ģ�֧���ļ���·����У׼����
    void flatCalibrations(const string& originalFileDirectory, const string& flatFieldPath, const string& saveLocation);
    void darkCalibrations(const string& originalFileDirectory, const string& darkFieldPath, const string& saveLocation);
    void biasCalibrations(const string& originalFileDirectory, const string& biasFieldPath, const string& saveLocation);

    // ���еĵ��ļ�������
    void flatCalibration(const string& originalFilePath, const string& flatFieldPath, const string& saveLocation);
    void darkCalibration(const string& originalFilePath, const string& darkFieldPath, const string& saveLocation);
    void biasCalibration(const string& originalFilePath, const string& biasFieldPath, const string& saveLocation);

    // �ۺ�У׼����
    void calibration(const string& originalFilePath, const string& flatFieldPath, const string& darkFieldPath, const string& biasFieldPath, const string& saveLocation);

private:
    // �ڲ�ʹ�õĸ�������
    Mat readFitsImage(const string& filePath);
    void saveFitsImage(const string& saveLocation, const string& outputFile, const Mat& image);
    Mat flatCalibrate(const Mat& originalImage, const Mat& calibrationField);
    Mat darkCalibrate(const Mat& originalImage, const Mat& calibrationField);
    Mat biasCalibrate(const Mat& originalImage, const Mat& calibrationField);
    Mat calibrate(const Mat& flatImage, const Mat& darkImage, const Mat& biasImage, const Mat& calibrationField);
};

#endif // FITS_CALIBRATION_H