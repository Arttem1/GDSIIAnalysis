#include "polygon2d.h"
#include "transformation2d.h"
#include "boundingbox2dhelper.h"

polygon2d::polygon2d()
{

}

polygon2d::polygon2d (const CoordinatesType& theCoordinates)
    : myCoordinates (theCoordinates)
{

}

const Box2d& polygon2d::BoundingBox()
{
    if (myBoundingBox.IsEmpty()) {
        myBoundingBox = BoundingBox2dHelper::Compute (*this);
    }

    return myBoundingBox;
}

const polygon2d::CoordinatesType& polygon2d::Coordinates() const
{
    return myCoordinates;
}

void polygon2d::SetCoordinates (CoordinatesType& theCoordinates)
{
    myCoordinates.swap (theCoordinates);
}

void polygon2d::AddPoint (const CoordinatesType::value_type& thePoint)
{
    myCoordinates.push_back (thePoint);
}

void polygon2d::Transform (const Transformation2D& theTrsf)
{
    for (auto& aCoordinate : myCoordinates) {
        aCoordinate.Transform (theTrsf);
    }
}

polygon2d polygon2d::Transformed (const Transformation2D& theTrsf) const
{
    polygon2d aCopy (*this);
    aCopy.Transform (theTrsf);
    return aCopy;
}
