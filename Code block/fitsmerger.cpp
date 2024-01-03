// fitsmerger.cpp

#include "fitsmerger.h"

FitsMerger::FitsMerger() : imageCount(0), bitpixType(0) {}

FitsMerger::~FitsMerger() {}

void FitsMerger::processFitsFiles(const vector<string>& inputFiles, const string& outputFile) {
    // 合并FITS文件
    for (const auto& filePath : inputFiles) {
        processFitsFile(filePath);
    }

    if (imageCount > 0) {
        Mat meanImage = sumImage / static_cast<float>(imageCount);

        // 保存均值图像到新的 FITS 文件
        fitsfile* outfptr;
        int status = 0;
        long fpixel = 1, naxis = 2, nelements;
        long naxes[2] = { meanImage.cols, meanImage.rows };
        nelements = naxes[0] * naxes[1];

        fits_create_file(&outfptr, outputFile.c_str(), &status);
        int fitsImgType = (bitpixType == 16) ? SHORT_IMG : FLOAT_IMG;
        fits_create_img(outfptr, fitsImgType, naxis, naxes, &status);
        fits_write_img(outfptr, (bitpixType == 16) ? TSHORT : TFLOAT, fpixel, nelements, meanImage.data, &status);

        if (status) {
            cout << "写入FITS文件时发生错误：" << outputFile << endl;
        }

        fits_close_file(outfptr, &status);
    }
}

void FitsMerger::processFitsFile(const string& filePath) {
    fitsfile* fptr;
    int status = 0, bitpix, naxis;
    long naxes[2], fpixel[2] = { 1, 1 };

    if (fits_open_file(&fptr, filePath.c_str(), READONLY, &status)) {
        cout << filePath << endl;
        return;
    }

    fits_get_img_param(fptr, 2, &bitpix, &naxis, naxes, &status);
    if (status) {
        cout << "从FITS文件读取参数时发生错误：" << filePath << endl;
        fits_close_file(fptr, &status);
        return;
    }

    if (imageCount == 0) {
        bitpixType = bitpix;
        sumImage = Mat::zeros(naxes[1], naxes[0], (bitpix == 16) ? CV_16U : CV_32F);
    }

    // 读取FITS文件像素数据
    Mat image(naxes[1], naxes[0], (bitpix == 16) ? CV_16U : CV_32F);
    long nelements = naxes[0] * naxes[1];
    int fitsType = (bitpix == 16) ? TUSHORT : TFLOAT;
    fits_read_pix(fptr, fitsType, fpixel, nelements, NULL, image.data, NULL, &status);

    if (status) {
        cout << "从FITS文件读取像素数据时发生错误：" << filePath << endl;
    }
    else {
        sumImage += image;
        imageCount++;
    }

    fits_close_file(fptr, &status);
}
