#ifndef GDS_REFERENCE_H
#define GDS_REFERENCE_H

#include "gds_structureelement.h"
#include "transformation2d.h"
#include "typedefs.h"

#include <memory>

class GDS_Structure;

class GDS_Reference : public GDS_StructureElement, public std::enable_shared_from_this<GDS_Reference>
{
public:
    GDS_Reference();
	GDS_Reference(const GDS_Reference& theReference);
	GDS_Reference(const GDS_StructurePtr& theReference, const Transformation2D& theTrsf);

    Transformation2D& Transformation();
    const Transformation2D& Transformation() const;
	void SetTransformation2D(const Transformation2D& theReference);

    const GDS_StructurePtr& Reference() const;
    void SetReference (const GDS_StructurePtr& theReference);

    void Accept (GDS_StructureElementVisitor& theVisitor) override;

    size_t Hash() const override;

    Box2d BoundingBox (LayerType theLayer) const override;

	Box2d BoundingBox() const override;

private:
    GDS_StructurePtr myReference;
    Transformation2D myTrsf;
};

class GDS_ReferenceHash
{
public:
    size_t operator()(const GDS_Reference& theReference);
};

/*

SREF: arc
SREF arc

"arc" : [obj1, obj2]

STR: "arc"

unordered_map<std::string, std::vector < GDS_Reference > >
*/

#endif // GDS_REFERENCE_H
