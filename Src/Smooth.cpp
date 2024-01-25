// smooth.cpp

#include "smooth.h"

Smooth::Smooth() {}

Smooth::~Smooth() {}

void Smooth::fitsError(int status) {
    if (status) {
        fits_report_error(stderr, status);
        exit(status);
    }
}

void Smooth::processFitsFile(const char* inputFitsPath, const char* outputFitsPath) {
    // 读取FITS文件
    fitsfile* fitsPtr;
    int status = 0;
    fits_open_file(&fitsPtr, inputFitsPath, READONLY, &status);
    fitsError(status);

    // 获取图像数据
    int bitpix, naxis;
    long naxes[2];
    fits_get_img_param(fitsPtr, 2, &bitpix, &naxis, naxes, &status);
    fitsError(status);

    // 分配内存读取图像数据
    float* imageData = new float[naxes[0] * naxes[1]];
    fits_read_img(fitsPtr, TFLOAT, 1, naxes[0] * naxes[1], NULL, imageData, NULL, &status);
    fitsError(status);

    // 关闭FITS文件
    fits_close_file(fitsPtr, &status);
    fitsError(status);

    // 转换为OpenCV图像
    cv::Mat inputImage(naxes[1], naxes[0], CV_32F, imageData);

    // 应用高斯滤波
    cv::Mat smoothedImage;
    cv::bilateralFilter(inputImage, smoothedImage, 9, 75, 75);

    // 归一化图像数据到 [0, 4096] 范围
    cv::normalize(smoothedImage, smoothedImage, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

    // 创建新的FITS文件
    fitsfile* newFitsPtr;
    fits_create_file(&newFitsPtr, outputFitsPath, &status);
    fitsError(status);

    // 写入图像参数，使用源图像信息
    fits_create_img(newFitsPtr, bitpix, naxis, naxes, &status);
    fitsError(status);

    // 写入图像数据，使用与源图像一致的数据类型
    fits_write_img(newFitsPtr, TFLOAT, 1, naxes[0] * naxes[1], smoothedImage.data, &status);
    fitsError(status);

    // 关闭新的FITS文件
    fits_close_file(newFitsPtr, &status);
    fitsError(status);

    std::cout << "平滑处理成功。输出的FITS文件保存在：" << outputFitsPath << std::endl;

    // 释放内存
    delete[] imageData;
}
