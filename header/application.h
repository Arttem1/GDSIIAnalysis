#ifndef APPLICATION_H
#define APPLICATION_H

#include "gds_graph.h"

class GDS_StructureInfo;

class Application
{
public:
    Application();

    bool Import (const std::string& theFilename);

    void SetContactLayers (const LayersSet& theContactLayers);

    void SetIsolatorLayers (const LayersSet& theIsolatorLayers);

    GDS_StructurePtr Structure (const std::string& theStructureName) const;

    const GDS_GraphPtr& GetGraph (const GDS_StructurePtr& theStructure);

    GDS_StructureInfo GetStructureInfo (const GDS_StructurePtr& theStructure);

private:
    GDS_File myFile;
};

// Enter element number: Box 15
// Box 18, Box 15, Bondary 2 connected by Path 13
//
// Print Box 22

#endif // APPLICATION_H
