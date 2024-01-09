
#ifndef FITSALIGNER_H
#define FITSALIGNER_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <fitsio.h>
#include <iostream>


    cv::Mat loadFitsImage(const char* filename, long* naxes);
    void detectAndComputeORB(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors);
    cv::Mat matchAndComputeHomography(const cv::Mat& descriptors1, const cv::Mat& descriptors2, std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2);
    cv::Mat warpImage(const cv::Mat& image1, const cv::Mat& H, const cv::Mat& image2);
    void saveFitsImage(const char* filename, cv::Mat& image, long* naxes);


#endif // FITSALIGNER_H
