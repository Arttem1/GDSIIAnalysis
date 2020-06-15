#ifndef GDS_PATH_H
#define GDS_PATH_H

#include "gds_element.h"


class GDS_Path : public GDS_Element
{
public:
   typedef int WidthType;

    enum PathType : short
    {
        Flush              = 0,
        HalfRoundExtension = 1,
        HalfWidthExtension = 2,
        Custom             = 3
    };

    GDS_Path();

    PathType Type();
    void SetType (PathType theType);

    WidthType Width();
    void SetWidth (WidthType theWidth);

    void ToPolygon();

    const CoordinatesType& Coordinates() override;
    void SetCoordinates (CoordinatesType& theCoordinates) override;

private:
    PathType  myType;
    WidthType myWidth;
    CoordinatesType myCoordinates;
    bool      myConvertedToPolygon = false;
};

#endif // GDS_PATH_H
