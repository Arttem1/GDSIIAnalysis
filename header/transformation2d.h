#ifndef TRANSFORMATION2D_H
#define TRANSFORMATION2D_H

#include "point2d.h"
#include "matrix2d.h"

class Transformation2D
{
public:
    typedef double          AngleType;
    typedef double          MagType;
    typedef Point2D<double> Point;

    enum TransformationType : unsigned short
    {
        Default       = 0x0,
        ReflectionX   = 0x8000,
        AbsoluteMag   = 0x4,
        AbsoluteAngle = 0x2
    };

    Transformation2D();
	Transformation2D(const Transformation2D& theTrsf);

    void Clear();

    TransformationType Type() const;
    void SetType (TransformationType theType);

    matrix2d RotationMatrix() const;
    AngleType Angle() const { return myAngle; }
    void SetAngle (const AngleType& theAngle);

    MagType Mag() const { return myMag; }
    void SetMag (const MagType& theMag);

    Point& Position() { return myPos; }
    const Point& Position() const { return myPos; }
    void SetPosition (const Point& thePosition);

    bool PosIsCustom() const { return myPosIsCustom; }

    void Multiply (const Transformation2D& theTrsf);

    Transformation2D Multiplied (const Transformation2D& theTrsf);

    Transformation2D operator* (const Transformation2D& theTrsf) {
        return Multiplied (theTrsf);
    }

    void operator *= (const Transformation2D& theTrsf) {
        Multiply (theTrsf);
    }

    void Invert();
    Transformation2D Inverted() const;

private:
    MagType   myMag;
    AngleType myAngle;
    Point     myPos;
    bool      myPosIsCustom;
    TransformationType myType;
};

class Transformation2DHash
{
public:
    size_t operator() (const Transformation2D& theValue) const;
};

#endif // TRANSFORMATION2D_H
