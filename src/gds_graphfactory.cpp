#include "gds_graphfactory.h"
#include "gds_graphelement.h"
#include "gds_structure.h"
#include "graphcreator.h"
#include "connectorsfounder.h"
#include "intersectionfounder.h"
#include "typedefs.h"

#include <tbb/parallel_for.h>

GDS_GraphFactoryPtr theInstance = nullptr;

GDS_GraphFactory::GDS_GraphFactory()
{

}

const GDS_GraphFactoryPtr& GDS_GraphFactory::Instance()
{
    if (!theInstance) {
        theInstance = std::shared_ptr<GDS_GraphFactory> (new GDS_GraphFactory());
    }

    return theInstance;
}

void GDS_GraphFactory::SetContactLayers (const LayersSet& theContactLayers)
{
    myContactLayers = theContactLayers;
}

void GDS_GraphFactory::SetIsolatorLayers (const LayersSet& theIsolatorLayers)
{
    myIsolatorLayers = theIsolatorLayers;
}


//! Create graph without connections and subgraphs
const GDS_GraphPtr& GDS_GraphFactory::CreateGraph (const GDS_StructurePtr& theStructure)
{
    auto& aCached = myCache[theStructure->Name()];

    if (!aCached) {
        aCached = std::make_shared<GDS_Graph> (theStructure);
    }

    return aCached;
}

namespace {
void FindIntersections (const GDS_GraphPtr& theGraph,
                        const LayersSet& theIsolatorLayers,
                        const GDS_GraphElementEdgePtr& theConnector)
{
    IntersectionFounder anIntersectionFounder (theGraph, theConnector, theIsolatorLayers);
    theGraph->Structure()->Accept (anIntersectionFounder);
}

void FillGraph (const GDS_GraphPtr& theGraph,
                const LayersSet& theIsolatorLayers,
                const LayersSet& theContactLayers)
{
    auto aGraphStructure = theGraph->Structure();

    GDS_GraphElementEdges theConnectors;

    ConnectorsFounder aConnectorsFounder (theGraph, theContactLayers, theIsolatorLayers, theConnectors);
    aGraphStructure->Accept (aConnectorsFounder);

    auto FindIntersectionsByIndex = [theConnectors, theGraph, theIsolatorLayers] (const tbb::blocked_range<size_t>& theRange) {
        for (size_t i = theRange.begin(); i != theRange.end(); ++i ) {
            FindIntersections (theGraph, theIsolatorLayers, theConnectors[i]);
        }
    };

    auto aFullRange = tbb::blocked_range<size_t> (0, theConnectors.size());

    if (USE_PARALLELISM && theConnectors.size() >= 1) {
        tbb::parallel_for (aFullRange, FindIntersectionsByIndex);
    } else {
        FindIntersectionsByIndex (aFullRange);
    }
}

}

//! Create graph with connections and subgraphs
const GDS_GraphPtr& GDS_GraphFactory::Graph (const GDS_StructurePtr& theStructure)
{
    auto& aCached = myCache[theStructure->Name()];

    if (!aCached || aCached->IsNull()) {
        // 1. Create graph with subgraphs without connections
        GraphCreator aCreator;
        theStructure->Accept (aCreator);
        const auto& aCreatedGraphs = aCreator.myCreatedGraphs;

        // 2. Search connectors and intersections for graph and subgraphs

        auto FillGraphByIndex = [aCreatedGraphs, this] ( const tbb::blocked_range<size_t>& theRange ) {
            for (size_t i = theRange.begin(); i != theRange.end(); ++i ) {
                FillGraph (aCreatedGraphs[i], myContactLayers, myIsolatorLayers);
            }
        };

        auto aFullRange = tbb::blocked_range<size_t> (0, aCreatedGraphs.size());

        if (USE_PARALLELISM) {
            tbb::parallel_for (aFullRange, FillGraphByIndex);
        } else {
            FillGraphByIndex (aFullRange);
        }
    }

    return aCached;
}

GDS_GraphElementPtr GDS_GraphFactory::GetElement (const GDS_ElementPtr& theElement, const AncetorsElements& theAncestors)
{
    GDS_GraphElementPtr aResult;
    auto aPair = std::make_pair (theElement, theAncestors);

    try {
        aResult = myElementsCache.at (aPair);
    } catch (...) {

        if (theAncestors.size() > 1) {
            auto anElement = GetElement (theElement, {theAncestors.back()});
            auto aPtr = new GDS_GraphElement (*anElement);
            aPtr->myAncestors = theAncestors;
            myElementsCache[aPair] = std::shared_ptr<GDS_GraphElement> (aPtr);
        } else {
            myElementsCache[aPair] = std::shared_ptr<GDS_GraphElement> (new GDS_GraphElement (theElement, theAncestors));
        }
        aResult = myElementsCache[aPair];
    }

    return aResult;
}
