#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <array>
#include "point2d.h"

class matrix2d
{
public:
    matrix2d();

    double Value (int theRow, int theColumn) const;
    void SetValue (int theRow, int theColumn, const double& theValue);

    void SetRotation (const double& theAngle);

    void Multiply (const matrix2d& theMatrix);

    void Invert();
    matrix2d Inverted (const matrix2d& theMatrix) const;

    void Transpose();

private:
    std::array<double, 4> myElements;
};

#endif // MATRIX2D_H
