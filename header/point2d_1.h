#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>


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

    inline T Modulus() const
    {
      return std::pow (myX * myX + myY * myY, 1.0/2.0);
    }

    inline void Add (const Point2D<T>& thePoint)
    {
        myX += thePoint.X();
        myY += thePoint.Y();
    }

    inline void Divide (const T& theScalar)
    {
        myX /= theScalar;
        myY /= theScalar;
    }

    inline void Multiply (const T& theScalar)
    {
        myX *= theScalar;
        myY *= theScalar;
    }

    inline void Translate (const vec2d<T>& theVector);

    const T& X() const { return myX; }
    const T& Y() const { return myY; }

    T& X() { return myX; }
    T& Y() { return myY; }

private:
    T myX;
    T myY;
};

#endif // POINT2D_H
