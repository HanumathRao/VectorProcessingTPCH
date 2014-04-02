#include "column.h"
void Column::InitColumn(string columnName,ColumnSqlTypes columnType,
               int rowCount,ColumnEncodingType columnEncoding)
{
    m_pdictionary = new StringDictionary();
    m_columnName = columnName;
    m_columnType = columnType;
    m_rowCount = rowCount;
    m_columnEncoding = columnEncoding;
    int columnSqlType = (int) m_columnType;
    m_lastRowID = 0;

    // If we are using a dictionary only store integer IDs
    if(m_columnEncoding == dictionary)
    {
        m_pdataDictionaryIds = new int[m_rowCount];
    }

    switch (columnSqlType) {
    case INTEGER:
    {
        m_pdataArray = new int[m_rowCount];
        m_columnNativeType = TOINT;
        break;
    }
    case CHAR:
    {
       m_pdataArray = new string[m_rowCount];
       m_columnNativeType = TOSTRING;
       break;
    }
    case DECIMAL:
    {
        m_pdataArray = new double[m_rowCount];
        m_columnNativeType = TODOUBLE;
        break;
     }
    case SMALLINT:
    {
        m_pdataArray = new int_fast16_t[m_rowCount];
        m_columnNativeType = TOINT_FAST16_T;
        break;
    }
    case REAL:
    {
        m_pdataArray = new float[m_rowCount];
        m_columnNativeType = TOFLOAT;
        break;
    }
    case FLOAT:
    {
        m_pdataArray = new float[m_rowCount];
        m_columnNativeType = TOFLOAT;
        break;
    }
    case DOUBLE:
    {
        m_pdataArray = new double[m_rowCount];
        m_columnNativeType = TODOUBLE;
        break;
    }
    case NUMERIC:
    {
        m_pdataArray = new int[m_rowCount];
        m_columnNativeType = TOINT;
        break;
    }
    case VARCHAR:
    {
        m_pdataArray = new string[m_rowCount];
        m_columnNativeType = TOSTRING;
        break;
    }
    case BIT:
    {
        m_pdataArray = new bool[m_rowCount];
        m_columnNativeType = TOBOOL;
        break;
    }
    case TINYINT:
    {
        m_pdataArray = new int_fast16_t[m_rowCount];
        m_columnNativeType = TOINT_FAST16_T;
        break;
    }
    case BIGINT:
    {
        m_pdataArray = new int64_t[m_rowCount];
        m_columnNativeType = TOINT64_T;
        break;
    }
    case BINARY:
    {
        m_pdataArray = new char[m_rowCount];
        m_columnNativeType = TOCHAR;
        break;
    }
    case VARBINARY:
    {
        m_pdataArray = new char[m_rowCount];
        m_columnNativeType = TOCHAR;
        break;
    }
//  case LONGVARBINARY:m_pdataArray = new CLongBinary[m_rowCount];
    case DATE:
    {
        m_pdataArray = new time_t[m_rowCount];
        m_columnNativeType = TOTIME_T;
        break;
    }
    case TIME:
    {
        m_pdataArray = new time_t[m_rowCount];
        m_columnNativeType = TOTIME_T;
        break;
    }
    case TIMESTAMP:
    {
        m_pdataArray = new time_t[m_rowCount];
        m_columnNativeType = TOTIME_T;
        break;
    }
    default:
        break;
    }
}

int Column::InsertIntoDictionary(string key)
{

    StringDictionary::const_iterator got = m_pdictionary->find (key);
    if(got == m_pdictionary->end())
    {
        (*m_pdictionary)[key] = 0;
        (*m_pdictionary)[key] = m_pdictionary->size();
    }
    return (*m_pdictionary)[key] - 1;
}

// Read the dictionary IDs and store them in an array for fast look up
template<class T>
void Column::GetDictionaryIds(int const dictionarySize,T* dictionaryArray)
{
     for ( auto it = m_pdictionary->begin(); it != m_pdictionary->end(); ++it )
    {
       int id = it->second - 1;
       T dictionaryValue = boost::lexical_cast<T>(it->first);
       cout << "ID " << id << " Value " << dictionaryValue << endl;
       dictionaryArray[id] = dictionaryValue;
    }

    for (int i=0; i< dictionarySize ; i++)
    {
        cout << dictionaryArray[i] << endl;
    }
}

