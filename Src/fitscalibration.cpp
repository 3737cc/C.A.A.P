#include "fitscalibration.h"
namespace fs = std::filesystem;

FitscaLibration::FitscaLibration() {}

FitscaLibration::~FitscaLibration() {}

// 将函数定义为类的成员函数
std::vector<std::string> FitscaLibration::getFitsFilesInDirectory(const std::string& directoryPath) {
    std::vector<std::string> fitsFiles;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".fits") { // 确保只处理 .fits 文件
            fitsFiles.push_back(entry.path().string());
        }
    }
    return fitsFiles;
}

// 批量处理文件夹中的 FITS 文件的函数
void FitscaLibration::flatCalibrations(const string& originalFileDirectory, const string& flatFieldPath, const string& saveLocation) {
    auto files = getFitsFilesInDirectory(originalFileDirectory);
    for (const auto& file : files) {
        Mat originalImage = readFitsImage(file);
        Mat flatField = readFitsImage(flatFieldPath);
        Mat calibratedImage = flatCalibrate(originalImage, flatField);
        saveFitsImage(saveLocation, "FlatCalibrated_" + fs::path(file).filename().string(), calibratedImage);
    }
}

void FitscaLibration::darkCalibrations(const string& originalFileDirectory, const string& darkFieldPath, const string& saveLocation) {
    auto files = getFitsFilesInDirectory(originalFileDirectory);
    for (const auto& file : files) {
        Mat originalImage = readFitsImage(file);
        Mat flatField = readFitsImage(darkFieldPath);
        Mat calibratedImage = darkCalibrate(originalImage, flatField);
        saveFitsImage(saveLocation, "DarkCalibrated_" + fs::path(file).filename().string(), calibratedImage);
    }
}

void FitscaLibration::biasCalibrations(const string& originalFileDirectory, const string& biasFieldPath, const string& saveLocation) {
    auto files = getFitsFilesInDirectory(originalFileDirectory);
    for (const auto& file : files) {
        Mat originalImage = readFitsImage(file);
        Mat flatField = readFitsImage(biasFieldPath);
        Mat calibratedImage = biasCalibrate(originalImage, flatField);
        saveFitsImage(saveLocation, "BiasCalibrated_" + fs::path(file).filename().string(), calibratedImage);
    }
}

//进行校准fits文件
void FitscaLibration::flatCalibration(const string& originalFilePath, const string& flatFieldPath, const string& saveLocation) {
    Mat originalImage = readFitsImage(originalFilePath);
    Mat flatField = readFitsImage(flatFieldPath);

    Mat calibratedImage = flatCalibrate(originalImage, flatField);

    saveFitsImage(saveLocation, "FlatCalibratedImage.fits", calibratedImage);
}

void FitscaLibration::darkCalibration(const string& originalFilePath, const string& darkFieldPath, const string& saveLocation) {
    Mat originalImage = readFitsImage(originalFilePath);
    Mat darkField = readFitsImage(darkFieldPath);

    Mat calibratedImage = darkCalibrate(originalImage, darkField);

    saveFitsImage(saveLocation, "DarkCalibratedImage.fits", calibratedImage);
}


void FitscaLibration::biasCalibration(const string& originalFilePath, const string& biasFieldPath, const string& saveLocation) {
    Mat originalImage = readFitsImage(originalFilePath);
    Mat biasField = readFitsImage(biasFieldPath);

    Mat calibratedImage = biasCalibrate(originalImage, biasField);

    saveFitsImage(saveLocation, "BiasCalibratedImage.fits", calibratedImage);
}

//一键校准
void FitscaLibration::calibration(const string& originalFilePath, const string& flatFieldPath, const string& darkFieldPath, const string& biasFieldPath, const string& saveLocation) {
    Mat originalImage = readFitsImage(originalFilePath);
    Mat flatField = readFitsImage(flatFieldPath);
    Mat darkField = readFitsImage(darkFieldPath);
    Mat biasField = readFitsImage(biasFieldPath);

    Mat calibratedImage = calibrate(originalImage, flatField, darkField, biasField);

    saveFitsImage(saveLocation, "One-clickCalibratedImage.fits", calibratedImage);


}

