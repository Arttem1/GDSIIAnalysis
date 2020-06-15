#include "boundingbox2dhelper.h"
#include "gds_structureelementvisitor.h"
#include "polygon2d.h"

BoundingBox2dHelper::BoundingBox2dHelper()
{

}

Box2d BoundingBox2dHelper::Compute (const polygon2d& thePolygon)
{
    const auto& aCoordinates = thePolygon.Coordinates();
    auto aFirstCoordinate = aCoordinates.front();

    double max1ByX = aFirstCoordinate.X();
    double min1ByX = aFirstCoordinate.X();
    double max1ByY = aFirstCoordinate.Y();
    double min1ByY = aFirstCoordinate.Y();

    for (const auto& aCoordinate : aCoordinates)
    {
        if (max1ByX < aCoordinate.X()) {
            max1ByX = aCoordinate.X();
        }

        if (min1ByX > aCoordinate.X()) {
            min1ByX = aCoordinate.X();
        }

        if (max1ByY < aCoordinate.Y()) {
            max1ByY = aCoordinate.Y();
        }
        if (min1ByY > aCoordinate.Y()) {
            min1ByY = aCoordinate.Y();
        }
    }
    Point2D<double> max(max1ByX, max1ByY);
    Point2D<double> min(min1ByX, min1ByY);

    return Box2d(min, max);
}
