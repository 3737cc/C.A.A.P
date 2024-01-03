// filehandler.h

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class FileHandler {
public:
    static vector<string> getInputFilePaths();
    static string getOutputFilePath();
};

#endif // FILEHANDLER_H
