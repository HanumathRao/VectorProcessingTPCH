#include "FileReader.h"

vector<string> FileReader::split(const string& s, const string& delim, const bool keep_empty) {
    vector<string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        string temp(substart, subend);
        if (keep_empty || !temp.empty()) {
            result.push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

FileReader::FileReader(const char* fileName, string delimiter)
{
    m_filename = fileName;

    streampos begin,end;
    string line;
    ifstream inputFileStream (fileName);
    vector <vector <string> > table;
    inputFileStream.seekg (0, inputFileStream.end);
    int length = inputFileStream.tellg();
    m_fileSize = inputFileStream.tellg();
    inputFileStream.seekg (0, inputFileStream.beg);
    char * buffer = new char [length];
    char * readBuffer = new char[m_bufferSize];
    cout << inputFileStream.tellg() << endl;
    cout << inputFileStream.end << endl;
    m_delimiter = delimiter;

    /*
        // Buffered reader , very efficient in terms of IO
        while ((inputFileStream.tellg() <= length) && (inputFileStream.tellg() >= 0))
        {
            currPos = inputFileStream.tellg();
            //printf("-------------\n" );
            printf("Current position %d \n",currPos);
            //printf("-------------\n" );
            inputFileStream.read (readBuffer,m_bufferSize);
        }*/

    /*
    if (inputFileStream.is_open())
    {
        int rowCount = 0;
        while ( getline(inputFileStream, line) )
        {
                  // Parser #3
            //vector<string> words = split(line,m_delimiter);
            table.push_back(split(line,m_delimiter));
            rowCount++;
        }
        //table.push_back(words)
        cout << rowCount << endl;
        inputFileStream.close();
    }
    else
    {
        cout << "Unable to open file";
    }
    */

}

vector <vector <string> > FileReader::ReadData(int readRowCount)
{
    vector <vector <string> > vectorTable;
    string line;
    ifstream inputFileStream (m_filename);
    m_fileSize = inputFileStream.tellg();
    inputFileStream.seekg (0, inputFileStream.beg);
    if (inputFileStream.is_open())
    {
        int rowCount = 0;
        while ( getline(inputFileStream, line) )
        {
            vectorTable.push_back(split(line,m_delimiter));
            rowCount++;
            if(rowCount == readRowCount)
            {
                break;
            }
        }
        m_rowCount = rowCount;
        cout <<"File" << m_filename << " row count " << rowCount << endl;
        inputFileStream.close();
    }
    else
    {
        cout << "Unable to open file";
    }
    return vectorTable;
}