template<class T>
void Column::GetCompressedData(T* outDataArray)
{
    if(m_columnEncoding == dictionary)
    {
        // Read the dictionary IDs and store them in an array for fast look up
        int const dictionarySize = m_pdictionary->size();
        T* dictionaryArray = new T[dictionarySize];
        GetDictionaryIds(dictionarySize,dictionaryArray);

        for (int i=0; i< dictionarySize ; i++)
        {
            cout << dictionaryArray[i] << endl;
        }

        for (int i = 0; i<m_rowCount;i++ )
        {
            int dicId = ((int*)m_pdataArray)[i];
            cout << " Value " << dicId << endl;
        }

        // For each value lookup the corresponding value from the dictionary
        for (int i = 0; i<m_rowCount;i++ )
        {
            int dicId = ((int*)m_pdataArray)[i];
            T value = dictionaryArray[dicId];
            cout << "ID " << dicId << " Value " << value << endl;
            outDataArray[i] = value;
        }

        for (int i = 0; i<m_rowCount;i++ )
        {
            cout<< outDataArray[i]<<endl;
        }
    }
}

void Column::GetCompressedDatas(float* outDataArray)
{
    if(m_columnEncoding == dictionary)
    {
        //GetCompressedDataT(outDataArray);
        // Read the dictionary IDs and store them in an array for fast look up
        int const dictionarySize = m_pdictionary->size() ;
        float* dictionaryArray = new float[dictionarySize];
        GetDictionaryIds(dictionarySize,dictionaryArray);

        for (int i=0; i< dictionarySize ; i++)
        {
            //cout << dictionaryArray[i] << endl;
        }

        // For each value lookup the corresponding value from the dictionary
        for (int i = 0; i<m_rowCount;i++ )
        {
            int dicId = ((int*)m_pdataArray)[i];
            float value = dictionaryArray[dicId];
            //cout << "ID " << dicId << " Value " << value << endl;
            outDataArray[i] = value;
        }
    }
}


template<class T>
void Column::InsertRow(string value,T valueType)
{
    // For columns that should use a dictionary
    // Lookup or store the string in the dictionary
    // Get the id
    // and store the id as an int
    if(m_columnEncoding == dictionary)
    {
        int dictionaryID = InsertIntoDictionary(value);
        m_pdataDictionaryIds[m_lastRowID] = dictionaryID;
    }
   ((T*) m_pdataArray)[m_lastRowID] = boost::lexical_cast<T>(value);

    m_lastRowID++;
}


static void ColumnUnitTest(string columnName,int fileRowCount,vector <vector <string> > lineITemData)
{
    Column* l_orderkey=new Column(columnName,NUMERIC,fileRowCount,dictionary);
    int* dataArray = (int*) l_orderkey->GetDataArray();
    int vectorRowCount = lineITemData.size();
    string L_SHIPDATE1 = "12/12/1982";
    string L_SHIPDATE2 = "13/12/1982";
    int found1=0;
    int found2=0;

    // Test the dictionary
    found1 = l_orderkey->InsertIntoDictionary(L_SHIPDATE1);
    found1 = l_orderkey->InsertIntoDictionary(L_SHIPDATE1);
    found2 = l_orderkey->InsertIntoDictionary(L_SHIPDATE2);

    for (int i=0; i< fileRowCount;i++)
    {
        int orderkey = atoi( lineITemData[i][0].c_str() );
        dataArray[i] = orderkey;
    }
}

