#ifndef GDS_STRUCTUREELEMENTVISITOR_H
#define GDS_STRUCTUREELEMENTVISITOR_H

#include "typedefs.h"

class GDS_StructureElementVisitor
{
public:
    virtual ~GDS_StructureElementVisitor() {}

    virtual bool Visit (const GDS_StructurePtr& theStructure) = 0;
    virtual void VisitEnd (const GDS_StructurePtr& theStructure) = 0;

    virtual bool Visit (const GDS_ReferencePtr& theReference) = 0;
    virtual void VisitEnd (const GDS_ReferencePtr& theReference) = 0;

    virtual bool Visit (const GDS_ArrayReferencePtr& theReference) = 0;
    virtual void VisitEnd (const GDS_ArrayReferencePtr& theReference) = 0;

    virtual void Visit (const GDS_ObjectPtr& theObject) = 0;

};

class GDS_StructureElementVisitorVoid : public GDS_StructureElementVisitor
{
public:
    bool Visit (const GDS_StructurePtr& /*theStructure*/) { return true; }
    void VisitEnd (const GDS_StructurePtr& /*theStructure*/) { }

    bool Visit (const GDS_ReferencePtr& /*theReference*/) { return true; }
    void VisitEnd (const GDS_ReferencePtr& /*theReference*/) { }

    bool Visit (const GDS_ArrayReferencePtr& /*theReference*/) { return true; }
    void VisitEnd (const GDS_ArrayReferencePtr& /*theReference*/) { }

    void Visit (const GDS_ObjectPtr& /*theObject*/) { }

};

#endif // GDS_STRUCTUREELEMENTVISITOR_H
