
// filehandler.cpp

#include "filehandler.h"

vector<string> FileHandler::getInputFilePaths() {
    cout << "����Ҫ��ȡ��FITS�ļ���·�����ÿո�ָ�����";
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
    cout << "���봦����FITS�ļ������·����";
    getline(cin, outputFilePath);
    return outputFilePath;
}
