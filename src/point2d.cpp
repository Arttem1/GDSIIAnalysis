#include "point2d.h"
#include "transformation2d.h"

template<>
void Point2D<double>::Transform (const Transformation2D& theTrsf)
{
    Multiply (theTrsf.Mag());
    Multiply (theTrsf.RotationMatrix());

    if (theTrsf.Type() & Transformation2D::ReflectionX) {
        myY = -myY + theTrsf.Position().Y();
    }

    Add (theTrsf.Position());
}

template<>
Point2D<double> Point2D<double>::Transformed (const Transformation2D& theTrsf)
{
    auto aCopy = *this;
    aCopy.Transform (theTrsf);
    return aCopy;
}

template<typename T>
void Point2D<T>::Multiply (const matrix2d& theMatrix)
{
    auto Xresult = theMatrix.Value (1, 1) * myX + theMatrix.Value (1, 2) * myY;
    myY          = theMatrix.Value (2, 1) * myX + theMatrix.Value (2, 2) * myY;
    myX          = Xresult;
}

template class Point2D<double>;
template class Point2D<int>;
