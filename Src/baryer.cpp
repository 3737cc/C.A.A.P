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


void Baryer::saveRGBToFITS(const char* inputFitsPath, const char* outputFitsPath) {
    //     fitsfile* fitsPtr;
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

    // 将 Bayer RG 图像转换RGGB拜耳排布
    Mat rgb_img;
    cvtColor(bayer_img, rgb_img, cv::COLOR_BayerRG2BGR);

    // 归一化图像数据到 [0, 255] 范围
    cv::normalize(rgb_img, rgb_img, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

    if (rgb_img.empty()) {
        std::cerr << "Error: Image could not be loaded." << std::endl;
        return;
    }

    fitsfile* newFitsPtr;
    char formattedFitsPath[256];
    snprintf(formattedFitsPath, sizeof(formattedFitsPath), "!%s", outputFitsPath); // 使能覆盖现有文件
    fits_create_file(&newFitsPtr, formattedFitsPath, &status);
    fitsError(status);

    long naxes3[3] = { rgb_img.cols, rgb_img.rows, 3 }; // 设置为3D立方体，第三维是通道数
    fits_create_img(newFitsPtr, BYTE_IMG, 3, naxes3, &status);
    fitsError(status);

    // 分离通道并写入FITS
    std::vector<cv::Mat> channels(3);
    cv::split(rgb_img, channels);

    for (int k = 0; k < 3; ++k) {
        long fpixel[3] = { 1, 1, k + 1 };
        if (!channels[k].isContinuous()) {
            channels[k] = channels[k].clone(); // 确保连续性
        }
        fits_write_pix(newFitsPtr, TBYTE, fpixel, naxes3[0] * naxes3[1], channels[k].data, &status);
        fitsError(status);
    }

    fits_close_file(newFitsPtr, &status);
    fitsError(status);
}
