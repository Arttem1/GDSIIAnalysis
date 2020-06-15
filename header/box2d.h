#ifndef BOX2D_H
#define BOX2D_H

#include "point2d.h"

class Box2d
{
public:
    Box2d();

    Box2d (const Box2d& theBox);

	Box2d(const Point2D<double>& theMin, const Point2D<double>& theMax);

    void Add (const Box2d&);

    bool IsOut (const Box2d& theAnotherBox);

    bool IsEmpty() const;

    Box2d Transformed(const Transformation2D& theTrsf) const;

    void Transform (const Transformation2D& theTrsf);

    const Point2D<double>& MinCorner() const;

    const Point2D<double>& MaxCorner() const;

    bool operator== (const Box2d& theBox) const;

private:
    void Copy (const Box2d& theBox);

private:
	Point2D<double> myMin;
	Point2D<double> myMax;
};

#endif // BOX2D_H
