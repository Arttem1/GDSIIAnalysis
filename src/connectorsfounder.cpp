#include "connectorsfounder.h"
#include "gds_graph.h"
#include "gds_graphfactory.h"
#include "gds_object.h"
#include "gds_element.h"
#include "gds_path.h"

ConnectorsFounder::ConnectorsFounder (const GDS_GraphPtr& theGraph,
                                      const LayersSet& theContactLayers,
                   const LayersSet& theIsolatorLayers,
                   GDS_GraphElementEdges& theConectors) :
    myGraph (theGraph),
    myContactLayers (theContactLayers),
    myIsolatorLayers (theIsolatorLayers),
    myConnectors (theConectors)
{

}

void ConnectorsFounder::Visit (const GDS_ObjectPtr& theObject)
{
    auto anElement      = theObject->Element();
    auto anElementLayer = anElement->Layer();

    if (IsIsolatorLayer (anElementLayer)) {
        return;
    }

    if (IsContactLayer (anElementLayer)
     || std::dynamic_pointer_cast<GDS_Path> (anElement)) {
        auto aConnectorWithAncestors = GDS_GraphFactory::Instance()->GetElement (anElement, myAncestors);
        myConnectors.push_back (aConnectorWithAncestors);
        myGraph->AddElement (aConnectorWithAncestors);
    }
}

bool ConnectorsFounder::IsContactLayer (LayerType theLayer)
{
    return myContactLayers.find (theLayer) != myContactLayers.end();
}

bool ConnectorsFounder::IsIsolatorLayer (LayerType theLayer)
{
    return myIsolatorLayers.find (theLayer) != myIsolatorLayers.end();
}

