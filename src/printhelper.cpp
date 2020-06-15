#include "printhelper.h"
#include "gds_structureinfo.h"
#include "gds_element.h"
#include "gds_graphelement.h"
#include "gds_structureinfo.h"
#include "gds_structure.h"
#include "gds_path.h"
#include "gds_bondary.h"
#include "gds_box.h"
#include "gds_arrayReference.h"
#include "gds_reference.h"
#include "Point2D.h"
#include "Transformation2D.h"
#include "printhelper.h"
#include "gds_graph.h"
#include "gds_helper.h"

#include <string>
#include <iostream>
#include <iterator>
#include <cassert>

PrintHelper::PrintHelper(std::ostream& theStream)
	: myStream (theStream)
{

}

PrintHelper::PrintHelper(const PrintHelper& theOther)
	: myStream (theOther.myStream)
{

}

void PrintHelper::PrintStructureInfo (const GDS_StructureInfo& theInfo)
{

    auto PrintLayers = [this] (LayersSet theList, int theSize) {
        int count = 0;
        for (const auto& aLayer : theList) {
            if (count == (theSize - 1)){
                myStream << aLayer;
                count++;
            }
            else{
                myStream << aLayer << ", ";
                count++;
            }
        }
    };

    myStream << "Active Layers: ";
    PrintLayers (theInfo.myActiveLayers, theInfo.myActiveLayers.size());
    myStream << std::endl;
    myStream << "Bondaries: " << theInfo.myBondaryCount << std::endl;
    myStream << "Boxes: " << theInfo.myBoxesCount << std::endl;
    myStream << "Paths: " << theInfo.myPathCount << std::endl;
    myStream << "References: " << theInfo.myReferencesCountByMe << std::endl;
}

std::string PrintHelper::ElementTypeString (const GDS_ElementPtr& theElement)
{
    if (std::dynamic_pointer_cast<GDS_Path> (theElement)) {
        return "PATH";
    } else if (std::dynamic_pointer_cast<GDS_Bondary> (theElement)) {
        return "BONDARY";
    } else if (std::dynamic_pointer_cast<GDS_Box> (theElement)) {
        return "BOX";
    }
    assert (false);
    return "";
}

void PrintHelper::PrintElementShortInformation (const GDS_GraphElementPtr& theElement)
{
    myStream << ElementTypeString (theElement->myElement) << " #" << theElement->myNumber;
}

void PrintHelper::PrintAncestors (const AncetorsElements& theAncestors)
{
    for (const auto& anAncestor : theAncestors) {

        if (auto aStructure = std::dynamic_pointer_cast<GDS_Structure> (anAncestor)) {

            myStream << "STRUCTURE " << aStructure->Name();
            myStream << std::endl;
        }

        else if (auto aReference = std::dynamic_pointer_cast<GDS_Reference> (anAncestor)) {
            myStream << "REFERENCE " << aReference->Reference()->Name();
            myStream << "[";
            myStream << "XY: "; PrintPoint(aReference->Transformation().Position()); myStream << ", ";
            myStream << "MAG: " << aReference->Transformation().Mag(); myStream << ", ";
            myStream << "ANGLE: " << aReference->Transformation().Angle(); myStream << ", ";
            myStream << "X-REFLECTION: ";
            if (aReference->Transformation().Type() & Transformation2D::ReflectionX){
                myStream << "true";
            }
            else {
                myStream << "false";
            }
            myStream << std::endl;
        }
        else if (auto aReference = std::dynamic_pointer_cast<GDS_ArrayReference> (anAncestor)) {
            myStream << "ARRAY_REFERENCE " << aReference->Reference()->Name() << ", ";
            myStream << std::endl;
        }
    }
}

void PrintHelper::PrintElementFullInfomation(const GDS_GraphElementPtr& theElement)
{
    auto anElement = theElement->myElement;
    myStream << ElementTypeString(anElement) << " Layer: " << anElement->Layer() << " #" << theElement->myNumber << " ";
    myStream << std::endl;
    myStream << "OLD COORDINATES:  ";
    myStream << "[";
    for (const auto& aPoint : anElement->Coordinates()) {
        PrintPoint(aPoint);
    }
    myStream << "]";
    myStream << std::endl;
    myStream << "NEW COORDINATES:  ";
    myStream << "[";

    auto aTrsf = GDS_Helper::CombinedTransformation (theElement->myAncestors);

    for (auto aPoint : anElement->Coordinates()) {
        aPoint.Transform(aTrsf);
        PrintPoint(aPoint);
    }
    myStream << "]";

    myStream << std::endl;
    myStream << "ANCESTORS:";
    myStream << std::endl;

    PrintAncestors (theElement->myAncestors);

    myStream << std::endl;
}

void PrintHelper::PrintConnections (const ConnectionsType& theConnections)
{
    myStream << "Connections number: " << theConnections.size() << std::endl;

    for (const auto& aConnection : theConnections) {
        const auto& aConnector         = aConnection.first;
        const auto& aConnectedElements = aConnection.second;

        myStream << "************************************************" << std::endl;

        PrintElementShortInformation (aConnector);

        if (aConnectedElements.empty()) {
            myStream << " doesn't connects anything." << std::endl;
            continue;
        }

        myStream << " connects:" << std::endl;

        for (const auto& anElement : aConnection.second) {

            PrintElementFullInfomation (anElement);//////////////////////////////////////////////////////////////////////
        }

        myStream << "************************************************" << std::endl;
        myStream << std::endl;
    }
}

void PrintHelper::PrintGraph (const GDS_GraphPtr& theGraph)
{
    const auto& aCoonnections = theGraph->Connections();

    PrintConnections (aCoonnections);
}
