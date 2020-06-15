#include "gds_object.h"
#include "gds_structureelementvisitor.h"
#include "gds_element.h"

#include <cassert>

GDS_Object::GDS_Object (const std::shared_ptr<GDS_Element>& theElement, ElementsType theType)
    : myType (theType),
      myElement (theElement)
{

}

GDS_Object::ElementsType GDS_Object::ElementType() const
{
    return myType;
}

void GDS_Object::SetElementType (ElementsType theType)
{
    myType = theType;
}

const std::shared_ptr<GDS_Element>& GDS_Object::Element() const
{
    return myElement;
}

void GDS_Object::Accept (GDS_StructureElementVisitor& theVisitor)
{
    theVisitor.Visit (shared_from_this());
}

Box2d GDS_Object::BoundingBox (LayerType theLayer) const
{
    if (myElement->Layer() == theLayer) {
        return myElement->BoundingBox();
    }

    return Box2d();
}
Box2d GDS_Object::BoundingBox() const
{
	return myElement->BoundingBox();
}


size_t GDS_Object::Hash() const
{
    assert (false);
    return 0; // not used in ancestors container
}
