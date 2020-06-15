#include "gds_reference.h"
#include "gds_structureelementvisitor.h"
#include "gds_structure.h"

GDS_Reference::GDS_Reference()
{

}

GDS_Reference::GDS_Reference(const GDS_Reference& theReference)
    :myReference (theReference.myReference), myTrsf (theReference.myTrsf)
{

}

GDS_Reference::GDS_Reference(const GDS_StructurePtr& theReference,const Transformation2D& theTrsf)
    : myReference(theReference), myTrsf(theTrsf)
{

}

Transformation2D& GDS_Reference::Transformation()
{
    return myTrsf;
}

const Transformation2D& GDS_Reference::Transformation() const
{
    return myTrsf;
}

void GDS_Reference::SetTransformation2D(const Transformation2D& theTrsf)
{
	myTrsf = theTrsf;
}

const GDS_StructurePtr& GDS_Reference::Reference() const
{
    return myReference;
}

void GDS_Reference::SetReference (const GDS_StructurePtr& theReference)
{
    myReference = theReference;
}

void GDS_Reference::Accept (GDS_StructureElementVisitor& theVisitor)
{
    auto aRes = theVisitor.Visit (shared_from_this());

    if (aRes) {
        myReference->Accept (theVisitor);
    }

    theVisitor.VisitEnd (shared_from_this());
}

Box2d GDS_Reference::BoundingBox (LayerType theLayer) const
{
    return myReference->BoundingBox (theLayer).Transformed (myTrsf);
}

Box2d GDS_Reference::BoundingBox() const
{
	return myReference->BoundingBox().Transformed(myTrsf);
}

size_t GDS_Reference::Hash() const
{
    GDS_ReferenceHash aHash;
    return aHash (*this);
}

size_t GDS_ReferenceHash::operator()(const GDS_Reference& theReference)
{    
    size_t aSeed = 0;

    auto aTrsfHash = Transformation2DHash() (theReference.Transformation());
    hash_combine (aSeed, aTrsfHash);

    auto aHash = std::hash<std::string>{}(theReference.Reference()->Name());
    hash_combine (aSeed, aHash);

    return aSeed;
}
