#include "gds_stream.h"
#include <math.h>
#include <fstream>
#include <iostream>
#include <cassert>

namespace {

std::string RecordType (unsigned char theValue)
{
    const char* aResult = nullptr;
    switch (theValue) {
    case 0x00: aResult = "HEADER"; break;
    case 0x01: aResult = "BGNLIB"; break;
    case 0x02: aResult = "LIBNAME"; break;
    case 0x03: aResult = "UNITS"; break;
    case 0x04: aResult = "ENDLIB"; break;
    case 0x05: aResult = "BGNSTR"; break;
    case 0x06: aResult = "STRNAME"; break;
    case 0x07: aResult = "ENDSTR"; break;
    case 0x08: aResult = "BONDARY"; break; //object
    case 0x09: aResult = "PATH"; break; //object
    case 0x0A: aResult = "SREF"; break; // reference to object
    case 0x0B: aResult = "AREF"; break; // reference to array
    case 0x0C: aResult = "TEXT"; break;
    case 0x0D: aResult = "LAYER"; break;  //property
    case 0x0E: aResult = "DATATYPE"; break;
    case 0x0F: aResult = "WIDTH"; break; // property
    case 0x10: aResult = "XY"; break; // property
    case 0x11: aResult = "ENDEL"; break;
    case 0x12: aResult = "SNAME"; break;
    case 0x13: aResult = "COLROW"; break;
    case 0x15: aResult = "NODE"; break;
    case 0x16: aResult = "TEXTTYPE"; break;
    case 0x17: aResult = "PRESENTATION"; break;
    case 0x19: aResult = "STRING"; break;
    case 0x1A: aResult = "STRANS"; break;
    case 0x1B: aResult = "MAG"; break; // property of transformation (magnification - scale)
    case 0x1C: aResult = "ANGLE"; break; // property of transformation
    case 0x1F: aResult = "REFLIBS"; break;
    case 0x20: aResult = "FONTS"; break;
    case 0x21: aResult = "PATHTYPE"; break;
    case 0x22: aResult = "GENERATIONS"; break;
    case 0x23: aResult = "ATTRATABLE"; break;
    case 0x26: aResult = "ELFLAGS"; break;
    case 0x2A: aResult = "NODETYPE"; break;
    case 0x2B: aResult = "PROPATTR"; break;
    case 0x2C: aResult = "PROPVALUE"; break;
    case 0x2D: aResult = "BOX"; break; // object
    case 0x2E: aResult = "BOXTYPE"; break;
    case 0x2F: aResult = "PLEX"; break;
    case 0x32: aResult = "TAPENUM"; break;
    case 0x33: aResult = "TAPECODE"; break;
    case 0x36: aResult = "FORMAT"; break;
    case 0x37: aResult = "MASK"; break;
    case 0x38: aResult = "ENDMASKS"; break;
    default: aResult = "UNKNOWN"; break;
    }
    return std::string (aResult);
}

void swapByteOrder(short& us)
{
    us = (us >> 8) |
         (us << 8);
}

void swapByteOrder(int& ui)
{
    ui = ((ui>>24) & 0xff) |      // move byte 3 to byte 0
         ((ui<<8)  & 0xff0000) |  // move byte 1 to byte 2
         ((ui>>8)  & 0xff00) |    // move byte 2 to byte 1
         ((ui<<24) & 0xff000000); // byte 0 to byte 3;
}

template <typename T>
T readValue(std::ifstream& theStream)
{
    T aValue;
    theStream.read ((char*)&aValue, sizeof(T));
    return aValue;
}

template <typename T> // short, int, etc..
T readIntValue (std::ifstream& theStream)
{
    auto aValue = readValue<T> (theStream);
    swapByteOrder (aValue);
    return aValue;
}

template <typename T> // float, double, etc..
T readRealValue (std::ifstream& theStream)
{
    char aByte1 = readValue<char> (theStream);
    char e = (aByte1 & 0x7F) - 64;

    bool aPositive = !(aByte1 & 0x80);

    uint64_t m = 0;
    for (size_t i = 0; i < sizeof (T) - 1; i++) {
        char aByteN = readValue<char> (theStream);
        m |= (static_cast<uint64_t> (aByteN) & 0xFF) << ((6-i)*8);
    }

    T d = m;
    d = d * static_cast<T> (std::pow (16, e-14));
    d = aPositive ? d : -d;

    return d;
}

} // end of unnamed namespace


bool GDS_Stream::Open (const std::string& theFileName)
{
    myStream.open (theFileName, std::fstream::in | std::fstream::binary);

    if (!myStream.is_open()) {
        return false;
    }

    return true;
}

bool GDS_Stream::ReadInfo()
{
    if (IsEnd()) {
        return false;
    }

    myCurrentRecordSize = readIntValue<short> (myStream);
    myCurrentRecordType = readValue<GDS_RecordType> (myStream);
    myCurrentDataType   = readValue<DataType> (myStream);

//    std::cout << "Size: " << myCurrentRecordSize
//              << " Type: " << RecordType (myCurrentRecordType)
//              << " Data type: " << myCurrentDataType << std::endl;

    return !IsEnd();
}

void GDS_Stream::Close()
{
    myStream.close();
}

bool GDS_Stream::IsEnd()
{
    return !myStream.is_open() || myStream.eof() || myCurrentRecordType == ENDLIB;
}

void GDS_Stream::ReadData (Point2D<int>& theValue)
{
    ReadData (theValue.X());
    ReadData (theValue.Y());
}

void GDS_Stream::ReadData (GDS_Element::CoordinatesType& theValue)
{
    size_t anXYCount    = (myCurrentRecordSize - 4) / sizeof (int);
    size_t aPointsCount = anXYCount / 2;

    for (size_t i = 0; i < aPointsCount; i++) {
        Point2D<int> aPoint;
        ReadData (aPoint);
        GDS_Element::CoordinatesType::value_type aNewPoint (aPoint.X(), aPoint.Y());
        theValue.push_back (aNewPoint);
    }
}

void GDS_Stream::ReadData (std::string& theValue)
{
    auto aSize = myCurrentRecordSize - 4;
    char* aValue = new char[aSize];
    myStream.read (aValue, aSize);
    theValue.assign (aValue, aSize - 1);
}

void GDS_Stream::ReadData (double& theValue)
{
    assert (CurrenDataBytesCount() == sizeof (theValue));
    theValue = readRealValue<double> (myStream);
}

void GDS_Stream::ReadData (float& theValue)
{
    assert (CurrenDataBytesCount() == sizeof (theValue));
    theValue = readRealValue<float> (myStream);
}

void GDS_Stream::ReadData (int& theValue)
{
    assert (CurrenDataBytesCount() == sizeof (theValue));
    theValue = readIntValue<int> (myStream);
}

void GDS_Stream::ReadData (short& theValue)
{
    if (myCurrentDataType != BitArray) {
        assert (CurrenDataBytesCount() == sizeof (theValue));
    }
    theValue = readIntValue<short> (myStream);
}

void GDS_Stream::SkipData()
{
//    std::cout << "Skip data: " << myCurrentRecordSize - 4 << std::endl;
    myStream.seekg (myCurrentRecordSize - 4, std::ios::cur);
}



int GDS_Stream::CurrenDataBytesCount()
{
    switch (myCurrentDataType) {
    case NoData:             // 1 byte
    case BitArray:           // 1 byte
    case ASCII: return 1;    // 1 byte
    case Int2: return 2;     // 2 byte
    case Int4:               // 4 byte
    case Real4: return 4;    // 4 byte
    case Real8: return 8;    // 8 byte
    default: assert (false); return 0;
    }
}

