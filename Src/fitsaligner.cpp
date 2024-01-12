#include <fitsaligner.h>

using namespace cv;
using namespace std;

int globalBitpix;

Mat loadFitsImage(const char* filename, long* naxes) {
    fitsfile* fptr;
    int status = 0, naxis;

    fits_open_file(&fptr, filename, READONLY, &status);
    fits_get_img_param(fptr, 2, &globalBitpix, &naxis, naxes, &status);

    if (naxis != 2) {
        cout << "Error: naxis != 2" << endl;
        exit(1);
    }

    long nelements = naxes[0] * naxes[1];
    double* imageArray = new double[nelements];
    fits_read_img(fptr, TDOUBLE, 1, nelements, NULL, imageArray, NULL, &status);

    Mat image(naxes[1], naxes[0], CV_64FC1, imageArray);
    normalize(image, image, 0, 255, NORM_MINMAX, CV_8UC1);

    fits_close_file(fptr, &status);
    delete[] imageArray;

    return image;
}

void detectAndComputeORB(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors) {
    Ptr<ORB> orb = ORB::create();

    // 添加检测语句
    std::cout << "Before detectAndComputeORB: Image size = " << image.size() << std::endl;

    orb->detectAndCompute(image, noArray(), keypoints, descriptors);

    // 添加检测语句
    std::cout << "After detectAndComputeORB: Detected keypoints = " << keypoints.size() << std::endl;
}//ORB检测没问题

//非刚性对齐
Mat matchAndComputeHomography(const Mat& descriptors1, const Mat& descriptors2, vector<KeyPoint>& keypoints1, vector<KeyPoint>& keypoints2) {
    // 检查输入矩阵是否为空
    if (descriptors1.empty() || descriptors2.empty() || keypoints1.empty() || keypoints2.empty()) {
        cerr << "Error: Input matrices or keypoints are empty." << endl;
        return Mat();
    }

    // 检查输入矩阵的维度是否符合预期
    if (descriptors1.cols != descriptors2.cols) {
        cerr << "Error: Descriptor dimensions do not match." << endl;
        return Mat();
    }

    if (keypoints1.size() != keypoints2.size()) {
        cerr << "Warning: KeyPoints vectors do not have the same size. Matching may be affected." << endl;
        // 可以选择继续执行，但最好添加一些额外的信息，以指示潜在的问题。
    }

    BFMatcher matcher(NORM_HAMMING);
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);
    double max_dist = 0; double min_dist = 100;
    for (int i = 0; i < descriptors1.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }
    vector<DMatch> good_matches;
    for (int i = 0; i < descriptors1.rows; i++) {
        if (matches[i].distance <= max(2 * min_dist, 30.0)) {
            good_matches.push_back(matches[i]);
        }
    }
    vector<Point2f> obj;
    vector<Point2f> scene;
    for (size_t i = 0; i < good_matches.size(); i++) {
        obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
        scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }
    Mat H = findHomography(obj, scene, RANSAC);

    return H;

}

Mat warpImage(const Mat& image1, const Mat& H, const Mat& image2) {
    // 检测输入图像是否为空
    if (image1.empty() || H.empty() || image2.empty()) {
        std::cerr << "Error: Input images or transformation matrix is empty." << std::endl;
        // 返回一个空的Mat，表示出现错误
        return Mat();
    }

    // 检测变换矩阵的大小是否合适
    if (H.rows != 3 || H.cols != 3) {
        std::cerr << "Error: Transformation matrix should be a 3x3 matrix." << std::endl;
        return Mat();
    }

    // 进行透视变换
    Mat result;
    warpPerspective(image1, result, H, image2.size());
    // 检测结果图像是否为空
    if (result.empty()) {
        std::cerr << "Error: Failed to warp the image." << std::endl;
        //// 添加显示结果图像的代码       
        //imshow("Error: Empty Result Image", image1);
        //waitKey(0);
        return Mat();
    }
    return result;
}


