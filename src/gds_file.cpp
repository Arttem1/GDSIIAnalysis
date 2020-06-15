#include "gds_file.h"
#include "gds_structureelementvisitor.h"
#include "gds_structure.h"

GDS_File::GDS_File (const std::string& theFileName)
    : myFilename (theFileName)
{

}

const GDS_File::StructurePtr& GDS_File::MainStructure() const
{
    return myMainStructure;
}

void GDS_File::SetMainStructure (const StructurePtr& theStructure)
{
    myMainStructure = theStructure;
}

void GDS_File::AddStucture (const StructurePtr& theStructure)
{
    myStructures[theStructure->Name()] = theStructure;
}

GDS_File::StructurePtr GDS_File::Structure (const std::string& theStructureName) const
{
    try {
        return myStructures.at (theStructureName);
    } catch (...) {
        return nullptr;
    }
}
GDS_StructureList GDS_File::Structures() const
{
    GDS_StructureList aList;
    for (const auto& aStructure : myStructures) {
        aList.push_front (aStructure.second);
    }
    return aList;
}

/*
void GDS_File::Accept (GDS_StructureElementVisitor& theVisitor)
{
    for (const auto& aStructure : myStructures) {
        aStructure->Accept (theVisitor);
    }
}*/
