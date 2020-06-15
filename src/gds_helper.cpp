#include "gds_helper.h"
#include "point2d.h"
#include "vec2d.h"
#include "gds_path.h"
#include "line2d.h"
#include "gds_reference.h"
#include "gds_arrayReference.h"

#include <iostream>
#include <algorithm>

namespace {
template<typename T>
double Distance(const Point2D<T>& thePoint1, const Point2D<T>& thePoint2)
{
    auto l1 = (thePoint1.X() - thePoint2.X());
    auto l2 = (thePoint1.Y() - thePoint2.Y());
    return std::pow (l1 * l1 + l2 * l2, 1.0 / 2.0);
}

template<typename T>
void PrintPoint (const Point2D<T>& thePoint)
{
    std::cout << thePoint.X() << " " << thePoint.Y() << std::endl;
}

} // end of unnamed namespace


Transformation2D GDS_Helper::CombinedTransformation (const AncetorsElements& theAncestors)
{
    Transformation2D aTrsf;
    for (const auto& anAncestor : theAncestors) {
        if (auto aReference = std::dynamic_pointer_cast<GDS_Reference> (anAncestor)) {
            aTrsf *= aReference->Transformation();
        }
        if (auto aReference = std::dynamic_pointer_cast<GDS_ArrayReference> (anAncestor)) {
            aTrsf *= aReference->Transformation();
        }
    }
    return aTrsf;
}

GDS_Element::CoordinatesType GDS_Helper::ToPolygon (GDS_Path& thePath)
{
    const auto& aCoodinates = thePath.Coordinates();

    GDS_Element::CoordinatesType aNewCoordinates1;
    GDS_Element::CoordinatesType aNewCoordinates2;

    auto aHalfWidth = thePath.Width() / 2.;

    for (size_t i = 0; i+1 < aCoodinates.size(); i++) {
        const auto& aFirstPoint  = aCoodinates[i];
        const auto& aSecondPoint = aCoodinates[i + 1];

        vec2d<double> aVector (aFirstPoint, aSecondPoint);
        auto aNormal1 = aVector.GetNormal();
        aNormal1.Normalize();

        auto aNormal2 = aNormal1;

        aNormal1.Multiply (aHalfWidth);
        aNormal2.Multiply (-aHalfWidth);

        auto aFirstPoint1 = aFirstPoint;
        auto aFirstPoint2 = aFirstPoint;

        aFirstPoint1.Translate (aNormal1);
        aFirstPoint2.Translate (aNormal2);

        auto aSecondPoint1 = aSecondPoint;
        auto aSecondPoint2 = aSecondPoint;

        aSecondPoint1.Translate(aNormal1);
        aSecondPoint2.Translate(aNormal2);

        if (thePath.Type() != GDS_Path::Flush) {
            if (i == 0) {
                auto aNormilizedVector = aVector;
                aNormilizedVector.Normalize();
                aNormilizedVector.Multiply(-aHalfWidth);
                aFirstPoint1.Translate(aNormilizedVector);
                aFirstPoint2.Translate(aNormilizedVector);
            }
            else if (i == (aCoodinates.size() - 2))
            {
                auto aNormilizedVector = aVector;
                aNormilizedVector.Normalize();
                aNormilizedVector.Multiply(aHalfWidth);
                aSecondPoint1.Translate(aNormilizedVector);
                aSecondPoint2.Translate(aNormilizedVector);
            }
        }

        if (i + 2 < aCoodinates.size()) {
            const auto& aThirdPoint = aCoodinates[i + 2];

            auto aFirstDistance  = Distance (aFirstPoint1, aThirdPoint);
            auto aSecondDistance = Distance (aFirstPoint2, aThirdPoint);

            if (std::fabs (aFirstDistance - aSecondDistance) > 0.001) {
                auto A = Distance(aFirstPoint, aSecondPoint);
                auto B = Distance(aSecondPoint, aThirdPoint);
                auto C = Distance(aFirstPoint, aThirdPoint);

                auto angle = std::acos ((A * A + B * B - C * C) / (2.0 * A * B));

                auto F = aHalfWidth / std::tan (angle/2.0) ;

                auto aNormilizedVector1 = aVector;
                aNormilizedVector1.Normalize();

                auto aNormilizedVector2 = aNormilizedVector1;
                if (Distance(aFirstPoint1, aThirdPoint) > Distance(aFirstPoint2, aThirdPoint)) {
                    aNormilizedVector1.Multiply(F);
                    aNormilizedVector2.Multiply(-F);
                }
                else {
                    aNormilizedVector1.Multiply(-F);
                    aNormilizedVector2.Multiply(F);
                }

                aSecondPoint1.Translate(aNormilizedVector1);
                aSecondPoint2.Translate(aNormilizedVector2);
            }
        }

        if (i == 0) {
            //aNewCoordinates.push_back (aFirstPoint);
            aNewCoordinates1.push_back (aFirstPoint1);
            aNewCoordinates2.push_back (aFirstPoint2);
        }

        //aNewCoordinates.push_back (aSecondPoint);
        aNewCoordinates1.push_back (aSecondPoint1);
        aNewCoordinates2.push_back (aSecondPoint2);

    }
//	for (size_t i = 0; i < aNewCoordinates1.size(); i++) {
//		PrintPoint(aNewCoordinates1[i]);
//	}
//	for (size_t i = 0; i < aNewCoordinates1.size(); i++) {
//		PrintPoint(aNewCoordinates2[i]);
//	}
//	std::cout << "             " << std::endl;
    for (int i = aNewCoordinates2.size() - 1; i >= 0; i--) {
        aNewCoordinates1.push_back(aNewCoordinates2[i]);
    }

  //  for (size_t i = 0; i < thePath.Coordinates().size(); i++) {
  //      PrintPoint (thePath.Coordinates()[i]);
  // }

    //thePath.SetCoordinates (aNewCoordinates1);
    return aNewCoordinates1;
}