template<class T>
void Columnt<T>::InitColumn(string columnName,ColumnSqlTypes columnType,
               int rowCount,ColumnEncodingType columnEncoding)
{
    m_lastRowID = 0;
    m_pdictionary = new StringDictionary;
    m_columnName = columnName;
    m_columnType = columnType;
    m_rowCount = rowCount;
    m_columnEncoding = columnEncoding;
    int columnSqlType = (int) m_columnType;
    m_pdataArray = new T[m_rowCount];

    // If we are using a dictionary only store integer IDs
    if(m_columnEncoding == dictionary)
    {
        m_pdataDictionaryIds = new int[m_rowCount];
    }

    switch (columnSqlType) {
    case INTEGER:
    {
        m_columnNativeType = TOINT;
        break;
    }
    case CHAR:
    {
       m_columnNativeType = TOSTRING;
       break;
    }
    case DECIMAL:
    {
        m_columnNativeType = TODOUBLE;
        break;
     }
    case SMALLINT:
    {
        m_columnNativeType = TOINT_FAST16_T;
        break;
    }
    case REAL:
    {
        m_columnNativeType = TOFLOAT;
        break;
    }
    case FLOAT:
    {
        m_columnNativeType = TOFLOAT;
        break;
    }
    case DOUBLE:
    {
        m_columnNativeType = TODOUBLE;
        break;
    }
    case NUMERIC:
    {
        m_columnNativeType = TOINT;
        break;
    }
    case VARCHAR:
    {
        m_columnNativeType = TOSTRING;
        break;
    }
    case BIT:
    {
        m_columnNativeType = TOBOOL;
        break;
    }
    case TINYINT:
    {
        m_columnNativeType = TOINT_FAST16_T;
        break;
    }
    case BIGINT:
    {
        m_columnNativeType = TOINT64_T;
        break;
    }
    case BINARY:
    {
        m_columnNativeType = TOCHAR;
        break;
    }
    case VARBINARY:
    {
        m_columnNativeType = TOCHAR;
        break;
    }
    case DATE:
    {
        m_columnNativeType = TOTIME_T;
        break;
    }
    case TIME:
    {
        m_columnNativeType = TOTIME_T;
        break;
    }
    case TIMESTAMP:
    {
        m_columnNativeType = TOTIME_T;
        break;
    }
    default:
        break;
    }
}

// Read the dictionary IDs and store them in an array for fast look up
template<class T>
void Columnt<T>::GetDictionaryIds(int const dictionarySize,T* dictionaryArray)
{
     for ( auto it = m_pdictionary->begin(); it != m_pdictionary->end(); ++it )
    {
       int id = it->second - 1;
       T dictionaryValue = boost::lexical_cast<T>(it->first);
       cout << "ID " << id << " Value " << dictionaryValue << endl;
       dictionaryArray[id] = dictionaryValue;
    }

    for (int i=0; i< dictionarySize ; i++)
    {
        cout << dictionaryArray[i] << endl;
    }
}

template<class T>
int Columnt<T>::InsertIntoDictionary(string key)
{

    StringDictionary::const_iterator got = m_pdictionary->find (key);
    if(got == m_pdictionary->end())
    {
        (*m_pdictionary)[key] = 0;
        (*m_pdictionary)[key] = m_pdictionary->size();
    }
    return (*m_pdictionary)[key] - 1;
}

template<class T>
void Columnt<T>::GetCompressedData(T* outDataArray)
{
    if(m_columnEncoding == dictionary)
    {
        // Read the dictionary IDs and store them in an array for fast look up
        int const dictionarySize = m_pdictionary->size();
        T* dictionaryArray = new T[dictionarySize];
        GetDictionaryIds(dictionarySize,dictionaryArray);

        for (int i=0; i< dictionarySize ; i++)
        {
            cout << dictionaryArray[i] << endl;
        }

        for (int i = 0; i<m_rowCount;i++ )
        {
            int dicId = ((int*)m_pdataArray)[i];
            cout << " Value " << dicId << endl;
        }

        // For each value lookup the corresponding value from the dictionary
        for (int i = 0; i<m_rowCount;i++ )
        {
            int dicId = ((int*)m_pdataArray)[i];
            T value = dictionaryArray[dicId];
            cout << "ID " << dicId << " Value " << value << endl;
            outDataArray[i] = value;
        }

        for (int i = 0; i<m_rowCount;i++ )
        {
            cout<< outDataArray[i]<<endl;
        }
    }
}

template<class T>
void Columnt<T>::InsertRow(string value)
{
    // For columns that should use a dictionary
    // Lookup or store the string in the dictionary
    // Get the id
    // and store the id as an int
    if(m_columnEncoding == dictionary)
    {
        int dictionaryID = InsertIntoDictionary(value);
        m_pdataDictionaryIds[m_lastRowID] = dictionaryID;
    }
    m_pdataArray[m_lastRowID] = boost::lexical_cast<T>(value);
    m_lastRowID++;
}

ColumnBaseClass::~ColumnBaseClass(){};
