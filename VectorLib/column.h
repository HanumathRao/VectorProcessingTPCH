#include <string>
#include <stdio.h>
#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include </Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include/c++/4.2.1/tr1/unordered_map>
#include <time.h>
#ifndef COLUMN_H
#define COLUMN_H
using namespace std;

enum ColumnSqlTypes {
    CHAR	,//	CString	,
    DECIMAL	,//	Double	,
    SMALLINT	,//	int	,
    REAL	,//	float	,
    INTEGER	,//	long	,
    FLOAT	,//	double	,
    DOUBLE	,//	double	,
    NUMERIC	,//	int	,
    VARCHAR	,//	CString	,
    LONGVARCHAR	,//	CLongBinary	,
    BIT	,//	BOOL	,
    TINYINT	,//	BYTE	,
    BIGINT	,//	Longlong	,
    BINARY	,//	CByteArray	,
    VARBINARY	,//	CByteArray	,
    LONGVARBINARY	,//	CLongBinary	,
    DATE	,//	Ctime	,
    TIME	,//	Ctime	,
    TIMESTAMP	//	Ctime
    };


enum ColumnNativetype {TOSTRING,TODOUBLE,TOINT_FAST16_T,TOINT,TOFLOAT,TOINT64_T,TOCHAR,TOTIME_T,TOBOOL};

enum ColumnEncodingType {dictionary,byValue,rle};

struct ColumnMetadata
{
    string columnName;
    ColumnSqlTypes columnType;
    int rowCount;
    ColumnEncodingType columnEncoding;
};

typedef std::tr1::unordered_map < std::string,int, boost::hash<string> > StringDictionary;

class ColumnBaseClass
{
public :
   ColumnBaseClass(string columnName,ColumnSqlTypes columnSqlType,int rowCount,
                   ColumnEncodingType columnEncoding)
   {
       m_rowCount = rowCount;
       m_columnName = columnName;
       m_columnType = columnSqlType;
       m_columnEncoding = columnEncoding;
       //m_columnNativeType = nativeType;
   };

   virtual ~ColumnBaseClass();
   virtual void* GetDataArray() = 0;

protected:
    int m_rowCount;
    string m_columnName;
    ColumnSqlTypes m_columnType;
    ColumnEncodingType m_columnEncoding;
    ColumnNativetype m_columnNativeType;
};

class Column
{
public:

    Column(ColumnMetadata columnMetaData)
    {
        Column::Column(columnMetaData.columnName,columnMetaData.columnType,
                       columnMetaData.rowCount,columnMetaData.columnEncoding);
    }

    Column(string columnName,ColumnSqlTypes columnType,
                   int rowCount,ColumnEncodingType columnEncoding)
    {
        InitColumn(columnName,columnType,
                   rowCount,columnEncoding);
    }

    void InitColumn(string columnName,ColumnSqlTypes columnType,
                    int rowCount,ColumnEncodingType columnEncoding);

    //static void ColumnUnitTest(string columnName,int fileRowCount,vector <vector <string> > lineITemData);

    // Getters
    void* GetDataArray() {
        return m_pdataArray;
    }

    void GetCompressedDatas(float* taxDataArray);

    template<class T>
    void GetCompressedData(T* taxDataArray);

    template<class T>
    void GetDictionaryIds(int const dictionarySize,T* dictionaryArray);

    template<class T>
    void InsertRow(string value,T valueType);

    ColumnNativetype GetColumnNativeType(){return m_columnNativeType;};
    ColumnEncodingType GetColumnEncoding(){return m_columnEncoding;};
    StringDictionary* GetDictionary(){return m_pdictionary;};
    string GetColumnName(){return m_columnName;};

    int InsertIntoDictionary(string key);

    void InsertValue(){};

private:
    void* m_pdataArray;
    void* m_parentRowset;
    StringDictionary* m_pdictionary;
    int* m_pdataDictionaryIds;
    int m_lastRowID;
    int m_rowCount;
    string m_columnName;
    ColumnSqlTypes m_columnType;
    ColumnEncodingType m_columnEncoding;
    ColumnNativetype m_columnNativeType;
};

template <class T>
class Columnt : public ColumnBaseClass
{
public:

    //Columnt(string columnName,ColumnSqlTypes columnType,int rowCount,ColumnEncodingType columnEncoding);
    //Columnt(ColumnMetadata columnMetaData);
    static void ColumnUnitTest(string columnName,int fileRowCount,vector <vector <string> > lineITemData);

    Columnt(ColumnMetadata columnMetaData):ColumnBaseClass(columnMetaData.columnName,columnMetaData.columnType,
                                                           columnMetaData.rowCount,columnMetaData.columnEncoding)
    {
        Columnt::Columnt(columnMetaData.columnName,columnMetaData.columnType,
                       columnMetaData.rowCount,columnMetaData.columnEncoding);
    }

    Columnt(string columnName,ColumnSqlTypes columnType,
                   int rowCount,ColumnEncodingType columnEncoding):ColumnBaseClass(columnName,columnType,
                                                                                   rowCount,columnEncoding)
    {
        InitColumn(columnName,columnType,
                   rowCount,columnEncoding);
    }

    void InitColumn(string columnName,ColumnSqlTypes columnType,
                    int rowCount,ColumnEncodingType columnEncoding);

    // Getters
    void GetDataArray(T* &p) {p=m_pdataArray;}

    void GetCompressedData(T* taxDataArray);

    void GetDictionaryIds(int const dictionarySize,T* dictionaryArray);

    void InsertRow(string value);

    int InsertIntoDictionary(string key);

    ColumnNativetype GetColumnNativeType(){return m_columnNativeType;};
    ColumnEncodingType GetColumnEncoding(){return m_columnEncoding;};
    StringDictionary* GetDictionary(){return m_pdictionary;};
    string GetColumnName(){return m_columnName;};

    void InsertValue(){};

private:
    T* m_pdataArray;
    int* m_pdataDictionaryIds;
    int m_rowCount;
    int m_lastRowID;
    void* m_parentRowset;
    string m_columnName;
    ColumnSqlTypes m_columnType;
    ColumnNativetype m_columnNativeType;
    ColumnEncodingType m_columnEncoding;
    StringDictionary* m_pdictionary;
};

#endif // COLUMN_H
