#include "fitscalibration.h"

FitscaLibration::FitscaLibration() {}

FitscaLibration::~FitscaLibration() {}

void FitscaLibration::darkCalibration(const string& originalFilePath, const string& darkFieldPath, const string& saveLocation) {
    Mat originalImage = readFitsImage(originalFilePath);
    Mat darkField = readFitsImage(darkFieldPath);

    Mat calibratedImage = calibrate(originalImage, darkField);

    saveFitsImage(saveLocation, "DarkCalibratedImage.fits", calibratedImage);
}

void FitscaLibration::flatCalibration(const string& originalFilePath, const string& flatFieldPath, const string& saveLocation) {
    Mat originalImage = readFitsImage(originalFilePath);
    Mat flatField = readFitsImage(flatFieldPath);

    Mat calibratedImage = calibrate(originalImage, flatField);

    saveFitsImage(saveLocation, "FlatCalibratedImage.fits", calibratedImage);
}

void FitscaLibration::biasCalibration(const string& originalFilePath, const string& biasFieldPath, const string& saveLocation) {
    Mat originalImage = readFitsImage(originalFilePath);
    Mat biasField = readFitsImage(biasFieldPath);

    Mat calibratedImage = calibrate(originalImage, biasField);

    saveFitsImage(saveLocation, "BiasCalibratedImage.fits", calibratedImage);
}

Mat FitscaLibration::readFitsImage(const string& filePath) {
    fitsfile* fptr;
    int status = 0, bitpix, naxis;
    long naxes[2], fpixel[2] = { 1, 1 };

    if (fits_open_file(&fptr, filePath.c_str(), READONLY, &status)) {
        cout << "�޷����ļ���" << filePath << endl;
        // ������ʵ��Ӧ���н��д�����
        return Mat();
    }

    fits_get_img_param(fptr, 2, &bitpix, &naxis, naxes, &status);

    Mat image(naxes[1], naxes[0], (bitpix == 16) ? CV_16U : CV_32F);
    long nelements = naxes[0] * naxes[1];
    int fitsType = (bitpix == 16) ? TUSHORT : TFLOAT;
    fits_read_pix(fptr, fitsType, fpixel, nelements, NULL, image.data, NULL, &status);

    if (status) {
        cout << "��FITS�ļ���ȡ��������ʱ��������" << filePath << endl;
        // ������ʵ��Ӧ���н��д�����
        return Mat();
    }

        bitpixType = bitpix;
        originalBitpixType = bitpix;  // ����ԭʼ FITS �ļ�����������

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
        cout << "д��FITS�ļ�ʱ��������" << fullOutputPath << endl;
        // ������ʵ��Ӧ���н��д�����
    }

    fits_close_file(outfptr, &status);
}

Mat FitscaLibration::calibrate(const Mat& originalImage, const Mat& calibrationField) {
    // У׼���̣�����ԭͼ��ȥ��ͼ
    Mat calibratedImage;
    subtract(originalImage, calibrationField, calibratedImage);
    return calibratedImage;
}
