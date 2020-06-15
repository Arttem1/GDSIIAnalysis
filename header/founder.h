#ifndef FOUNDER_H
#define FOUNDER_H

#include "typedefs.h"
#include "transformation2d.h"
#include "gds_structureelementvisitor.h"

class Founder : public GDS_StructureElementVisitor
{
public:
    bool Visit (const GDS_StructurePtr& theStructure) override;

    void VisitEnd (const GDS_StructurePtr& /*theStructure*/) override;

    bool Visit (const GDS_ReferencePtr& theReference) override;

    void VisitEnd (const GDS_ReferencePtr& theReference) override;

    bool Visit (const GDS_ArrayReferencePtr& theReference) override;

    void VisitEnd (const GDS_ArrayReferencePtr& theReference) override;

    Transformation2D myTrsf;
    AncetorsElements  myAncestors;
};


#endif // FOUNDER_H