bool PointOnLine(const line2d<double>& TheLine, const Point2D<double>& ThePoint)
{
    double x = ((ThePoint.X() - TheLine.FirstPointX()) / (TheLine.SecondPointX() - TheLine.FirstPointX()));
    double y = ((ThePoint.Y() - TheLine.FirstPointY()) / (TheLine.SecondPointY() - TheLine.FirstPointY()));
    //(x-x1)/(x2-x1)=(y-y1)/(y2-y1)

    if ((std::fabs(x - y) < 0.001) &&
        std::min(TheLine.FirstPointX(), TheLine.SecondPointX()) <= ThePoint.X() &&
        std::max(TheLine.FirstPointX(), TheLine.SecondPointX()) >= ThePoint.X()) {

        return true;
    }

    return false;
}



bool IntersectionLines(const line2d<double>& TheLine1, const line2d<double>& TheLine2)
{

    line2d<double> aLine1(TheLine1);
    line2d<double> aLine2(TheLine2);

    if (TheLine1.SecondPointX() < TheLine1.FirstPointX()) {
        Point2D<double> tmp(TheLine1.FirstPoint());
        aLine1.SetFirstPoint(TheLine1.SecondPoint());
        aLine1.SetSecondPoint(tmp);
    }

    if (TheLine2.SecondPointX() < TheLine2.FirstPointX()) {
        Point2D<double> tmp(TheLine2.FirstPoint());
        aLine2.SetFirstPoint(TheLine2.SecondPoint());
        aLine2.SetSecondPoint(tmp);
    }


    if (aLine1.SecondPointX() < aLine2.FirstPointX()) {
        return false;
    }

    //если оба отрезка вертикальные
    if ((aLine1.FirstPointX() - aLine1.SecondPointX() == 0) && (aLine2.FirstPointX() - aLine1.SecondPointX() == 0)) {

        //если они лежат на одном X
        if (aLine1.FirstPointX() == aLine2.FirstPointX()) {

            //проверим пересекаются ли они, т.е. есть ли у них общий Y
            //для этого возьмём отрицание от случая, когда они НЕ пересекаются
            if (!((std::max(aLine1.FirstPointY(), aLine1.SecondPointY()) < std::min(aLine2.FirstPointY(), aLine2.SecondPointY())) ||
                (std::min(aLine1.FirstPointY(), aLine1.SecondPointY()) > std::max(aLine2.FirstPointY(), aLine2.SecondPointY())))) {

                return true;
            }
        }

        return false;
    }
    /*
        //если оба отрезка горизонтальные
        if ((aLine1.FirstPointX() - aLine1.SecondPointX() == 0) && (aLine2.FirstPointX() - aLine1.SecondPointX() == 0)) {

            //если они лежат на одном Y
            if (aLine1.FirstPointX() == aLine2.FirstPointX()) {

                //проверим пересекаются ли они, т.е. есть ли у них общий X
                //для этого возьмём отрицание от случая, когда они НЕ пересекаются
                if (!((std::max(aLine1.FirstPointX(), aLine1.SecondPointX()) < std::min(aLine2.FirstPointX(), aLine2.SecondPointX())) ||
                    (std::min(aLine1.FirstPointX(), aLine1.SecondPointX()) > std::max(aLine2.FirstPointX(), aLine2.SecondPointX())))) {

                    return true;
                }
            }

            return false;
        }
    */
    //если первый отрезок вертикальный
    if (aLine1.FirstPointX() - aLine1.SecondPointX() == 0) {

        //найдём Xa, Ya - точки пересечения двух прямых
        double Xa = aLine1.FirstPointX();
        double k2 = (aLine2.FirstPointY() - aLine2.SecondPointY()) / (aLine2.FirstPointX() - aLine2.SecondPointX());
        double b2 = aLine2.FirstPointY() - k2 * aLine2.FirstPointX();
        double Ya = k2 * Xa + b2;

        if (aLine2.FirstPointX() <= Xa && aLine2.SecondPointX() >= Xa && std::min(aLine1.FirstPointY(), aLine1.SecondPointY()) <= Ya &&
            std::max(aLine1.FirstPointY(), aLine1.SecondPointY()) >= Ya) {

            return true;
        }

        return false;
    }

    //если второй отрезок вертикальный
    if (aLine2.FirstPointX() - aLine2.SecondPointX() == 0) {

        //найдём Xa, Ya - точки пересечения двух прямых
        double Xa = aLine2.FirstPointX();
        double k1 = (aLine1.FirstPointY() - aLine1.SecondPointY()) / ((aLine1.FirstPointX() - aLine1.SecondPointX()));
        double b1 = aLine1.FirstPointY() - k1 * aLine1.FirstPointX();
        double Ya = k1 * Xa + b1;

        if (aLine1.FirstPointX() <= Xa && aLine1.SecondPointX() >= Xa && std::min(aLine2.FirstPointY(), aLine2.SecondPointY()) <= Ya &&
            std::max(aLine2.FirstPointY(), aLine2.SecondPointY()) >= Ya) {

            return true;
        }

        return false;
    }

    //оба отрезка невертикальные
    double k1 = (aLine1.FirstPointY() - aLine1.SecondPointY()) / (aLine1.FirstPointX() - aLine1.SecondPointX());
    double k2 = (aLine2.FirstPointY() - aLine2.SecondPointY()) / (aLine2.FirstPointX() - aLine2.SecondPointX());
    double b1 = aLine1.FirstPointY() - k1 * aLine1.FirstPointX();
    double b2 = aLine2.FirstPointY() - k2 * aLine2.FirstPointX();

    //отрезки параллельны
    if (k1 == k2) {
        if (PointOnLine(aLine1, aLine2.FirstPoint()) || PointOnLine(aLine1, aLine2.SecondPoint()) ||
            PointOnLine(aLine2, aLine1.FirstPoint()) || PointOnLine(aLine2, aLine1.SecondPoint()))
            return true;
        else
            return false;
    }

    //Xa - абсцисса точки пересечения двух прямых
    double Xa = (b2 - b1) / (k1 - k2);

    if ((Xa < std::max(aLine1.FirstPointX(), aLine2.FirstPointX())) || (Xa > std::min(aLine1.SecondPointX(), aLine2.SecondPointX()))) {
        return false; //точка Xa находится вне пересечения проекций отрезков на ось X
    }
    else {
        return true;
    }

}