Mat FitscaLibration::readFitsImage(const string& filePath) {
    fitsfile* fptr;
    int status = 0, bitpix, naxis;
    long naxes[2], fpixel[2] = { 1, 1 };

    if (fits_open_file(&fptr, filePath.c_str(), READONLY, &status)) {
        cout << "无法打开文件：" << filePath << endl;
        // 可以在实际应用中进行错误处理
        return Mat();
    }

    fits_get_img_param(fptr, 2, &bitpix, &naxis, naxes, &status);

    Mat image(naxes[1], naxes[0], (bitpix == 16) ? CV_16U : CV_32F);
    long nelements = naxes[0] * naxes[1];
    int fitsType = (bitpix == 16) ? TUSHORT : TFLOAT;
    fits_read_pix(fptr, fitsType, fpixel, nelements, NULL, image.data, NULL, &status);

    if (status) {
        cout << "从FITS文件读取像素数据时发生错误：" << filePath << endl;
        // 可以在实际应用中进行错误处理
        return Mat();
    }

        bitpixType = bitpix;
        originalBitpixType = bitpix;  // 保存原始 FITS 文件的数据类型

    fits_close_file(fptr, &status);

    return image;
}

void FitscaLibration::saveFitsImage(const string& saveLocation, const string& outputFile, const Mat& image) {
    fitsfile* outfptr;
    int status = 0;
    long fpixel = 1, naxis = 2, nelements;
    long naxes[2] = { image.cols, image.rows };
    nelements = naxes[0] * naxes[1];

    string fullOutputPath = saveLocation + "/" + outputFile;

    Mat fitsImage;
    if (originalBitpixType == 16) {
        fitsImage = Mat(image.size(), CV_16U);
        image.convertTo(fitsImage, CV_16U);
    }
    else {
        fitsImage = Mat(image.size(), CV_32F);
        image.convertTo(fitsImage, CV_32F);
    }




    int fitsImgType;
    if (originalBitpixType == 8) {
        fitsImgType = BYTE_IMG;
    }
    else if (originalBitpixType == 16) {
        fitsImgType = SHORT_IMG;
    }
    else {
        fitsImgType = FLOAT_IMG;
    }

    fits_create_file(&outfptr, fullOutputPath.c_str(), &status);
    fits_create_img(outfptr, fitsImgType, naxis, naxes, &status);
    fits_write_img(outfptr, (originalBitpixType == 16) ? TSHORT : TFLOAT, fpixel, nelements, fitsImage.data, &status);

    if (status) {
        cout << "写入FITS文件时发生错误：" << fullOutputPath << endl;
        // 可以在实际应用中进行错误处理
    }

    fits_close_file(outfptr, &status);
}

//平场校准
Mat FitscaLibration::flatCalibrate(const Mat& originalImage, const Mat& flatImage) {
    Mat calibratedImage;
    subtract(originalImage, flatImage, calibratedImage);
    return calibratedImage;
}

//暗场校准
Mat FitscaLibration::darkCalibrate(const Mat& originalImage, const Mat& darkImage) {
    Mat calibratedImage;
    subtract(originalImage, darkImage, calibratedImage);
    return calibratedImage;
}

//偏置场校准
Mat FitscaLibration::biasCalibrate(const Mat& originalImage, const Mat& biasImage) {
    Mat calibratedImage;
    subtract(originalImage, biasImage, calibratedImage);
    return calibratedImage;
}

Mat FitscaLibration::calibrate(const Mat& flatImage, const Mat& darkImage, const Mat& biasImage, const Mat& originalImage) {
    Mat calibratedImage;
    subtract(originalImage, flatImage, calibratedImage);
    subtract(originalImage, darkImage, calibratedImage);
    subtract(originalImage, biasImage, calibratedImage);//减去三个场图
    return calibratedImage;
}
