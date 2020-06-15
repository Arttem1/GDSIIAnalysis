#ifndef VEC2D_H
#define VEC2D_H

#include "point2d.h"

template<typename T>
class vec2d
{
public:
    vec2d(const T& theX, const T& theY) :
        myCoord (theX, theY)
    {

    }

    vec2d GetNormal()
    {
        return vec2d (myCoord.Y() , -myCoord.X());
    }

    vec2d (const Point2D<T>& theFirstPoint, const Point2D<T>& theSecondPoint)
    {
        myCoord.X() = theSecondPoint.X() - theFirstPoint.X();
        myCoord.Y() = theSecondPoint.Y() - theFirstPoint.Y();
    }

    T X() { return myCoord.X(); }
    T Y() { return myCoord.Y(); }


    void Normalize()
    {
      double D = myCoord.Modulus();
      myCoord.Divide (D);
    }

    inline void Multiply (const T& theScalar)
    {
        myCoord.Multiply (theScalar);
    }

    const Point2D<T>& Coord() const { return myCoord; }


private:
    Point2D<T> myCoord;
};

#endif // VEC2D_H
