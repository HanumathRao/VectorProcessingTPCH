#include "table.h"

Table::Table(string tableName, int rowCount)
{
    m_tableName = tableName;
    m_rowCount = rowCount;
    m_prowIdentifier = new std::vector<int> (
                boost::counting_iterator<int>(0),
                boost::counting_iterator<int>(m_rowCount));
  //  m_pcolumns = new std::vector<Column>(m_columnCount);
}

void Table::Init(vector <ColumnMetadata> columnMetadata)
{


}

void Table::InitForLineItem()
{
    vector <ColumnMetadata> tableMetaData = CreateLineItemMetaData();
    for ( auto &i : tableMetaData ) {
       m_columnsVctr.push_back(*(new Column(i.columnName,i.columnType,m_rowCount,i.columnEncoding)));
    }
}

vector <ColumnMetadata> Table::CreateLineItemMetaData()
{
    vector <ColumnMetadata> tableMetaData;
    ColumnMetadata L_ORDERKEY;
    L_ORDERKEY.columnEncoding= byValue;
    L_ORDERKEY.columnName = "L_ORDERKEY";
    L_ORDERKEY.columnType = INTEGER;

    ColumnMetadata L_PARTKEY;
    L_PARTKEY.columnEncoding= byValue;
    L_PARTKEY.columnName = "L_PARTKEY";
    L_PARTKEY.columnType = INTEGER;

    ColumnMetadata L_SUPPKEY;
    L_SUPPKEY.columnEncoding= byValue;
    L_SUPPKEY.columnName = "L_SUPPKEY";
    L_SUPPKEY.columnType = INTEGER;

    ColumnMetadata L_LINENUMBER;
    L_LINENUMBER.columnEncoding= rle;
    L_LINENUMBER.columnName = "L_SUPPKEY";
    L_LINENUMBER.columnType = INTEGER;

    ColumnMetadata L_QUANTITY;
    L_QUANTITY.columnEncoding= dictionary;
    L_QUANTITY.columnName = "L_QUANTITY";
    L_QUANTITY.columnType = FLOAT;

    ColumnMetadata L_EXTENDEDPRICE;
    L_EXTENDEDPRICE.columnEncoding= byValue;
    L_EXTENDEDPRICE.columnName = "L_EXTENDEDPRICE";
    L_EXTENDEDPRICE.columnType = FLOAT;

    ColumnMetadata L_DISCOUNT;
    L_DISCOUNT.columnEncoding= byValue;
    L_DISCOUNT.columnName = "L_DISCOUNT";
    L_DISCOUNT.columnType = FLOAT;

    ColumnMetadata L_TAX;
    L_TAX.columnEncoding= dictionary;
    L_TAX.columnName = "L_TAX";
    L_TAX.columnType = FLOAT;

    ColumnMetadata L_RETURNFLAG;
    L_RETURNFLAG.columnEncoding= dictionary;
    L_RETURNFLAG.columnName = "L_RETURNFLAG";
    L_RETURNFLAG.columnType = CHAR;

    ColumnMetadata L_LINESTATUS;
    L_LINESTATUS.columnEncoding= dictionary;
    L_LINESTATUS.columnName = "L_LINESTATUS";
    L_LINESTATUS.columnType = CHAR;

    ColumnMetadata L_SHIPDATE;
    L_SHIPDATE.columnEncoding= dictionary;
    L_SHIPDATE.columnName = "L_SHIPDATE";
    L_SHIPDATE.columnType = DATE;

    ColumnMetadata L_COMMITDATE;
    L_COMMITDATE.columnEncoding= dictionary;
    L_COMMITDATE.columnName = "L_COMMITDATE";
    L_COMMITDATE.columnType = DATE;

    ColumnMetadata L_RECEIPTDATE;
    L_RECEIPTDATE.columnEncoding= dictionary;
    L_RECEIPTDATE.columnName = "L_RECEIPTDATE";
    L_RECEIPTDATE.columnType = DATE;

    ColumnMetadata L_SHIPINSTRUCT;
    L_SHIPINSTRUCT.columnEncoding= dictionary;
    L_SHIPINSTRUCT.columnName = "L_SHIPINSTRUCT";
    L_SHIPINSTRUCT.columnType = CHAR;

    ColumnMetadata L_SHIPMODE;
    L_SHIPMODE.columnEncoding= dictionary;
    L_SHIPMODE.columnName = "L_SHIPMODE";
    L_SHIPMODE.columnType = CHAR;

    ColumnMetadata L_COMMENT;
    L_COMMENT.columnEncoding= byValue;
    L_COMMENT.columnName = "L_COMMENT";
    L_COMMENT.columnType = CHAR;

    tableMetaData.push_back(L_ORDERKEY);
    tableMetaData.push_back(L_PARTKEY);
    tableMetaData.push_back(L_SUPPKEY);
    tableMetaData.push_back(L_LINENUMBER);
    tableMetaData.push_back(L_QUANTITY);
    tableMetaData.push_back(L_EXTENDEDPRICE);
    tableMetaData.push_back(L_DISCOUNT);
    tableMetaData.push_back(L_TAX);
    tableMetaData.push_back(L_RETURNFLAG);
    tableMetaData.push_back(L_LINESTATUS);
    tableMetaData.push_back(L_SHIPDATE);
    tableMetaData.push_back(L_COMMITDATE);
    tableMetaData.push_back(L_RECEIPTDATE);
    tableMetaData.push_back(L_SHIPINSTRUCT);
    tableMetaData.push_back(L_COMMENT);

    return tableMetaData;
}

