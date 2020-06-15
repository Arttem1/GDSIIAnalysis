#include "gds_arrayReference.h"
#include "gds_element.h"
#include "gds_graph.h"
#include "gds_graphfactory.h"
#include "gds_helper.h"
#include "gds_object.h"
#include "gds_path.h"
#include "gds_reference.h"
#include "gds_structure.h"
#include "gds_structureelementvisitor.h"
#include "typedefs.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>

using namespace std;

GDS_Graph::GDS_Graph (const GDS_StructurePtr& theStructure)
	: myStructure (theStructure)
{

}

GDS_GraphElementPtr GDS_Graph::SearchElement (const uint64_t& theElementNumber) const
{
    auto aSearchElement = std::shared_ptr<GDS_GraphElement> (new GDS_GraphElement (theElementNumber));
    auto anIt = mySearchContainer.find (aSearchElement);

    if (anIt != mySearchContainer.end()) {
        return *anIt;
    }

    for (const auto& aSubGraph : mySubGraphs) {
       auto anElement = aSubGraph->SearchElement (theElementNumber);
       if (anElement) {
           return anElement;
       }
    }

    return GDS_GraphElementPtr();
}

ConnectionsType GDS_Graph::Connections() const
{
    ConnectionsType aConnections = myConnections; // copy

    std::list<ConnectionsType> aSubGraphConnectionsList;

    for (const auto& aSubGraph : mySubGraphs) { // A
       aSubGraphConnectionsList.push_front (aSubGraph->Connections());
    }

    for (auto& aConnection : aConnections) {
        const auto& aConnectionAncestors = aConnection.first->myAncestors;

        if (aConnectionAncestors.size() <= 2) {
            continue;
        }

        auto aEdgeToSearch = aConnection.first->CopyWithout2FirstAncestors();

        for (auto& aSubGraphConnections : aSubGraphConnectionsList) { // A

            try {
                auto& aConnectedElements = aSubGraphConnections.at (aEdgeToSearch); // find

                for (const auto& aConnectedElement : aConnectedElements) {
                    auto anAncestors = aConnectedElement->myAncestors; // A
                    anAncestors.push_front (aConnectionAncestors[1]); // ref a
                    anAncestors.push_front (aConnectionAncestors[0]); // B

                    auto anElement = GDS_GraphFactory::Instance()->GetElement (aConnectedElement->myElement, anAncestors); // B, ref A, A}
                    aConnection.second.push_front (anElement);
                }

                // если нашли
                //std::copy (aConnectedElements.begin(), aConnectedElements.end(), std::front_inserter (aConnection.second));
                aSubGraphConnections.erase (aEdgeToSearch); // ?
            } catch (...) {

            }
        }
    }

    return aConnections;
}

const GDS_GraphList& GDS_Graph::SubGraphs() const
{
    return mySubGraphs;
}

void GDS_Graph::AddSubGraph (const GDS_GraphPtr& theSubGraph)
{
    auto aSubGraphStructureName = theSubGraph->Structure()->Name();
    auto aNotFound = mySubGraphs.empty() ||
            (std::find_if (mySubGraphs.begin(), mySubGraphs.end(), [aSubGraphStructureName] (const GDS_GraphPtr& theSubGraph) {
                                    return aSubGraphStructureName == theSubGraph->Structure()->Name();
    }) == mySubGraphs.end());

    if (aNotFound) {
        mySubGraphs.push_back (theSubGraph);
    }
}

ConnectionsType GDS_Graph::SearchConnectedWith (const GDS_GraphElementPtr& theElement)
{
    auto aConnections = Connections();

    ConnectionsType anElementConnections;

    const auto& aConnectedElementsByConnector = aConnections[theElement];

    if (!aConnectedElementsByConnector.empty()) {
        anElementConnections[theElement] = aConnectedElementsByConnector;
    }

    for (const auto& aConnection : aConnections) {
        for (const auto& aConnectedElement : aConnection.second) {
            if (aConnectedElement->myElement == theElement->myElement) {
                anElementConnections.insert (aConnection);
                break;
            }
        }
    }

    return anElementConnections;
}


void GDS_Graph::AddConnection (const GDS_GraphElementEdgePtr& theEdge, const GDS_GraphElementNodePtr& theNode)
{
    auto& aConnectedElements = myConnections[theEdge];
    if (theNode) {
        aConnectedElements.push_front (theNode);
    }
}