inline size_t control(const size_t& index, const size_t& size)
{
    if (index == size) {
        return 0;
    }
    return index;
}


//template<typename T>
bool GDS_Helper::IntersectionPolygon(const polygon2d& ThePolygon1,
                                     const polygon2d& ThePolygon2)
{
    if (PolygonInPolygon(ThePolygon1, ThePolygon2)){
        return true;
    }
    polygon2d::Point max1ByX(ThePolygon1.Coordinates()[0]);
    polygon2d::Point min1ByX(ThePolygon1.Coordinates()[0]);
    polygon2d::Point max1ByY(ThePolygon1.Coordinates()[0]);
    polygon2d::Point min1ByY(ThePolygon1.Coordinates()[0]);
    polygon2d::Point max2ByX(ThePolygon2.Coordinates()[0]);
    polygon2d::Point min2ByX(ThePolygon2.Coordinates()[0]);
    polygon2d::Point max2ByY(ThePolygon2.Coordinates()[0]);
    polygon2d::Point min2ByY(ThePolygon2.Coordinates()[0]);

    auto Size1 = ThePolygon1.Coordinates().size();
    auto Size2 = ThePolygon2.Coordinates().size();
    /*
    double max1ByX = ThePolygon1.Coordinates[0].X();
    double min1ByX = ThePolygon1.Coordinates[0].X();
    double max1ByY = ThePolygon1.Coordinates[0].Y();
    double min1ByY = ThePolygon1.Coordinates[0].Y();
    double max2ByX = ThePolygon2.Coordinates[0].X();
    double min2ByX = ThePolygon2.Coordinates[0].X();
    double max2ByY = ThePolygon2.Coordinates[0].Y();
    double min2ByY = ThePolygon2.Coordinates[0].Y();
    */

    for (size_t i = 1; i < ThePolygon1.Coordinates().size(); i++)
    {
        if (max1ByX.X() < ThePolygon1.Coordinates()[i].X()) {
            max1ByX.X() = ThePolygon1.Coordinates()[i].X();
            //IndexMax1ByX = i;
        }

        if (min1ByX.X() > ThePolygon1.Coordinates()[i].X()) {
            min1ByX.X() = ThePolygon1.Coordinates()[i].X();
            //IndexMin1ByX = i;
        }

        if (max1ByY.Y() < ThePolygon1.Coordinates()[i].Y())
            max1ByY.Y() = ThePolygon1.Coordinates()[i].Y();

        if (min1ByY.Y() > ThePolygon1.Coordinates()[i].Y())
            min1ByY.Y() = ThePolygon1.Coordinates()[i].Y();
    }

    for (size_t i = 1; i < ThePolygon2.Coordinates().size(); i++)
    {
        if (max2ByX.X() < ThePolygon2.Coordinates()[i].X()) {
            max2ByX.X() = ThePolygon2.Coordinates()[i].X();
            //IndexMax2ByX = i;
        }

        if (min2ByX.X() > ThePolygon2.Coordinates()[i].X()) {
            min2ByX.X() = ThePolygon2.Coordinates()[i].X();
            //IndexMin2ByX = i;
        }


        if (max2ByY.Y() < ThePolygon2.Coordinates()[i].Y())
            max2ByY.Y() = ThePolygon2.Coordinates()[i].Y();

        if (min2ByY.Y() > ThePolygon2.Coordinates()[i].Y())
            min2ByY.Y() = ThePolygon2.Coordinates()[i].Y();
    }

    enum theBiggest {
        firstPolygon =  1,
        secondPolygon = 2
    };

    theBiggest theBiggest;

    if (max1ByX.X() >= max2ByX.X()) {
        theBiggest = firstPolygon;
        if (min1ByX.X() > max2ByX.X()){
         return false;
        }
    }
    else {
        theBiggest = secondPolygon;
        if (min2ByX.X() > max1ByX.X()) {
            return false;
        }

    }

    if (max1ByY.Y() >= max2ByY.Y()) {
        if (min1ByY.Y() > max2ByY.Y()) {
            return false;
        }
    }
    else {
        if (min2ByY.Y() > max1ByY.Y()) {
            return false;
        }

    }


    std::vector<line2d<double>> AllLines;
    size_t QuantityLinesFirstPolygon = 0;
    size_t QuantityLinesSecondPolygon = 0;

    if (theBiggest == firstPolygon) {

        for (size_t i = 0; i < ThePolygon1.Coordinates().size(); i++) {

            if ((ThePolygon1.Coordinates()[i].X() <= max2ByX.X()) ||
                (ThePolygon1.Coordinates()[control(i + 1, Size1)].X() <= max2ByX.X())) {

                line2d<double> line(ThePolygon1.Coordinates()[i], ThePolygon1.Coordinates()[control(i + 1, Size1)]);
                AllLines.push_back(line);
                ++QuantityLinesFirstPolygon;
            }

        }
        for (size_t i = 0; i < ThePolygon2.Coordinates().size(); i++) {

            if ((ThePolygon2.Coordinates()[i].X() >= min1ByX.X()) ||
                (ThePolygon2.Coordinates()[control(i + 1, Size2)].X() >= min1ByX.X())) {

                line2d<double> line(ThePolygon2.Coordinates()[i], ThePolygon2.Coordinates()[control(i + 1, Size2)]);
                AllLines.push_back(line);
                ++QuantityLinesSecondPolygon;
            }
        }
    }
    else {

        for (size_t i = 0; i < ThePolygon2.Coordinates().size(); i++) {

            if ((ThePolygon2.Coordinates()[i].X() <= max1ByX.X()) ||
                (ThePolygon2.Coordinates()[control(i + 1, Size2)].X() <= max1ByX.X())) {

                line2d<double> line(ThePolygon2.Coordinates()[i], ThePolygon2.Coordinates()[control(i + 1, Size2)]);
                AllLines.push_back(line);
                ++QuantityLinesSecondPolygon;
            }
        }

        for (size_t i = 0; i < ThePolygon1.Coordinates().size(); i++) {

            if ((ThePolygon1.Coordinates()[i].X() >= min2ByX.X()) ||
                (ThePolygon1.Coordinates()[control(i + 1, Size1)].X() >= min2ByX.X())) {

                line2d<double> line(ThePolygon1.Coordinates()[i], ThePolygon1.Coordinates()[control(i + 1, Size1)]);
                AllLines.push_back(line);
                ++QuantityLinesFirstPolygon;
            }
        }
    }
    for (size_t i = 0; i < AllLines.size(); i++)
    {
        /*
        std::cout << AllLines[i].FirstPointX() << "       ";
        std::cout << AllLines[i].FirstPointY() << std::endl;
        std::cout << AllLines[i].SecondPointX() << "       ";
        std::cout << AllLines[i].SecondPointY() << std::endl;*/
    }

    if (theBiggest == firstPolygon) {
        for (size_t i = 0; i < QuantityLinesFirstPolygon; i++) {
            for (size_t j = QuantityLinesFirstPolygon; j < QuantityLinesSecondPolygon + QuantityLinesFirstPolygon; j++) {
                if (IntersectionLines(AllLines[i], AllLines[j]))
                    return true;
            }
        }
    } else {
        for (size_t i = 0; i < QuantityLinesSecondPolygon; i++) {
            for (size_t j = QuantityLinesSecondPolygon; j < QuantityLinesSecondPolygon + QuantityLinesFirstPolygon; j++) {
                if (IntersectionLines(AllLines[i], AllLines[j]))
                    return true;
            }
        }
    }

    return false;
}

