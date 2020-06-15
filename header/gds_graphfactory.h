#ifndef GDS_GRAPHFACTORY_H
#define GDS_GRAPHFACTORY_H

#include <unordered_map>
#include <string>
#include "gds_graph.h"

#include "typedefs.h"

class GraphCreator;

class GDS_GraphFactory
{
public:
    static const GDS_GraphFactoryPtr& Instance();

    void SetContactLayers (const LayersSet& theContactLayers);

    void SetIsolatorLayers (const LayersSet& theIsolatorLayers);

    const GDS_GraphPtr& Graph (const GDS_StructurePtr& theStructure);

    GDS_GraphElementPtr GetElement (const GDS_ElementPtr& theElement, const AncetorsElements& theAncestors);


protected:
    GDS_GraphFactory();

private:
    friend class GraphCreator;
    const GDS_GraphPtr& CreateGraph (const GDS_StructurePtr& theStructure);

private:
    FactoryGraphCacheType    myCache;
    FactoryElementsCacheType myElementsCache;

    LayersSet myContactLayers;
    LayersSet myIsolatorLayers;
};

#endif // GDS_GRAPHFACTORY_H
