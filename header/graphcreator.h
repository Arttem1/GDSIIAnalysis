#ifndef GRAPHCREATOR_H
#define GRAPHCREATOR_H

#include "gds_structureelementvisitor.h"
#include "gds_graph.h"
#include "gds_graphfactory.h"
#include "typedefs.h"
#include "gds_structure.h"

#include <algorithm>

class GraphCreator : public GDS_StructureElementVisitorVoid
{
public:
    bool Visit (const GDS_StructurePtr& theStructure) override
    {
        auto aGraph = GDS_GraphFactory::Instance()->CreateGraph (theStructure);

        if (!myCurrentGraphs.empty()) {
            myCurrentGraphs.back()->AddSubGraph (aGraph);
        }

        myCurrentGraphs.push_back (aGraph);
		AddCreatedGraph(aGraph);

        return true;
    }

    void VisitEnd (const GDS_StructurePtr& theStructure) override
    {
        myCurrentGraphs.pop_back();
    }

	void AddCreatedGraph (const GDS_GraphPtr& theGraphArg)
	{
		auto anIsNotFound = std::find_if(myCreatedGraphs.begin(), myCreatedGraphs.end(), [theGraphArg](const GDS_GraphPtr& theGraph) {
			return theGraph->Structure()->Name() == theGraphArg->Structure()->Name();
		}) == myCreatedGraphs.end();

		if (!anIsNotFound) {
			myCreatedGraphs.push_back(theGraphArg);
		}
	}

    GDS_GraphList   myCurrentGraphs;
    GDS_GraphList   myCreatedGraphs;
};

#endif // GRAPHCREATOR_H
