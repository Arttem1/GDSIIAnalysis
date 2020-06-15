#ifndef GDS_FILE_H
#define GDS_FILE_H

#include <memory>
#include <unordered_map>
#include "typedefs.h"

class GDS_Structure;
class GDS_StructureElementVisitor;

class GDS_File
{
public:
    typedef std::shared_ptr<GDS_Structure>                StructurePtr;
    typedef std::unordered_map<std::string, StructurePtr> StructuresType;

    GDS_File() {}

    GDS_File (const std::string& theFileName);

    const StructurePtr& MainStructure() const;
    void SetMainStructure (const StructurePtr& theStructure);

    StructurePtr Structure (const std::string& theStructureName) const;
    GDS_StructureList Structures() const;
    void AddStucture (const StructurePtr& theStructure);

    //void Accept (GDS_StructureElementVisitor& theVisitor);

private:
    StructurePtr   myMainStructure;
    StructuresType myStructures;
    std::string    myFilename;
};

#endif // GDS_FILE_H
