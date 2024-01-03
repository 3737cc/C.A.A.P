// fitsmerger.h

#ifndef FITSMERGER_H
#define FITSMERGER_H

#include <opencv2/opencv.hpp>
#include "fitsio.h"
#include <vector>

using namespace cv;
using namespace std;

class FitsMerger {
public:
    FitsMerger();
    ~FitsMerger();

    void processFitsFiles(const vector<string>& inputFiles, const string& outputFile);

private:
    Mat sumImage;
    int imageCount;
    int bitpixType;

    void processFitsFile(const string& filePath);
};

#endif // FITSMERGER_H
