#include <boost/iterator/counting_iterator.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/lexical_cast.hpp>
#include "column.h"
#include <inttypes.h>
#ifndef TABLE_H
#define TABLE_H

class Table
{
public:
    Table(string tableName, int rowCount);
    ~Table()
    {
        m_columnsVctr.clear();
    }

    void Init(vector <ColumnMetadata> columnMetadata);

    vector <ColumnMetadata> CreateLineItemMetaData();

    void InitForLineItem ();

    void LoadDataIntoTable (vector <vector <string> > rawData);

    Column* GetColumnAtPos(int columnId) {return &m_columnsVctr[columnId];};

    Column* GetColumnByName(string columnName);

    string GetTableName() {return m_tableName;};

    vector <int>* GetRowIdentifier() {return m_prowIdentifier;};

    vector <Column>* GetpColumns(){return &m_columnsVctr;};

private:

    vector <Column> m_columnsVctr;
    string m_tableName;
    int m_rowCount;
    int m_columnCount;
    vector <int>* m_prowIdentifier;
};

#endif // TABLE_H