void Table::LoadDataIntoTable (vector <vector <string> > rawData)
{
    ColumnNativetype columnNativeTypeInt;
    void* pdataArray;
    string rawDataString;
    StringDictionary* pcolumnDictionary;
    int columnEncodingType;
    int dictionaryID;
    vector <ColumnNativetype> columnNativeTypeVector;
    vector <ColumnEncodingType> columnEncodingTypeVector;
    vector <StringDictionary*> pcolumnDictionaryVector;
    vector <void*> columnDataArrayVector;

    // Save the vector types on the stack to avoid cache missies
    for ( int j =0 ; j < m_columnsVctr.size() ; j++)
    {
        columnNativeTypeVector.push_back(m_columnsVctr[j].GetColumnNativeType());
        columnEncodingTypeVector.push_back(m_columnsVctr[j].GetColumnEncoding());
        columnDataArrayVector.push_back(m_columnsVctr[j].GetDataArray());
        pcolumnDictionaryVector.push_back(m_columnsVctr[j].GetDictionary());
    }

    // Iterate over each row from the flat file
    for (int i=0; i< m_rowCount;i++)
    {
        // rawData[i][j]
        // i is Row
        // j is Column

        // Iterate over each column
        for ( int j =0 ; j < m_columnsVctr.size() ; j++)
        {
            Column currentColumn = m_columnsVctr[j];
            rawDataString = rawData[i][j];
            columnNativeTypeInt = columnNativeTypeVector[j];
            columnNativeTypeInt = currentColumn.GetColumnNativeType();
            columnEncodingType = (int)columnEncodingTypeVector[j];
            pdataArray = columnDataArrayVector[j];
            pcolumnDictionary = pcolumnDictionaryVector[j];

            typeid(currentColumn).name();
            //currentColumn.InsertRow(rawDataString);
            //continue;

            // For columns that should use a dictionary
            // Lookup or store the string in the dictionary
            // Get the id
            // and store the id as an int
            if(columnEncodingType == dictionary)
            {
                pcolumnDictionary;
                dictionaryID = m_columnsVctr[j].InsertIntoDictionary(rawDataString);
                ((int*) pdataArray)[i] = dictionaryID;
                continue;
            }

            if (columnNativeTypeInt == TOSTRING)
            {
                ((string*) pdataArray)[i] = rawData[i][j];
                //currentColumn.InsertRow(rawDataString,string t );
                continue;
            }

            if(columnNativeTypeInt == TODOUBLE)
            {
                ((double*) pdataArray)[i] = boost::lexical_cast<double>(rawData[i][j]);
                continue;
            }

            if (columnNativeTypeInt == TOINT_FAST16_T)
            {
                ((int_fast16_t*) pdataArray)[i] = boost::lexical_cast<int_fast16_t>(rawData[i][j]);
                continue;
            }

            if (columnNativeTypeInt == TOINT)
            {
                //((int*) pdataArray)[i] = atoi( rawData[i][j].c_str() );
                ((int*) pdataArray)[i] = boost::lexical_cast<int>(rawData[i][j]);
                continue;
            }

            if (columnNativeTypeInt == TOFLOAT)
            {
                //((float_t*) pdataArray)[i] = atoi( rawData[i][j].c_str() );
                float_t value = boost::lexical_cast<float>(rawData[i][j]);
                ((float_t*) pdataArray)[i] = boost::lexical_cast<float>(rawData[i][j]);
                continue;
            }

            if (columnNativeTypeInt == TOINT64_T)
            {
                ((int64_t*) pdataArray)[i] = boost::lexical_cast<int64_t>(rawData[i][j]);
                continue;
            }

            if (columnNativeTypeInt == TOCHAR)
            {
                ((char*) pdataArray)[i] = boost::lexical_cast<char>(rawData[i][j]);
                continue;
            }
            if (columnNativeTypeInt == TOTIME_T)
            {
                ((time_t*) pdataArray)[i] = boost::lexical_cast<time_t>(rawData[i][j]);
                continue;
            }
            if (columnNativeTypeInt == TOBOOL)
            {
                ((bool*) pdataArray)[i] = boost::lexical_cast<bool>(rawData[i][j]);
                continue;
            }

            /*switch (columnNativeTypeInt)
            {
               TOSTRING:
                {
                    ((string*) pdataArray)[i] = rawData[i][j];
                    break;
                }
                TODOUBLE:
                {
                    //((double*) pdataArray)[i] = atof(rawData[i][j].c_str());
                    ((double*) pdataArray)[i] = boost::lexical_cast<double>(rawData[i][j]);
                    break;
                }
                TOINT_FAST16_T:
                {
                    //((int_fast16_t*) pdataArray)[i] = (int_fast16_t) atoi( rawData[i][j].c_str() );
                    ((int_fast16_t*) pdataArray)[i] = boost::lexical_cast<int_fast16_t>(rawData[i][j]);
                    break;
                }
                TOINT:
                {
                    //((int*) pdataArray)[i] = atoi( rawData[i][j].c_str() );
                    ((int*) pdataArray)[i] = boost::lexical_cast<int>(rawData[i][j]);
                    break;
                }
                TOFLOAT:
                {
                    //((float_t*) pdataArray)[i] = atoi( rawData[i][j].c_str() );
                    ((float_t*) pdataArray)[i] = boost::lexical_cast<float>(rawData[i][j]);
                    break;
                }
                TOINT64_T:
                {
                    ((int64_t*) pdataArray)[i] = boost::lexical_cast<int64_t>(rawData[i][j]);
                    break;
                }
                TOCHAR:
                {
                    ((char*) pdataArray)[i] = boost::lexical_cast<char>(rawData[i][j]);
                    break;
                }
                TOTIME_T:
                {
                    ((time_t*) pdataArray)[i] = boost::lexical_cast<time_t>(rawData[i][j]);
                    break;
                }
                TOBOOL:
                {
                    ((bool*) pdataArray)[i] = boost::lexical_cast<bool>(rawData[i][j]);
                    break;
                }
                default:
                    break;
            }*/
        }
        //int orderkey = atoi( rawData[i][0].c_str() );
        //dataArray[i] = orderkey;
    }
}
Column* Table::GetColumnByName(string columnName)
{
    // Iterate over each column
    for ( int j =0 ; j < m_columnsVctr.size() ; j++)
    {
        Column currentColumn = m_columnsVctr[j];
        if(currentColumn.GetColumnName() == columnName)
        {
            return &m_columnsVctr[j];
        }
    }


}
