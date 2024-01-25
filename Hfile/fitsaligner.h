
#ifndef FITSALIGNER_H
#define FITSALIGNER_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <fitsio.h>
#include <iostream>
using namespace cv;
using namespace std;
    /*Mat performGrayStretch(const Mat& inputImage);*/
    Mat loadFitsImage(const char* filename, long* naxes);

    /*void orthorotation(const mat& rotation, mat& orthogonalrotation);*/
    void detectAndComputeORB(const Mat& image, std::vector<KeyPoint>& keypoints, Mat& descriptors);

    //非刚性对齐
    Mat matchAndComputeHomography(const Mat& descriptors1, const Mat& descriptors2, std::vector<KeyPoint>& keypoints1, std::vector<KeyPoint>& keypoints2);
    Mat warpImage(const Mat& image1, const Mat& H, const Mat& image2);

    //刚性对齐
    Mat matchAndComputeAffine(const Mat& descriptors1, const Mat& descriptors2, std::vector<KeyPoint>& keypoints1, std::vector<KeyPoint>& keypoints2);
    Mat matchAndComputeRigidTransform(const Mat& descriptors1, const Mat& descriptors2,vector<KeyPoint>& keypoints1, vector<KeyPoint>& keypoints2);
    Mat matchAndComputeAffineTransform(const Mat& descriptors1, const Mat& descriptors2,vector<KeyPoint>& keypoints1, vector<KeyPoint>& keypoints2);
    Mat warpImageAffine(const Mat& image1, const Mat& H, const Mat& image2);
    void saveFitsImage(const char* filename, Mat& image, long* naxes);

    //中心星点标注
    void annotateStarCenters(Mat& image, const vector<KeyPoint>& keypoints);

    /*void processFITSFiles(const char* charFolderPath, const char* charTargetFilePath, const char* charsavePath);*/
    void processFITSFilesAffine(const char* charFolderPath, const char* charTargetFilePath, const char* charsavePath);


#endif // FITSALIGNER_H
