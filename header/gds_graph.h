#ifndef GDS_GRAPH_H
#define GDS_GRAPH_H

#include "gds_file.h"
#include "gds_structureelement.h"
#include "typedefs.h"
#include "gds_graphelement.h"

class GDS_Graph : public std::enable_shared_from_this<GDS_Graph>
{
public:
    GDS_Graph (const GDS_StructurePtr& theStructure);

    ConnectionsType Connections() const;

    const GDS_GraphList& SubGraphs() const;
    void AddSubGraph (const GDS_GraphPtr& theSubGraph);

    GDS_GraphElementPtr SearchElement (const uint64_t& theElementNumber) const;

    ConnectionsType SearchConnectedWith (const GDS_GraphElementPtr& theElement);

    const GDS_StructurePtr& Structure() { return myStructure; }

    bool IsNull() { return mySearchContainer.empty(); }

    void AddElement (const GDS_GraphElementPtr& theElement) { mySearchContainer.insert (theElement); }

    void AddConnection (const GDS_GraphElementEdgePtr& theEdge, const GDS_GraphElementNodePtr& theNode);

    SearchContainer         mySearchContainer;

private:
    GDS_StructurePtr        myStructure;
    GDS_GraphList           mySubGraphs;
    ConnectionsType         myConnections;
};

#endif // GDS_GRAPH_H
