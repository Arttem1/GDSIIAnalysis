#include "transformation2d.h"
#include "typedefs.h"

#define _USE_MATH_DEFINES
#include <cassert>
#include <math.h>

Transformation2D::Transformation2D()
    : myMag (1), myAngle (0), myPosIsCustom (false), myType (Default)
{

}

Transformation2D::Transformation2D(const Transformation2D& theTrsf)
	: myMag (theTrsf.myMag), myAngle(theTrsf.myAngle), myPosIsCustom(theTrsf.myPosIsCustom), 
	  myType(theTrsf.myType), myPos(theTrsf.myPos)
{
	
}

void Transformation2D::Clear()
{
    myAngle = 0;
    myMag = 1;
    myPos = Point();
    myPosIsCustom = false;
	myType = Default;
}

Transformation2D::TransformationType Transformation2D::Type() const
{
    return myType;
}

void Transformation2D::SetType (TransformationType theType)
{
    assert(!(theType & AbsoluteMag) && !(theType & AbsoluteAngle));
    myType = theType;
}

namespace {
double ToRadians (const double& theDegree) {
    return theDegree * M_PI / 180;
}
}

matrix2d Transformation2D::RotationMatrix() const
{
    matrix2d aMatrix;
    if (myType & ReflectionX) {
        aMatrix.SetRotation (ToRadians (360 - myAngle));
    } else {
        aMatrix.SetRotation (ToRadians (myAngle));
    }
    return aMatrix;
}

void Transformation2D::SetAngle (const AngleType& theAngle)
{
    myAngle = theAngle;
}

void Transformation2D::SetMag (const MagType& theMag)
{
    myMag = theMag;
}

void Transformation2D::SetPosition (const Point& thePosition)
{
    myPos = thePosition;
    myPosIsCustom = true;
}

Transformation2D Transformation2D::Multiplied (const Transformation2D& theTrsf)
{
    auto aCopy = *this;
    aCopy.Multiply (theTrsf);
    return aCopy;
}

void Transformation2D::Multiply (const Transformation2D& theTrsf)
{
	myPos.Add(theTrsf.Position());
	myMag *= theTrsf.Mag();
	myAngle += theTrsf.Angle();

    auto aThisHasReflection = myType & ReflectionX;
    auto anOtherHasReflection = theTrsf.myType & ReflectionX;

    if (anOtherHasReflection) {
        if (aThisHasReflection) {
            myType = static_cast<TransformationType> (myType & ~ReflectionX);
        } else {
            myType = static_cast<TransformationType> (myType | ReflectionX);
        }
    }
}

void Transformation2D::Invert()
{
    myMag = 1.0 / myMag;
    myAngle = -myAngle;
    myPos.Multiply(-1);
}

Transformation2D Transformation2D::Inverted() const
{
    Transformation2D aCopy (*this);
    aCopy.Invert();
    return aCopy;
}

size_t Transformation2DHash::operator() (const Transformation2D& theValue) const
{
    size_t aSeed = 0;
    hash_combine (aSeed, theValue.Position().X());
    hash_combine (aSeed, theValue.Position().Y());
    hash_combine (aSeed, theValue.Mag());
    hash_combine (aSeed, theValue.Angle());
    hash_combine (aSeed, static_cast<int> (theValue.Type()));

    return aSeed;
}

