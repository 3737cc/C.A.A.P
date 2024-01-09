
// filehandler.cpp

#include "filehandler.h"

vector<string> FileHandler::getInputFilePaths() {
    cout << "输入要读取的FITS文件的路径（用空格分隔）：";
    string filePaths;
    getline(cin, filePaths);

    istringstream iss(filePaths);
    vector<string> paths;
    string filePath;
    while (iss >> filePath) {
        paths.push_back(filePath);
    }

    return paths;
}

string FileHandler::getOutputFilePath() {
    string outputFilePath;
    cout << "输入处理后的FITS文件的输出路径：";
    getline(cin, outputFilePath);
    return outputFilePath;
}
