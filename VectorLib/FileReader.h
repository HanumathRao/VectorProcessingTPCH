#ifndef FileReader_H
#define FileReader_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <sstream>      // std::istringstream


using namespace std;

class FileReader
{
public:
    // Init the parameters etc..
    FileReader(const char* fileName,string delimiter);

    // Read the acutal file
    vector <vector <string> > ReadData(int readRowCount);

    vector<string> split(const string& s, const string& delim, const bool keep_empty = true);
    int GetRowCount (){return m_rowCount;};

private:
    const char* m_filename;
    static const int m_bufferSize = 65536;
    int m_fileSize;
    int m_rowCount;
    string m_delimiter;
};

#endif // FileReader_H
