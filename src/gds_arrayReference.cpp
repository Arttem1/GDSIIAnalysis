#include "gds_arrayReference.h"
#include "gds_structureelementvisitor.h"
#include "gds_structure.h"

GDS_ArrayReference::GDS_ArrayReference()
{

}

Transformation2D& GDS_ArrayReference::Transformation()
{
    return myTrsf;
}
const Transformation2D& GDS_ArrayReference::Transformation() const
{
	return myTrsf;
}

const GDS_StructurePtr& GDS_ArrayReference::Reference() const
{
    return myReferences.front().Reference();
}

void GDS_ArrayReference::SetReference (const GDS_StructurePtr& theReference)
{
	GDS_Reference aReference(theReference, myTrsf);
	myReferences.push_back(aReference);
}

const Point2D<double>& GDS_ArrayReference::StepX() const
{
    return myStepX;
}

void GDS_ArrayReference::SetStepX (const Point2D<double>& theStep) 
{
    myStepX = theStep;
}

const Point2D<double>& GDS_ArrayReference::StepY() const
{
    return myStepY;
}

void GDS_ArrayReference::SetStepY (const Point2D<double>& theStep) 
{
    myStepY = theStep;
}

GDS_ArrayReference::COL GDS_ArrayReference::Columns() const
{
    return myCol;
}

void GDS_ArrayReference::SetColumns (COL theColumns)
{
    myCol = theColumns;
}

GDS_ArrayReference::ROW GDS_ArrayReference::Rows() const
{
    return myRow;
}

void GDS_ArrayReference::SetRows (ROW theRows)
{
    myRow = theRows;
}

void GDS_ArrayReference::Fill()
{
	auto afirstpoint_X = myTrsf.Position().X();
	auto afirstpoint_Y = myTrsf.Position().Y();
	auto aSpacingX_X = (myStepX.X() - afirstpoint_X) / myCol;
	auto aSpacingX_Y = (myStepX.Y() - afirstpoint_Y) / myRow;
	auto aSpacingY_X = (myStepY.X() - afirstpoint_X) / myCol;
	auto aSpacingY_Y = (myStepY.Y() - afirstpoint_Y) / myRow;

	Point2D<double> aStep_X(aSpacingX_X, aSpacingX_Y);
	Point2D<double> aStep_Y(aSpacingY_X, aSpacingY_Y);

	auto aReferences = myReferences.front().Reference();
	Transformation2D aTrsf(myTrsf);
	
	for (int i = 0; i < myCol; i++) { // 1 , 2
		
		if (i != 0) {
			aTrsf.SetPosition(aStep_Y.Multiplied(i));
			GDS_Reference aReference(aReferences, aTrsf);
			myReferences.push_back(aReference);
		}

		for (int j = 1; j < myRow; j++) {
 
			aTrsf.SetPosition(aStep_Y.Multiplied(j));
			GDS_Reference aReference(aReferences, aTrsf);
			myReferences.push_back(aReference);
        }
	}
}

void GDS_ArrayReference::Accept (GDS_StructureElementVisitor& theVisitor)
{
    auto aRes = theVisitor.Visit (shared_from_this());

    if (aRes) {
        if (myReferences.size() == 1) {
            Fill();
        }

        for (auto& aReference : myReferences) {
            aReference.Accept (theVisitor);
        }
    }

    theVisitor.VisitEnd (shared_from_this());
}

Box2d GDS_ArrayReference::BoundingBox (LayerType theLayer) const
{
    Box2d aBox;

    for (auto& aReference : myReferences) {
        aBox.Add (aReference.BoundingBox (theLayer));
    }

    return aBox;
}

Box2d GDS_ArrayReference::BoundingBox() const
{
	Box2d aBox;

	for (auto& aReference : myReferences) {
        aBox.Add (aReference.BoundingBox());
	}

	return aBox;
}


size_t GDS_ArrayReference::Hash() const
{
    GDS_ArrayReferenceHash aHash;
    return aHash (*this);
}

size_t GDS_ArrayReferenceHash::operator()(const GDS_ArrayReference& theReference)
{
    size_t aSeed = 0;

    auto aTrsfHash = Transformation2DHash() (theReference.Transformation());
    hash_combine (aSeed, aTrsfHash);

    auto aHash = std::hash<std::string>{}(theReference.Reference()->Name());
    hash_combine (aSeed, aHash);

    hash_combine (aSeed, theReference.Rows());
    hash_combine (aSeed, theReference.Columns());

    return aSeed;
}
