#ifndef PRINTHELPER_H
#define PRINTHELPER_H

#include "typedefs.h"
#include "point2d.h"

#include <iostream>

class GDS_StructureInfo;

class PrintHelper
{
public:
	PrintHelper(std::ostream& theStream);
	PrintHelper(const PrintHelper& theOther);
    void PrintStructureInfo (const GDS_StructureInfo& theInfo);

    std::string ElementTypeString (const GDS_ElementPtr& theElement);
    void PrintElementShortInformation (const GDS_GraphElementPtr& theElement);
    void PrintElementFullInfomation(const GDS_GraphElementPtr& theElement);


    void PrintAncestors (const AncetorsElements& theAncestors);
    void PrintConnections (const ConnectionsType& theConnections);

    void PrintGraph (const GDS_GraphPtr& theGraph);

    template<typename T>
    void PrintPoint(const Point2D<T>& thePoint)
    {
		myStream << "(" << thePoint.X() << ", " << thePoint.Y() << ")";
    }

	std::ostream& myStream;
};

#endif // PRINTHELPER_H
