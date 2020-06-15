#include "intersectionfounder.h"
#include "gds_helper.h"
#include "gds_graph.h"
#include "gds_graphfactory.h"
#include "gds_structure.h"
#include "gds_object.h"
#include "gds_path.h"

#include <algorithm>

IntersectionFounder::IntersectionFounder (const GDS_GraphPtr& theGraph,
                     const GDS_GraphElementEdgePtr& theConnector,
                     const LayersSet& theIsolatorLayers)
    : myIsolatorLayers (theIsolatorLayers),
      myGraph (theGraph),
      myConnector (theConnector)
{
    const auto& aConnectorElement = myConnector->myElement;

    auto aTrsf = GDS_Helper::CombinedTransformation (myConnector->myAncestors);

    myConnectorTransformedPolygon = aConnectorElement->Polygon().Transformed (aTrsf);
    myConnectorTransformedBox2d   = aConnectorElement->BoundingBox().Transformed (aTrsf);
}

bool IntersectionFounder::AncestorsIsSameToConnector()
{
    auto aSameSize = myConnector->myAncestors.size() == myAncestors.size();

    if (aSameSize) {
        return std::equal (myAncestors.begin(), myAncestors.end(), myConnector->myAncestors.begin());
    }

    return false;
}

bool IntersectionFounder::Visit (const GDS_StructurePtr& theStructure)
{
    Founder::Visit (theStructure);

    if (std::dynamic_pointer_cast<GDS_Path> (myConnector->myElement))
    {
        auto aStructureBndBox = theStructure->BoundingBox (myConnector->myElement->Layer());
        auto aStructureTransformedBndBox = aStructureBndBox.Transformed(myTrsf);

        if (myConnectorTransformedBox2d.IsOut(aStructureTransformedBndBox)) {
            return false; // no need to go deeper - there are no any intersections with structure area
        }
    }
    else {
        auto aStructureBndBox = theStructure->BoundingBox();
        auto aStructureTransformedBndBox = aStructureBndBox.Transformed(myTrsf);

        if (myConnectorTransformedBox2d.IsOut(aStructureTransformedBndBox)) {
            return false; // no need to go deeper - there are no any intersections with structure area
        }
    }

    auto aStructureForCreateGraph = myGraph->Structure();
    auto aGoDeeper = aStructureForCreateGraph == theStructure || !AncestorsIsSameToConnector();

    return aGoDeeper;
}

void IntersectionFounder::Visit (const GDS_ObjectPtr& theObject)
{
    auto anElement = theObject->Element();
    auto anElementLayer = anElement->Layer();

    if (IsIsolatorLayer (anElementLayer)) {
        return;
    }

    if (std::dynamic_pointer_cast<GDS_Path> (myConnector->myElement)) {
        if (myConnector->myElement->Layer() != anElement->Layer()) {
            return;
        }
    }

    if (anElement == myConnector->myElement && AncestorsIsSameToConnector()) {
        return;
    }

    const auto& anElementPolygon = anElement->Polygon().Transformed (myTrsf);

    auto aRes = GDS_Helper::IntersectionPolygon (myConnectorTransformedPolygon, anElementPolygon);

    if (aRes) {
        auto anGraphElement = GDS_GraphFactory::Instance()->GetElement (anElement, myAncestors);

        myGraph->AddConnection (myConnector, anGraphElement);
        myGraph->AddElement (anGraphElement);
    } else {
        myGraph->AddConnection (myConnector, nullptr);
    }
}

bool IntersectionFounder::IsIsolatorLayer (LayerType theLayer)
{
    return std::find (myIsolatorLayers.begin(), myIsolatorLayers.end(), theLayer) != myIsolatorLayers.end();
}

