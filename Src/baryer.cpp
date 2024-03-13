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
    // ��ȡFITS�ļ�
    fitsfile* fitsPtr;
    int status = 0;
    fits_open_file(&fitsPtr, inputFitsPath, READONLY, &status);
    fitsError(status);

    // ��ȡͼ������
    int bitpix, naxis;
    long naxes[2];
    fits_get_img_param(fitsPtr, 2, &bitpix, &naxis, naxes, &status);
    fitsError(status);

    // �����ڴ��ȡͼ������
    float* imageData = new float[naxes[0] * naxes[1]];
    fits_read_img(fitsPtr, TFLOAT, 1, naxes[0] * naxes[1], NULL, imageData, NULL, &status);
    fitsError(status);

    // �ر�FITS�ļ�
    fits_close_file(fitsPtr, &status);
    fitsError(status);

    // ת��ΪOpenCVͼ��
    cv::Mat inputImage(naxes[1], naxes[0], CV_32F, imageData);

    // ������ͼ������ת��Ϊ CV_8UC3
    cv::Mat bayer_img;
    inputImage.convertTo(bayer_img, CV_8UC3);


    // �� Bayer RG ͼ��ת��Ϊ BGR ��ʽ
    Mat rgb_img;
    cvtColor(inputImage, rgb_img, cv::COLOR_BayerRG2BGR);

    // ��һ��ͼ�����ݵ� [0, 255] ��Χ
    cv::normalize(rgb_img, rgb_img, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

    // �����µ�FITS�ļ�
    fitsfile* newFitsPtr;
    fits_create_file(&newFitsPtr, outputFitsPath, &status);
    fitsError(status);

    // д��ͼ�������ʹ��Դͼ����Ϣ
    fits_create_img(newFitsPtr, bitpix, naxis, naxes, &status);
    fitsError(status);

    // д��ͼ�����ݣ�ʹ����Դͼ��һ�µ���������
    fits_write_img(newFitsPtr, TFLOAT, 1, naxes[0] * naxes[1], rgb_img.data, &status);
    fitsError(status);

    // �ر��µ�FITS�ļ�
    fits_close_file(newFitsPtr, &status);
    fitsError(status);

    std::cout << "����ɹ��������FITS�ļ������ڣ�" << outputFitsPath << std::endl;

    // �ͷ��ڴ�
    delete[] imageData;
}
