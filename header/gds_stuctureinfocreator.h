#ifndef GDS_STUCTUREINFOCREATOR_H
#define GDS_STUCTUREINFOCREATOR_H

#include "gds_structureelementvisitor.h"
#include "gds_structureinfo.h"

class GDS_StuctureInfoCreator : public GDS_StructureElementVisitorVoid
{
public:
    GDS_StuctureInfoCreator();

    const GDS_StructureInfo& Result() const;

    //bool Visit (const GDS_StructurePtr& /*theStructure*/) override;

    bool Visit (const GDS_ReferencePtr& /*theReference*/) override;

    bool Visit (const GDS_ArrayReferencePtr& /*theReference*/) override;

    void Visit (const GDS_ObjectPtr& /*theObject*/) override;

private:
    GDS_StructureInfo myInfo;
};

#endif // GDS_STUCTUREINFOCREATOR_H
