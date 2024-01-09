// fitsmerger.cpp

#include "fitsmerger.h"

FitsMerger::FitsMerger() : imageCount(0), bitpixType(0) {}

FitsMerger::~FitsMerger() {}

void FitsMerger::processFitsFiles(const vector<string>& inputFiles, const string& outputFile) {
    // �ϲ�FITS�ļ�
    for (const auto& filePath : inputFiles) {
        processFitsFile(filePath);
    }

    if (imageCount > 0) {
        Mat meanImage = sumImage / static_cast<float>(imageCount);

        // �����ֵͼ���µ� FITS �ļ�
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
            cout << "д��FITS�ļ�ʱ��������" << outputFile << endl;
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
        cout << "��FITS�ļ���ȡ����ʱ��������" << filePath << endl;
        fits_close_file(fptr, &status);
        return;
    }

    if (imageCount == 0) {
        bitpixType = bitpix;
        sumImage = Mat::zeros(naxes[1], naxes[0], (bitpix == 16) ? CV_16U : CV_32F);
    }

    // ��ȡFITS�ļ���������
    Mat image(naxes[1], naxes[0], (bitpix == 16) ? CV_16U : CV_32F);
    long nelements = naxes[0] * naxes[1];
    int fitsType = (bitpix == 16) ? TUSHORT : TFLOAT;
    fits_read_pix(fptr, fitsType, fpixel, nelements, NULL, image.data, NULL, &status);

    if (status) {
        cout << "��FITS�ļ���ȡ��������ʱ��������" << filePath << endl;
    }
    else {
        sumImage += image;
        imageCount++;
    }

    fits_close_file(fptr, &status);
}
