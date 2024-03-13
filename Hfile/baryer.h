#pragma once
// baryer.h

#ifndef BARYER_H
#define BARYER_H

#include <fitsio.h>
#include <opencv2/opencv.hpp>

class Baryer {
public:
    Baryer();
    ~Baryer();

    void saveRGBToFITS(const char* inputFitsPath, const char* outputFitsPath);

private:
    void fitsError(int status);

};
#endif // BILATERAL_H
