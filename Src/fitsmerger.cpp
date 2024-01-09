#include <fitsmerger.h>

FitsMerger::FitsMerger() : imageCount(0), bitpixType(0), superpositionCounter(0), sourceBitpixType(0) {}

FitsMerger::~FitsMerger() {}

void FitsMerger::processFitsFiles(const vector<string>& inputFiles, const string& saveLocation) {
    // 生成带有递增数字的初始文件名
    string initialOutputFile = "Superposition_" + to_string(superpositionCounter++) + ".fits";

    // 合并FITS文件
    for (const auto& filePath : inputFiles) {
        processFitsFile(filePath);
    }

    if (imageCount > 0) {
        Mat meanImage = sumImage / static_cast<float>(imageCount);

        // 保存均值图像到新的 FITS 文件
        saveMeanImageToFits(saveLocation, initialOutputFile, meanImage);
    }
}

void FitsMerger::processFitsFile(const string& filePath) {
    fitsfile* fptr;
    int status = 0, bitpix, naxis;
    long naxes[2], fpixel[2] = { 1, 1 };

    if (fits_open_file(&fptr, filePath.c_str(), READONLY, &status)) {
        cout << "无法打开文件：" << filePath << endl;
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
        originalBitpixType = bitpix;  // 保存原始 FITS 文件的数据类型
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

void FitsMerger::saveMeanImageToFits(const string& saveLocation, const string& outputFile, const Mat& meanImage) {
    fitsfile* outfptr;
    int status = 0;
    long fpixel = 1, naxis = 2, nelements;
    long naxes[2] = { meanImage.cols, meanImage.rows };
    nelements = naxes[0] * naxes[1];

    string fullOutputPath = saveLocation + "/" + outputFile;

    Mat originalTypeImage;
    if (originalBitpixType == 16) {
        originalTypeImage = Mat(meanImage.size(), CV_16U);
        meanImage.convertTo(originalTypeImage, CV_16U);
    }
    else {
        originalTypeImage = Mat(meanImage.size(), CV_32F);
        meanImage.convertTo(originalTypeImage, CV_32F);
    }

    // 决定 FITS 数据类型
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

    // 写入FITS文件
    fits_create_file(&outfptr, fullOutputPath.c_str(), &status);
    fits_create_img(outfptr, fitsImgType, naxis, naxes, &status);
    fits_write_img(outfptr, (originalBitpixType == 16) ? TSHORT : TFLOAT, fpixel, nelements, originalTypeImage.data, &status);

    if (status) {
        cout << "写入FITS文件时发生错误：" << fullOutputPath << endl;
    }

    fits_close_file(outfptr, &status);
}
