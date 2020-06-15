#ifndef GDS_STRUCTURE_H
#define GDS_STRUCTURE_H

#include "gds_structureelement.h"
#include "typedefs.h"

#include <vector>
#include <memory>

class GDS_Structure : public GDS_StructureElement, public std::enable_shared_from_this<GDS_Structure>
{
public:
    GDS_Structure();

    const std::string& Name() const;
    void SetName (const std::string& theName);

    const GDS_StructureElements& Elements() const;
    void AddElement (const GDS_StructureElementPtr& theElement);

    void Accept (GDS_StructureElementVisitor& theVisitor) override;

    size_t Hash() const override;

    Box2d BoundingBox (LayerType theLayer) const override;

	Box2d BoundingBox() const override;

private:
    GDS_StructureElements myElements;
    std::string           myName;
};

class GDS_StructureHash
{
public:
    size_t operator()(const GDS_Structure& theStructure) const;
};

#endif // GDS_STRUCTURE_H
