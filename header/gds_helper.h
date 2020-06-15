#ifndef GDS_HELPER_H
#define GDS_HELPER_H

#include "gds_element.h"
#include <memory>
#include "polygon2d.h"

class GDS_Path;

class GDS_Helper
{
public:
	static Transformation2D CombinedTransformation (const AncetorsElements& theAncestors);

	static bool IntersectionPolygon(const polygon2d& ThePolygon1, const polygon2d& ThePolygon2);

	static bool PointInPolygon(const Point2D<double>& , const polygon2d& );
	static bool PolygonInPolygon(const polygon2d&, const polygon2d&);
    
    static GDS_Element::CoordinatesType ToPolygon (GDS_Path& thePath);
};

#endif // GDS_HELPER_H
