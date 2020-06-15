#include "matrix2d.h"
#include <cassert>
#include <cmath>

matrix2d::matrix2d()
{

}

double matrix2d::Value (int theRow, int theColumn) const
{
    assert (theRow >= 1 && theRow <= 2);
    assert (theColumn >= 1 && theColumn <= 2);

    return myElements[(theRow - 1) * 2 + (theColumn - 1)];

    //0                        1
    //theRow = 1 theColumn = 1   theRow = 1 theColumn = 2

    //2                        3
    //theRow = 2 theColumn = 1   theRow = 2 theColumn = 2

}

void matrix2d::SetValue (int theRow, int theColumn, const double& theValue) 
{
    assert (theRow >= 1 && theRow <= 2);
    assert (theColumn >= 1 && theColumn <= 2);
	myElements[(theRow - 1) * 2 + (theColumn - 1)] = theValue;
}

void matrix2d::SetRotation (const double& theAngle)
{
    auto aSinA = sin(theAngle);
    auto aCosA = cos(theAngle);
    SetValue (1, 1, aCosA);
    SetValue (2, 2, aCosA);
    SetValue (1, 2, -aSinA);
    SetValue (2, 1, aSinA);
}

void matrix2d::Multiply (const matrix2d& theMatrix)
{
    double T11 = Value (1, 1) * theMatrix.Value (1, 1) + Value (1, 2) * theMatrix.Value (2, 1);
    double T21 = Value (2, 1) * theMatrix.Value (1, 1) + Value (2, 2) * theMatrix.Value (2, 1);
    double T12 = Value (1, 1) * theMatrix.Value (1, 2) + Value (1, 2) * theMatrix.Value (2, 2);
    double T22 = Value (2, 1) * theMatrix.Value (1, 2) + Value (2, 2) * theMatrix.Value (2, 2);

    SetValue (1, 1, T11);
    SetValue (2, 1, T21);
    SetValue (1, 2, T12);
    SetValue (2, 2, T22);
}


void matrix2d::Invert()
{
    double det;

    double T11 = Value (2, 2);
    double T12 = -Value (1, 2);
    double T21 = -Value (2, 1);
    double T22 = Value (1, 1);

    det = T11 * T22 - T12 * T21;

    assert (std::fabs (det) > 0.001);

    det = 1.0 / det;

    SetValue (1, 1, T11 * det);
    SetValue (2, 1, T21 * det);
    SetValue (1, 2, T12 * det);
    SetValue (2, 2, T22 * det);
}

void matrix2d::Transpose()
{
  double aTemp = Value (1, 2);
  SetValue (1, 2, Value (2, 1));
  SetValue (2, 1, aTemp);
}

matrix2d matrix2d::Inverted (const matrix2d& theMatrix) const
{
    auto aCopy = theMatrix;
    aCopy.Multiply (theMatrix);
    return aCopy;
}