bool GDS_Helper::PointInPolygon(const Point2D<double>& ThePoint, const polygon2d& ThePolygon)
{
    size_t numbercrosslines = 0;
    for (size_t i = 0; i < ThePolygon.Coordinates().size() - 1; i++) {
        if (((ThePolygon.Coordinates()[i].Y() <= ThePoint.Y()) && (ThePolygon.Coordinates()[i + 1].Y() >= ThePoint.Y()) ||
            (ThePolygon.Coordinates()[i].Y() >= ThePoint.Y()) && (ThePolygon.Coordinates()[i + 1].Y() <= ThePoint.Y())) &&
            (ThePolygon.Coordinates()[i].X() <= ThePoint.X()) && (ThePolygon.Coordinates()[i + 1].X() <= ThePoint.X())) {
            ++numbercrosslines;
        }
    }
    if ((numbercrosslines % 2) == 0)
        return false;
    return true;
}

bool GDS_Helper::PolygonInPolygon(const polygon2d& ThePolygon1, const polygon2d& ThePolygon2)
{
    if (PointInPolygon(ThePolygon1.Coordinates()[0], ThePolygon2)) {
        return true;
    }
    if (PointInPolygon(ThePolygon2.Coordinates()[0], ThePolygon1)) {
        return true;
    }
    return false;
}
