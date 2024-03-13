// smooth.cpp

#include "baryer.h"
using namespace cv;

Baryer::Baryer() {}

Baryer::~Baryer() {}

void Baryer::fitsError(int status) {
    if (status) {
        fits_report_error(stderr, status);
        exit(status);
    }
}

void Baryer::sequence(const char* inputFitsPath, const char* outputFitsPath) {
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

    // 将输入图像类型转换为 CV_8UC3
    cv::Mat bayer_img;
    inputImage.convertTo(bayer_img, CV_8UC3);


    // 将 Bayer RG 图像转换为 BGR 格式
    Mat rgb_img;
    cvtColor(inputImage, rgb_img, cv::COLOR_BayerRG2BGR);

    // 归一化图像数据到 [0, 255] 范围
    cv::normalize(rgb_img, rgb_img, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

    // 创建新的FITS文件
    fitsfile* newFitsPtr;
    fits_create_file(&newFitsPtr, outputFitsPath, &status);
    fitsError(status);

    // 写入图像参数，使用源图像信息
    fits_create_img(newFitsPtr, bitpix, naxis, naxes, &status);
    fitsError(status);

    // 写入图像数据，使用与源图像一致的数据类型
    fits_write_img(newFitsPtr, TFLOAT, 1, naxes[0] * naxes[1], rgb_img.data, &status);
    fitsError(status);

    // 关闭新的FITS文件
    fits_close_file(newFitsPtr, &status);
    fitsError(status);

    std::cout << "处理成功。输出的FITS文件保存在：" << outputFitsPath << std::endl;

    // 释放内存
    delete[] imageData;
}
