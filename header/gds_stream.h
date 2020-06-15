#ifndef GDS_STREAM_H
#define GDS_STREAM_H

#include <string>
#include <fstream>
#include "point2d.h"
#include "gds_recordtype.h"
#include "gds_element.h"

class GDS_Stream
{
public:
    bool Open (const std::string& theFileName);
    void Close();
    bool IsEnd();

    bool ReadInfo();

    void ReadData (Point2D<int>& theValue);
    void ReadData (GDS_Element::CoordinatesType& theValue);
    void ReadData (std::string& theValue);
    void ReadData (double& theValue);
    void ReadData (float& theValue);
    void ReadData (short& theValue);
    void ReadData (int& theValue);

    void SkipData();

    GDS_RecordType CurrentRecordType() { return myCurrentRecordType; }

private:
    void CheckDataType();

    enum DataType : unsigned char
    {
        NoData = 0x00,   // 1 byte
        BitArray = 0x01, // 1 byte
        Int2 = 0x02,     // 2 byte
        Int4 = 0x03,     // 4 byte
        Real4 = 0x04,    // 4 byte
        Real8 = 0x05,    // 8 byte
        ASCII = 0x06     // 1 byte
    };

    int CurrenDataBytesCount();

private:
    std::ifstream myStream;
    GDS_RecordType    myCurrentRecordType;
    short         myCurrentRecordSize;
    DataType      myCurrentDataType;
};

#endif // GDS_STREAM_H
