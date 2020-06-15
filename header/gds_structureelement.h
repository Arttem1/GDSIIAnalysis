#ifndef GDS_STRUCTUREELEMENT_H
#define GDS_STRUCTUREELEMENT_H

#include <memory>
#include "typedefs.h"
#include "box2d.h"

class GDS_StructureElementVisitor;

class GDS_StructureElement
{
public:
    virtual ~GDS_StructureElement() { }

    virtual void Accept (GDS_StructureElementVisitor& theVisitor) = 0;

    virtual Box2d BoundingBox (LayerType theLayer) const = 0;

	virtual Box2d BoundingBox() const  = 0;

    virtual size_t Hash() const = 0;
};

#endif // GDS_STRUCTUREELEMENT_H
