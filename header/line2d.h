#ifndef LINE2D_H
#define LINE2D_H

#include "point2d.h"

template<typename T>
class line2d
{
public:
        line2d() { }

	line2d(const Point2D<T>& theFirstPoint, const Point2D<T>& theSecondPoint) {
		myCoordFirstPoint.X() = theFirstPoint.X();
		myCoordFirstPoint.Y() = theFirstPoint.Y();
		myCoordSecondPoint.X() = theSecondPoint.X();
		myCoordSecondPoint.Y() = theSecondPoint.Y();
	}

	line2d(const line2d<T>& TheLine) {
		myCoordFirstPoint.X() = TheLine.FirstPointX();
		myCoordFirstPoint.Y() = TheLine.FirstPointY();
		myCoordSecondPoint.X() = TheLine.SecondPointX();
		myCoordSecondPoint.Y() = TheLine.SecondPointY();
	}

	const T FirstPointX() const{ return myCoordFirstPoint.X(); }
	const T FirstPointY() const{ return myCoordFirstPoint.Y(); }
	const T SecondPointX() const{ return myCoordSecondPoint.X(); }
	const T SecondPointY() const{ return myCoordSecondPoint.Y(); }

	void SetFirstPoint ( const Point2D<T>& ThePoint ) { myCoordFirstPoint = ThePoint; }

	void SetSecondPoint (const Point2D<T>& ThePoint) { myCoordSecondPoint = ThePoint; }



	const Point2D<T>  FirstPoint() const { return myCoordFirstPoint; }
	const Point2D<T>  SecondPoint() const { return myCoordSecondPoint; }

private:
	Point2D<T> myCoordFirstPoint;
	Point2D<T> myCoordSecondPoint;
};
#endif // LINE2D_H

