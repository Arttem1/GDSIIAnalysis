#include "gds_path.h"
#include "gds_helper.h"

GDS_Path::GDS_Path()
{

}

GDS_Path::PathType GDS_Path::Type()
{
    return myType;
}

GDS_Path::WidthType GDS_Path::Width()
{
    return myWidth;
}

void GDS_Path::SetWidth (WidthType theWidth)
{
    myWidth = theWidth;
}

void GDS_Path::SetType (PathType theType)
{
    myType = theType;
}

void GDS_Path::ToPolygon()
{
    if (!myConvertedToPolygon) {

        auto aCoorinates = GDS_Helper::ToPolygon (*this);
        myPolygon = aCoorinates;

        myConvertedToPolygon = true;
    }
}

const GDS_Path::CoordinatesType& GDS_Path::Coordinates()
{
    return myCoordinates;
}

void GDS_Path::SetCoordinates (CoordinatesType& theCoordinates)
{
    myCoordinates = theCoordinates;
}
