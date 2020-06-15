#include "founder.h"
#include "gds_reference.h"
#include "gds_arrayReference.h"
#include "gds_structure.h"

bool Founder::Visit (const GDS_StructurePtr& theStructure)
{
    myAncestors.push_back (theStructure);
    return true;
}

void Founder::VisitEnd (const GDS_StructurePtr& /*theStructure*/)
{
    myAncestors.pop_back();
}

bool Founder::Visit (const GDS_ReferencePtr& theReference)
{
    myAncestors.push_back (theReference);
    myTrsf *= theReference->Transformation();
    return true;
}

void Founder::VisitEnd (const GDS_ReferencePtr& theReference)
{
    myAncestors.pop_back();
    myTrsf *= theReference->Transformation().Inverted();
}

bool Founder::Visit (const GDS_ArrayReferencePtr& theReference)
{
    myAncestors.push_back (theReference);
    myTrsf *= theReference->Transformation();
    return true;
}

void Founder::VisitEnd (const GDS_ArrayReferencePtr& theReference)
{
    myAncestors.pop_back();
    myTrsf *= theReference->Transformation().Inverted();
}
