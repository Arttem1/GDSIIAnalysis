#ifndef BOUNDINGBOX2DHELPER_H
#define BOUNDINGBOX2DHELPER_H

#include "box2d.h"
#include "typedefs.h"

class polygon2d;

class BoundingBox2dHelper
{
public:
    BoundingBox2dHelper();

    static Box2d Compute (const polygon2d& thePolygon);
};

#endif // BOUNDINGBOX2DHELPER_H
