#ifndef GDS_ARRAYREFERENCE_H
#define GDS_ARRAYREFERENCE_H

#include "gds_reference.h"
#include "gds_structureelement.h"
#include <vector>

class GDS_ArrayReference : public GDS_StructureElement, public std::enable_shared_from_this<GDS_ArrayReference>
{
public:
    typedef short COL;
    typedef short ROW;

    typedef std::vector<Point2D<double>>  CoordinatesType;

	GDS_ArrayReference();

    Transformation2D& Transformation();
	const Transformation2D& Transformation() const;

    const GDS_StructurePtr& Reference() const;
    void SetReference (const GDS_StructurePtr& theReference);

    void Accept (GDS_StructureElementVisitor& theVisitor) override;

    const Point2D<double>& StepX() const;
    void SetStepX (const Point2D<double>&);

    const Point2D<double>& StepY() const;
    void SetStepY (const Point2D<double>&);

    COL Columns() const;
    void SetColumns (COL theColumns);

    ROW Rows() const;
    void SetRows (ROW theRows);

    size_t Hash() const override;

    Box2d BoundingBox (LayerType theLayer) const override;
	Box2d BoundingBox() const;

private:
    void Fill();

private:
    std::vector<GDS_Reference> myReferences;
	COL myCol;
    ROW myRow;
    Transformation2D myTrsf;
    Point2D<double> myStepX;
    Point2D<double> myStepY;
};

class GDS_ArrayReferenceHash
{
public:
    size_t operator()(const GDS_ArrayReference& theReference);
};

#endif // GDS_ARRAYREFERENCE_H