//刚性对齐
Mat matchAndComputeAffine(const Mat& descriptors1, const Mat& descriptors2, vector<KeyPoint>& keypoints1, vector<KeyPoint>& keypoints2) {
    if (descriptors1.empty() || descriptors2.empty() || keypoints1.empty() || keypoints2.empty()) {
        cerr << "Error: Input matrices or keypoints are empty." << endl;
        return Mat();
    }

    if (descriptors1.cols != descriptors2.cols) {
        cerr << "Error: Descriptor dimensions do not match." << endl;
        return Mat();
    }

    if (keypoints1.size() != keypoints2.size()) {
        cerr << "Warning: KeyPoints vectors do not have the same size. Matching may be affected." << endl;
    }

    BFMatcher matcher(NORM_HAMMING);
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);
    double max_dist = 0; double min_dist = 10;
    for (int i = 0; i < descriptors1.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }
    vector<DMatch> good_matches;
    for (int i = 0; i < descriptors1.rows; i++) {
        if (matches[i].distance <= max(2 * min_dist, 20.0)) {
            good_matches.push_back(matches[i]);
        }
    }
    vector<Point2f> obj;
    vector<Point2f> scene;
    for (size_t i = 0; i < good_matches.size(); i++) {
        obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
        scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }

    Mat affineMatrix = estimateAffine2D(obj, scene);

    return affineMatrix;
}

Mat warpImageAffine(const Mat& image1, const Mat& affineMatrix, const Mat& image2) {
    if (image1.empty() || affineMatrix.empty() || image2.empty()) {
        std::cerr << "Error: Input images or transformation matrix is empty." << std::endl;
        return Mat();
    }

    if (affineMatrix.rows != 2 || affineMatrix.cols != 3) {
        std::cerr << "Error: Affine transformation matrix should be a 2x3 matrix." << std::endl;
        return Mat();
    }

    Mat result;
    // 修改为使用最近邻插值
    warpAffine(image1, result, affineMatrix, image2.size(), INTER_NEAREST, BORDER_CONSTANT);

    if (result.empty()) {
        std::cerr << "Error: Failed to warp the image." << std::endl;
        return Mat();
    }

    return result;
}

void saveFitsImage(const char* filename, Mat& image, long* naxes) {
    fitsfile* fptr;
    int status = 0;
    long fpixel = 1;
    int bitpix = globalBitpix;
    long nelements = naxes[0] * naxes[1];
    fits_create_file(&fptr, filename, &status);
    fits_create_img(fptr, bitpix, 2, naxes, &status);
    // 使用16位整型数组来保存数据
    short* outputArray = new short[nelements];
    if (!outputArray) {
        cerr << "Memory allocation failed for outputArray." << endl;
        exit(1);
    }

    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            // 验证索引是否在数组范围内
            long index = i * image.cols + j;
            if (index < nelements) {
                // 将uchar数据转换为16位整型
                outputArray[index] = static_cast<short>(image.at<uchar>(i, j));
            }
            else {
                cerr << "Index out of bounds: " << index << endl;
                delete[] outputArray;
                exit(1);
            }
        }
    }

    // 确保使用正确的数据类型写入FITS文件
    fits_write_img(fptr, TSHORT, fpixel, nelements, outputArray, &status);
    fits_close_file(fptr, &status);
    delete[] outputArray; // 使用 delete[] 替代 free()

}

void processFITSFiles(const char* charFolderPath, const char* charTargetFilePath, const char* charsavePath) {
    long naxes[2];

    Mat image = loadFitsImage(charFolderPath, naxes);
    Mat targetImage = loadFitsImage(charTargetFilePath, naxes);

    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detectAndComputeORB(image, keypoints1, descriptors1);
    detectAndComputeORB(targetImage, keypoints2, descriptors2);
    Mat H = matchAndComputeHomography(descriptors1, descriptors2, keypoints1, keypoints2);
    Mat alignedImage = warpImage(image, H, targetImage);

    saveFitsImage(charsavePath, alignedImage, naxes);
}

void processFITSFilesAffine(const char* charFolderPath, const char* charTargetFilePath, const char* charsavePath) {
    long naxes[2];

    Mat image = loadFitsImage(charFolderPath, naxes);
    Mat targetImage = loadFitsImage(charTargetFilePath, naxes);

    //// 图像预处理：灰度拉伸
    //Mat preprocessedImage = performGrayStretch(image);
    //Mat preprocessedTargetImage = performGrayStretch(targetImage);

    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detectAndComputeORB(image, keypoints1, descriptors1);
    detectAndComputeORB(targetImage, keypoints2, descriptors2);
    Mat H = matchAndComputeAffine(descriptors1, descriptors2, keypoints1, keypoints2);

    // 对齐前的原始图像
    imshow("Original Image", image);
    waitKey(0);

    // 对齐后的图像
    Mat alignedImage = warpImageAffine(image, H, targetImage);
    imshow("Aligned Image", alignedImage);
    waitKey(0);

    // 保存对齐后的图像
    saveFitsImage(charsavePath, alignedImage, naxes);
}



