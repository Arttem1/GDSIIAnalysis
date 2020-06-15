#ifndef INTERSECTIONFOUNDER_H
#define INTERSECTIONFOUNDER_H

#include "founder.h"
#include "box2d.h"
#include "polygon2d.h"

class IntersectionFounder : public Founder
{
public:
    IntersectionFounder (const GDS_GraphPtr& theGraph,
                         const GDS_GraphElementEdgePtr& theConnector,
                         const LayersSet& theIsolatorLayers);

    bool Visit (const GDS_StructurePtr& theStructure) override;

    void Visit (const GDS_ObjectPtr& theObject) override;

    bool AncestorsIsSameToConnector();

    bool IsIsolatorLayer (LayerType theLayer);

    const LayersSet&             myIsolatorLayers;
    GDS_GraphPtr                 myGraph;
    GDS_GraphElementEdgePtr      myConnector;

    // For perfomance
    polygon2d          myConnectorTransformedPolygon;
    Box2d              myConnectorTransformedBox2d;
};


#endif // INTERSECTIONFOUNDER_H
