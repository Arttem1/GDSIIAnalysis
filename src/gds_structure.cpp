#include "gds_structure.h"
#include "gds_structureelementvisitor.h"

#include <iostream>
#include <string>

GDS_Structure::GDS_Structure()
{

}

const std::string& GDS_Structure::Name() const
{
    return myName;
}

void GDS_Structure::SetName (const std::string& theName)
{
    myName = theName;
}

void GDS_Structure::AddElement (const GDS_StructureElementPtr& theElement)
{
    myElements.push_back (theElement);
}

void GDS_Structure::Accept (GDS_StructureElementVisitor& theVisitor)
{
    auto aRes = theVisitor.Visit (shared_from_this());

    if (aRes) {
        for (const auto& anElement : myElements) {
            anElement->Accept(theVisitor);
        }
    }

    theVisitor.VisitEnd (shared_from_this());
}

Box2d GDS_Structure::BoundingBox (LayerType theLayer) const
{
    Box2d aBox;
    for (const auto& anElement : myElements) {
        aBox.Add (anElement->BoundingBox (theLayer));
    }
    return aBox;
}

Box2d GDS_Structure::BoundingBox() const
{
	Box2d aBox;
	for (const auto& anElement : myElements) {
		aBox.Add(anElement->BoundingBox());
	}
	return aBox;
}

size_t GDS_Structure::Hash() const
{
    GDS_StructureHash aHash;
    return aHash (*this);
}

const GDS_StructureElements& GDS_Structure::Elements() const
{
    return myElements;
}


size_t GDS_StructureHash::operator()(const GDS_Structure& theStructure) const
{
    size_t aSeed = 0;

    auto aHash = std::hash<std::string>{}(theStructure.Name());
    hash_combine (aSeed, aHash);
    hash_combine (aSeed, theStructure.Elements().size());

    return aSeed;
}
