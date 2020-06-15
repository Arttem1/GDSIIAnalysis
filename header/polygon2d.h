#ifndef POLYGON2D_H
#define POLYGON2D_H

#include "point2d.h"
#include "box2d.h"
#include <vector>

class Transformation2D;

class polygon2d
{
public:
    typedef Point2D<double>     Point;
    typedef std::vector<Point>  CoordinatesType;

    polygon2d();

    polygon2d (const CoordinatesType& theCoordinates);

    const Box2d& BoundingBox();

    const CoordinatesType& Coordinates() const;
    void SetCoordinates (CoordinatesType& theCoordinates);

    void AddPoint (const CoordinatesType::value_type& thePoint);

    void Transform (const Transformation2D& theTrsf);
    polygon2d Transformed (const Transformation2D& theTrsf) const;

private:
    CoordinatesType myCoordinates;
    Box2d           myBoundingBox;
};

#endif // POLYGON2D_H
