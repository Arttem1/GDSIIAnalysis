#ifndef GDSELEMENT_H
#define GDSELEMENT_H

#include <vector>
#include "point2d.h"
#include "polygon2d.h"
#include "typedefs.h"

#include <memory>

class GDS_Element
{
public:
    typedef Point2D<double>     Point;
    typedef std::vector<Point>    CoordinatesType;

    GDS_Element();

    virtual ~GDS_Element() { }

    LayerType Layer() const;
    void SetLayer (LayerType theLayer);

    Point Postiton() const;

    virtual const CoordinatesType& Coordinates();
    virtual void SetCoordinates (CoordinatesType& theCoordinates);

    void AddPoint (const Point& thePoint);

    const polygon2d& Polygon() const;

    const Box2d& BoundingBox();

    // ignore data type

protected:
    LayerType myLayer;
    polygon2d myPolygon;
};

class GDS_ElementHash
{
public:
    size_t operator() (const GDS_ElementPtr& theValue) const;
};

#endif // GDSELEMENT_H
