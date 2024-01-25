#pragma once
// bilateral.h

#ifndef BILATERAL_H
#define BILATERAL_H

#include <fitsio.h>
#include <opencv2/opencv.hpp>
#include "filehandler.h"

class Bilateral {
public:
    Bilateral();
    ~Bilateral();

    void processFitsFile(const char* inputFitsPath, const char* outputFitsPath);

private:
    void fitsError(int status);
};

#endif // BILATERAL_H
