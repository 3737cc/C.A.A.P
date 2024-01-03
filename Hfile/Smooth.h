// smooth.h

#ifndef SMOOTH_H
#define SMOOTH_H

#include <fitsio.h>
#include <opencv2/opencv.hpp>
#include "filehandler.h"

class Smooth {
public:
    Smooth();
    ~Smooth();

    void processFitsFile(const char* inputFitsPath, const char* outputFitsPath);

private:
    void fitsError(int status);
};

#endif // SMOOTH_H
