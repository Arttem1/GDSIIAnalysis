#ifndef GDS_OBJECT_H
#define GDS_OBJECT_H

#include <memory>

#include "gds_structureelement.h"

class GDS_Element;

class GDS_Object : public GDS_StructureElement, public std::enable_shared_from_this<GDS_Object>
{
public:
    enum ElementsType {
        Bondary,
        Path,
        Box,
        Unknown
    };

    GDS_Object (const std::shared_ptr<GDS_Element>& theElement, ElementsType theType);

    ElementsType ElementType() const;

    void SetElementType (ElementsType theType);

    const std::shared_ptr<GDS_Element>& Element() const;

    void Accept (GDS_StructureElementVisitor& theVisitor) override;

    size_t Hash() const override;

    Box2d BoundingBox (LayerType theLayer) const override;
	
	Box2d BoundingBox() const override;

private:
    ElementsType                 myType = Unknown;
    std::shared_ptr<GDS_Element> myElement;
};

#endif // GDS_OBJECT_H
