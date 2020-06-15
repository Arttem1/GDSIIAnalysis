#include "gds_element.h"

GDS_Element::GDS_Element()
{

}

LayerType GDS_Element::Layer() const
{
    return myLayer;
}

void GDS_Element::SetLayer (LayerType theLayer)
{
    myLayer = theLayer;
}

void GDS_Element::AddPoint (const Point& thePoint)
{
    myPolygon.AddPoint (thePoint);
}

const polygon2d& GDS_Element::Polygon() const
{
    return myPolygon;
}

GDS_Element::Point GDS_Element::Postiton() const
{
    return myPolygon.Coordinates().front();
}

const GDS_Element::CoordinatesType& GDS_Element::Coordinates()
{
    return myPolygon.Coordinates();
}

void GDS_Element::SetCoordinates (CoordinatesType& theCoordinates)
{
    myPolygon.SetCoordinates (theCoordinates);
}

const Box2d& GDS_Element::BoundingBox()
{
    return myPolygon.BoundingBox();
}

size_t GDS_ElementHash::operator() (const GDS_ElementPtr& theValue) const
{
    size_t aSeed = 0;

    const auto& aCoordinates = theValue->Coordinates();
    if (!aCoordinates.empty()) {
        auto aFrontCoordinate = theValue->Coordinates().front();

        hash_combine (aSeed, aFrontCoordinate.X());
        hash_combine (aSeed, aFrontCoordinate.Y());

        auto aBackCoordinate = theValue->Coordinates().back();
        hash_combine (aSeed, aBackCoordinate.X());
        hash_combine (aSeed, aBackCoordinate.Y());
    }

    hash_combine (aSeed, aCoordinates.size());

    const auto& aBndBox = theValue->BoundingBox();

    hash_combine (aSeed, aBndBox.MaxCorner().X());
    hash_combine (aSeed, aBndBox.MaxCorner().Y());
    hash_combine (aSeed, aBndBox.MinCorner().X());
    hash_combine (aSeed, aBndBox.MinCorner().Y());

    return aSeed;
}
