#include "gds_stuctureinfocreator.h"
#include "gds_object.h"
#include "gds_element.h"

GDS_StuctureInfoCreator::GDS_StuctureInfoCreator()
{

}

const GDS_StructureInfo& GDS_StuctureInfoCreator::Result() const
{
    return myInfo;
}

bool GDS_StuctureInfoCreator::Visit (const GDS_ReferencePtr& /*theReference*/)
{
    myInfo.myReferencesCountByMe++;
    return true;
}

bool GDS_StuctureInfoCreator::Visit (const GDS_ArrayReferencePtr& /*theReference*/)
{
    myInfo.myReferencesCountByMe++;
    return true;
}

void GDS_StuctureInfoCreator::Visit (const GDS_ObjectPtr& theObject)
{
    auto aType = theObject->ElementType();

    switch (aType) {
    case GDS_Object::Path:    myInfo.myPathCount++; break;
    case GDS_Object::Box:     myInfo.myBoxesCount++; break;
    case GDS_Object::Bondary: myInfo.myBondaryCount++; break;
    default:
        break;
    }

    myInfo.myActiveLayers.insert (theObject->Element()->Layer());
}
