#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>

class matrix2d;
class Transformation2D;

template<typename T>
class Point2D
{
public:
    Point2D()
        : myX (0), myY (0)
    {

    }

    Point2D (const T& theX, const T& theY)
        : myX (theX), myY (theY)
    {

    }

	Point2D (const Point2D<T>& thepoint2d)
		: myX(thepoint2d.X()), myY(thepoint2d.Y())
	{

	}

    bool operator==(const Point2D<T>& thepoint2d) const
    {
        return std::fabs (myX - thepoint2d.X()) < 0.00001 && std::fabs(myY - thepoint2d.Y()) < 0.00001;
    }

	Point2D<T>& operator=(const Point2D<T>& thepoint2d)
	{
		myX = thepoint2d.X();
		myY = thepoint2d.Y();
		return *this;
	}

    bool operator< (const Point2D<T>& thepoint2d) const
    {
        return myX + myY < thepoint2d.X() + thepoint2d.Y();
    }

    inline T Modulus() const
    {
      return std::pow (myX * myX + myY * myY, 1.0/2.0);
    }

    inline void Add (const Point2D<T>& thePoint)
    {
        myX += thePoint.X();
        myY += thePoint.Y();
    }

    Point2D Subtracted (const Point2D<T>& thePoint)
    {
        auto aCopy = *this;
        aCopy.Subtract (thePoint);
        return aCopy;
    }

    inline void Subtract (const Point2D<T>& thePoint)
    {
        myX -= thePoint.X();
        myY -= thePoint.Y();
    }

    inline void Divide (const T& theScalar)
    {
        myX /= theScalar;
        myY /= theScalar;
    }

    Point2D Multiplied (const T& theScalar)
    {
        Point2D aCopy (*this);
        aCopy.Multiply (theScalar);
        return aCopy;
    }

    inline void Multiply (const T& theScalar)
    {
        myX *= theScalar;
        myY *= theScalar;
    }

    void Multiply (const matrix2d& theMatrix);

    template<typename Vec>
    void Translate (const Vec& theVector)
    {
        Add (theVector.Coord());
    }

    void Transform (const Transformation2D& theTrsf);

    Point2D Transformed (const Transformation2D& theTrsf);

    const T& X() const { return myX; }
    const T& Y() const { return myY; }

    T& X() { return myX; }
    T& Y() { return myY; }

private:
    T myX;
    T myY;
};

#endif // POINT2D_H
