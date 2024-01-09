// fitsmerger.h

#ifndef FITSMERGER_H
#define FITSMERGER_H

#include <iostream>
#include <vector>
#include <string>
#include <fitsio.h>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

class FitsMerger {
private:
    int imageCount;
    int bitpixType;
    int superpositionCounter;
    Mat sumImage;
    int sourceBitpixType;
    int originalBitpixType;


public:
    FitsMerger();
    ~FitsMerger();

    void processFitsFiles(const vector<string>& inputFiles, const string& saveLocation);
    void processFitsFile(const string& filePath);

private:
    void saveMeanImageToFits(const string& saveLocation, const string& outputFile, const Mat& meanImage);
};

#endif // FITSMERGER_H
