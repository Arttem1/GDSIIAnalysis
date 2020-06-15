#include "application.h"

#include "gds_graphfactory.h"
#include "gds_structure.h"
#include "gds_stuctureinfocreator.h"
#include "gds_reader.h"


Application::Application()
{

}

bool Application::Import (const std::string& theFilename)
{
    GDS_Reader aReader;
    auto aRes = aReader.ReadFile (theFilename);

    if (!aRes) {
        return false;
    }

    myFile = aReader.File();
    return true;
}

void Application::SetContactLayers (const LayersSet& theContactLayers)
{
   GDS_GraphFactory::Instance()->SetContactLayers (theContactLayers);
}

void Application::SetIsolatorLayers (const LayersSet& theIsolatorLayers)
{
    GDS_GraphFactory::Instance()->SetIsolatorLayers (theIsolatorLayers);
}

GDS_StructurePtr Application::Structure (const std::string& theStructureName) const
{
    return myFile.Structure (theStructureName);
}

const GDS_GraphPtr& Application::GetGraph (const GDS_StructurePtr& theStructure)
{
    return GDS_GraphFactory::Instance()->Graph (theStructure);
}

GDS_StructureInfo Application::GetStructureInfo (const GDS_StructurePtr& theStructure)
{
    GDS_StuctureInfoCreator anInfoCreator;
    theStructure->Accept (anInfoCreator);
    return anInfoCreator.Result();
}
