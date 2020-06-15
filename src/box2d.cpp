#include "box2d.h"
#include <algorithm>

Box2d::Box2d()
{

}

Box2d::Box2d (const Box2d& theBox)
{
    Copy (theBox);
}

Box2d::Box2d(const Point2D<double>& theMin, const Point2D<double>& theMax)
	:myMin(theMin), myMax(theMax)
{
	
}

void Box2d::Copy (const Box2d& theBox)
{
    myMin = theBox.myMin;
    myMax = theBox.myMax;
}

void Box2d::Add (const Box2d& theBox)
{
    if (theBox.IsEmpty()) {
        return;
    }

    if (IsEmpty()) {
        return Copy (theBox);
    }

    myMin.X() = std::min (myMin.X(), theBox.myMin.X());
    myMin.Y() = std::min (myMin.Y(), theBox.myMin.Y());
    myMax.X() = std::max (myMax.X(), theBox.myMax.X());
    myMax.Y() = std::max (myMax.Y(), theBox.myMax.Y());
}

bool Box2d::IsEmpty() const
{
    return myMin == myMax && myMin == Point2D<double> (0, 0);
}

Box2d Box2d::Transformed(const Transformation2D& theTrsf) const
{
    if (IsEmpty()) {
        return *this;
    }

    Box2d aCopy (*this);
    aCopy.Transform(theTrsf);
    return aCopy;
}

void Box2d::Transform (const Transformation2D& theTrsf)
{
    if (IsEmpty()) {
        return;
    }

    myMin.Transform (theTrsf);
    myMax.Transform (theTrsf);
}

bool Box2d::IsOut (const Box2d& theAnotherBox)
{
    if (IsEmpty() || theAnotherBox.IsEmpty()) {
        return false;
    }

	if (myMin.X() > theAnotherBox.myMax.X()) {
		return true;
	}
	if (theAnotherBox.myMin.X() > myMax.X()) {
		return true;
	}
	if (myMin.Y() > theAnotherBox.myMax.Y()) {
		return true;
	}
	if (theAnotherBox.myMin.Y() > myMax.Y()) {
		return true;
	}
	return false;
}

const Point2D<double>& Box2d::MinCorner() const
{
    return myMin;
}

const Point2D<double>& Box2d::MaxCorner() const
{
    return myMax;
}

bool Box2d::operator== (const Box2d& theBox) const
{
    return myMax == theBox.MaxCorner()
        && myMin == theBox.MinCorner();
}
