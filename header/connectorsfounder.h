#ifndef CONNECTORSFOUNDER_H
#define CONNECTORSFOUNDER_H

#include "founder.h"
#include "typedefs.h"

class ConnectorsFounder : public Founder
{
public:
    ConnectorsFounder (const GDS_GraphPtr& theGraph,
                       const LayersSet& theContactLayers,
                       const LayersSet& theIsolatorLayers,
                       GDS_GraphElementEdges& theConectors);

    void Visit (const GDS_ObjectPtr& theObject) override;

    bool IsContactLayer (LayerType theLayer);

    bool IsIsolatorLayer (LayerType theLayer);

    const LayersSet& myContactLayers;
    const LayersSet& myIsolatorLayers;
    GDS_GraphElementEdges& myConnectors;
    GDS_GraphPtr     myGraph;
};


#endif // CONNECTORSFOUNDER_H
